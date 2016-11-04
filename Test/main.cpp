#include "qtquick2controlsapplicationviewer.h"
#include <QSqlDatabase>
#include <iostream>
#include <QTreeView>
#include <QSqlQuery>
#include <QDebug>
#include "imageprovider.h"
#include <QStandardItemModel>
int main(int argc, char *argv[])
{
    /*QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("DB_Lectures");
    db.open();
    QSqlQuery query;
    bool ok = query.exec("SELECT * FROM huiznaetchto");
    query.next();
    qDebug() << query.executedQuery();
    std::cout << ok << std::endl;*/

    Application app(argc, argv);
    ImageProvider model("../Test/DB_Lectures");
    QTreeView view;
    view.setModel(&model);
    view.setWindowTitle(QObject::tr("Lectures"));
    view.show();

    /*QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/Test/main.qml"));
    viewer.show();*/

    return app.exec();
}
