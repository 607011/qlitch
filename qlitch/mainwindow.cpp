// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagewidget.h"

#include <QtCore/QDebug>
#include <QtGlobal>
#include <QFileDialog>
#include <QBuffer>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mAlgorithm(ALGORITHM_XOR)
{
    ui->setupUi(this);
    imageWidget = new ImageWidget;
    ui->verticalLayout->addWidget(imageWidget);
    QObject::connect(ui->actionOpenImage, SIGNAL(triggered()), SLOT(openImage()));
    QObject::connect(ui->actionSaveImageAs, SIGNAL(triggered()), SLOT(saveImageAs()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    QObject::connect(ui->qualitySlider, SIGNAL(valueChanged(int)), SLOT(updateImageWidget()));
    QObject::connect(ui->percentageSlider, SIGNAL(valueChanged(int)), SLOT(updateImageWidget()));
    QObject::connect(ui->iterationsSlider, SIGNAL(valueChanged(int)), SLOT(updateImageWidget()));
    QObject::connect(imageWidget, SIGNAL(imageDropped(QImage)), SLOT(setImage(QImage)));
    QObject::connect(imageWidget, SIGNAL(refresh()), SLOT(updateImageWidget()));
    ui->actionOne->setData(ALGORITHM_ONE);
    ui->actionZero->setData(ALGORITHM_ZERO);
    ui->actionXOR->setData(ALGORITHM_XOR);
    QObject::connect(ui->actionOne, SIGNAL(triggered()), SLOT(setAlgorithm()));
    QObject::connect(ui->actionZero, SIGNAL(triggered()), SLOT(setAlgorithm()));
    QObject::connect(ui->actionXOR, SIGNAL(triggered()), SLOT(setAlgorithm()));
    qsrand(QTime::currentTime().msec());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete imageWidget;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}

static double random(double lo, double hi) {
    if (qFuzzyCompare(lo, hi))
        return lo;
    return lo + ((hi - lo) * qrand() / (RAND_MAX + 1.0));
}

void MainWindow::updateImageWidget(void)
{
    if (mImage.isNull())
        return;
    int quality = ui->qualitySlider->value();
    int iterations = ui->iterationsSlider->value();
    int percent = ui->percentageSlider->value();
    QByteArray raw;
    QBuffer buffer(&raw);
    buffer.open(QIODevice::WriteOnly);
    mImage.save(&buffer, "JPG", quality);
    int firstPos = (long)(1e-2 * raw.size() * percent);
    for (int i = 0; i < iterations; ++i) {
        int pos = int(random(firstPos, raw.size()));
        int bit = qrand() % 8;

        switch (mAlgorithm) {
        default:
          // fall-through
        case ALGORITHM_XOR:
          raw[pos] = raw.at(pos) ^(1 << bit);
          break;
        case ALGORITHM_ONE:
          raw[pos] = raw.at(pos) | (1 << bit);
          break;
        case ALGORITHM_ZERO:
          raw[pos] = raw.at(pos) & ~(1 << bit);
          break;
        }
    }
    ui->statusBar->showMessage(tr("Resulting image size: %1 bytes").arg(raw.size()), 3000);
    imageWidget->setRaw(raw);
}

void MainWindow::setAlgorithm(void)
{
    QAction* action = reinterpret_cast<QAction*>(sender());
    if (action == NULL)
        return;
    mAlgorithm = (Algorithm)action->data().toInt();
    ui->statusBar->showMessage(tr("Algorithm: %1").arg(mAlgorithm), 1000);
    updateImageWidget();
}

void MainWindow::setImage(const QImage &img)
{
    mImage = img;
    updateImageWidget();
}

void MainWindow::openImage(void)
{
    const QString &imgFileName = QFileDialog::getOpenFileName(this, tr("Open image ..."));
    if (imgFileName.isEmpty())
        return;
    mImage.load(imgFileName);
    if (mImage.isNull())
        return;
    updateImageWidget();
}

void MainWindow::saveImageAs(void)
{
    const QString &imgFileName = QFileDialog::getSaveFileName(this, tr("Save image as ..."));
    if (imgFileName.isEmpty())
        return;
    imageWidget->image().save(imgFileName);
}
