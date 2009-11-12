#ifndef __SMUGMUG_CATEGORY_H_INCL__
#define __SMUGMUG_CATEGORY_H_INCL__

#include "SmugObject.h"


namespace SmugMug {

   class Category : public SmugObject {

      Q_OBJECT
   
      public:
         const QString CategoryId;
         
         Category (const QString &Id, QObject *parent = 0);
         ~Category ();

         virtual void update (QDomElement &root);
   };
};


#endif // __SMUGMUG_CATEGORY_H_INCL__
