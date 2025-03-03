#include "mainwindow.h"
#include "qlitchapplication.h"

int main(int argc, char *argv[])
{
    QlitchApplication a(argc, argv);

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
