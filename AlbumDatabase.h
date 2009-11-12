#ifndef __SMUGMUG_ALBUM_DATABASE_H_INCL__
#define __SMUGMUG_ALBUM_DATABASE_H_INCL__

#include "AlbumInfo.h"

class QString;
class AlbumDatabasePriv;


class AlbumDatabase {

   public:
      AlbumDatabase ();
      ~AlbumDatabase ();

      bool isValid () const;

      void setDatabasePath (const QString &Path);

      AlbumInfo::List scanAlbums ();

      int addAlbum (const AlbumInfo &Info);
      void setAlbumTitle ();
   protected:
};

#endif // __SMUGMUG_ALBUM_DATABASE_H_INCL__