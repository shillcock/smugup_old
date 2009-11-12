#ifndef __SMUGMUG_IMAGE_H_INCL__
#define __SMUGMUG_IMAGE_H_INCL__

#include <QString>

namespace SmugMug {

   class Image {
   
      public:
         const QString Id;
         QString albumId;
         QString fileName;
         QString caption;
         QString keywords;
         QString position;
         QString date;
         QString format;
         QString serial;
         QString watermark;
         QString size;
         QString width;
         QString height;
         QString md5sum;
         QString lastUpdated;
         QString originalUrl;
         QString largeUrl;
         QString mediumUrl;
         QString smallUrl;
         QString thumbUrl;
         QString albumUrl;
         
         Image (const QString &Id);
         ~Image ();
   };
};

#endif // __SMUGMUG_IMAGE_H_INCL__
