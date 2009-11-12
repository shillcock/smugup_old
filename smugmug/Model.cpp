#include "Model.h"
#include "Item.h"

#include <QtGui>


namespace SmugMug {

   Model::Model (const QString &data, QObject *parent) :
         QAbstractItemModel (parent) {

      QList<QVariant> rootData;
      rootData << "Title" << "Summary";
      _rootItem = new Item (rootData);

      _setupModelData(data.split (QString ("\n")), _rootItem);
   }


   Model::~Model() {

      delete _rootItem;
   }


   int
   Model::columnCount (const QModelIndex &parent) const {

      if (parent.isValid()) {

         return static_cast<Item*>(parent.internalPointer())->columnCount();
      }
      else {

         return rootItem->columnCount();
      }
   }

   QVariant
   Model::data (const QModelIndex &index, int role) const {

      if (!index.isValid()) {

         return QVariant();
      }

      if (role != Qt::DisplayRole) {

         return QVariant();
      }

      Item *item = static_cast<Item*> (index.internalPointer ());

      return item->data (index.column ());
   }

   Qt::ItemFlags
   Model::flags (const QModelIndex &index) const {

      if (!index.isValid()) {

         return Qt::ItemIsEnabled;
      }

      return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
   }

   QVariant
   Model::headerData (int section, Qt::Orientation orientation, int role) const {

      if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
       
         return _rootItem->data (section);
      }

      return QVariant();
   }

   QModelIndex
   Model::index(int row, int column, const QModelIndex &parent) const {

      Item *parentItem;

      if (!parent.isValid ()) {

         parentItem = _rootItem;
      }
      else {

         parentItem = static_cast<Item*> (parent.internalPointer ());
      }

      Item *childItem = parentItem->child (row);

      if (childItem) {

         return createIndex (row, column, childItem);
      }
      else {

         return QModelIndex ();
      }
   }


   QModelIndex
   Model::parent (const QModelIndex &index) const {

      if (!index.isValid()) {

         return QModelIndex();
      }

      Item *childItem = static_cast<Item*>(index.internalPointer ());
      Item *parentItem = childItem->parent ();

      if (parentItem == _rootItem) {

         return QModelIndex();
      }

      return createIndex (parentItem->row (), 0, parentItem);
   }


   int
   Model::rowCount (const QModelIndex &parent) const {

      Item *parentItem;

      if (!parent.isValid ()) {

         parentItem = rootItem;
      }
      else {

         parentItem = static_cast<Item*> (parent.internalPointer ());
      }

      return parentItem->childCount ();
   }

   void
   TreeModel::_setupModelData (const QStringList &lines, Item *parent) {

      QList<Item*> parents;
      QList<int> indentations;
      parents << parent;
      indentations << 0;

      int number = 0;

      while (number < lines.count()) {

         int position = 0;

         while (position < lines[number].length()) {

            if (lines[number].mid(position, 1) != " ")
               break;

            position++;
         }

         QString lineData = lines[number].mid(position).trimmed();

         if (!lineData.isEmpty()) {

            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
               columnData << columnStrings[column];

            if (position > indentations.last()) {
               // The last child of the current parent is now the new parent
               // unless the current parent has no children.

               if (parents.last()->childCount() > 0) {
                  parents << parents.last()->child(parents.last()->childCount()-1);
                  indentations << position;
               }
            } else {
               while (position < indentations.last() && parents.count() > 0) {
                  parents.pop_back();
                  indentations.pop_back();
               }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new TreeItem(columnData, parents.last()));
         }

         number++;
      }
   }
}