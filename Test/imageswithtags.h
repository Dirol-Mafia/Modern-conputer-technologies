#ifndef IMAGESWITHTAGS_H
#define IMAGESWITHTAGS_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QDebug>
#include <QVector>
#include <imageeditform.h>
#include "config.h"

namespace Ui {
class ImagesWithTags;
}

class ImagesWithTags : public QWidget
{
    Q_OBJECT

public:
    explicit ImagesWithTags(QStringList paths, QWidget *parent = 0);
    ~ImagesWithTags();

private slots:
    void on_printButton_clicked();
    void drawImagesOnSheet(QPrinter* printer);
    void on_editButton_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_selectAllCheckBox_toggled(bool checked);

private:
    void callPrinter();
    void getSelectedItems();
    void showImages(QStringList paths, bool checked);
    void setButtonsEnabled();
    Ui::ImagesWithTags *ui;
    QStringList paths;
    QVector <int> selectedItems;
    Config config;
};

#endif // IMAGESWITHTAGS_H
