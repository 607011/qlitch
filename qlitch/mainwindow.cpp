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
#include <QSettings>


class MainWindowPrivate {
public:
    explicit MainWindowPrivate(void)
        : algorithm(MainWindow::Algorithm::ALGORITHM_XOR)
        , imageWidget(new ImageWidget)

    { /* ... */ }
    ~MainWindowPrivate()
    {
        delete imageWidget;
    }
    ImageWidget *imageWidget;
    MainWindow::Algorithm algorithm;
    QImage image;
    QString imageFilename;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , d_ptr(new MainWindowPrivate)
{
    ui->setupUi(this);
    setWindowTitle(tr("%1 %2").arg(AppName).arg(AppVersion));
    ui->verticalLayout->addWidget(d_ptr->imageWidget);
    QObject::connect(ui->actionOpenImage, SIGNAL(triggered()), SLOT(openImage()));
    QObject::connect(ui->actionSaveImageAs, SIGNAL(triggered()), SLOT(saveImageAs()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), SLOT(about()));
    QObject::connect(ui->actionAboutQt, SIGNAL(triggered()), SLOT(aboutQt()));
    QObject::connect(ui->qualitySlider, SIGNAL(valueChanged(int)), SLOT(updateImageWidget()));
    QObject::connect(ui->percentageSlider, SIGNAL(valueChanged(int)), SLOT(updateImageWidget()));
    QObject::connect(ui->iterationsSlider, SIGNAL(valueChanged(int)), SLOT(updateImageWidget()));
    QObject::connect(d_ptr->imageWidget, SIGNAL(imageDropped(QImage)), SLOT(setImage(QImage)));
    QObject::connect(d_ptr->imageWidget, SIGNAL(refresh()), SLOT(updateImageWidget()));
    ui->actionOne->setData(ALGORITHM_ONE);
    QObject::connect(ui->actionOne, SIGNAL(triggered()), SLOT(setAlgorithm()));
    ui->actionZero->setData(ALGORITHM_ZERO);
    QObject::connect(ui->actionZero, SIGNAL(triggered()), SLOT(setAlgorithm()));
    ui->actionXOR->setData(ALGORITHM_XOR);
    QObject::connect(ui->actionXOR, SIGNAL(triggered()), SLOT(setAlgorithm()));
    RAND::initialize();
    restoreSettings();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::restoreSettings(void)
{
    Q_D(MainWindow);
    QSettings settings(Company, AppName);
    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    setAlgorithm((Algorithm)settings.value("Options/algorithm", ALGORITHM_ONE).toInt());
    d->imageFilename = settings.value("Options/recentImageFilename", "qrc:/images/default.jpg").toString();
    if (!d->imageFilename.isEmpty())
        openImage(d->imageFilename);
    ui->percentageSlider->setValue(settings.value("Options/percent", 70).toInt());
    ui->iterationsSlider->setValue(settings.value("Options/iterations", 2).toInt());
    ui->qualitySlider->setValue(settings.value("Options/quality", 50).toInt());
}


void MainWindow::saveSettings(void)
{
    Q_D(MainWindow);
    QSettings settings(Company, AppName);
    settings.setValue("MainWindow/geometry", saveGeometry());
    settings.setValue("Options/algorithm", d->algorithm);
    settings.setValue("Options/recentImageFilename", d->imageFilename);
    settings.setValue("Options/percent", ui->percentageSlider->value());
    settings.setValue("Options/iterations", ui->iterationsSlider->value());
    settings.setValue("Options/quality", ui->qualitySlider->value());
}


void MainWindow::closeEvent(QCloseEvent *)
{
    saveSettings();
}


void MainWindow::keyPressEvent(QKeyEvent *e)
{
    e->ignore();
}


void MainWindow::updateImageWidget(void)
{
    Q_D(MainWindow);
    if (d->image.isNull())
        return;
    qDebug() << "updateImageWidget()";
    QByteArray raw;
    QBuffer buffer(&raw);
    buffer.open(QIODevice::WriteOnly);
    d->image.save(&buffer, "JPG", ui->qualitySlider->value());
    int firstPos = (long)(1e-2 * raw.size() *  ui->percentageSlider->value());
    const int N = ui->iterationsSlider->value();
    for (int i = 0; i < N; ++i) {
        int pos = RAND::rnd(firstPos, raw.size());
        int bit = 1 << (RAND::rnd() % 8);
        switch (d->algorithm) {
        default:
          // fall-through
        case ALGORITHM_XOR:
          raw[pos] = raw.at(pos) ^ bit;
          break;
        case ALGORITHM_ONE:
          raw[pos] = raw.at(pos) | bit;
          break;
        case ALGORITHM_ZERO:
          raw[pos] = raw.at(pos) & ~bit;
          break;
        }
    }
    ui->statusBar->showMessage(tr("Resulting image size: %1 bytes").arg(raw.size()), 3000);
    buffer.close();
    d->imageWidget->setRaw(raw);
}


void MainWindow::setAlgorithm(Algorithm a)
{
    Q_D(MainWindow);
    QAction *action = reinterpret_cast<QAction*>(sender());
    if (action) {
        d->algorithm = (Algorithm)action->data().toInt();
    }
    else {
        d->algorithm = a;
    }
    ui->actionZero->setChecked(false);
    ui->actionOne->setChecked(false);
    ui->actionXOR->setChecked(false);
    switch (d->algorithm) {
    case ALGORITHM_ONE:
        ui->actionOne->setChecked(true);
        break;
    case ALGORITHM_ZERO:
        ui->actionZero->setChecked(true);
        break;
    case ALGORITHM_XOR:
        ui->actionXOR->setChecked(true);
        break;
    case ALGORITHM_NONE:
        // fall-through
    default:
        break;
    }
    ui->statusBar->showMessage(tr("Algorithm: %1").arg(d->algorithm), 1000);
    updateImageWidget();
}


void MainWindow::setImage(const QImage &img)
{
    Q_D(MainWindow);
    d->image = img;
    updateImageWidget();
}


void MainWindow::openImage(const QString &filename)
{
    Q_D(MainWindow);
    qDebug() << "MainWindow::openImage(" << filename << ")";
    d->image.load(filename);
    if (d->image.isNull())
        return;
    updateImageWidget();
}


void MainWindow::openImage(void)
{
    Q_D(MainWindow);
    const QString &imgFileName = QFileDialog::getOpenFileName(this, tr("Open image ..."));
    if (imgFileName.isEmpty())
        return;
    d->imageFilename = imgFileName;
    openImage(d->imageFilename);
}


void MainWindow::saveImageAs(void)
{
    Q_D(MainWindow);
    const QString &imgFileName = QFileDialog::getSaveFileName(this, tr("Save image as ..."));
    if (imgFileName.isEmpty())
        return;
    d->imageWidget->image().save(imgFileName);
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
