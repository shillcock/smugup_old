#ifndef SMUGMUG_ITEM_DOT_H
#define SMUGMUG_ITEM_DOT_H

#include <QList>
#include <QVariant>


namespace SmugMug {

   class Item {

      public:
         Item (const QList<QVariant> &Data, Item *parent = 0);
         ~Item ();

         void appendChild (Item *child);

         Item *child (int row);

         int childCount () const;
         int columnCount () const;

         QVariant data (int column) const;

         int row () const;

         Item *parent ();

      protected:
         QList<Item *> _childItems;
         QList<QVariant> _itemData;
         Item *_parentItem;
   };
}


#endif // SMUGMUG_ITEM_DOT_H
