#ifndef IMAGEEDITFORM_H
#define IMAGEEDITFORM_H

#include <QDialog>
#include <QImage>
#include <QDebug>
#include <QMessageBox>

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

private:
    Ui::ImageEditForm *ui;
    QString pathToImage;
    QImage *image;

    void showImage(QString _pathToImage);
};

#endif // IMAGEEDITFORM_H
