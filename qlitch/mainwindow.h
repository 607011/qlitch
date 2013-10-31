// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QDebug>
#include <QMainWindow>
#include <QByteArray>
#include <QScopedPointer>
#include <QFile>

#include "imagewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum Algorithm {
        ALGORITHM_ZERO,
        ALGORITHM_ONE,
        ALGORITHM_XOR
    };


private:
    Ui::MainWindow *ui;
    ImageWidget *imageWidget;
    QImage mImage;

    Algorithm mAlgorithm;

protected:
    void keyPressEvent(QKeyEvent*);

private slots:
    void setImage(const QImage&);
    void openImage(void);
    void saveImageAs(void);
    void updateImageWidget(void);
    void setAlgorithm(void);
    void about(void);
    void aboutQt(void);
};

#endif // MAINWINDOW_H
