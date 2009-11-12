#ifndef __SMUGMUG_SMUG_OBJECT_H_INCL__
#define __SMUGMUG_SMUG_OBJECT_H_INCL__

#include <QObject>
#include <QString>

class QDomElement;


namespace SmugMug {

   class SmugObject  : public QObject {

      Q_OBJECT

      public:
         SmugObject (QObject *parent = 0);
         virtual ~SmugObject ();

         void setAttribute (
            const QString &Name,
            const QString &Attr,
            const QDomElement &Elem);

         QString attribute (const QString &Name) const;
         
         virtual void update (QDomElement &root) = 0;
   };
}

#endif // __SMUGMUG_SMUG_OBJECT_H_INCL__
