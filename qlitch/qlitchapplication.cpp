#include "qlitchapplication.h"
#include <QtCore/QDebug>


QlitchApplication::QlitchApplication(int &argc, char *argv[])
    : QApplication(argc, argv)
{
    setOrganizationName(Company);
    setOrganizationDomain(Company);
    setApplicationName(Name);
    setApplicationVersion(VersionNoDebug);
}


QlitchApplication::~QlitchApplication()
{ /* ... */ }


const QString QlitchApplication::Company = "c't";
const QString QlitchApplication::Name = "Qlitch";
const QString QlitchApplication::Url = "https://github.com/607011/qlitch";
const QString QlitchApplication::Author = "Oliver Lau";
const QString QlitchApplication::AuthorMail = "ola@ct.de";
const QString QlitchApplication::VersionNoDebug = "1.0.2";
const QString QlitchApplication::MinorVersion = "";
#ifdef QT_NO_DEBUG
const QString QlitchApplication::Version = QlitchApplication::VersionNoDebug + QlitchApplication::MinorVersion;
#else
const QString QlitchApplication::Version = QlitchApplication::VersionNoDebug + QlitchApplication::MinorVersion + " [DEBUG]";
#endif
#if defined(_M_X64) || defined(_WIN64) || defined(__x86_64__)
const QString QlitchApplication::Platform = "x64";
#else
const QString QlitchApplication::Platform = "x86";
#endif
