#include "imageswithtags.h"
#include "ui_imageswithtags.h"

ImagesWithTags::ImagesWithTags(QStringList _paths, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImagesWithTags)
{
    ui->setupUi(this);
    paths = _paths;
    QStandardItemModel *imagesModel = new QStandardItemModel;
    ui->editButton->setEnabled(false);
    ui->printButton->setEnabled(false);

    for (int i = 0; i < paths.size(); ++i)
    {
        QPixmap *pixmap = new QPixmap(paths[i]);
        *pixmap = pixmap->scaled(250, 250, Qt::KeepAspectRatio, Qt::FastTransformation);
        QStandardItem *imageItem = new QStandardItem();
        imageItem->setCheckable(true);
        imageItem->setCheckState(Qt::Unchecked);
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

void ImagesWithTags::callPrinter()
{
    QPrinter *printer = new QPrinter;
    printer->setOrientation(QPrinter::Portrait);
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(printer);
    connect(dialog, SIGNAL(paintRequested(QPrinter*)), this, SLOT(drawImagesOnSheet(QPrinter*)));
    dialog->exec();
}

void ImagesWithTags::getSelectedItems()
{
    selectedItems.clear();
    for (int i = 0; i < paths.size(); ++i)
    {
        QModelIndex index = ui->tableView->model()->index(i,0);
        if (index.data(Qt::CheckStateRole) == Qt::Checked)
            selectedItems.push_back(i);
    }
}

void ImagesWithTags::drawImagesOnSheet(QPrinter* printer)
{
    QPainter painter;
    painter.begin(printer);

    getSelectedItems();

    int pagesAmount = selectedItems.size() - 1;
    int currentPage = 0;
    for (int i = 0; i <= pagesAmount; ++i)
    {
        int indexOfPath = selectedItems[i];
        QImage currentImage(paths[indexOfPath]);
        QImage scaledImage = currentImage.scaled(640, 640, Qt::KeepAspectRatio);
        QRect rect(scaledImage.rect());
        QRect devRect(0, 0, painter.device()->width(), painter.device()->height());
        rect.moveCenter(devRect.center());
        painter.drawImage(rect.topLeft(), scaledImage);
        if (currentPage != pagesAmount) printer->newPage();
        ++currentPage;
    }
    painter.end();
}

void ImagesWithTags::on_printButton_clicked()
{
    callPrinter();
}

void ImagesWithTags::on_editButton_clicked()
{
    getSelectedItems();
    QString imagePath = paths[selectedItems[0]];
    ImageEditForm *editForm = new ImageEditForm(imagePath);
    editForm->show();
}

void ImagesWithTags::on_tableView_clicked(const QModelIndex &index)
{
    getSelectedItems();
    ui->editButton->setEnabled(selectedItems.size() == 1);
    ui->printButton->setEnabled(selectedItems.size() > 0);
}
