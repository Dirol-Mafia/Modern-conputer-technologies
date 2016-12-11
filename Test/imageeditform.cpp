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

void ImageEditForm::on_saveButton_clicked()
{
    QMessageBox* imageSaved = new QMessageBox;
    imageSaved->setWindowTitle("Изображение сохранено");
    imageSaved->setText("Изображение было успешно сохранено");
    imageSaved->setIcon(QMessageBox::Information);

    if (image->save(pathToImage)) imageSaved->exec();

}

void ImageEditForm::on_increaseSaturation_clicked()
{
    QImage tmp(pathToImage);
    int width = tmp.width();
    int heigth = tmp.height();
    double saturationLevel = 1;
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < heigth; ++j)
        {
            QColor color = tmp.pixel(i, j);
            int hue = color.hue();
            int saturation = color.saturation();
            color.setHsv(hue, saturation * saturationLevel, color.value(), color.alpha());
            tmp.setPixel(i, j, color.rgb());
        }

    delete image;
    image = new QImage(tmp);
    showImage(pathToImage);
}
