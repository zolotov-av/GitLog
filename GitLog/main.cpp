#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngineExtensionPlugin>
#include "LogWindow.h"
#include "DBusInterface.h"
#include <GitApplication.h>

Q_IMPORT_QML_PLUGIN(GitCorePlugin)

using namespace git;

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    GitApplication gitApp;

    DBusInterface dbus;

    if ( dbus.isRunning() )
    {
        printf("already running...\n");
        dbus.showGitLog();
        return 0;
    }

    LogWindow w;

    QObject::connect(&dbus, &DBusInterface::showGitLogRequested, &w, &LogWindow::showWindowRequested, Qt::DirectConnection);
    QObject::connect(&dbus, &DBusInterface::exitRequested, &w, &LogWindow::exit, Qt::DirectConnection);

    if ( !dbus.registerObject() )
    {
        printf("cannot register dbus service...");
        return 1;
    }

    return a.exec();
}
