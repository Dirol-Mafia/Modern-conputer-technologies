#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#endif // IMAGEPROVIDER_H
#include <QAbstractItemModel>
#include <QTreeView>
#include <QSqlDatabase>

enum h_type {ROOT, TERM, COURSE, THEME, IMAGE};

struct DataWrapper
{
    int id;
    int type;
    void* data;
    int number;
    DataWrapper* parent;
    QList <DataWrapper*> children;
    int count; /// explicit number of children
};

struct IData
{
    int id;
    int p_id;
    int number;
    QString path;
    QString comment;
    QVector <QString> tags;
};

struct HData
{
    int id;
    int p_id;
    QString type;
    QString name;
    QString comment;
    int number;
};

class ImageProvider: public QAbstractItemModel
{
public:
    ImageProvider(QString dbname, QObject *parent = nullptr);
    ~ImageProvider();
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    void fetchAll(const QModelIndex &parent);
    void fecthMore(const QModelIndex &parent);
    bool canFetchMore(const QModelIndex &parent) const;
private:
    DataWrapper d {0, ROOT, nullptr, 0, nullptr, {}, -1};
    //void DataWrapper(const QModelIndex &parent);
    int getChildCount(h_type type, int pid) const;
    const DataWrapper *dataForIndex(const QModelIndex &index) const;
    DataWrapper *dataForIndex(const QModelIndex &index);
    QSqlDatabase db;
};
