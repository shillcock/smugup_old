#include "SubCategory.h"
#include <QDomElement>
#include <QVariant>
#include <QDebug>


SmugMug::SubCategory::SubCategory (const QString &Id, QObject *parent) :
         SmugObject (parent),
         SubCategoryId (Id) {

   setObjectName (parent->objectName () + "::");
}


SmugMug::SubCategory::~SubCategory () {;}


void
SmugMug::SubCategory::update (QDomElement &root) {
   
   setAttribute ("Title", "Title", root);

   setObjectName (objectName () + attribute ("Title"));
}
