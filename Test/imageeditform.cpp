#include "imageeditform.h"
#include "ui_imageeditform.h"
#include <QScrollArea>

ImageEditForm::ImageEditForm(const QString &_pathToImage, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageEditForm)
{
    ui->setupUi(this);
    imageHandler = new EditingImageHandler(_pathToImage);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    imageHandler->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(imageHandler);
    ui->imageLayout->addWidget(scrollArea);
    ui->imageLayout->setAlignment(scrollArea, Qt::AlignTop);
    imageHandler->showImage(imageHandler->scaleParameter);
}

ImageEditForm::~ImageEditForm()
{
    delete ui;
}

void ImageEditForm::on_greyscaleButton_clicked()
{
    imageHandler->makeGreyscale();
    imageHandler->showImage(imageHandler->scaleParameter);
}

void ImageEditForm::on_saveButton_clicked()
{
    QMessageBox* imageSaved = new QMessageBox;
    imageSaved->setWindowTitle("Изображение сохранено");
    imageSaved->setText("Изображение было успешно сохранено");
    imageSaved->setIcon(QMessageBox::Information);

    if (imageHandler->saveImage()) imageSaved->exec();
}

void ImageEditForm::on_increaseSaturation_clicked()
{
    imageHandler->increaseSaturation();
    imageHandler->showImage(imageHandler->scaleParameter);
}

void ImageEditForm::on_pushButton_clicked()
{
    imageHandler->rotateImage();
    imageHandler->showImage(imageHandler->scaleParameter);
}

void ImageEditForm::on_increaseButton_clicked()
{
    imageHandler->scaleImage(1);
}

void ImageEditForm::on_reduceButton_clicked()
{
    imageHandler->scaleImage(-1);
}
