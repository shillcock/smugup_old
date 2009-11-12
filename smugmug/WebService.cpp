#include "SmugMug.h"
#include "../md5/md5_util.h"
#include <QtGUi>
#include <QtNetwork>
#include <QtXml>

static const QString ApiKey ("gbfs9t6XZtuPQSScjAzrKclpu2qEoG13");
static const QString RestEndpoint ("http://api.smugmug.com/services/api/rest/1.2.1/");
static const QString SecureRestEndpoint ("https://api.smugmug.com/services/api/rest/1.2.1/");
static const QString UploadEndpoint ("http://upload.smugmug.com/");


SmugMug::WebService::WebService (QObject *parent) :
         QObject (parent),
         _sessionId (QString::null),
         _userName (QString::null),
         _userId (QString::null),
         _passwordHash (QString::null),
         _loggedIn (false),
         _http (this),
         _https (this),
         _ssl (false),
         _dataIn (),
         _dataOut (),
         _idLogin (0),
         _idLogout (0),
         _idUserGetTree (0),
         _idAlbumGetAll (0),
         _idAlbumGetInfo (0),
         _idImageGetAll (0),
         _idImageGetInfo (0),
         _idImageUpload (0),
         _idCategoryGetAll (0),
         _idSubCategoryGetAll (0),
         _downloadHttp (this),
         _downloadData (),
         _downloadBuffer (&_downloadData, this),
         _downloadId (0),
         _downloadAborted (false),
         _albumTable (),
         _categoryTable (),
         _subCategoryTable () {

   _connectHttp (&_http);

//   QtSslSocket *sslSocket = new QtSslSocket (QtSslSocket::Client, this);
//   if (sslSocket) {
//
//      _https.setSocket (sslSocket);
//      _connectHttp (&_https);
//   }

   connect(&_downloadHttp, SIGNAL(requestFinished(int, bool)),
      this, SLOT(_slotDownloadHttpRequestFinished(int, bool)));

   connect(&_downloadHttp, SIGNAL(dataReadProgress(int, int)),
      this, SLOT(_slotDownloadHttpDataReadProgress(int, int)));

   connect(&_downloadHttp, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
      this, SLOT(_slotDownloadHttpReadyRead(const QHttpResponseHeader &)));
  
   QSettings settings;
   settings.beginGroup (QLatin1String ("WebService"));
   _userId = settings.value("userId", "").toString ();
   _userName = settings.value(QLatin1String ("userName"), "").toString ();
   _passwordHash = settings.value("passwordHash", "").toString ();
   settings.endGroup ();
}


SmugMug::WebService::~WebService () {

   _http.abort ();
   _https.abort ();
   _downloadHttp.abort ();
}



void
SmugMug::WebService::loginWithPassword (const QString &Name, const QString &Password) {

   if (!_idLogin) {

      _userName = Name;

      QUrl url (SecureRestEndpoint);
      url.setPort (443);
      url.addQueryItem ("method", "smugmug.login.withPassword");
      url.addQueryItem ("EmailAddress", Name);
      url.addQueryItem ("Password", Password);
      url.addQueryItem ("APIKey", ApiKey);

      _idLogin = _httpGet (url);
qDebug () << "loginWithPassword:" << _idLogin << url.toString ();

      if (_idLogin) { emit loginStarted (); }
   }
}


void
SmugMug::WebService::loginWithHash () {

   if (!_userId.isEmpty () && !_passwordHash.isEmpty ()) {
      
      loginWithHash (_userId, _passwordHash);
   }
}


void
SmugMug::WebService::loginWithHash (const QString &UserId, const QString &Hash) {

   if (!_idLogin) {

      QUrl url (SecureRestEndpoint);
      url.setPort (443);
      url.addQueryItem ("method", "smugmug.login.withHash");
      url.addQueryItem ("UserID", UserId);
      url.addQueryItem ("PasswordHash", Hash);
      url.addQueryItem ("APIKey", ApiKey);

      _idLogin = _httpGet (url);
qDebug () << "loginWithHash:" << _idLogin << url.toString ();

      if (_idLogin) { emit loginStarted (); }
   }
}


void
SmugMug::WebService::logout () {

   if (!_idLogout) {

      QUrl url (RestEndpoint);
      url.addQueryItem ("method", "smugmug.logout");
      url.addQueryItem ("SessionID", _sessionId);

      _idLogout = _httpGet (url);
qDebug () << "logout:" << _idLogout << url.toString ();

      if (_idLogout) { emit logoutStarted (); }
   }
}


