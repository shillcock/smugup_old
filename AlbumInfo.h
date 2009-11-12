#ifndef __SMUGMUG_ALBUM_INFO_H_INCL__
#define __SMUGMUG_ALBUM_INFO_H_INCL__

#include <QList>
#include <QString>

class AlbumInfo {

   public:
      typedef QList<AlbumInfo> List;

      AlbumInfo ();
      ~AlbumInfo ();

      QString id;
      QString title;
      QString categoryId;
      QString description;
      QString imgCount;
      QString keywords;

      bool operator< (const AlbumInfo &Info) {

         return id < Info.id;
      }
};

#endif // __SMUGMUG_ALBUM_INFO_H_INCL__