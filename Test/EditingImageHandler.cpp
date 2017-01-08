#include "EditingImageHandler.h"

EditingImageHandler::EditingImageHandler(const QString& _pathToImage, QWidget* parent)
    : QLabel(parent)
{
    image = new QImage(_pathToImage);
    pathToImage = _pathToImage;
    selectionStarted = false;
    this->showImage();
    QAction *cropAction = contextMenu.addAction("Crop");
    connect(cropAction,SIGNAL(triggered()),this,SLOT(crop()));
}

EditingImageHandler::~EditingImageHandler()
{
}

void EditingImageHandler::makeGreyscale()
{
    int width = image->width();
    int heigth = image->height();

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < heigth; ++j)
        {
            int gray = qGray(image->pixel(i, j));
            image->setPixel(i, j, QColor(gray, gray, gray).rgb());
        }
}

void EditingImageHandler::increaseSaturation()
{
    int width = image->width();
    int heigth = image->height();
    double brightnessLevel = 200;
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < heigth; ++j)
        {
            QColor color = image->pixel(i, j);
            int brightness = color.value();
            if (brightness > brightnessLevel) brightness = 255;
            if (brightness < 50)
            {
                brightness = 0;
            }
            color.setHsv(color.hue(), color.saturation(), brightness, color.alpha());
            image->setPixel(i, j, color.rgb());
        }
}

void EditingImageHandler::scaleImage(int multipier)
{
    scaleParameter += multipier * 0.2;
    showImage(scaleParameter);
}

void EditingImageHandler::showImage(double scaleParam)
{
    this->setFixedHeight(image->height() * scaleParam);
    this->setFixedWidth(image->width() * scaleParam);
    this->setScaledContents(true);
    this->setPixmap(QPixmap::fromImage(*image));
}

bool EditingImageHandler::saveImage()
{
    if (image->save(pathToImage)) return true;
    return false;
}

void EditingImageHandler::paintEvent(QPaintEvent *e)
{
    QLabel::paintEvent(e);
    if (selectionStarted)
    {
        painter = new QPainter(this);
        painter->setPen(QPen(QBrush(QColor(0,0,0,180)),1,Qt::DashLine));
        painter->setBrush(QBrush(QColor(255,255,255,120)));
        painter->drawRect(selectionRect);
    }

}

void EditingImageHandler::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        if (selectionRect.contains(event->pos())) contextMenu.exec(this->mapToGlobal(event->pos()));
    }
    else
    {
        selectionStarted=true;
        selectionRect.setTopLeft(event->pos());
        selectionRect.setBottomRight(event->pos());
    }
}

void EditingImageHandler::mouseMoveEvent(QMouseEvent *e)
{
    if (selectionStarted)
    {
        selectionRect.setBottomRight(e->pos());
        repaint();
    }
}

void EditingImageHandler::mouseReleaseEvent(QMouseEvent *e)
{
    selectionStarted=false;
}

void EditingImageHandler::crop()
{
    QPixmap tmp = this->pixmap()->copy(selectionRect);
    selectionStarted=false;
    delete image;
    QImage tmpImg = this->pixmap()->copy(selectionRect).toImage();
    image = new QImage(tmpImg);
    showImage();
}

void EditingImageHandler::rotateImage()
{
    QPixmap pixmap = *(this->pixmap());
    QMatrix rm;
    rm.rotate(90);
    pixmap = pixmap.transformed(rm);
    delete image;
    QImage tmpImg = pixmap.toImage();
    image = new QImage(tmpImg);
}
