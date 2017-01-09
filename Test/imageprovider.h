#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#endif // IMAGEPROVIDER_H
#include <QAbstractItemModel>
#include <QTreeView>
#include <QSqlDatabase>
#include <QSortFilterProxyModel>


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
    bool isChecked;

    bool insertChildren(int position, int num, int columns = 1);
    bool removeChildren(int position, int num);
    bool setData (int col, const QVariant& value);
};
Q_DECLARE_METATYPE(DataWrapper)

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
Q_DECLARE_METATYPE(IData)

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
Q_DECLARE_METATYPE(HData)

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
    void fetchMore(const QModelIndex& parent);
    bool canFetchMore(const QModelIndex& parent) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    DataWrapper* dataForIndex(const QModelIndex& index);
    const DataWrapper* dataForIndex(const QModelIndex& index) const;
    const DataWrapper* getRoot() const;

private:
    DataWrapper d {0, ROOT, nullptr, 0, nullptr, {}, -1, false};
    int getChildrenCount(h_type type, int pid) const;
    QSqlDatabase db;
};

class MySortFilterProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MySortFilterProxyModel(QObject *parent = 0);
   // Qt::ItemFlags flags(const QModelIndex& index) const;
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};
 
// Other Functions
QString getCatName(h_type type);
QString getSubcatName(h_type type);
QString getCatChildName(h_type type);
