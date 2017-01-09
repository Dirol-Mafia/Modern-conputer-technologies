#ifndef CLICKABLELABLE_H
#define CLICKABLELABLE_H

#endif // CLICKABLELABLE_H
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QAction>

class EditingImageHandler : public QLabel
{
Q_OBJECT
public:
    explicit EditingImageHandler( const QString& pathToImage, QWidget* parent=0 );
    ~EditingImageHandler();
    void showImage(double param = 0.7);
    void makeGreyscale();
    void increaseSaturation();
    bool saveImage();
    void rotateImage();
    void scaleImage(int multiplier);
    double scaleParameter = 0.7;
signals:
    void clicked();
private slots:
    void crop();
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
private:
    bool selectionStarted;
    QRect selectionRect;
    QMenu contextMenu;
    QString pathToImage;
    QImage *image;
    QPainter *painter;
};
