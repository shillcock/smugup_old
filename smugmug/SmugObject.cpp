#include "Album.h"
#include <QDomElement>
#include <QVariant>
#include <QDebug>


SmugMug::SmugObject::SmugObject (QObject *parent) :
         QObject (parent) {;}


SmugMug::SmugObject::~SmugObject () {;}


void
SmugMug::SmugObject::setAttribute (const QString &Name, const QString &Attr, const QDomElement &Elem) {

   if (Elem.hasAttribute (Attr)) {

      QVariant var (Elem.attribute (Attr));
      QObject::setProperty (Name.toLatin1 (), var);
   }
}


QString
SmugMug::SmugObject::attribute (const QString &Name) const {

   return property (Name.toLatin1 ()).toString ();
}

