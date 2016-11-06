#include "qtquick2controlsapplicationviewer.h"
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
