#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include "imageprovider.h"

#include <QMainWindow>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QMenuBar>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>

class MyScrollArea;

class ImageViewer : public QMainWindow
{
  Q_OBJECT
public:
  ImageViewer();
  ImageViewer(int cur_pic, DataWrapper *pic_par);

  void setPicture(int number);
  void delegateEvent(QKeyEvent* event);

signals:

private:
    int current_picture = -1;
    DataWrapper* picture_parent;
    QLabel* imageLabel;
    MyScrollArea* scrollArea;
    double scaleFactor;

    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;

    QMenuBar* customMenuBar;
    QMenu *viewMenu;

    void createActions();
    void createMenus();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

private slots:
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();

protected:
    virtual void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;

};

class MyScrollArea : public QScrollArea
{
public:
    MyScrollArea(ImageViewer* viewer): parent(viewer) {}

protected:
    virtual void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

private:
    ImageViewer* parent;
};

#endif // IMAGEVIEWER_H
