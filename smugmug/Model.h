#ifndef SMUGMUG_MODEL_DOT_H
#define SMUGMUG_MODEL_DOT_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>


namespace SmugMug {

   class Item;

   class Model : public QAbstractItemModel {

      Q_OBJECT

      public:
         Model (const QString &data, QObject *parent = 0);
         ~Model ();

         QVariant data (const QModelIndex &index, int role) const;

         Qt::ItemFlags flags (const QModelIndex &index) const;

         QVariant headerData (
            int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const;

         QModelIndex index (
            int row,
            int column,
            const QModelIndex &parent = QModelIndex()) const;

         QModelIndex parent (const QModelIndex &index) const;

         int rowCount (const QModelIndex &parent = QModelIndex()) const;
         int columnCount (const QModelIndex &parent = QModelIndex()) const;

      private:
         void _setupModelData (const QStringList &lines, Item *parent);

         Item *_rootItem;
   };
}


#endif // SMUGMUG_MODEL_DOT_H
