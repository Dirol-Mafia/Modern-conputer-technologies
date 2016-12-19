#ifndef IMAGEEDITFORM_H
#define IMAGEEDITFORM_H

#include <QDialog>
#include <QImage>
#include <QDebug>
#include <QMessageBox>
#include "EditingImageHandler.h"

namespace Ui {
class ImageEditForm;
}

class ImageEditForm : public QDialog
{
    Q_OBJECT

public:
     explicit ImageEditForm(const QString &pathToImage, QWidget *parent = 0);
    ~ImageEditForm();

private slots:
    void on_greyscaleButton_clicked();
    void on_saveButton_clicked();
    void on_increaseSaturation_clicked();
    void on_pushButton_clicked();

private:
    Ui::ImageEditForm *ui;
    EditingImageHandler *imageHandler;
};

#endif // IMAGEEDITFORM_H
