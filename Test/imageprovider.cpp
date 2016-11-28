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
    else if (role == Qt::DecorationRole) {
        if (elem->type == IMAGE) {
            QPixmap pix;
            pix.load(static_cast<IData*> (elem->data)->path);
            pix = pix.scaled(250, 250, Qt::KeepAspectRatio);
            if (role == Qt::DecorationRole){
                return pix;
            }
            else {
                return pix.size();
            }
        }
    }
    if (role == Qt::CheckStateRole && elem->type == IMAGE)
    {
        return elem->isChecked;
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


Qt::ItemFlags ImageProvider::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    return flags | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

bool ImageProvider::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) return false;
    DataWrapper* data = dataForIndex(index);
    if (role == Qt::CheckStateRole)
    {
        data->isChecked = data->isChecked ? false : true;
        return true;
        emit dataChanged(index, index);
    }
    return false;
}

bool ImageProvider::insertRows(int position, int rows, const QModelIndex &parent)
{
  DataWrapper* parent_data = dataForIndex(parent);
  bool success;

  beginInsertRows(parent, position, position + rows - 1);
  success = parent_data->insertChildren(position, rows, 1);
  endInsertRows();

  return success;

}

bool ImageProvider::removeRows(int position, int rows, const QModelIndex &parent)
{
  DataWrapper* parent_data = dataForIndex(parent);
  bool success = true;

  beginRemoveRows(parent, position, position + rows - 1);
  success = parent_data->removeChildren(position, rows);

  return success;
}

bool MySortFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    if (index.isValid())
    {
        DataWrapper* dw = static_cast<DataWrapper *>(index.internalPointer());
        return dw->type != IMAGE;
    }
    return false;
}

MySortFilterProxyModel::MySortFilterProxyModel(QObject *parent): QSortFilterProxyModel(parent)
{

}

//Non-class functions implementation
QString getCatName(h_type type)
{
  switch (type) 
    {
      case ROOT:
        return " ничего";
      case TERM:
        return " семестр";
      case SUBJECT:
        return " предмет";
      case THEME:
        return " тему";
      case PARAGRAPH:
        return " параграф";
      case IMAGE:
        return " скан лекции";
      default:
        return "";
  }  
}

QString getSubcatName(h_type type)
{
  switch (type)
    {
      case ROOT:
        return " семестр";
      case TERM:
        return " предмет";
      case SUBJECT:
        return " тему";
      case THEME:
        return " параграф";
      case PARAGRAPH:
        return " скан лекции";
      case IMAGE:
        return "";
      default:
        return "";
  }
}

QString getCatChildName(h_type type)
{
  switch (type) {
    case ROOT:
      return " семестр";
    case TERM:
    default:
      break;
    }
}

/* DataWrapper methods implementation */
bool DataWrapper::insertChildren(int position, int num, int columns)
{
  if (position < 0 || position > children.size())
    return false;

  for (int row = 0; row < num; ++row){
      DataWrapper* child = new DataWrapper();
      QSqlQuery query;
      if (type != PARAGRAPH){
        query.prepare("INSERT INTO categories (p_id, Type, Name, Comment, Number) VALUES (:id, :t, \"defname\", \"defcomm\", :n)");
        query.bindValue(":t", (int)(type) + 1);
        }
      else
        query.prepare("INSERT INTO lectures (p_id, No, File_name, Comment, Tags) VALUES (:id, :n, \"defpath\", \"defcomm\", \"deftags\")");
      query.bindValue(":id", id);
      query.bindValue(":n", position);
      query.exec();
      //query.next();
      qlonglong new_id = static_cast<qlonglong>(query.lastInsertId());
      child->id = new_id;

      QSqlQuery update;
      update.prepare("UPDATE :table SET :field = :field + 1 WHERE :field > :pos");
      update.bindValue(":pos", position);
      if (type != PARAGRAPH){
        update.bindValue(":table", "categories");
        update.bindValue(":field", "Number");
        }
      else{
          update.bindValue(":table", "lectures");
          update.bindValue(":field", "No");
        }

      update.exec();
      children.insert(position, child);
      ++count;
    }

  return true;
}

bool DataWrapper::removeChildren(int position, int count)
{
  if (position < 0 || position + count > children.size())
    return false;

  for (int row = 0; row < count; ++row)
    delete children.takeAt(position);

  return true;
}

bool DataWrapper::setData(int col, const QVariant &value)
{
  if (col != 0)
    return false;

  //data = &value;
  return true;
}
