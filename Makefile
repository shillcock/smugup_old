#############################################################################
# Makefile for building: smugup.app/Contents/MacOS/smugup
# Generated by qmake (2.01a) (Qt 4.6.0) on: Fri Oct 30 16:53:41 2009
# Project:  smugup.pro
# Template: app
# Command: /usr/bin/qmake -spec /usr/local/Qt4.6/mkspecs/macx-g++ -macx -o Makefile smugup.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_SQL_LIB -DQT_XML_LIB -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -gdwarf-2 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -g -gdwarf-2 -Wall -W $(DEFINES)
INCPATH       = -I/usr/local/Qt4.6/mkspecs/macx-g++ -I. -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/usr/include/QtCore -I/Library/Frameworks/QtNetwork.framework/Versions/4/Headers -I/usr/include/QtNetwork -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/usr/include/QtGui -I/Library/Frameworks/QtXml.framework/Versions/4/Headers -I/usr/include/QtXml -I/Library/Frameworks/QtSql.framework/Versions/4/Headers -I/usr/include/QtSql -I/usr/include -Ismugmug -Ibuild -Ibuild -F/Library/Frameworks
LINK          = g++
LFLAGS        = -headerpad_max_install_names
LIBS          = $(SUBLIBS) -F/Library/Frameworks -L/Library/Frameworks -framework QtSql -L/tmp/qt-stuff/source/qt-everywhere-opensource-src-4.6.0-beta1/lib -F/tmp/qt-stuff/source/qt-everywhere-opensource-src-4.6.0-beta1/lib -framework QtXml -framework QtGui -framework QtNetwork -framework QtCore 
AR            = ar cq
RANLIB        = ranlib -s
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
export MACOSX_DEPLOYMENT_TARGET = 10.4

####### Output directory

OBJECTS_DIR   = build/

####### Files

SOURCES       = smugmug/WebService.cpp \
		LoginDialog.cpp \
		LogDialog.cpp \
		MainWindow.cpp \
		md5/md5_util.cpp \
		md5/md5.c \
		main.cpp build/moc_WebService.cpp \
		build/moc_LoginDialog.cpp \
		build/moc_LogDialog.cpp \
		build/moc_MainWindow.cpp \
		qrc_smugup.cpp
OBJECTS       = build/WebService.o \
		build/LoginDialog.o \
		build/LogDialog.o \
		build/MainWindow.o \
		build/md5_util.o \
		build/md5.o \
		build/main.o \
		build/moc_WebService.o \
		build/moc_LoginDialog.o \
		build/moc_LogDialog.o \
		build/moc_MainWindow.o \
		build/qrc_smugup.o
DIST          = /usr/local/Qt4.6/mkspecs/common/unix.conf \
		/usr/local/Qt4.6/mkspecs/common/mac.conf \
		/usr/local/Qt4.6/mkspecs/common/mac-g++.conf \
		/usr/local/Qt4.6/mkspecs/qconfig.pri \
		/usr/local/Qt4.6/mkspecs/features/qt_functions.prf \
		/usr/local/Qt4.6/mkspecs/features/qt_config.prf \
		/usr/local/Qt4.6/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt4.6/mkspecs/features/default_pre.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/default_pre.prf \
		smugmug/smugmug.pri \
		/usr/local/Qt4.6/mkspecs/features/mac/dwarf2.prf \
		/usr/local/Qt4.6/mkspecs/features/debug.prf \
		/usr/local/Qt4.6/mkspecs/features/default_post.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/default_post.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/objective_c.prf \
		/usr/local/Qt4.6/mkspecs/features/qt.prf \
		/usr/local/Qt4.6/mkspecs/features/unix/thread.prf \
		/usr/local/Qt4.6/mkspecs/features/moc.prf \
		/usr/local/Qt4.6/mkspecs/features/warn_on.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/rez.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/sdk.prf \
		/usr/local/Qt4.6/mkspecs/features/resources.prf \
		/usr/local/Qt4.6/mkspecs/features/uic.prf \
		/usr/local/Qt4.6/mkspecs/features/yacc.prf \
		/usr/local/Qt4.6/mkspecs/features/lex.prf \
		smugup.pro
