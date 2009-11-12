#include "LogDialog.h"
#include <QSettings>


LogDialog::LogDialog (QWidget *parent) :
        QDialog (parent),
        _ui () {

   _settingsLoad ();

   _ui.setupUi (this);
}


LogDialog::~LogDialog () {


}


void
LogDialog::_settingsLoad () {

   QSettings settings;
}


void
LogDialog::_settingsSave () {

   QSettings settings;
}

