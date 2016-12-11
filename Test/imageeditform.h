#ifndef IMAGEEDITFORM_H
#define IMAGEEDITFORM_H

#include <QDialog>
#include <QImage>

namespace Ui {
class ImageEditForm;
}

class ImageEditForm : public QDialog
{
    Q_OBJECT

public:
     explicit ImageEditForm(const QString &pathToImage, QWidget *parent = 0);
    ~ImageEditForm();

private:
    Ui::ImageEditForm *ui;
};

#endif // IMAGEEDITFORM_H