void
SmugMug::WebService::userGetTree (bool heavy) {

   if (!_idUserGetTree) {

      QUrl url (RestEndpoint);
      url.addQueryItem ("method", "smugmug.users.getTree");
      url.addQueryItem ("SessionID", _sessionId);
      url.addQueryItem ("Heavy", heavy ? "1" : "0");

      _idUserGetTree = _httpGet (url);
qDebug () << "userGetTree:" << _idUserGetTree << url.toString ();

      if (_idUserGetTree) { emit userGetTreeStarted (); }
   }
}


void
SmugMug::WebService::albumGetAll (bool heavy) {

   if (!_idAlbumGetAll) {

      QUrl url (RestEndpoint);
      url.addQueryItem ("method", "smugmug.albums.get");
      url.addQueryItem ("SessionID", _sessionId);
      url.addQueryItem ("Heavy", heavy ? "1" : "0");

      _idAlbumGetAll = _httpGet (url);
qDebug () << "albumGetAll:" << _idAlbumGetAll << url.toString ();

      if (_idAlbumGetAll){ emit albumGetAllStarted (); }
   }
}


void
SmugMug::WebService::albumGetInfo (const QString &AlbumId, const QString &Password) {

   if (!_idAlbumGetInfo) {

      QUrl url (RestEndpoint);
      url.addQueryItem ("method", "smugmug.albums.getInfo");
      url.addQueryItem ("SessionID", _sessionId);
      url.addQueryItem ("AlbumID", AlbumId);

      if (!Password.isEmpty ()) {

         url.addQueryItem ("Password", Password);
      }

      _idAlbumGetInfo = _httpGet (url);
qDebug () << "albumGetInfo:" << _idAlbumGetInfo << url.toString ();

      if (_idAlbumGetInfo) { emit albumGetInfoStarted (); }
   }
}


void
SmugMug::WebService::imageGetAll (const QString &AlbumId, bool heavy, const QString &Password) {

   if (!_idImageGetAll) {

      QUrl url (RestEndpoint);
      url.addQueryItem ("method", "smugmug.images.get");
      url.addQueryItem ("SessionID", _sessionId);
      url.addQueryItem ("AlbumID", AlbumId);
      url.addQueryItem ("Heavy", heavy ? "1" : "0");
      
      if (!Password.isEmpty ()) {

         url.addQueryItem ("Password", Password);
      }

      _idImageGetAll = _httpGet (url);
qDebug () << "imageGetAll:" << _idImageGetAll << url.toString ();

      if (_idImageGetAll) { emit imageGetInfoStarted (); }
   }
}


void
SmugMug::WebService::imageGetInfo (const QString &ImageId, const QString &Password) {

   if (!_idImageGetInfo) {

      QUrl url (RestEndpoint);
      url.addQueryItem ("method", "smugmug.images.getInfo");
      url.addQueryItem ("SessionID", _sessionId);
      url.addQueryItem ("ImageID", ImageId);

      if (!Password.isEmpty ()) {

         url.addQueryItem ("Password", Password);
      }

      _idImageGetInfo = _httpGet (url);
qDebug () << "imageGetInfo:" << _idImageGetInfo << url.toString ();

      if (_idImageGetInfo) { emit imageGetInfoStarted (); }
   }
}


void
SmugMug::WebService::imageUpload (
      const QString &AlbumId,
      const QString &FileName,
      const QString &Caption) {

#if 1
   _uploadHttpPut (AlbumId, FileName, Caption);
#else
   _uploadMultipartHttpPost (AlbumId, FileName, Caption);
#endif

   if (_idImageUpload) {
      
      emit imageUploadStarted ();
   }
}


void
SmugMug::WebService::imageDownload (const QString &ImageUrl) {

   if (!_downloadId) {
      
      if (_downloadBuffer.open (QIODevice::WriteOnly)) {

         QUrl url (ImageUrl);

         _downloadAborted = false;
         _downloadHttp.setHost (url.host (), url.port (80));
         _downloadId = _downloadHttp.get (url.path (), &_downloadBuffer);

qDebug () << "imageDownload:" << _downloadId << url.toString ();

         if (_downloadId) {

            emit imageDownloadStarted ();
         }
      }
   }
}


void
SmugMug::WebService::categoryGetAll () {

   if (!_idCategoryGetAll) {

      QUrl url (RestEndpoint);
      url.addQueryItem ("method", "smugmug.categories.get");
      url.addQueryItem ("SessionID", _sessionId);

      _idCategoryGetAll = _httpGet (url);
qDebug () << "categroyGetAll:" << _idCategoryGetAll << url.toString ();

      if (_idCategoryGetAll) { emit categoryGetAllStarted (); }
   }
}


