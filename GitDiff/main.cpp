#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngineExtensionPlugin>
#include <GitTools/GitApplication.h>
#include <git2.h>
#include "GitDiff.h"

Q_IMPORT_QML_PLUGIN(GitCorePlugin)

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);
    GitApplication gitApp;

    GitDiff view;

    return a.exec();
}
