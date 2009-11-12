#
# smugup - smugmug uploading tool
#

TEMPLATE = app

LANGUAGE = C++

CONFIG += qt debug

OBJECTS_DIR = build

MOC_DIR = build

UI_DIR = build

QT += core gui xml sql

RESOURCES = smugup.qrc
 
include(smugmug/smugmug.pri)

FORMS += \
   LoginDialog.ui \
   LogDialog.ui \
   MainWindow.ui
   
HEADERS += \
   LoginDialog.h \
   LogDialog.h \
   MainWindow.h

SOURCES += \
   LoginDialog.cpp \
   LogDialog.cpp \
   MainWindow.cpp \
   md5/md5_util.cpp \
   md5/md5.c \
   main.cpp