void
SmugMug::WebService::subCategoryGetAll () {

   if (!_idSubCategoryGetAll) {

      QUrl url (RestEndpoint);
      url.addQueryItem ("method", "smugmug.subcategories.getAll");
      url.addQueryItem ("SessionID", _sessionId);

      _idSubCategoryGetAll = _httpGet (url);
qDebug () << "subCategroyGetAll:" << _idSubCategoryGetAll << url.toString ();

      if (_idSubCategoryGetAll) { emit subCategoryGetAllStarted (); }
   }
}


void
SmugMug::WebService::_connectHttp (QHttp *http) {

   if (http) {

      connect (
         http, SIGNAL (requestStarted (int)),
         this, SLOT (_slotHttpRequestStarted (int)));

      connect (
         http, SIGNAL (dataReadProgress (int, int)),
         this, SLOT (_slotHttpDataReadProgress (int, int)));

      connect (
         http, SIGNAL (dataReadProgress (int, int)),
         this, SIGNAL (dataReadProgress (int, int)));

      connect (
         http, SIGNAL (dataSendProgress (int, int)),
         this, SLOT (_slotHttpDataSendProgress (int, int)));

      connect (
         http, SIGNAL (dataSendProgress (int, int)),
         this, SIGNAL (dataSendProgress (int, int)));

      connect (
         http, SIGNAL (readyRead (const QHttpResponseHeader &)),
         this, SLOT (_slotHttpReadyRead (const QHttpResponseHeader &)));

      connect (
         http, SIGNAL (requestFinished (int, bool)),
         this, SLOT (_slotHttpRequestFinished (int, bool)));

      connect (
         http, SIGNAL (done (bool)),
         this, SLOT (_slotHttpRequestDone (bool)));

      connect (
         http, SIGNAL (stateChanged (int)),
         this, SLOT (_slotHttpStateChanged (int)));
   }
}


int
SmugMug::WebService::_httpGet (const QUrl &Url, QIODevice *to) {

   int id = 0;

   if (Url.port (80) == 443) {

      _ssl = true;
      _https.setHost (Url.host (), Url.port (443));
      id = _https.get (Url.toEncoded (), to);
   }
   else {

      _ssl = false;
      _http.setHost (Url.host (), Url.port (80));
      id = _http.get (Url.toEncoded (), to);
   }

   if (id) {

      emit actionStarted ();
qDebug () << "actionStarted: " << id;
   }

   return id;
}


int
SmugMug::WebService::_httpRequest (
      const QUrl &Url,
      const QHttpRequestHeader &Header,
      const QByteArray &Data) {

   int id = 0;

   if (Url.port (80) == 443) {

      _ssl = true;
      _https.setHost (Url.host (), Url.port (80));
      id = _https.request (Header, Data);
   }
   else {

      _ssl = false;
      _http.setHost (Url.host (), Url.port (80));
      id = _http.request (Header, Data);
   }

   if (id) {

      emit actionStarted ();
qDebug () << "actionStarted: " << id;
   }

   return id;
}


void
SmugMug::WebService::_logHttpError (const QString &Msg) {

   QHttp &theHttp = (_ssl ? _https : _http);

qWarning () << "---- Http Error -----------------------------------";

if (!Msg.isEmpty ()) { qWarning () << Msg; }

qWarning () << "Error code:" << theHttp.error () << endl
            << "Error text:" << theHttp.errorString () << endl
            << "Request:" << theHttp.currentRequest ().path () << endl
            << "Bytes returned:" << theHttp.bytesAvailable () << endl
            << "-----------------------------------------------------" << endl;
}


bool
SmugMug::WebService::_isResponseValid (QDomElement &root) {

   bool retVal (false);

   if (root.tagName () == QLatin1String ("rsp")) {
   
      if (root.attribute (QLatin1String ("stat")) == QLatin1String ("ok")) {

         retVal = true;
      }
   }

   return retVal;
}


void
SmugMug::WebService::_responseFailed (QDomElement &root) {

   QDomElement errorElem = root.firstChildElement ("err");

qWarning () << "Response stat = failed";
qWarning () << "Error:" << errorElem.attribute ("code") << "-"
            << errorElem.attribute ("msg");
}


