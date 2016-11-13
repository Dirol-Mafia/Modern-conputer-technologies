#include "imageprovider.h"
#include <stdexcept>
#include "QSql"
#include "QSqlDatabase"
#include <QSqlQuery>
#include <QDebug>
#include <QString>

using namespace std;

ImageProvider::ImageProvider(QString dbname, QObject *parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbname);
    if (!db.open()){
        throw invalid_argument("Cannot open db");
    }
    fetchAll(QModelIndex());
}

ImageProvider::~ImageProvider()
{
    db.close();
}

int ImageProvider::rowCount(const QModelIndex &parent) const
{
    const DataWrapper *parent_pointer = dataForIndex(parent);
    return parent_pointer->count;
}

int ImageProvider::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED (parent); //dont used in body
    return 1;
}

QModelIndex ImageProvider::index(int row, int column, const QModelIndex &parent) const
{
    if (column != 0) {
        return QModelIndex();
    }

    const DataWrapper *parent_pointer = dataForIndex(parent);

    if (!parent.isValid()){
        return createIndex(row, column, d.children[row]);
    }

    if (parent_pointer->type == IMAGE){
        return QModelIndex();
    }
    if (parent_pointer->children.size() <= row){
        return QModelIndex();
    }
    return createIndex(row, column, parent_pointer->children[row]);
}

const DataWrapper* ImageProvider::dataForIndex(const QModelIndex &index) const
{
    if (!index.isValid()){
        return &d;
    }
    return static_cast<const DataWrapper *>(index.internalPointer());
}

DataWrapper* ImageProvider::dataForIndex(const QModelIndex &index)
{
    if (!index.isValid()){
        return &d;
    }
    return static_cast<DataWrapper *>(index.internalPointer());
}

QVariant ImageProvider::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) {
        return {};
      }
    const DataWrapper *elem = dataForIndex(index);
    if (role == Qt::DisplayRole) {
        switch (elem->type) {
            case ROOT:
            case TERM:
            case SUBJECT:
            case THEME:
            case PARAGRAPH:{
                return static_cast<HData*>(elem->data)->name;
            }
            case IMAGE:{
                return static_cast<IData*>(elem->data)->comment;
            }
            default:
                return QVariant();
        }
    }
    else if (role == Qt::DecorationRole || Qt::SizeHintRole) {
        if (elem->type == IMAGE) {
            QPixmap pix;
            pix.load(static_cast<IData*> (elem->data)->path);
            if (role == Qt::DecorationRole){
                return pix;
            }
            else {
                return pix.size();
            }
        }
    }
    return QVariant();
}

int ImageProvider::getChildrenCount(h_type type, int pid) const
{
    QSqlQuery query;
    switch(type){
        case ROOT:
        case TERM:
        case SUBJECT:
        case THEME:
            query.prepare("SELECT COUNT (*) from categories where p_id = :id");
            break;
      case PARAGRAPH:
            query.prepare("SELECT COUNT (*) from lectures where p_id = :id");
            break;
        case IMAGE:
            return 0;
        default:
            return 0;
    }
    query.bindValue(":id", pid);
    query.exec();
    query.next();
    int count = query.value(0).toInt();
    return count;
}

QModelIndex ImageProvider::parent(const QModelIndex& child) const
{
    if (!child.isValid()) return QModelIndex();
    const DataWrapper *child_pointer = dataForIndex(child);
    if (!child_pointer->parent) return QModelIndex();
    return createIndex(child_pointer->parent->number, 0, static_cast<void *>(child_pointer->parent));
}

void ImageProvider::fetchMore(const QModelIndex& parent)
{
    fetchAll(parent);
}

bool ImageProvider::canFetchMore(const QModelIndex& parent) const
{
    const DataWrapper* data = dataForIndex(parent);
    return (data->children.size() < data->count);
}

void ImageProvider::fetchAll(const QModelIndex& parent)
{
    DataWrapper* data = dataForIndex(parent);
    data->children.clear();
    QSqlQuery query;

    if (!parent.isValid())
      data->type = ROOT;

    if (data->type != PARAGRAPH){
        query.prepare("SELECT * from categories where p_id = :id");
    } else {
         query.prepare("SELECT * from lectures where p_id = :id");
    }

    if (parent.isValid())
      query.bindValue(":id", data->id);
    else{
      query.bindValue(":id", 0);
      }

    query.exec();
    while (query.next()){
        int id = query.value("id").toUInt();
        QString comment = query.value("Comment").toString();
        QStringList tags;
        int number;
        if (data->type == PARAGRAPH)
            tags = query.value("Tags").toStringList();
        if (data->type != PARAGRAPH)
            number = query.value("Number").toInt();
        else
            number = query.value("No").toInt();
        switch (data->type){
            case ROOT:
            case TERM:
            case SUBJECT:
            case THEME: {
                auto type = query.value("Type").toInt();
                QString name = query.value("Name").toString();
                data->children.append(
                            new DataWrapper{id, (h_type)type,
                                            new HData{type, name, comment},
                            number, data, {}, getChildrenCount((h_type)type, id)});
                data->count = data->children.size();
                break;
            }
            case PARAGRAPH: {
                auto path = query.value("File_name").toString();
                data->children.append(
                            new DataWrapper{id, IMAGE, new IData{path, comment, tags},
                                      number, data, {}, getChildrenCount(IMAGE, id)});
                data->count = data->children.size();
                break;
            }
            default:
              data->count = 0;
              break;
        }
    }
}

bool MySortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    qDebug() << "#filterAcceptsRow index: " << index;
    qDebug() << "#filterAcceptsRow source_row: " << source_row;
    qDebug() << "#filterAcceptsRow source_parent: " << source_parent;

    if (index.isValid())
    {
        DataWrapper* dw = static_cast<DataWrapper *>(index.internalPointer());
        qDebug() << "#filterAcceptsRow type:" << dw->type;
        qDebug() << "#filterAcceptsRow source_row:" << source_row;
        qDebug() << (dw->type == PARAGRAPH);
        return dw->type == PARAGRAPH;
    }
    qDebug() << "index is not valid!";
    return true;
}

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent): QSortFilterProxyModel(parent)
{

}
