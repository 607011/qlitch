// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include "main.h"
#include "mainwindow.h"
#include <QApplication>
#include <QString>

const QString Company = "c't";
const QString AppName = "Qlitch";
const QString AppUrl = "http://qlitch.googlecode.com/";
const QString AppAuthor = "Oliver Lau";
const QString AppAuthorMail = "ola@ct.de";
const QString AppVersionNoDebug = "1.0";
const QString AppMinorVersion = ".0";
#ifdef QT_NO_DEBUG
const QString AppVersion = AppVersionNoDebug + AppMinorVersion;
#else
const QString AppVersion = AppVersionNoDebug + AppMinorVersion + " [DEBUG]";
#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
