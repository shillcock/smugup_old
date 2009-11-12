#include <QtGui>
#include <QtSql>

#include "MainWindow.h"

int
main (int argc, char *argv[]) {
	
   int retVal = 0;
    QApplication app (argc, argv);
    Q_INIT_RESOURCE(smugup);

    QCoreApplication::setOrganizationName("Shillcock");
    QCoreApplication::setOrganizationDomain("shillcock.net");
    QCoreApplication::setApplicationName("SmugUp");
    
//    QSqlDatabase db = QSqlDatabase::addDatabase ("QSQLITE");

//    db.setDatabaseName ("c:/temp/smugup.db");

//    if (db.open ()){

         MainWindow mainWin;
         mainWin.show ();
         retVal = app.exec ();
//    }

    return retVal;
}