QMAKE_TARGET  = smugup
DESTDIR       = 
TARGET        = smugup.app/Contents/MacOS/smugup

####### Custom Compiler Variables
QMAKE_COMP_QMAKE_OBJECTIVE_CFLAGS = -pipe \
		-g \
		-gdwarf-2 \
		-Wall \
		-W


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile smugup.app/Contents/PkgInfo smugup.app/Contents/Resources/empty.lproj smugup.app/Contents/Info.plist $(TARGET)

$(TARGET): build/ui_LoginDialog.h build/ui_LogDialog.h build/ui_MainWindow.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) smugup.app/Contents/MacOS/ || $(MKDIR) smugup.app/Contents/MacOS/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: smugup.pro  /usr/local/Qt4.6/mkspecs/macx-g++/qmake.conf /usr/local/Qt4.6/mkspecs/common/unix.conf \
		/usr/local/Qt4.6/mkspecs/common/mac.conf \
		/usr/local/Qt4.6/mkspecs/common/mac-g++.conf \
		/usr/local/Qt4.6/mkspecs/qconfig.pri \
		/usr/local/Qt4.6/mkspecs/features/qt_functions.prf \
		/usr/local/Qt4.6/mkspecs/features/qt_config.prf \
		/usr/local/Qt4.6/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt4.6/mkspecs/features/default_pre.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/default_pre.prf \
		smugmug/smugmug.pri \
		/usr/local/Qt4.6/mkspecs/features/mac/dwarf2.prf \
		/usr/local/Qt4.6/mkspecs/features/debug.prf \
		/usr/local/Qt4.6/mkspecs/features/default_post.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/default_post.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/objective_c.prf \
		/usr/local/Qt4.6/mkspecs/features/qt.prf \
		/usr/local/Qt4.6/mkspecs/features/unix/thread.prf \
		/usr/local/Qt4.6/mkspecs/features/moc.prf \
		/usr/local/Qt4.6/mkspecs/features/warn_on.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/rez.prf \
		/usr/local/Qt4.6/mkspecs/features/mac/sdk.prf \
		/usr/local/Qt4.6/mkspecs/features/resources.prf \
		/usr/local/Qt4.6/mkspecs/features/uic.prf \
		/usr/local/Qt4.6/mkspecs/features/yacc.prf \
		/usr/local/Qt4.6/mkspecs/features/lex.prf \
		/Library/Frameworks/QtSql.framework/QtSql.prl \
		/Library/Frameworks/QtCore.framework/QtCore.prl \
		/Library/Frameworks/QtXml.framework/QtXml.prl \
		/Library/Frameworks/QtGui.framework/QtGui.prl \
		/Library/Frameworks/QtNetwork.framework/QtNetwork.prl
	$(QMAKE) -spec /usr/local/Qt4.6/mkspecs/macx-g++ -macx -o Makefile smugup.pro
