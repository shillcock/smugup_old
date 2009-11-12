# Pri file for QtSslSocket. Include this .pri file into your .pro file
# with the include(<path>) statement.

DEPENDPATH += $$PWD

unix {
    # Unix users, if your SSL libraries or include files are not
    # found, insert your paths here.
    exists(/usr/local/include):INCLUDEPATH += /usr/local/include
    exists(/usr/local/ssl/include):INCLUDEPATH += /usr/local/ssl/include
    exists(/usr/local/openssl/include):INCLUDEPATH += /usr/local/openssl/include
    exists(/opt/ssl/include):INCLUDEPATH += /opt/ssl/include
    exists(/opt/openssl/include):INCLUDEPATH += /opt/openssl/include
    exists(/usr/local/lib):LIBS += -L/usr/local/lib
    exists(/usr/local/ssl/lib):LIBS += -L/usr/local/ssl/lib
    exists(/usr/local/openssl/lib):LIBS += -L/usr/local/openssl/lib
    exists(/opt/ssl/lib):LIBS += -L/opt/ssl/lib
    exists(/opt/openssl/lib):LIBS += -L/opt/openssl/lib

    LIBS += -lssl -lcrypto
}

win32 {
    OPENSSL_HOME = C:\devel\libs\openssl
    # Windows users, if your SSL libraries or include files are not
    # found, insert your paths here.
    exists($$OPENSSL_HOME\include):INCLUDEPATH += $$OPENSSL_HOME\include
    exists($$OPENSSL_HOME\lib\VC):LIBS += -L$$OPENSSL_HOME\lib\VC
    win32-msvc*:exists($$OPENSSL_HOME\lib\VC):LIBS += -L$$OPENSSL_HOME\lib\VC
    LIBS += ssleay32MT.lib libeay32MT.lib
}

INCLUDEPATH += $$PWD
SOURCES += $$PWD/qtsslsocket.cpp
HEADERS += $$PWD/qtsslsocket.h

QT += network

win32:contains(TEMPLATE, lib):contains(CONFIG, shared) {
    DEFINES += QT_QTSSLSOCKET_EXPORT=__declspec(dllexport)
}
