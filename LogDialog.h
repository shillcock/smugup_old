#ifndef LOG_DIALOG_DOT_H
#define LOG_DIALOG_DOT_H

#include "ui_LogDialog.h"
#include <QDialog>


class LogDialog : public QDialog {

	Q_OBJECT

	public:
    	LogDialog (QWidget *parent = 0);
		~LogDialog ();
		
   protected:
      void _settingsLoad ();
      void _settingsSave ();

	protected:
		Ui::LogDialog _ui;
};

#endif // LOG_DIALOG_DOT_H
