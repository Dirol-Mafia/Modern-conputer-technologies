#include <MainWindow.h>

MainWindow::MainWindow(QWidget *parent) : QDialog(parent)
{
    menuBar = new QMenuBar(this);
    menuEdit = new QMenu("Edit");
    menuAdd = new QMenu("Add");
    menuEdit->addAction("test");
    menuEdit->addAction("test2");
    menuAdd->addAction("test");
    menuAdd->addAction("test2");
    menuBar->addMenu(menuEdit);
    menuBar->addMenu(menuAdd);
    menuBar->setNativeMenuBar(false);
    menuLayout = new QVBoxLayout;

    menuLayout->setMenuBar(menuBar);

    model = new ImageProvider("../Test/DB_Lectures");
    view = new QTreeView;
    view->setModel(model);
    view->setFixedSize(320, 320);

    dataLayout = new QVBoxLayout;
    dataLayout->addWidget(view);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(menuLayout);
    mainLayout->addLayout(dataLayout);

    setFixedSize(512, 512);
    setLayout(mainLayout);
}
