#include "imageeditform.h"
#include "ui_imageeditform.h"

ImageEditForm::ImageEditForm(const QString &_pathToImage, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageEditForm)
{
    ui->setupUi(this);
    image = new QImage(_pathToImage);
    pathToImage = _pathToImage;
    showImage(pathToImage);
}

ImageEditForm::~ImageEditForm()
{
    delete ui;
}

void ImageEditForm::showImage(QString _pathToImage)
{
    ui->label->setPixmap(QPixmap::fromImage(*image));
    ui->label->setFixedHeight(image->height()*0.7);
    ui->label->setFixedWidth(image->width()*0.7);
    ui->label->setScaledContents(true);
}

void ImageEditForm::on_greyscaleButton_clicked()
{
    QImage tmp(pathToImage);
    int width = tmp.width();
    int heigth = tmp.height();

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < heigth; ++j)
        {
            int gray = qGray(tmp.pixel(i, j));
            tmp.setPixel(i, j, QColor(gray, gray, gray).rgb());
        }

    delete image;
    image = new QImage(tmp);
    showImage(pathToImage);
}
