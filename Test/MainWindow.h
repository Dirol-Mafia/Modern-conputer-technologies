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
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QActionEvent>
#include <QListView>
#include <QSortFilterProxyModel>

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
    MySortFilterProxyModel *filteredModel;
    QTreeView *treeView;
    QListView *listView;
    QLabel *lb;
    QVBoxLayout *dataLayout;
    QVBoxLayout *imagesLayout;
    QHBoxLayout *mainLayout;
    QPushButton *editButton;
    QPushButton *printButton;
    QAction *addCategory;
    QAction *addLecture;
    void createActions();
    void createMenus();

    void on_treeView_customContextMenuRequested();

    void addCategoryToDb();
    void addLectureToDb();
    void editCategory();
    void editLecture();

private slots:
    void showImages(const QModelIndex &index);
};