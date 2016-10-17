#include "qtquick2controlsapplicationviewer.h"
#include "imageprovider.h"
#include <QSqlDatabase>
#include <iostream>
#include <QTreeView>

int main(int argc, char *argv[])
{
    Application app(argc, argv);

   /* QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("DB_Lectures");

    if (db.open())
      std::cout << "Opened!" << std::endl;*/

    /*QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/Test/main.qml"));
    viewer.show();*/

    QTreeView tree;

    ImageProvider ip("DB_Lections", nullptr);

    tree.setModel(&ip);
    tree.show();

    return app.exec();
}
