#include "Category.h"
#include <QDomElement>
#include <QVariant>
#include <QDebug>


SmugMug::Category::Category (const QString &Id, QObject *parent) :
         SmugObject (parent),
         CategoryId (Id) {;}


SmugMug::Category::~Category () {;}


void
SmugMug::Category::update (QDomElement &root) {
   
   setAttribute ("Title", "Title", root);
   setAttribute ("Name", "Name", root);

   setObjectName (attribute ("Title"));
}
