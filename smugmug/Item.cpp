#include "Item.h"

#include <QStringList>

namespace SmugMug {

   Item::Item (const QList<QVariant> &Data, Item *parent) :
      _childItems (),
      _itemData (Data),
      _parentItem (parent) {;}

   Item::~Item () {

      qDeleteAll (_childItems);
   }

   void
   Item::appendChild (Item *child) {

      _childItems.append (child);
   }

   Item *
   Item::child (int row) {

      return _childItems.value (row);
   }

   int
   Item::childCount () const {

      return _childItems.count();
   }

   int
   Item::columnCount () const {

      return _itemData.count();
   }

   QVariant
   Item::data (int column) const {

      return _itemData.value (column);
   }

   Item *
   Item::parent () {

      return _parentItem;
   }

   int
   Item::row () const {

      if (_parentItem) {

         return _parentItem->childItems.indexOf(const_cast<Item*>(this));
      }

      return 0;
   }
}
