#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#endif // IMAGEPROVIDER_H
#include <QAbstractItemModel>
#include <QTreeView>
#include <QSqlDatabase>

enum h_type {ROOT = 0, TERM = 1, SUBJECT = 2, THEME = 3, PARAGRAPH = 4, IMAGE = 5};

/**
 * @brief The DataWrapper struct
 */
struct DataWrapper
{
    int id;
    h_type type;
    void* data; /// Can be either HData or IData
    int number; /// Consecutive number
    DataWrapper* parent;
    QList <DataWrapper*> children;
    int count; /// explicit number of children
};

/**
 * @brief The IData (Image Data) struct
 * Defined accoring to "Lections" table in the DBLections
 */
struct IData
{
    /*int id; /// Primary key
    int p_id; /// Parent id
    int number; /// Consecutive number*/
    QString path; /// Path to the image
    QString comment; /// Comments (contains extra information)
    QStringList tags; /// Tags
};

/**
 * @brief The HData (Hierarchy Data) struct
 * Defined according to "Categories" table in the DBLections
 */
struct HData
{
    /*int id; /// Primary key
    int p_id; /// Parent id*/
    int type; /// Type: term, subject, theme, paragraph
    QString name; /// Name
    QString comment; /// Comments (contains extra information)
    //int number; /// Consecutive number
};

class ImageProvider: public QAbstractItemModel
{
    Q_OBJECT

public:
    ImageProvider(QString dbname, QObject* parent = nullptr);
    ~ImageProvider();
    virtual int rowCount(const QModelIndex& parent) const;
    virtual int columnCount(const QModelIndex& parent) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    void fetchAll(const QModelIndex& parent);
    void fecthMore(const QModelIndex& parent);
    bool canFetchMore(const QModelIndex& parent) const;
private:
    DataWrapper d {0, ROOT, nullptr, 0, nullptr, {}, -1};
    //void DataWrapper(const QModelIndex &parent);
    int getChildrenCount(h_type type, int pid) const;
    const DataWrapper* dataForIndex(const QModelIndex& index) const;
    DataWrapper* dataForIndex(const QModelIndex& index);
    QSqlDatabase db;
};
