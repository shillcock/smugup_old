#include "LoginDialog.h"
#include <SmugMug.h>
#include <QSettings>
#include <QString>


LoginDialog::LoginDialog (SmugMug::WebService &smugmug, QWidget *parent) :
        QDialog (parent, Qt::Sheet),
        _ui (),
        _smugmug (smugmug){

   _ui.setupUi (this);
   
   _ui.userNameLineEdit->setText (_smugmug.userName ());
}


LoginDialog::~LoginDialog () {;}


void
LoginDialog::accept () {

   QString userName = _ui.userNameLineEdit->text ();
   QString password = _ui.passwordLineEdit->text ();

   if (!userName.isEmpty () && !password.isEmpty ()) {

      _smugmug.loginWithPassword (userName, password);
   }

   QDialog::accept ();
}
