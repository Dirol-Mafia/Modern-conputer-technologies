#include "qtquick2controlsapplicationviewer.h"
#include <QSqlDatabase>
#include <iostream>

int main(int argc, char *argv[])
{
    Application app(argc, argv);

   /* QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("DB_Lectures");

    if (db.open())
      std::cout << "Opened!" << std::endl;*/

    QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/Test/main.qml"));
    viewer.show();

    return app.exec();
}