void
SmugMug::WebService::_handleResponse (int id, QDomElement &root, bool error) {

   if (id == _idLogin) {

      if (!error) { _parseLogin (root); }
      _idLogin = 0;
      emit loginFinished (error);
   }
   else if (id == _idLogout) {

      _sessionId = QString::null;
      _loggedIn = false;
      _idLogout = 0;
      emit logoutFinished ();
   }
   else if (id == _idUserGetTree) {

      if (!error) { _parseUserGetTree (root); }
      _idUserGetTree = 0;
      emit userGetTreeFinished (error);
   }
   else if (id == _idAlbumGetAll) {

      if (!error) { _parseAlbumGetAll (root); }
      _idAlbumGetAll = 0;
      emit albumGetAllFinished (error);
   }
   else if (id == _idAlbumGetInfo) {

      if (!error) { _parseAlbumGetInfo (root); }
      _idAlbumGetInfo = 0;
      emit albumGetInfoFinished (error);
   }
   else if (id == _idImageGetAll) {

      if (!error) { _parseImageGetAll (root); }
      _idImageGetAll = 0;
      emit imageGetAllFinished (error);
   }
   else if (id == _idImageGetInfo) {

      if (!error) { _parseImageGetInfo (root); }
      _idImageGetInfo = 0;
      emit imageGetInfoFinished (error);
   }
   else if (id == _idImageUpload) {

      if (!error) { _parseImageUpload (root); }
      _idImageUpload = 0;
      emit imageUploadFinished (error);
   }
   else if (id == _idCategoryGetAll) {

      if (!error) { _parseCategoryGetAll (root); }
      _idCategoryGetAll = 0;
      emit categoryGetAllFinished (error);
   }
   else if (id == _idSubCategoryGetAll) {

      if (!error) { _parseSubCategoryGetAll (root); }
      _idSubCategoryGetAll = 0;
      emit subCategoryGetAllFinished (error);
   }
}


void
SmugMug::WebService::_parseLogin (QDomElement &root) {

qDebug () << "_parseLogin";

   QSettings settings;
   settings.beginGroup (QLatin1String ("WebService"));
   settings.setValue (QLatin1String ("userName"), _userName);

   QDomElement loginElem = root.firstChildElement (QLatin1String ("Login"));
   if (!loginElem.isNull ()) {

      if (loginElem.hasAttribute (QLatin1String ("PasswordHash"))) {

         _passwordHash = loginElem.attribute (QLatin1String ("PasswordHash"));
         settings.setValue (QLatin1String ("passwordHash"), _passwordHash);

qDebug () << "PasswordHash:" << _passwordHash;
      }
   }

   QDomNode node = loginElem.firstChild ();
   while (!node.isNull ()) {

      if (node.isElement ()) {

         QDomElement elem = node.toElement ();

         if (elem.tagName () == QLatin1String ("Session")) {

            _sessionId = elem.attribute (QLatin1String ("id"));
qDebug () << "Session id:" << _sessionId;

            _loggedIn = !_sessionId.isEmpty ();
         }
         else if (elem.tagName () == QLatin1String ("User")) {

            _userId = elem.attribute (QLatin1String ("id"));
            settings.setValue (QLatin1String ("userId"), _userId);
qDebug () << "User id:" << _userId;
         }
      }

      node = node.nextSibling ();
   }

   settings.endGroup ();
}


void
SmugMug::WebService::_parseUserGetTree (QDomElement &root) {

   QDomNodeList catList = root.elementsByTagName (QLatin1String ("Category"));
   for (int cix = 0; cix < catList.count (); cix++) {

      QDomElement catElem = catList.item (cix).toElement ();

      _parseUserGetTreeCategory (catElem);
   }
}


void
SmugMug::WebService::_parseUserGetTreeCategory (QDomElement &root) {

   if (root.hasAttribute (QLatin1String ("id"))) {

      QString catId = root.attribute (QLatin1String ("id"));

      if (!_categoryTable.contains (catId)) {

         QString catName = root.attribute (QLatin1String ("Name"));

         _categoryTable.insert (catId, catName);
         emit categoryUpdate (catId, catName);
//qDebug () << "category" << catId << catName;
      }
      
      QDomNodeList subCatList = root.elementsByTagName (QLatin1String ("SubCategory"));
      for (int six = 0; six < subCatList.count (); six++) {

         QDomElement subCatElem = subCatList.item (six).toElement ();

         _parseUserGetTreeSubCategory (catId, subCatElem);
      }

      const QString SubCatId = QLatin1String ("0");

      QDomNodeList albumList = root.elementsByTagName (QLatin1String ("Album"));
      for (int aix = 0; aix < albumList.count (); aix++) {

         QDomElement albumElem = albumList.item (aix).toElement ();

         _parseUserGetTreeAlbum (catId, SubCatId, albumElem);
      }
   }
}


void
SmugMug::WebService::_parseUserGetTreeSubCategory (const QString &CatId, QDomElement &root) {

   if (root.hasAttribute (QLatin1String ("id"))) {

      QString subCatId = root.attribute (QLatin1String ("id"));

      if (!_subCategoryTable.contains (subCatId)) {

         QString subCatName = root.attribute (QLatin1String ("Name"));

         _subCategoryTable.insert (subCatId, subCatName);
//qDebug () << "subCategory" << subCatId << subCatName;
         emit subCategoryUpadte (CatId, subCatId, subCatName);
      }

      QDomNodeList albumList = root.elementsByTagName (QLatin1String ("Album"));
      for (int aix = 0; aix < albumList.count (); aix++) {

         QDomElement albumElem = albumList.item (aix).toElement ();

         _parseUserGetTreeAlbum (CatId, subCatId, albumElem);
      }
   }    
}