/usr/local/Qt4.6/mkspecs/common/unix.conf:
/usr/local/Qt4.6/mkspecs/common/mac.conf:
/usr/local/Qt4.6/mkspecs/common/mac-g++.conf:
/usr/local/Qt4.6/mkspecs/qconfig.pri:
/usr/local/Qt4.6/mkspecs/features/qt_functions.prf:
/usr/local/Qt4.6/mkspecs/features/qt_config.prf:
/usr/local/Qt4.6/mkspecs/features/exclusive_builds.prf:
/usr/local/Qt4.6/mkspecs/features/default_pre.prf:
/usr/local/Qt4.6/mkspecs/features/mac/default_pre.prf:
smugmug/smugmug.pri:
/usr/local/Qt4.6/mkspecs/features/mac/dwarf2.prf:
/usr/local/Qt4.6/mkspecs/features/debug.prf:
/usr/local/Qt4.6/mkspecs/features/default_post.prf:
/usr/local/Qt4.6/mkspecs/features/mac/default_post.prf:
/usr/local/Qt4.6/mkspecs/features/mac/objective_c.prf:
/usr/local/Qt4.6/mkspecs/features/qt.prf:
/usr/local/Qt4.6/mkspecs/features/unix/thread.prf:
/usr/local/Qt4.6/mkspecs/features/moc.prf:
/usr/local/Qt4.6/mkspecs/features/warn_on.prf:
/usr/local/Qt4.6/mkspecs/features/mac/rez.prf:
/usr/local/Qt4.6/mkspecs/features/mac/sdk.prf:
/usr/local/Qt4.6/mkspecs/features/resources.prf:
/usr/local/Qt4.6/mkspecs/features/uic.prf:
/usr/local/Qt4.6/mkspecs/features/yacc.prf:
/usr/local/Qt4.6/mkspecs/features/lex.prf:
/Library/Frameworks/QtSql.framework/QtSql.prl:
/Library/Frameworks/QtCore.framework/QtCore.prl:
/Library/Frameworks/QtXml.framework/QtXml.prl:
/Library/Frameworks/QtGui.framework/QtGui.prl:
/Library/Frameworks/QtNetwork.framework/QtNetwork.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/local/Qt4.6/mkspecs/macx-g++ -macx -o Makefile smugup.pro

smugup.app/Contents/PkgInfo: 
	@$(CHK_DIR_EXISTS) smugup.app/Contents || $(MKDIR) smugup.app/Contents 
	@$(DEL_FILE) smugup.app/Contents/PkgInfo
	@echo "APPL????" >smugup.app/Contents/PkgInfo
smugup.app/Contents/Resources/empty.lproj: 
	@$(CHK_DIR_EXISTS) smugup.app/Contents/Resources || $(MKDIR) smugup.app/Contents/Resources 
	@touch smugup.app/Contents/Resources/empty.lproj
	
smugup.app/Contents/Info.plist: 
	@$(CHK_DIR_EXISTS) smugup.app/Contents || $(MKDIR) smugup.app/Contents 
	@$(DEL_FILE) smugup.app/Contents/Info.plist
	@sed -e "s,@ICON@,,g" -e "s,@EXECUTABLE@,smugup,g" -e "s,@TYPEINFO@,????,g" /usr/local/Qt4.6/mkspecs/macx-g++/Info.plist.app >smugup.app/Contents/Info.plist
dist: 
	@$(CHK_DIR_EXISTS) build/smugup1.0.0 || $(MKDIR) build/smugup1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) build/smugup1.0.0/ && $(COPY_FILE) --parents smugmug/WebService.h LoginDialog.h LogDialog.h MainWindow.h build/smugup1.0.0/ && $(COPY_FILE) --parents smugup.qrc build/smugup1.0.0/ && $(COPY_FILE) --parents smugmug/WebService.cpp LoginDialog.cpp LogDialog.cpp MainWindow.cpp md5/md5_util.cpp md5/md5.c main.cpp build/smugup1.0.0/ && $(COPY_FILE) --parents LoginDialog.ui LogDialog.ui MainWindow.ui build/smugup1.0.0/ && (cd `dirname build/smugup1.0.0` && $(TAR) smugup1.0.0.tar smugup1.0.0 && $(COMPRESS) smugup1.0.0.tar) && $(MOVE) `dirname build/smugup1.0.0`/smugup1.0.0.tar.gz . && $(DEL_FILE) -r build/smugup1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) -r smugup.app
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: build/moc_WebService.cpp build/moc_LoginDialog.cpp build/moc_LogDialog.cpp build/moc_MainWindow.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) build/moc_WebService.cpp build/moc_LoginDialog.cpp build/moc_LogDialog.cpp build/moc_MainWindow.cpp
build/moc_WebService.cpp: smugmug/WebService.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ smugmug/WebService.h -o build/moc_WebService.cpp

build/moc_LoginDialog.cpp: build/ui_LoginDialog.h \
		LoginDialog.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ LoginDialog.h -o build/moc_LoginDialog.cpp

