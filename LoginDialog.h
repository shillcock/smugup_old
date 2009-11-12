#ifndef LOGIN_DIALOG_DOT_H
#define LOGIN_DIALOG_DOT_H

#include "ui_LoginDialog.h"
#include <QDialog>

namespace SmugMug {

   class WebService;
}


class LoginDialog : public QDialog {

	Q_OBJECT

	public:
      LoginDialog (SmugMug::WebService &smugmug, QWidget *parent);
		~LoginDialog ();
		
      virtual void accept ();

	protected:
		Ui::LoginDialog _ui;
      SmugMug::WebService &_smugmug;
};

#endif // LOGIN_DIALOG_DOT_H