void
SmugMug::WebService::_parseUserGetTreeAlbum (const QString &CatId, const QString &SubCatId, QDomElement &root) {

   if (root.hasAttribute (QLatin1String ("id"))) {

      QString albumId = root.attribute (QLatin1String ("id"));

      if (!_albumTable.contains (albumId)) {

         QString albumTitle = root.attribute (QLatin1String ("Title"));

         _albumTable.insert (albumId, albumTitle);

         SmugMug::Attributes attr;
         attr.insert ("Id", albumId);
         attr.insert ("Title", albumTitle);
         attr.insert ("CategoryId", CatId);
         attr.insert ("SubCategoryId", SubCatId);

         emit albumUpdate (albumId, attr);
      }
   }
}


void
SmugMug::WebService::_parseAlbumGetAll (QDomElement &root) {

   QDomNodeList albumList = root.elementsByTagName (QLatin1String ("Album"));
   for (int aix = 0; aix < albumList.count (); aix++) {

      QDomElement albumElem = albumList.item (aix).toElement ();
      if (!albumElem.isNull () && albumElem.hasAttribute (QLatin1String ("id"))) {

         QString albumId = albumElem.attribute (QLatin1String("id"));
         QString albumTitle = albumElem.attribute (QLatin1String("Title"));

         if (!_albumTable.contains (albumId)) {

            _albumTable.insert (albumId, albumTitle);
         }

         QString catId ("0");
         QDomElement elem = root.firstChildElement (QLatin1String ("Category"));
         if (!elem.isNull ()) {

            catId = elem.attribute (QLatin1String("id"));

            if (!_categoryTable.contains (catId)) {

               QString catName = elem.attribute (QLatin1String("Name"));

               _categoryTable.insert (catId, catName);
               emit categoryUpdate (catId, catName);
            }
         }

         QString subCatId ("0");
         elem = root.firstChildElement (QLatin1String ("SubCategory"));
         if (!elem.isNull ()) {

            subCatId = elem.attribute (QLatin1String("id"));

            if (!_subCategoryTable.contains (subCatId)) {

               QString subCatName = elem.attribute (QLatin1String("Name"));

               _subCategoryTable.insert (subCatId, subCatName);
               emit subCategoryUpadte (catId, subCatId, subCatName);
            }
         }

         SmugMug::Attributes attr;
         attr.insert ("Id", albumId);
         attr.insert ("Title", albumTitle);
         attr.insert ("CategoryId", catId);
         attr.insert ("SubCategoryId", subCatId);

         emit albumUpdate (albumId, attr);
      }
   }
}


void
SmugMug::WebService::_parseAlbumGetInfo (QDomElement &root) {

   QDomElement albumElem = root.firstChildElement ("Album");

   if (!albumElem.hasAttribute ("id")) {

      albumElem = albumElem.firstChildElement ("Album");
   }

   if (!albumElem.isNull () && albumElem.hasAttribute ("id")) {

      QString albumId = albumElem.attribute (QLatin1String ("id"));
      if (!albumId.isEmpty ()) {

         SmugMug::Attributes attr;

         attr.insert ("Id", albumId);
         attr.insert ("Title", albumElem.attribute ("Title"));
         attr.insert ("ImageCount", albumElem.attribute ("ImageCount"));
         attr.insert ("Description", albumElem.attribute ("Description"));
         attr.insert ("Keywords", albumElem.attribute ("Keywords"));

         QString catId = QString::null;
         QDomElement catElem = albumElem.firstChildElement ("Category");
         if (!catElem.isNull ()) {

            attr.insert ("CategoryId", catElem.attribute ("id"));
         }

         QString subCatId = QString::null;
         QDomElement subCcatElem = albumElem.firstChildElement ("SubCategory");
         if (!subCcatElem.isNull ()) {

            attr.insert ("SubCategoryId", subCcatElem.attribute ("id"));
         }

         emit albumUpdate (albumId, attr);
      }
   }
}


