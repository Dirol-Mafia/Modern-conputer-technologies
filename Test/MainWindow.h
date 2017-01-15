#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#endif // MAINWINDOW_H

#include "imageprovider.h"
#include "imageeditform.h"
#include "imageswithtags.h"
#include "imageviewer.h"

#include <QSqlDatabase>
#include <iostream>
#include <stdexcept>
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
#include <QToolBar>
#include <QLineEdit>
#include <QSqlQueryModel>
#include "QSql"
#include "QSqlDatabase"
#include <QSqlQuery>
#include <QSqlError>
#include <QApplication>
#include <QPlainTextEdit>
#include <QCheckBox>

#include <QVector>

class MyLabel;

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
    QSqlQueryModel *imagesModel;
    QTreeView *treeView;
    QListView *imagesView;
    QLabel *lb;
    QModelIndex currentParagraphIndex;
    QToolBar *toolbar;
    QLineEdit *searchInput;
    QPushButton *searchButton;

    QVBoxLayout *dataLayout;
    QVBoxLayout *imagesLayout;
    QHBoxLayout *mainLayout;

    QLabel* catComLabel;
    QLabel* imgComLabel;
    QLabel* imgTagLabel;

    ///Tree Editing layouts
    QFormLayout* editLayout;
    QFormLayout* addLayout;
    QGridLayout* pictureGrid;
    QVector<QFormLayout*> picLayouts;
    QVector<QFormLayout*> comLayouts;
    QVector<QFormLayout*> tagLayouts;
    QVector<QFormLayout*> butLayouts;
    QVector<MyLabel*> picLabels;

    ///Tree Editing widgets
    QWidget* addWindow;
    QWidget* editWindow;
    QWidget* editPicInfo;
    QWidget* deleteWindow;
    QWidget* editPicturesWin;
    QScrollArea* editPicScrollAlrea;

    ///Text Editing Forms
    QLineEdit* nameAdd;
    QPlainTextEdit* commentAdd;
    QPlainTextEdit* tagAdd;
    QLineEdit* nameEdit;
    QPlainTextEdit* commentEdit;
    QPlainTextEdit* tagEdit;
    QList<QTextEdit*> pictureComments;
    QList<QTextEdit*> pictureTags;

    ///Pictures paths
    QStringList picturePaths;

    ///Categories editing buttons
    QPushButton* newSemester;
    ///Picture editing buttons
    QPushButton *editButton;
    QPushButton *printButton;
    QPushButton *deleteButton;
    QPushButton *addButton;
    ///Vector of delete buttons (in the Picture adding window)
    QVector<QPushButton*> delButtons;
    QCheckBox *selectAllCheckBox;

    QAction *addCategory;
    QAction *addLecture;

    ///Editing actions
    QAction* actionAdd;
    QAction* actionEdit;
    QAction* actionDelete;

    QAction* actionAddLect;
    QAction* actionEditLect;
    QAction* actionDeleteLect;
    QAction* print;
    QAction* editPic;

    QPrinter printer;
    QPrintPreviewDialog *dialog;

    QPainter painter;    

    bool addSemester;
    int selectedImagesCount;
    QVector<bool> selectedImages;
    void getSelectedItems();

    void createActions();
    void createMenus();
    void addCategoryToDb();
    void renderImagesLayout();
    void renderCategoriesLayout();
    void renderToolbar();
    const DataWrapper* itemData();
    void setEnableButtons();
    void addNewSemester();
    void searchByTags();
    void noSuchTags();

/// Actions for context menu (dialog windows)
    //void addCategoryToDb();
    void addLectureToDb();
    void editCategory();
    void editPictureInfo();
    void emptyAction();
    void deleteAction();
    void areYouSure();
    void addingAction();

/// Actions for context menu (editing model)
    void add();
    void addPictures();
    void edit();
    void editInfo();
    void browsePic();
    void remove();
    void removePictures();
    void updateActions();
    void addLectures();

    void on_treeView_customContextMenuRequested();


private slots:
    void showImages(const QModelIndex &index);
    void callPrinter();
    void drawImagesOnSheet(QPrinter* printer);
    void callEditForm();
    void areYouSureDelPics();
    void showComments(const QModelIndex& index);
    void onImageClick();
    void onImageDoubleClick();
    void removePicFromSelection();
    void onSearchButtonClick();
    void lineEditKeyPressed();
    void selectAllImages();
};

class MyLabel : public QLabel
{
public:
    MyLabel(const QString p);
private:
    QString path;
protected:
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
};
