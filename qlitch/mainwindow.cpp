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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
    qsrand(QTime::currentTime().msec());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete imageWidget;
}

static double random(double x, double y) {
    if (qFuzzyCompare(x, y))
        return x;
    double hi = qMax(x, y), lo = qMin(x, y);
    return lo + ((hi - lo) * qrand() / (RAND_MAX + 1.0));
}

void MainWindow::updateImageWidget(void)
{
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
        raw[pos] = raw.at(pos) | (1 << bit);
    }
    imageWidget->setRaw(raw);
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
