#ifndef IMAGESWITHTAGS_H
#define IMAGESWITHTAGS_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QStandardItem>
#include <QStandardItemModel>

namespace Ui {
class ImagesWithTags;
}

class ImagesWithTags : public QWidget
{
    Q_OBJECT

public:
    explicit ImagesWithTags(QStringList paths, QWidget *parent = 0);
    ~ImagesWithTags();

private:
    Ui::ImagesWithTags *ui;
};

#endif // IMAGESWITHTAGS_H
