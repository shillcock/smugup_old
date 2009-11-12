#ifndef __SMUGMUG_WEBSERVICE_H_INCL__
#define __SMUGMUG_WEBSERVICE_H_INCL__

#include <QObject>
#include <QString>
#include <QUrl>
#include <QHash>
#include <QHttp>
#include <QBuffer>

class QDomElement;
class QUrl;
class QHttpResponseHeader;
class QPixmap;


namespace SmugMug {

//   typedef QObject Category;
//   typedef QObject SubCategory;
//   typedef QObject Album;
//   typedef QObject Image;

   typedef QHash<QString, QString> Attributes;


   class WebService : public QObject {

      Q_OBJECT

      public:
         
         enum Method {
            
            mLogin,
            mUserGetTree,
            mAlbumGetAll,
            mAlbumGetInfo,
            mImageGetAll,
            mImageGetInfo,
            mCategoryGetAll,
            mSubCategoryGetAll
         };
         
         WebService (QObject *parent = 0);
         ~WebService ();

         QString sessionId () const { return _sessionId; }
         QString userName () const { return _userName; }
         QString userId () const { return _userId; }
//         QString passwordHash () const { return _passwordHash; }
         bool loggedIn () const { return _loggedIn; }

      public slots:
         void loginWithPassword (const QString &Name, const QString &Password);

         void loginWithHash ();
         void loginWithHash (const QString &UserId, const QString &Hash);

         void logout ();

         // Users
         void userGetTree (bool heavy = false);

         // Albums
         void albumGetAll (bool heavy = false);
         void albumGetInfo (const QString &AlbumId, const QString &Password = QString::null);
         
         // Images
         void imageGetAll (const QString &AlbumId, bool heavy = false, const QString &Password = QString::null);
         void imageGetInfo (const QString &ImageId, const QString &Password = QString::null);

         void imageUpload (
            const QString &AlbumId,
            const QString &FileName,
            const QString &Caption = QString::null);
            
         void imageDownload (const QString &Url);

         // Categories
         void categoryGetAll ();
               
         // SubCategories
         void subCategoryGetAll ();
         
      signals:
         void actionStarted ();
         void dataAvailable (const QByteArray &Data);
         void dataReadProgress (int done, int total);
         void dataSendProgress (int done, int total);
         void actionFinished ();

         void loginStarted ();
         void loginFinished (bool error);

         void logoutStarted ();
         void logoutFinished ();

         void stateChanged (const QString &);

         // Users
         void userGetTreeStarted ();
         void userGetTreeFinished (bool error);
         void userGetTransferStatsStarted ();
         void userGetTransferStatsFinished (bool error);

         // Albums
         void albumGetAllStarted ();
         void albumGetAllFinished (bool error);
         void albumGetInfoStarted ();
         void albumGetInfoFinished (bool error);
         void albumCreateStarted ();
         void albumCreateFinished (bool error);
         void albumChangeSettingsStarted ();
         void albumChangeSettingsFinshed (bool error);
         void albumDeleteStarted ();
         void albumDeleteFinished (bool error);
         void albumGetStatsStarted ();
         void albumGetStatsFinished (bool error);
         
         void albumUpdate (const QString &AlbumId, const SmugMug::Attributes &Attr);

         // Images
         void imageGetAllStart ();
         void imageGetAllFinished (bool error);
         void imageGetUrlsStarted ();
         void imageGetUrlsFinsihed (bool error);
         void imageGetInfoStarted ();
         void imageGetInfoFinished (bool error);
         void imageGetExifStarted ();
         void imageGetExifFinished (bool error);
         void imageChangeSettingsStarted ();
         void imageChangeSettingsFinished (bool error);
         void imageChangePositionStarted ();
         void imageChangePositionFinished (bool error);
         void imageDeleteStarted ();
         void imageDeleteFinished (bool error);
         void imageGetStatsStarted ();
         void imageGetStatsFinished (bool error);
         void imageUploadStarted ();
         void imageUploadFinished (bool error);
         void imageDownloadStarted ();
         void imageDownloadFinished (const QPixmap &Image);

         void imageUpdate (const QString &ImageId, const SmugMug::Attributes &Attr);

