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

class ImageViewer : public QMainWindow
{
  Q_OBJECT
public:
  ImageViewer();
  ImageViewer(int cur_pic, DataWrapper *pic_par);

signals:

private:
    int current_picture = -1;
    DataWrapper* picture_parent;
    QLabel* imageLabel;
    QScrollArea* scrollArea;
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

};

#endif // IMAGEVIEWER_H
