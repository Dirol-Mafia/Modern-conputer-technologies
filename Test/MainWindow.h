#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#endif // MAINWINDOW_H

#include <QSqlDatabase>
#include <iostream>
#include <QTreeView>
#include <QSqlQuery>
#include <QDebug>
#include "imageprovider.h"
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QDialog>
#include<QMainWindow>
#include <QLabel>
#include <QAction>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QListView>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
private:
    QWidget *window;
    QMenuBar *menuBar;
    QMenu *menuEdit;
    QMenu *menuAdd;
    ImageProvider *model;
    QTreeView *treeView;
    QLabel *lb;
    QVBoxLayout *dataLayout;
    QVBoxLayout *menuLayout;
    QVBoxLayout *mainLayout;
    QAction *addCategory;
    QAction *addLecture;
    void createActions();
    void createMenus();
    void addCategoryToDb();
    void addLectureToDb();
};
