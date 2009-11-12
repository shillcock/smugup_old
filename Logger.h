/***************************************************************************
 *   Copyright (C) 2005 - 2006 by                                          *
 *      Christian Muehlhaeuser, Last.fm Ltd <chris@last.fm>                *
 *      Erik Jaelevik, Last.fm Ltd <erik@last.fm>                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02111-1307, USA.          *
 ***************************************************************************/

#ifndef LOQQER_H
#define LOQQER_H

#include <QMutex>
#include <QThread>
#include <QString>
#include <QDebug>
#include <QtGlobal>

#include <string>
#include <fstream>
#include <ios>
#include <iomanip>
#include <ctime>

#ifdef Q_WS_WIN
    #define _FUNCTION_AND_LINE __FUNCTION__ << "(" << __LINE__ << ")" << " - "
#else
    #define _FUNCTION_AND_LINE ""
#endif

// Global LOG macro
#define LOG(level, msg)                                                      \
{                                                                            \
    QMutexLocker loggerLock(&gLogger.mMutex);                                \
    if (gLogger.mFileOut && (level) <= gLogger.GetLevel())                   \
    {                                                                        \
        gLogger.mFileOut <<                                                  \
            gLogger.GetTime() << " - " <<                                    \
            std::setw(4) << QThread::currentThreadId() << " - " <<           \
            _FUNCTION_AND_LINE <<                                            \
            "L" << level << "\n  " << msg << std::endl;                      \
    }                                                                        \
}

#define LOGL(level, msg) LOG(level, msg << "\n")

// Global VERFIY macro
// Wrap these around statements that should always be exeucted to verify that
// they're always true (connects are a good example)
#define VERIFY(statement)                                                    \
{                                                                            \
    bool success = (statement);                                              \
    Q_ASSERT( success );                                                     \
}

class CLoqqer;
extern CLoqqer gLogger;

/*************************************************************************/ /**
    Extra inserter to handle QStrings.
******************************************************************************/
inline std::ostream& 
operator<<(std::ostream& os, const QString& qs)
{
    os << qs.toStdString();
    return os;
}

/*************************************************************************/ /**
    Simple logging class
******************************************************************************/
class CLoqqer
{
public:

    std::ofstream mFileOut;
    QMutex mMutex;

    /*********************************************************************/ /**
        Ctor
    **************************************************************************/
    CLoqqer() : mnLevel(2) { }

    /*********************************************************************/ /**
        Dtor
    **************************************************************************/
    virtual
    ~CLoqqer()
    {
        mFileOut.close();
    }

    /*********************************************************************/ /**
        Initalises the logger.

        @param[in] sFilename The file to log to.
        @param[in] bOverwrite If true, the file is wiped before starting.
    **************************************************************************/
    void
    Init(
        QString sFilename,
        bool bOverwrite = true);

    /*********************************************************************/ /**
        Sets debug level.

        1 = critical
        2 = warning
        3 = info
        4 = detail
    **************************************************************************/
    void
    SetLevel(
        int nLevel) { mnLevel = nLevel; }

    /*********************************************************************/ /**
        Gets debug level.
    **************************************************************************/
    int
    GetLevel() { return mnLevel; }

    /*********************************************************************/ /**
        Returns formatted time string.
    **************************************************************************/
    std::string
    GetTime()
    {
        time_t now;
        time(&now);
        struct tm* tmnow;
        tmnow = gmtime(&now);
        char acTmp[128];
        strftime(acTmp, 127, "%y%m%d %H:%M:%S", tmnow);
        std::string sTime(acTmp);
        return sTime;
    }

    /*********************************************************************/ /**
        Returns a formatted string with the OS version.
    **************************************************************************/
    QString
    GetOSVersion();

private:

    int mnLevel;
};

#endif // LOGGER_H
