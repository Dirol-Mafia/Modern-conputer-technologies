#include "qtquick2controlsapplicationviewer.h"
#include <QSqlDatabase>
#include <iostream>
#include <QTreeView>
#include "imageprovider.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv);
    ImageProvider model("DB_Lectures", nullptr);
    QTreeView view;
    view.setModel(&model);
    view.show();

    /*QtQuick2ControlsApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/Test/main.qml"));
    viewer.show();*/

    return app.exec();
}
