#include "Album.h"
#include "Category.h"
#include "SubCategory.h"
#include <QDomElement>
#include <QVariant>
#include <QDebug>


SmugMug::Album::Album (const QString &Id, QObject *parent) :
         SmugObject (parent),
         AlbumId (Id) {;}


SmugMug::Album::~Album () {;}


void
SmugMug::Album::update (QDomElement &root) {

   setAttribute ("Title", "Title", root);

   QDomElement elem = root.firstChildElement (QLatin1String ("Category"));
   if (!elem.isNull ()) {

      setAttribute ("CategoryId", "id", elem);
      setAttribute ("CategoryName", "Name", elem);
   }

   elem = root.firstChildElement (QLatin1String ("SubCategory"));
   if (!elem.isNull ()) {

      setAttribute ("SubCategoryId", "id", elem);
      setAttribute ("SubCategoryName", "Name", elem);
   }
//qDebug () << "Album Update -----------------------------------";
//qDebug () << AlbumId << attribute ("Title");
//qDebug () << attribute ("CategoryId") << attribute ("CategoryName");
//qDebug () << attribute ("SubCategoryId") << attribute ("SubCategoryName");

   setObjectName (
      attribute ("CategoryName") + "::" +
      attribute ("SubCategoryName") + "::" +
      attribute ("Title"));
}


SmugMug::Category *
SmugMug::Album::category () {
qDebug () << objectName ();
   return dynamic_cast<SmugMug::Category *> (parent ());
}


SmugMug::SubCategory *
SmugMug::Album::subCategory () {

qDebug () << objectName ();
   QObject *super = parent ();
   if (super) { super = super->parent (); }
   return dynamic_cast<SmugMug::SubCategory *> (super);
}

