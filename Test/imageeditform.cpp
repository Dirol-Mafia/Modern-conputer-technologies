#include "imageeditform.h"
#include "ui_imageeditform.h"

ImageEditForm::ImageEditForm(const QString &_pathToImage, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageEditForm)
{
    ui->setupUi(this);
    imageHandler = new EditingImageHandler(_pathToImage);
    ui->imageLayout->addWidget(imageHandler);
    imageHandler->showImage();
}

ImageEditForm::~ImageEditForm()
{
    delete ui;
}

void ImageEditForm::on_greyscaleButton_clicked()
{
    imageHandler->makeGreyscale();
    imageHandler->showImage();
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
    imageHandler->showImage();
}