void
SmugMug::WebService::_parseImage (QDomElement &root) {
   
   if (!root.isNull () && root.hasAttribute (QLatin1String ("id"))) {

      QString imageId = root.attribute (QLatin1String ("id"));
      if (!imageId.isEmpty ()) {

         SmugMug::Attributes attr;
         
         attr.insert ("Id", imageId);
         attr.insert ("FileName", root.attribute ("FileName"));
         attr.insert ("Caption", root.attribute ("Caption"));
         attr.insert ("Keywords", root.attribute ("Keywords"));
         attr.insert ("Date", root.attribute ("Date"));
         attr.insert ("Format", root.attribute ("Format"));
         attr.insert ("Size", root.attribute ("Size"));
         attr.insert ("Width", root.attribute ("Width"));
         attr.insert ("Height", root.attribute ("Height"));
         attr.insert ("MD5Sum", root.attribute ("MD5Sum"));
         attr.insert ("LastUpdated", root.attribute ("LastUpdated"));
         attr.insert ("OriginalUrl", root.attribute ("OriginalURL"));
         attr.insert ("LargeUrl", root.attribute ("LargeURL"));
         attr.insert ("MediumUrl", root.attribute ("MediumURL"));
         attr.insert ("SmallUrl", root.attribute ("SmallURL"));
         attr.insert ("ThumbUrl", root.attribute ("ThumbURL"));
         attr.insert ("AlbumUrl", root.attribute ("AlbumURL"));

         QDomElement albumElem = root.firstChildElement ("Album");
         if (!albumElem.isNull ()) {

            attr.insert ("AlbumId", albumElem.attribute ("id"));
         }

         emit imageUpdate (imageId, attr);
      }
   }
}


void
SmugMug::WebService::_parseImageGetAll (QDomElement &root) {

   QDomNodeList imageNodeList = root.elementsByTagName ("Image");
   for (unsigned int ix = 0; ix < imageNodeList.length (); ix++) {
   
      QDomElement imageElem = imageNodeList.item (ix).toElement ();
      _parseImage (imageElem);
   }
}


void
SmugMug::WebService::_parseImageGetInfo (QDomElement &root) {

   QDomElement imageElem = root.firstChildElement (QLatin1String ("Image"));
   _parseImage (imageElem);
}


void
SmugMug::WebService::_parseImageUpload (QDomElement &root) {

   QDomElement imageElem = root.firstChildElement (QLatin1String ("Image"));
   if (!imageElem.isNull ()) {

       QString imageId = imageElem.attribute (QLatin1String ("id"));
   }
}


void
SmugMug::WebService::_parseCategoryGetAll (QDomElement &root) {

   QDomNodeList nodeList = root.elementsByTagName (QLatin1String ("Category"));
   for (int ix = 0; ix < nodeList.count (); ix++) {

      QDomElement elem = nodeList.item (ix).toElement ();
      if (!elem.isNull () && elem.hasAttribute (QLatin1String ("id"))) {

         QString catId = elem.attribute (QLatin1String("id"));

         if (!_categoryTable.contains (catId)) {

            QString catName = elem.attribute (QLatin1String("Title"));

            _categoryTable.insert (catId, catName);
            emit categoryUpdate (catId, catName);
         }
      }
   }
}


void
SmugMug::WebService::_parseSubCategoryGetAll (QDomElement &root) {

   QDomNodeList nodeList = root.elementsByTagName (QLatin1String ("SubCategory"));
   for (int ix = 0; ix < nodeList.count (); ix++) {

      QDomElement elem = nodeList.item (ix).toElement ();
      if (!elem.isNull () && elem.hasAttribute (QLatin1String ("id"))) {

         QString subCatId = elem.attribute (QLatin1String("id"));
         
         if (!_subCategoryTable.contains (subCatId)) {

            QString catId;
            QDomElement catElem = elem.firstChildElement (QLatin1String ("Category"));
            if (!catElem.isNull () && catElem.hasAttribute (QLatin1String ("id"))) {

               catId = catElem.attribute (QLatin1String("id"));
            }
         
            if (!catId.isEmpty ()) {
               
               QString subCatName = elem.attribute (QLatin1String("Title"));
               
               _subCategoryTable.insert (subCatId, subCatName);
               emit subCategoryUpadte (catId, subCatId, subCatName);
            }
         }
      }
   }
}


void
SmugMug::WebService::_slotHttpDataReadProgress (int done, int total) {

   if ((done > 0) && (total > 0)) {

//      emit dataReadProgress (done, total);
//qDebug () << "< Data Read: " << done << " of " << total;
   }
}


void
SmugMug::WebService::_slotHttpDataSendProgress (int done, int total) {

   if ((done > 0) && (total > 0)) {

//      emit dataSendProgress (done, total);
//qDebug () << "> Data Send: " << done << " of " << total;
   }
}


