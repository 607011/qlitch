// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include "main.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imagewidget.h"
#include "random/rnd.h"

#include <QtCore/QDebug>
#include <QtGlobal>
#include <QFileDialog>
#include <QBuffer>
#include <QTime>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mAlgorithm(ALGORITHM_XOR)
{
    ui->setupUi(this);
    setWindowTitle(tr("%1 %2").arg(AppName).arg(AppVersion));
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
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    QObject::connect(ui->actionAboutQt, SIGNAL(triggered()), SLOT(aboutQt()));
    RAND::initialize();
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
        int pos = RAND::rnd(firstPos, raw.size());
        int bit = RAND::rnd() % 8;
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
    ui->actionZero->setChecked(false);
    ui->actionOne->setChecked(false);
    ui->actionXOR->setChecked(false);
    action->setChecked(true);
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


void MainWindow::about(void)
{
    QMessageBox::about(this, tr("About %1 %2%3").arg(AppName).arg(AppVersionNoDebug).arg(AppMinorVersion),
                       tr("<p><b>%1</b> produces a JPG glitch effect in images.\n"
                          "See <a href=\"%2\" title=\"%1 project homepage\">%2</a> for more info.</p>"
                          "<p>Copyright &copy; 2013 %3 &lt;%4&gt;, Heise Zeitschriften Verlag.</p>"
                          "<p>This program is free software: you can redistribute it and/or modify "
                          "it under the terms of the GNU General Public License as published by "
                          "the Free Software Foundation, either version 3 of the License, or "
                          "(at your option) any later version.</p>"
                          "<p>This program is distributed in the hope that it will be useful, "
                          "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                          "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
                          "GNU General Public License for more details.</p>"
                          "You should have received a copy of the GNU General Public License "
                          "along with this program. "
                          "If not, see <a href=\"http://www.gnu.org/licenses/gpl-3.0\">http://www.gnu.org/licenses</a>.</p>")
                       .arg(AppName).arg(AppUrl).arg(AppAuthor).arg(AppAuthorMail));
}


void MainWindow::aboutQt(void)
{
    QMessageBox::aboutQt(this);
}
