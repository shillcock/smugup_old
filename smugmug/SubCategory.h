#ifndef __SMUGMUG_SUB_CATEGORY_H_INCL__
#define __SMUGMUG_SUB_CATEGORY_H_INCL__

#include "SmugObject.h"


namespace SmugMug {

   class SubCategory : public SmugObject {
      
      Q_OBJECT

      public:
         const QString SubCategoryId;
         
         SubCategory (const QString &Id, QObject *parent = 0);

         ~SubCategory ();

         virtual void update (QDomElement &root);
   };
};

#endif // __SMUGMUG_SUB_CATEGORY_H_INCL__