void
SmugMug::WebService::_slotHttpReadyRead (const QHttpResponseHeader &resp) {

   QHttp &theHttp = (_ssl ? _https : _http);

   if (resp.statusCode () != 200) {

      QString msg = QString ("[%1] %2").arg (resp.statusCode ()).arg (resp.reasonPhrase ());
      _logHttpError (msg);
      theHttp.abort();
   }
   else {

      const qint64 MaxLen (theHttp.bytesAvailable ());

      if (MaxLen) {

         char *buffer = new char [MaxLen+1];
         memset (buffer, 0, MaxLen+1);

         qint64 count = theHttp.read (buffer, MaxLen);

         _dataIn.append (buffer);

         delete [] buffer;
         buffer = 0;

qDebug () << "Read Ready: " << count << " of " << MaxLen;
      }
   }
}


void
SmugMug::WebService::_slotHttpRequestStarted (int id) {

   _dataIn.clear ();
   _albumTable.clear ();
   _categoryTable.clear ();
   _subCategoryTable.clear ();

qDebug () << "Request Started: " << id;
}


void
SmugMug::WebService::_slotHttpRequestFinished (int id, bool error) {

qDebug () << "Request Finished: " << id;

   if (error) {

      _logHttpError (QString ("Request Finished: %1").arg (id));
   }
   else {

      emit dataAvailable (_dataIn);

      bool err = false;

      QDomDocument doc;
      if (doc.setContent (_dataIn)) {

         QDomElement root = doc.documentElement();

         if (!_isResponseValid (root)) {

            err = true;
            _responseFailed (root);
         }

         _handleResponse (id, root, err);
      }

      emit actionFinished ();
qDebug () << "actionFinished: " << id;
   }
}


void
SmugMug::WebService::_slotHttpRequestDone (bool error) {

   if (error) {

      _logHttpError ("Request Done");
   }
}


void
SmugMug::WebService::_slotHttpStateChanged (int state) {

   QString stateAsStr ("Unknown");

   switch (state) {

      case QHttp::Unconnected: stateAsStr = "Unconnected"; break;
      case QHttp::HostLookup: stateAsStr = "HostLookup"; break;
      case QHttp::Connecting: stateAsStr = "Connecting"; break;
      case QHttp::Sending: stateAsStr = "Sending"; break;
      case QHttp::Reading: stateAsStr = "Reading"; break;
      case QHttp::Connected: stateAsStr = "Connected"; break;
      case QHttp::Closing: stateAsStr = "Closing"; break;
   }

   emit stateChanged (stateAsStr);
}


void
SmugMug::WebService::_slotDownloadHttpDataReadProgress (int done, int total) {

   if (_downloadId) {

qDebug () << "_slotDownloadHttpDataReadProgress";

      if (!_downloadAborted) {

//         emit downloadProgress (done, total);
qDebug () << "< Download Progress: " << done << " of " << total;
      }
   }
}


void
SmugMug::WebService::_slotDownloadHttpReadyRead (const QHttpResponseHeader &resp) {

   if (_downloadId) {

qDebug () << "_slotDownloadHttpReadyRead" << resp.statusCode () << resp.reasonPhrase ();

      if (resp.statusCode () != 200) {

         qWarning () << "---- Download Http Error -----------------------------------";
         qWarning () << "Response Reason:" << resp.reasonPhrase () << endl
            << "Request:" << _downloadHttp.currentRequest ().path () << endl
            << "Bytes returned:" << _downloadHttp.bytesAvailable () << endl
            << "-----------------------------------------------------" << endl;

         _downloadAborted = true;
         _downloadHttp.abort();
      }
   }
}


void
SmugMug::WebService::_slotDownloadHttpRequestFinished (int id, bool error) {

qDebug () << "_slotDownloadHttpRequestFinished" << id;

   if (_downloadAborted) {

      _downloadBuffer.close ();
   }
   else if (id == _downloadId) {

      _downloadBuffer.close ();

      if (error) {

//         _logHttpError (QString ("Request Finished: %1").arg (id));
      }
      else {

//         emit dataAvailable (_downloadData);

         QPixmap image;
         if (image.loadFromData (_downloadData)) {\

            emit imageDownloadFinished (image);
         }
      }

qDebug () << "imageDownloadFinished";
      _downloadId = 0;
   }
}


