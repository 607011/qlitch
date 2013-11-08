// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#include "main.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QString>

const QString Company = "c't";
const QString AppName = "Qlitch";
const QString AppUrl = "http://qlitch.googlecode.com/";
const QString AppAuthor = "Oliver Lau";
const QString AppAuthorMail = "ola@ct.de";
const QString AppVersionNoDebug = "1.0";
const QString AppMinorVersion = "";
#ifdef QT_NO_DEBUG
const QString AppVersion = AppVersionNoDebug + AppMinorVersion;
#else
const QString AppVersion = AppVersionNoDebug + AppMinorVersion + " [DEBUG]";
#endif
#ifdef _M_X64
const QString AppPlatform = "x64";
#else
const QString AppPlatform = "x86";
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(Company);
    a.setOrganizationDomain(Company);
    a.setApplicationName(AppName);
    a.setApplicationVersion(AppVersionNoDebug);
    qApp->addLibraryPath("plugins");
    qApp->addLibraryPath("./plugins");
    qApp->addLibraryPath(".");

#ifdef Q_OS_MAC
    qApp->addLibraryPath("../plugins");
#endif

    QTranslator translator;
    bool ok = translator.load(":/translations/qlitch_" + QLocale::system().name());
#ifndef QT_NO_DEBUG
    if (!ok)
        qWarning() << "Could not load translations for" << QLocale::system().name() << "locale";
#endif
    if (ok)
        a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
