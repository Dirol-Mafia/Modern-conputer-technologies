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

    void on_checkAll_clicked();

private:
    void callPrinter();
    void getSelectedItems();
    Ui::ImagesWithTags *ui;
    QStringList paths;
    QVector <int> selectedItems;
};

#endif // IMAGESWITHTAGS_H