build/moc_LogDialog.cpp: build/ui_LogDialog.h \
		LogDialog.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ LogDialog.h -o build/moc_LogDialog.cpp

build/moc_MainWindow.cpp: build/ui_MainWindow.h \
		MainWindow.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ MainWindow.h -o build/moc_MainWindow.cpp

compiler_rcc_make_all: qrc_smugup.cpp
compiler_rcc_clean:
	-$(DEL_FILE) qrc_smugup.cpp
qrc_smugup.cpp: smugup.qrc \
		images/album-icon.png \
		images/album.png \
		images/nav_blue.png \
		images/category-icon.png \
		images/nav.png \
		images/connect.png \
		images/image-icon.png \
		images/disconnect.png \
		images/log-icon.png \
		images/category.png \
		images/home.png \
		images/sync-icon.png \
		images/subcategory-icon.png \
		images/subcategory.png
	/Developer/Tools/Qt/rcc -name smugup smugup.qrc -o qrc_smugup.cpp

compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all: build/ui_LoginDialog.h build/ui_LogDialog.h build/ui_MainWindow.h
compiler_uic_clean:
	-$(DEL_FILE) build/ui_LoginDialog.h build/ui_LogDialog.h build/ui_MainWindow.h
build/ui_LoginDialog.h: LoginDialog.ui
	/Developer/Tools/Qt/uic LoginDialog.ui -o build/ui_LoginDialog.h

build/ui_LogDialog.h: LogDialog.ui
	/Developer/Tools/Qt/uic LogDialog.ui -o build/ui_LogDialog.h

build/ui_MainWindow.h: MainWindow.ui
	/Developer/Tools/Qt/uic MainWindow.ui -o build/ui_MainWindow.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_rcc_clean compiler_uic_clean 

####### Compile

build/WebService.o: smugmug/WebService.cpp smugmug/SmugMug.h \
		smugmug/WebService.h \
		smugmug/SmugObject.h \
		smugmug/Image.h \
		smugmug/Album.h \
		smugmug/Category.h \
		smugmug/SubCategory.h \
		md5/md5_util.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/WebService.o smugmug/WebService.cpp

build/LoginDialog.o: LoginDialog.cpp LoginDialog.h \
		build/ui_LoginDialog.h \
		smugmug/SmugMug.h \
		smugmug/WebService.h \
		smugmug/SmugObject.h \
		smugmug/Image.h \
		smugmug/Album.h \
		smugmug/Category.h \
		smugmug/SubCategory.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/LoginDialog.o LoginDialog.cpp

build/LogDialog.o: LogDialog.cpp LogDialog.h \
		build/ui_LogDialog.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/LogDialog.o LogDialog.cpp

build/MainWindow.o: MainWindow.cpp MainWindow.h \
		build/ui_MainWindow.h \
		LoginDialog.h \
		build/ui_LoginDialog.h \
		smugmug/SmugMug.h \
		smugmug/WebService.h \
		smugmug/SmugObject.h \
		smugmug/Image.h \
		smugmug/Album.h \
		smugmug/Category.h \
		smugmug/SubCategory.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/MainWindow.o MainWindow.cpp

build/md5_util.o: md5/md5_util.cpp md5/md5.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/md5_util.o md5/md5_util.cpp

build/md5.o: md5/md5.c md5/md5.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o build/md5.o md5/md5.c

build/main.o: main.cpp MainWindow.h \
		build/ui_MainWindow.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/main.o main.cpp

build/moc_WebService.o: build/moc_WebService.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_WebService.o build/moc_WebService.cpp

build/moc_LoginDialog.o: build/moc_LoginDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_LoginDialog.o build/moc_LoginDialog.cpp

build/moc_LogDialog.o: build/moc_LogDialog.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_LogDialog.o build/moc_LogDialog.cpp

build/moc_MainWindow.o: build/moc_MainWindow.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/moc_MainWindow.o build/moc_MainWindow.cpp

build/qrc_smugup.o: qrc_smugup.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o build/qrc_smugup.o qrc_smugup.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

