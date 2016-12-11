#include "imageeditform.h"
#include "ui_imageeditform.h"

ImageEditForm::ImageEditForm(const QString &pathToImage, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageEditForm)
{
    ui->setupUi(this);
    QImage image(pathToImage);
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->setFixedHeight(image.height()*0.7);
    ui->label->setFixedWidth(image.width()*0.7);
    ui->label->adjustSize();
    ui->label->setScaledContents(true);
}

ImageEditForm::~ImageEditForm()
{
    delete ui;
}
