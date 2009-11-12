#ifndef __SMUGMUG_ALBUM_H_INCL__
#define __SMUGMUG_ALBUM_H_INCL__

#include "SmugObject.h"


namespace SmugMug {

   class Category;
   class SubCategory;


   class Album  : public SmugObject {

      Q_OBJECT

      public:
         const QString AlbumId;

         Album (const QString &Id, QObject *parent = 0);
         ~Album ();

         Category *category ();
         SubCategory *subCategory ();

         virtual void update (QDomElement &root);
   };
}

#endif // __SMUGMUG_ALBUM_H_INCL__
