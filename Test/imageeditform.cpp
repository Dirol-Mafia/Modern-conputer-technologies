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
    double brightnessLevel = 200;
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < heigth; ++j)
        {
            QColor color = tmp.pixel(i, j);
            int brightness = color.value();
            if (brightness > brightnessLevel) brightness = 255;
            if (brightness < 50)
            {
                brightness = 0;
            }
            color.setHsv(color.hue(), color.saturation(), brightness, color.alpha());
            tmp.setPixel(i, j, color.rgb());
        }

    delete image;
    image = new QImage(tmp);
    showImage(pathToImage);
}
