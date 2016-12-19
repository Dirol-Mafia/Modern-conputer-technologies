#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#endif // MAINWINDOW_H

#include "imageprovider.h"
#include "imageeditform.h"

#include <QSqlDatabase>
#include <iostream>
#include <QTreeView>
#include <QSqlQuery>
#include <QDebug>
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
#include <QFileDialog>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTextEdit>

#include <QVector>

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

    //Tree Editing layouts
    QFormLayout* editLayout;
    QFormLayout* addLayout;

    //Tree Editing widgets
    QWidget* addWindow;
    QWidget* editWindow;
    QWidget* deleteWindow;

    //Line Editing Forms
    QLineEdit* nameAdd;
    QLineEdit* commentAdd;
    QLineEdit* tagAdd;
    QLineEdit* nameEdit;
    QLineEdit* commentEdit;
    QLineEdit* tagEdit;
    QList<QTextEdit*> pictureComments;
    QList<QTextEdit*> pictureTags;

    //Pictures paths
    QStringList picturePaths;

    QPushButton *editButton;
    QPushButton *printButton;

    QAction *addCategory;
    QAction *addLecture;

    //Editing actions
    QAction* actionAdd;
    QAction* actionEdit;
    QAction* actionDelete;

    QPrinter printer;
    QPrintPreviewDialog *dialog;

    QPainter painter;    

    int selectedImagesCount;
    QVector<bool> selectedImages;

    void createActions();
    void createMenus();
    void addCategoryToDb();
    void renderImagesLayout();
    void renderCategoriesLayout();
    const DataWrapper* itemData();
    void setEnableButtons();

// Actions for context menu (dialog windows)void addCategoryToDb();
    void addLectureToDb();
    void editCategory();
    void emptyAction();
    void deleteAction();
    void areYouSure();
    void addingAction();

// Actions for context menu (editing model)
    void add();
    void edit();
    void remove();
    void updateActions();
    void addLectures();

    void on_treeView_customContextMenuRequested();


private slots:
    void showImages(const QModelIndex &index);
    void callPrinter();
    void drawImagesOnSheet(QPrinter* printer);
    void callEditForm();
    void onImageClick();
};
