#include "imageviewer.h"

ImageViewer::ImageViewer()
{  
    current_picture = -1;
    picture_parent = nullptr;
    picture_path = "";

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea = new MyScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();

    setWindowTitle("Просмотр");
    resize(500, 400);

    //setFocusPolicy(Qt::NoFocus);
}

ImageViewer::ImageViewer(int cur_pic, const DataWrapper *pic_par, QString path)
{
    current_picture = cur_pic;
    picture_parent = pic_par;
    picture_path = path;

    QString filename;

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    if (picture_path == "")
    {
        IData* picture_data = static_cast<IData*>(picture_parent->children.at(current_picture)->data);
        filename = picture_data->path;
        QString comm = picture_data->comment;
        if (comm == "")
            comm = "-";
        setWindowTitle(comm);
    }
    else
    {
        filename = picture_path;
        setWindowTitle(filename);
    }
    QImage image(filename);

    if (image.isNull())
    {
        QMessageBox::information(this, tr("Ошибка"), tr("Невозможно отобразить изображение %1.").arg(filename));
        return;
    }
    imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;

    scrollArea = new MyScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    setCentralWidget(scrollArea);

    createActions();
    createMenus();

    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();

    resize(600, 500);
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ImageViewer::setPicture(int number)
{
    if (number > picture_parent->count)
        return;
    current_picture = number;

    IData* picture_data = static_cast<IData*>(picture_parent->children.at(current_picture)->data);

    QString filename = picture_data->path;
    QImage image(filename);
    if (image.isNull())
    {
        QMessageBox::information(this, tr("Ошибка"), tr("Невозможно отобразить изображение %1.").arg(filename));
        return;
    }

    imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;

    if (!fitToWindowAct->isChecked())
        imageLabel->resize(imageLabel->pixmap()->size());

    QString comm = picture_data->comment;
    if (comm == "")
        comm = "-";
    setWindowTitle(comm);
}

void ImageViewer::zoomIn()
{
    scaleImage(1.25);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.8);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow) {
        normalSize();
    }
    updateActions();
}

void ImageViewer::createActions()
{
    zoomInAct = new QAction(tr("Увеличить (25%)"), this);
    zoomInAct->setShortcut(tr("Ctrl++"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(zoomIn()));

    zoomOutAct = new QAction(tr("Уменьшить (25%)"), this);
    zoomOutAct->setShortcut(tr("Ctrl+-"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

    normalSizeAct = new QAction(tr("&Обычный размер"), this);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&По размеру окна"), this);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    addAction(zoomInAct);
    addAction(zoomOutAct);
    addAction(normalSizeAct);
    addAction(fitToWindowAct);
}

void ImageViewer::createMenus()
{
    viewMenu = new QMenu(tr("&Параметры просмотра"), this);
    viewMenu->addAction(zoomInAct);
    viewMenu->addAction(zoomOutAct);
    viewMenu->addAction(normalSizeAct);
    viewMenu->addSeparator();
    viewMenu->addAction(fitToWindowAct);

    menuBar()->addMenu(viewMenu);
}

void ImageViewer::updateActions()
{
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageViewer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right)
    {
        if (picture_parent == nullptr || current_picture == picture_parent->count - 1)
          return;
        ++current_picture;
        setPicture(current_picture);
    }

    if (event->key() == Qt::Key_Left)
    {
        if (picture_parent == nullptr || current_picture == 0)
          return;
        --current_picture;
        setPicture(current_picture);
    }
}

void ImageViewer::delegateEvent(QKeyEvent* event)
{
    keyPressEvent(event);
}

void MyScrollArea::keyPressEvent(QKeyEvent *event)
{
    parent->delegateEvent(event);
}
