#include "md5.h"
#include <QByteArray>
#include <QString>


void
md5_digest (const QByteArray &src, unsigned char *digest) {

   md5_state_t md5state;

   md5_init (&md5state);
   md5_append (&md5state, (unsigned char *)src.data (), src.size ());
   md5_finish (&md5state, digest);
}


QString
md5_digest (const QByteArray &src) {

   unsigned char digest[16];
   md5_digest (src, digest);

   QString output, tmp;
   for (int i = 0; i < 16; ++i) {

      output += tmp.sprintf ("%02x", digest[i]);
   }

   return output;
}