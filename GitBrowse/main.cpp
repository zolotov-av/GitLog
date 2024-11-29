#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngineExtensionPlugin>
#include <QQmlContext>
#include <GitApplication.h>
#include <awCore/thread.h>
#include <awCore/trace.h>

Q_IMPORT_QML_PLUGIN(GitCorePlugin)

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    aw::setCurrentThreadName("main");
    aw::setLogStream(stderr);
    const aw::trace fn("main");

    GitApplication gitApp;
    QQmlApplicationEngine engine;

    constexpr auto pathQtQml = "qrc:/qt/qml";
    if ( !engine.importPathList().contains(pathQtQml) )
    {
        // Отладочная информация
        qWarning() << "Import paths:" << engine.importPathList();
        qWarning() << "Add path: " << pathQtQml;
        engine.addImportPath(pathQtQml);
        qWarning() << "New import paths:" << engine.importPathList();
    }

    engine.load(QUrl{"qrc:/Browse/GitBrowse.qml"});

    return a.exec();
}
