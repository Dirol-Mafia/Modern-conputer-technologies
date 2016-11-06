#include "qtquick2controlsapplicationviewer.h"
#include <QSqlDatabase>
#include <iostream>
#include <QTreeView>
#include <QSqlQuery>
#include <QDebug>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <MainWindow.h>

int main(int argc, char *argv[])
{

    Application app(argc, argv);

    MainWindow *window = new MainWindow();
    window->show();

    /*QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/Test/main.qml"));
    viewer.show();*/

    return app.exec();
}
