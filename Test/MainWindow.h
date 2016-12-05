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
#include <QMainWindow>
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
#include <QTableView>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QImage>
#include <QRect>
#include <QStringList>
#include <QPrintPreviewDialog>

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = 0);
private:
    Q_OBJECT
    QWidget *window;
    QMenuBar *menuBar;
    QMenu *menuEdit;
    QMenu *menuAdd;
    ImageProvider *model;
    MySortFilterProxyModel *filteredModel;
    QTreeView *treeView;
    QListView *imagesView;
    QLabel *lb;
    QModelIndex currentParagraphIndex;

    QVBoxLayout *dataLayout;
    QVBoxLayout *imagesLayout;
    QHBoxLayout *mainLayout;

    QPushButton *editButton;
    QPushButton *printButton;

    QAction *addCategory;
    QAction *addLecture;

    QPrinter printer;
    QPrintPreviewDialog *dialog;

    QPainter painter;

    int selectedItemsCount;

    void createActions();
    void createMenus();
    void renderImagesLayout();
    void renderCategoriesLayout();
    const DataWrapper* itemData();

    void addCategoryToDb();
    void addLectureToDb();
    void editCategory();
    void editLecture();
    void emptyAction();
    void deleteAction();
    void areYouSure();
    void addingAction();

    void on_treeView_customContextMenuRequested();


private slots:
    void showImages(const QModelIndex &index);
    void setEnableButtons();
    void callPrinter();
    void drawImagesOnSheet(QPrinter* printer);
};
