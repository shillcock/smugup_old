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

#include "Loqqer.h"

#include <QSysInfo>
#include <QFile>

using namespace std;

CLoqqer gLogger;

/******************************************************************************
    Init
******************************************************************************/
void
CLoqqer::Init(
    QString sFilename,
    bool bOverwrite)
{
    // Trim file size if above 500k
    QFile qf( sFilename );
    if ( qf.size() > 500000 )
    {
        ifstream inFile( qPrintable(sFilename) );
        inFile.seekg( static_cast<streamoff>( qf.size() - 400000 ) );
        istreambuf_iterator<char> bufReader( inFile ), end;
        string sFile;
        sFile.reserve( 400005 );
        sFile.assign( bufReader, end );
        inFile.close();
        ofstream outFile( qPrintable(sFilename) );
        outFile << sFile << flush;
        outFile.close();
    }

    ios::openmode flags = ios::out;
    if (!bOverwrite)
    {
        flags |= ios::app;
    }
    mFileOut.open(qPrintable(sFilename), flags);

    if (!mFileOut)
    {
        qWarning() << "Could not open log file" << sFilename;
        return;
    }

    SetLevel(2);

    QString osVer = GetOSVersion();
    // TODO: read from registry when new settingsmgr ready
    LOG(1, "************************************* STARTUP ********************************************\n");
    LOG(1, "OS: " << osVer << "\n");
}

/******************************************************************************
    GetOSVersion
******************************************************************************/
QString
CLoqqer::GetOSVersion()
{
    QString version;

    #ifdef Q_WS_WIN
    switch (QSysInfo::WindowsVersion)
    {
        case QSysInfo::WV_32s:        version = "Windows 3.1 with Win32s"; break;
        case QSysInfo::WV_95:         version = "Windows 95"; break;
        case QSysInfo::WV_98:         version = "Windows 98"; break;
        case QSysInfo::WV_Me:         version = "Windows Me"; break;
        case QSysInfo::WV_DOS_based:  version = "MS-DOS-based Windows"; break;

        case QSysInfo::WV_NT:         version = "Windows NT"; break;
        case QSysInfo::WV_2000:       version = "Windows 2000"; break;
        case QSysInfo::WV_XP:         version = "Windows XP"; break;
        case QSysInfo::WV_2003:       version = "Windows Server 2003"; break;
        case QSysInfo::WV_VISTA:      version = "Windows Vista"; break;
        case QSysInfo::WV_NT_based:   version = "NT-based Windows"; break;

        case QSysInfo::WV_CE:         version = "Windows CE"; break;
        case QSysInfo::WV_CENET:      version = "Windows CE.NET"; break;
        case QSysInfo::WV_CE_based:   version = "CE-based Windows"; break;

        default:                      version = "Qt doesn't know"; break;
    }
    #elif defined Q_WS_MAC
    switch (QSysInfo::MacintoshVersion)
    {
        case QSysInfo::MV_Unknown:    version = "Unknown Mac"; break;
        case QSysInfo::MV_9:          version = "Mac OS 9"; break;
        case QSysInfo::MV_10_0:       version = "Mac OS X 10.0"; break;
        case QSysInfo::MV_10_1:       version = "Mac OS X 10.1"; break;
        case QSysInfo::MV_10_2:       version = "Mac OS X 10.2"; break;
        case QSysInfo::MV_10_3:       version = "Mac OS X 10.3"; break;
        case QSysInfo::MV_10_4:       version = "Mac OS X 10.4"; break;

        default:                      version = "Qt doesn't know"; break;
    }
    #else
    version = "Unix/Linux";
    #endif // Q_WS_WIN

    return version;
}
