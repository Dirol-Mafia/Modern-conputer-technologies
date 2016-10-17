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

    if (!parent.isValid()){
        return createIndex(row, column, d.children[row]);
    }

    const DataWrapper *parent_pointer = dataForIndex(parent);
    if (parent_pointer->type == IMAGE){
        return QModelIndex();
    }
    if (parent_pointer->children.size() <= row){
        return QModelIndex();
    }
    return createIndex(row, column, parent_pointer->children[row]);
}

//TODO linking error
const DataWrapper* ImageProvider::dataForIndex(const QModelIndex &index) const
{
    if (!index.isValid()){
        return &d;
    }
    return static_cast<DataWrapper *>(index.internalPointer());
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
    if (!index.isValid()) return {};
    if (role == Qt::DisplayRole) {
        if (index.isValid()){
            const DataWrapper *elem = dataForIndex(index);
            switch (elem->type) {
                case TERM:
                case SUBJECT:
                case THEME:
                case PARAGRAPH:
                    return static_cast<HData*>(elem->data)->name;
                case IMAGE:
                    return static_cast<IData*>(elem->data)->path;
                default:
                    break;
            }
        }
    }
}

int ImageProvider::getChildrenCount(h_type type, int pid) const
{
    QSqlQuery query;
    switch(type){
        case ROOT:
        case TERM:
        case SUBJECT:
            query.prepare("SELECT COUNT (*) from categories where p_id = :id");
            break;
        case THEME:
            query.prepare("SELECT COUNT (*) from lectures where p_id = :id");
            break;
        case IMAGE:
            return 0;
        default:
            break;
    }
    query.bindValue(":id", pid);
    query.exec();
    query.next();
    qDebug() << query.executedQuery();
    //qDebug() << query.lastError();
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

void ImageProvider::fecthMore(const QModelIndex& parent)
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
    if (data->type != THEME){
        query.prepare("SELECT * from categories where p_id = :id");
    } else {
         query.prepare("SELECT * from lectures where p_id = :id");
    }
    query.bindValue(":id", data->id);
    query.exec();
    while (query.next()){
        int id = query.value("id").toUInt();
        QString comment = query.value("Comment").toString();
        QStringList tags;
        int number;
        if (data->type == THEME)
            tags = query.value("Tags").toStringList();
        if (data->type != THEME)
            number = query.value("Number").toInt();
        else
            number = query.value("No").toInt();
        switch (data->type){
            case ROOT:
            case TERM:
            case SUBJECT: {
                auto type = query.value("Type").toInt();
                QString name = query.value("Name").toString();
                data->children.append(
                            new DataWrapper{id, (h_type)type,
                                            new HData{type, name, comment},
                            number, data, {}, getChildrenCount((h_type)type, id)});
                break;
            }
            case THEME: {
                auto path = query.value("File_name").toString();
                data->children.append(
                            new DataWrapper{id, IMAGE, new IData{path, comment, tags},
                                      number, data, {}, getChildrenCount(IMAGE, id)});
                break;
            }
            default: break;
        }
    }
}
