#include "imageprovider.h"
#include <stdexcept>
#include "QSql"
//#include "QSqlDatabase"
#include <QTreeView>
#include <QSqlDatabase>

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

const DataWrapper *ImageProvider::dataForIndex(const QModelIndex &index) const
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
            case COURSE:
            case THEME:
                return static_cast<HData*>(elem->data)->name;
            case IMAGE:
                return static_cast<IData*>(elem->data)->path;
            default:
                break;
            }
        }
    }
}

