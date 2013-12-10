// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __MAINWINDOW_H_
#define __MAINWINDOW_H_

#include <QtCore/QDebug>
#include <QMainWindow>
#include <QScopedPointer>

#include "imagewidget.h"

namespace Ui {
class MainWindow;
}

enum Algorithm {
    ALGORITHM_NONE = -1,
    ALGORITHM_ZERO = 0,
    ALGORITHM_ONE = 1,
    ALGORITHM_XOR
};

class MainWindowPrivate;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = NULL);
    virtual ~MainWindow();

private: // methods
    void saveSettings(void);
    void restoreSettings(void);

private: // variables
    Ui::MainWindow *ui;
    QScopedPointer<MainWindowPrivate> d_ptr;
    Q_DECLARE_PRIVATE(MainWindow)
    Q_DISABLE_COPY(MainWindow)

protected:
    void keyPressEvent(QKeyEvent*);
    void closeEvent(QCloseEvent*);

private slots:
    void setImage(const QImage&);
    bool openImage(const QString &filename);
    void openImage(void);
    void saveImageAs(void);
    void updateImageWidget(void);
    void positionChanged(int, int);
    void singleBitModeChanged(bool);
    void setAlgorithm(Algorithm a = ALGORITHM_NONE);
    void copyToClipboard(void);
    void pasteFromClipboard(void);
    void about(void);
    void aboutQt(void);
};

#endif // __MAINWINDOW_H_
