// Copyright (c) 2013 Oliver Lau <ola@ct.de>, Heise Zeitschriften Verlag
// All rights reserved.

#ifndef __QLITCHAPPLICATION_H_
#define __QLITCHAPPLICATION_H_

#include <QApplication>
#include <QEvent>
#include <QTranslator>
#include <QLocale>
#include <QString>


class QlitchApplication : public QApplication
{
public:
    explicit QlitchApplication(int &argc, char *argv[]);
    virtual ~QlitchApplication();

public:
    static const QString Company;
    static const QString Name;
    static const QString Url;
    static const QString Author;
    static const QString AuthorMail;
    static const QString Version;
    static const QString MinorVersion;
    static const QString VersionNoDebug;
    static const QString Platform;
};

#endif // __QLITCHAPPLICATION_H_