         // Categories
         void categoryGetAllStarted ();
         void categoryGetAllFinished (bool error);
         void categoryCreateStarted ();
         void categoryCreateFinished (bool error);
         void categoryDeleteStarted ();
         void categoryDeleteFinished (bool error);
         void categoryRenameStarted ();
         void categoryRenameFinished (bool error);

         void categoryUpdate (
            const QString &CategoryId,
            const QString &CategoryName);

         // SubCategories
         void subCategoryGetStarted ();
         void subCategoryGetFinishd (bool error);
         void subCategoryGetAllStarted ();
         void subCategoryGetAllFinished (bool error);
         void subCategoryCreateStarted ();
         void subCategoryCreateFinished (bool error);
         void subCategoryDeleteStarted ();
         void subCategoryDeleteFinished (bool error);
         void subCategoryRenameStarted ();
         void subCategoryRenameFinished (bool error);
         
         void subCategoryUpadte (
            const QString &CategoryID,
            const QString &SubCategoryId,
            const QString &SubCategoryName);

      protected slots:
         void _slotHttpDataReadProgress (int done, int total);
         void _slotHttpDataSendProgress (int done, int total);
         void _slotHttpReadyRead (const QHttpResponseHeader &resp);
         void _slotHttpRequestStarted (int id);
         void _slotHttpRequestFinished (int id, bool error);
         void _slotHttpRequestDone (bool error);
         void _slotHttpStateChanged (int state);

         void _slotDownloadHttpDataReadProgress (int done, int total);
         void _slotDownloadHttpReadyRead (const QHttpResponseHeader &resp);
         void _slotDownloadHttpRequestFinished (int id, bool error);

      protected:
         void _connectHttp (QHttp *http);

         int _httpGet (const QUrl &Url, QIODevice *to = 0);

         int _httpRequest (
            const QUrl &Url,
            const QHttpRequestHeader &Header,
            const QByteArray &Data);

         void _logHttpError (const QString &Msg);

         bool _isResponseValid (QDomElement &root);
         void _responseFailed (QDomElement &root);
         void _handleResponse (int id, QDomElement &root, bool error);

         void _setProperty (QObject &obj, QLatin1String &tag, QDomElement &elem);

         // Login
         void _parseLogin (QDomElement &root);

         // Users
         void _parseUserGetTree (QDomElement &root);
         void _parseUserGetTreeCategory (QDomElement &root);
         void _parseUserGetTreeSubCategory (const QString &CatId, QDomElement &root);
         void _parseUserGetTreeAlbum (const QString &CatId, const QString &SubCatId, QDomElement &root);

         // Albums
         void _parseAlbumGetAll (QDomElement &root);
         void _parseAlbumGetInfo (QDomElement &root);

         // Images
         void _parseImage (QDomElement &root);
         void _parseImageGetAll (QDomElement &root);
         void _parseImageGetInfo (QDomElement &root);
         void _parseImageUpload (QDomElement &root);

         // Category
         void _parseCategoryGetAll (QDomElement &root);
         
         // SubCategory
         void _parseSubCategoryGetAll (QDomElement &root);
         
         // Http
         void _uploadHttpPut (
            const QString &AlbumId,
            const QString &FileName,
            const QString &Caption = QString::null);

         void _uploadMultipartHttpPost (
            const QString &AlbumId,
            const QString &FileName,
            const QString &Caption = QString::null);

      protected:
         QString _sessionId;
         QString _userName;
         QString _userId;
         QString _passwordHash;

         bool _loggedIn;

         QHttp _http;
         QHttp _https;
         bool _ssl;

         QByteArray _dataIn;
         QByteArray _dataOut;

         int _idLogin;
         int _idLogout;
         int _idUserGetTree;
         int _idAlbumGetAll;
         int _idAlbumGetInfo;
         int _idImageGetAll;
         int _idImageGetInfo;
         int _idImageUpload;
         int _idCategoryGetAll;
         int _idSubCategoryGetAll;

         QHttp _downloadHttp;
         QBuffer _downloadBuffer;
         QByteArray _downloadData;
         int _downloadId;
         bool _downloadAborted;

         QHash<QString, QString> _albumTable;
         QHash<QString, QString> _categoryTable;
         QHash<QString, QString> _subCategoryTable;
   };
};

#endif // __SMUGMUG_WEBSERVICE_H_INCL__
