#ifndef __MD5_UTIL_H_INCL__
#define __MD5_UTIL_H_INCL__

class QByteArray;

void md5_digest (const QByteArray &src, unsigned char *digest);

QString md5_digest (const QByteArray &src);

#endif