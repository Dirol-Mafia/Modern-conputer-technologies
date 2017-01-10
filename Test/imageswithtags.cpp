#include "imageswithtags.h"
#include "ui_imageswithtags.h"

ImagesWithTags::ImagesWithTags(QStringList paths, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagesWithTags)
{
    ui->setupUi(this);

    QStandardItemModel *imagesModel = new QStandardItemModel;

    for (int i = 0; i < paths.size(); ++i)
    {
        QStandardItem *imageItem = new QStandardItem();
        imageItem->setCheckable(true);
        imageItem->setCheckState(Qt::Unchecked);
        QPixmap *pixmap = new QPixmap(paths[i]);
        *pixmap = pixmap->scaled(250, 250, Qt::KeepAspectRatio, Qt::FastTransformation);
        imageItem->setData(QVariant(*pixmap), Qt::DecorationRole);
        imagesModel->setItem(i, 0, imageItem);
        delete pixmap;
    }
    ui->tableView->setModel(imagesModel);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();
}

ImagesWithTags::~ImagesWithTags()
{
    delete ui;
}
