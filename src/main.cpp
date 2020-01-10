#include <QApplication>

#include "../../lsMisc/stdQt/ambapp.h"

#include "consts.h"
#include "mainwindow.h"

using namespace Consts;
using namespace AmbiesoftQt;

bool processCommandLine()
{
    return true;
}
int main2(AmbApp& theApp)
{
    QScopedPointer<IniSettings> settings;
    if(!theApp.InitApplication(&settings))
        return 1;

    MainWindow w(*settings);
    w.show();

    return theApp.exec();
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION);
    QCoreApplication::setOrganizationDomain(APPDOMAIN);
    QCoreApplication::setApplicationName(APPNAME);
    QCoreApplication::setApplicationVersion(APPVERSION);

    AmbApp::AmbAppArgs args;
    args.keyLanguage = KEY_LANGUAGE;
    args.keyStyle = KEY_STYLE;
    args.singleInstance = false;
    AmbApp app(argc, argv, args);

#ifdef QT_NO_DEBUG
    if ( !app.arguments().contains(QLatin1String("--with-debug") ))
    {
        qInstallMessageHandler(noMessageOutput);
    }
#endif

    if(!processCommandLine())
        return 0;

    int ret = main2(app);
    return ret;
}
