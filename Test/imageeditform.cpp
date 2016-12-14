#include "imageeditform.h"
#include "ui_imageeditform.h"

ImageEditForm::ImageEditForm(const QString &_pathToImage, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageEditForm)
{
    ui->setupUi(this);
    image = new QImage(_pathToImage);
    pathToImage = _pathToImage;
    showImage();
}

ImageEditForm::~ImageEditForm()
{
    delete ui;
}

void ImageEditForm::showImage()
{
    ui->label->setPixmap(QPixmap::fromImage(*image));
    ui->label->setFixedHeight(image->height()*0.7);
    ui->label->setFixedWidth(image->width()*0.7);
    ui->label->setScaledContents(true);
}

void ImageEditForm::on_greyscaleButton_clicked()
{
    int width = image->width();
    int heigth = image->height();

    for (int i = 0; i < width; ++i)
        for (int j = 0; j < heigth; ++j)
        {
            int gray = qGray(image->pixel(i, j));
            image->setPixel(i, j, QColor(gray, gray, gray).rgb());
        }

    showImage();
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

    showImage();
}
