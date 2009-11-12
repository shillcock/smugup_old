/****************************************************************************
**
** Copyright (C) 2003-2006 Trolltech ASA. All rights reserved.
**
** This file is part of a Qt Solutions component.
**
** Licensees holding valid Qt Solutions licenses may use this file in
** accordance with the Qt Solutions License Agreement provided with the
** Software.
**
** See http://www.trolltech.com/products/qt/addon/solutions/ 
** or email sales@trolltech.com for information about Qt Solutions
** License Agreements.
**
** Contact info@trolltech.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef QTSSLSOCKET_H
#define QTSSLSOCKET_H

#include <QtNetwork/QTcpSocket>

#if defined(Q_WS_WIN)
#  if !defined(QT_QTSSLSOCKET_EXPORT) && !defined(QT_QTSSLSOCKET_IMPORT)
#    define QT_QTSSLSOCKET_EXPORT
#  elif defined(QT_QTSSLSOCKET_IMPORT)
#    if defined(QT_QTSSLSOCKET_EXPORT)
#      undef QT_QTSSLSOCKET_EXPORT
#    endif
#    define QT_QTSSLSOCKET_EXPORT __declspec(dllimport)
#  elif defined(QT_QTSSLSOCKET_EXPORT)
#    undef QT_QTSSLSOCKET_EXPORT
#    define QT_QTSSLSOCKET_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTSSLSOCKET_EXPORT
#endif

class QtSslSocketPrivate;
class QT_QTSSLSOCKET_EXPORT QtSslSocket : public QTcpSocket
{
    Q_OBJECT
public:
    enum VerifyResult {
        VerifyOk,
        SelfSigned,
        Expired,
        NotYetValid,
        UnableToVerify
    };
    enum Mode {
        Client,
        Server
    };

    QtSslSocket(Mode mode = Client, QObject *parent = 0);
    ~QtSslSocket();

    void connectToHost(const QString &hostName, quint16 port,
		       OpenMode openMode = QIODevice::ReadWrite);
    void disconnectFromHost();

    void setPathToCertificate(const QString &path);
    QString pathToCertificate() const;

    void setPathToPrivateKey(const QString &path);
    QString pathToPrivateKey() const;

    void setPathToCACertFile(const QString &path);
    QString pathToCACertFile() const;

    void setPathToCACertDir(const QString &path);
    QString pathToCACertDir() const;

    void setCiphers(const QString &ciphers);
    QString ciphers() const;

    QString localCertificate() const;
    QString peerCertificate() const;

    qint64 bytesAvailable() const;
    qint64 bytesToWrite() const;
    bool canReadLine() const;
    void close();
    bool isSequential() const;

    bool atEnd() const;

    bool waitForReadyRead(int msecs = 30000);
    bool waitForBytesWritten(int msecs = 30000);

    QTcpSocket *socket() const;
    void setSocket(QTcpSocket *socket);

public slots:
    bool sslConnect();
    bool sslAccept();

signals:
    void connectionVerificationDone(QtSslSocket::VerifyResult result,
                                    bool hostNameMatched,
                                    const QString &description);

protected slots:
    void connectToHostImplementation(const QString &hostName, quint16 port,
                                     OpenMode openMode);
    void disconnectFromHostImplementation();

protected:
    qint64 readData(char *data, qint64 maxSize);
    qint64 readLineData(char *data, qint64 maxSize);
    qint64 writeData(const char *data, qint64 size);

private slots:
    void handleSocketError(QAbstractSocket::SocketError error);
    void readFromSocket();
    void writeToSocket();
    void socketBytesWritten(qint64 bytes);

private:
    QString verifyErrorString(int err);
    bool sslAcceptOrConnect(bool accept);
    bool initSsl();

    QtSslSocketPrivate *d;
    friend class QtSslSocketPrivate;
};

#endif