void
SmugMug::WebService::_uploadHttpPut (
      const QString &AlbumId,
      const QString &FileName,
      const QString &Caption) {

   QFileInfo fi (FileName);

   _dataOut.clear ();

   QFile file (fi.absoluteFilePath ());
   if (file.open (QIODevice::ReadOnly)) {

      _dataOut = file.readAll ();
      file.close ();
   }

#if 0
   QUrl url (UploadEndpoint + "photos/xmlrawadd.mg");
   QHttpRequestHeader header("POST", url.path ());
#else
   QUrl url (UploadEndpoint + fi.fileName ());
   QHttpRequestHeader header("PUT", url.path ());
#endif

   header.setValue("Host", url.host ());
   header.setContentLength (_dataOut.length ());
//   header.setContentType ("image/jpeg");
//   header.setValue("content-length", QString::number (_dataOut.length ()));
   header.setValue("Content-MD5", md5_digest (_dataOut));
//   header.setValue("Content-MD5", "a234ab01efe2775e9f69477831c3d3ca");
//   header.setValue("Content-Transfer-Encoding", "binary");

//   header.setValue("Keep-Alive", "100");
//   header.setValue("Connection", "keep-alive");

   header.setValue("X-Smug-SessionID", _sessionId);
   header.setValue("X-Smug-Version", "1.2.0");
   header.setValue("X-Smug-ResponseType", "REST");
   header.setValue("X-Smug-AlbumID", AlbumId);
   header.setValue("X-Smug-FileName", fi.fileName ());

   if (!Caption.isEmpty ()) {

//      header.setValue("X-Smug-Caption", Caption);
   }

qDebug () << "----- http request header --------------------------" << endl
   << header.toString ();

   _idImageUpload = _httpRequest (url, header, _dataOut);

qDebug () << "imageUpload: " << url.toString ();
}


void
SmugMug::WebService::_uploadMultipartHttpPost (
      const QString &AlbumId,
      const QString &FileName,
      const QString &Caption) {

   const QString BOUNDARY = "----------ThIs_Is_tHe_bouNdaRY_$";
   const QString CFLF = "\r\n";

   QUrl url (UploadEndpoint + "/photos/xmladd.mg");

   _dataOut.clear ();

   QByteArray imageData;
   QFileInfo fi (FileName);
   QFile file (fi.absoluteFilePath ());
   if (file.open (QIODevice::ReadOnly)) {

      imageData = file.readAll ();
      file.close ();
   }

   // Album ID
   _dataOut += QString ("--") + BOUNDARY + CFLF;
   _dataOut += QString ("Content-Disposition: form-data; name=\"AlbumID\"") + CFLF + CFLF;
   _dataOut += AlbumId + CFLF;

   // Session ID
   _dataOut += QString ("--") + BOUNDARY + CFLF;
   _dataOut += QString ("Content-Disposition: form-data; name=\"SessionID\"") + CFLF + CFLF;
   _dataOut += _sessionId + CFLF;

   // Byte Count
   _dataOut += QString ("--") + BOUNDARY + CFLF;
   _dataOut += QString ("Content-Disposition: form-data; name=\"ByteCount\"") + CFLF + CFLF;
   _dataOut += QString::number (imageData.length ()) + CFLF;

   // Response Type
   _dataOut += QString ("--") + BOUNDARY + CFLF;
   _dataOut += QString ("Content-Disposition: form-data; name=\"ResponseType\"") + CFLF + CFLF;
   _dataOut += QString ("REST") + CFLF;
   
   // MD5 Sum
   _dataOut += QString ("--") + BOUNDARY + CFLF;
   _dataOut += QString ("Content-Disposition: form-data; name=\"MD5Sum\"") + CFLF + CFLF;
   _dataOut += md5_digest (imageData) + CFLF;
//   _dataOut += QString ("a234ab01efe2775e9f69477831c3d3ca") + CFLF;
   
   // Caption
   if (!Caption.isEmpty ()) {
        
      _dataOut += QString ("--") + BOUNDARY + CFLF;
      _dataOut += QString ("Content-Disposition: form-data; name=\"Caption\"") + CFLF + CFLF;
      _dataOut += Caption + CFLF;
   }

   // Add the file
   _dataOut += QString ("--") + BOUNDARY + CFLF;
   
   _dataOut += QString ("Content-Disposition: form-data; name=\"Image\"") +
           QString ("; filename=\"%1\"").arg (fi.fileName ()) + CFLF;

   _dataOut += QString ("Content-Type: image/jpeg") + CFLF;
   _dataOut += QString ("Content-length: %1").arg (imageData.length ()) + CFLF + CFLF;

qDebug () << _dataOut;
   
   _dataOut += imageData;
   _dataOut += CFLF;

   _dataOut += QString ("--") + BOUNDARY + "--" + CFLF;

   QHttpRequestHeader header ("POST", url.path ());
   header.setValue ("Host", url.host ());
//   header.setValue ("Cookie", QString ("SMSESS=") + _sessionId);
   header.setContentType (QString ("multipart/form-data; boundary=") + BOUNDARY);
   header.setContentLength (_dataOut.length ());
//   header.setValue ("Connection", "Keep-Alive");

qDebug () << header.toString ();

   _idImageUpload = _httpRequest (url, header, _dataOut);

qDebug () << "imageUpload:" << fi.absoluteFilePath ();
}
