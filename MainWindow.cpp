#include "MainWindow.h"
#include "LoginDialog.h"
#include <SmugMug.h>
#include <QtGui>
#include <QtNetwork>

struct Category;
struct Image;

struct Album {

   QString id;
   QString title;
   Category *category;

   QString description;
   QString imgCount;
   QString keywords;

   QHash <QString, Image *> imageTable;

   Album () : category (0) {;}

   void setCategory (Category *cat);
   void addImage (Image *image);
   void removeImage (Image *image);
   
   int imageCount () { return imgCount.toInt (); }
};

struct Image {
   
   QString id;
   Album *album;
   
   QString fileName;
   QString caption;
   QString keywords;
   QString date;
   QString format;
   QString size;
   QString width;
   QString height;
   QString md5sum;
   QString lastUpdated;
   QString originalUrl;
   QString largeUrl;
   QString mediumUrl;
   QString smallUrl;
   QString thumbUrl;
   QString albumUrl;
   
   Image () : album (0) {;}
   
   void setAlbum (Album *album);
};

struct Category {

   QString id;
   QString title;
   Category *parent;

   QHash<QString, Category *> subCatTable;
   QHash<QString, Album *> albumTable;

   Category () : parent (0) {;}

   void setParent (Category *cat);
   void addSubCategory (Category *cat);
   void removeSubCategory (Category *cat);
   void addAlbum (Album *album);
   void removeAlbum (Album *album);
   int albumCount ();
};

struct MainWindow::impl {

   enum Mode {

      mInit,
      mSync,
      mNormal
   };

   SmugMug::WebService smugmug;
   QLabel *connectionStateLabel;
   QProgressBar *statusProgressBar;

   QHash<QString, Category *> catTable;
//   QHash<QString, Category *>::iterator catIt;
   
   QHash<QString, Category *> subCatTable;
//   QHash<QString, Category *>::iterator subCatIt;
   
   QHash<QString, Album *> albumTable;
   QHash<QString, Album *>::iterator albumIt;
   
   QHash<QString, Image *> imageTable;
   QHash<QString, Image *>::iterator imageIt;

//   QQueue<QString> catQueue;
   QQueue<QString> albumQueue;
   QQueue<QString> imageQueue;
   
   QHash<QListWidgetItem *, Image *> imageItemTable;

   QIcon catIcon;
   QIcon subCatIcon;
   QIcon albumIcon;

   Category *navCat;
   Album *navCatAlbum;

   Category *navSubCat;
   Album *navSubCatAlbum;

   QMenu *navCatMenu;
   QMenu *navSubCatMenu;
   QMenu *navAlbumMenu;

   Mode mode;
   bool doingSync;

   QGraphicsScene *scene;
   QGraphicsPixmapItem *pxItem;
   
   impl ();
   ~impl ();
};


MainWindow::impl::impl () :
      smugmug (),
      connectionStateLabel (0),
      statusProgressBar (0),
      catTable (),
      subCatTable (),
      albumTable (),
      imageTable (),
      imageItemTable (),
      catIcon (),
      subCatIcon (),
      albumIcon (),
      navCat (0),
      navCatAlbum (0),
      navSubCat (0),
      navSubCatAlbum (0),
      navCatMenu (0),
      navSubCatMenu (0),
      navAlbumMenu (0),
      mode (mInit),
      doingSync (false),
      scene (0),
      pxItem (0) {;}


MainWindow::impl::~impl () {;}


MainWindow::MainWindow () :
        QMainWindow (),
        _ui (),
        _impl (* (new impl ())) {

   setAcceptDrops (true);

//   ui.infoTreeWidget->setAcceptDrops (true);
//   ui.infoTreeWidget->installEventFilter (this);
//   ui.infoTreeWidget->viewport ()->installEventFilter (this);

    _ui.setupUi (this);
    _settingsLoad ();

    connect (
       &_impl.smugmug, SIGNAL (actionStarted ()),
       this, SLOT (_slotActionStarted ()));

    connect (
       &_impl.smugmug, SIGNAL (actionFinished ()),
       this, SLOT (_slotActionFinished ()));

   connect (
      &_impl.smugmug, SIGNAL (dataAvailable (const QByteArray &)),
      this, SLOT (_slotDataAvailable (const QByteArray &)));

   connect (
      &_impl.smugmug, SIGNAL (dataReadProgress (int, int)),
      this, SLOT (_slotDataReadProgress (int, int)));

   connect (
      &_impl.smugmug, SIGNAL (dataSendProgress (int, int)),
      this, SLOT (_slotDataSendProgress (int, int)));

   connect (
      &_impl.smugmug, SIGNAL (stateChanged (const QString &)),
      this, SLOT (_slotCommectionStateChanged (const QString &)));

   // Login
   connect (
      &_impl.smugmug, SIGNAL (loginStarted ()),
      this, SLOT (_slotLoginStarted ()));

   connect (
      &_impl.smugmug, SIGNAL (loginFinished (bool)),
      this, SLOT (_slotLoginFinished (bool)));

   // Logout
   connect (
      &_impl.smugmug, SIGNAL (logoutFinished ()),
      this, SLOT (_slotLogoutFinished ()));

   // Users
   connect (
      &_impl.smugmug, SIGNAL (userGetTreeFinished (bool)),
      this, SLOT (_slotUserGetTreeFinished (bool)));

   // Albums
   connect (
      &_impl.smugmug, SIGNAL (albumGetAllFinished (bool)),
      this, SLOT (_slotAlbumGetAllFinished (bool)));

   connect (
      &_impl.smugmug, SIGNAL (albumUpdate (const QString &, const SmugMug::Attributes &)),
      this, SLOT (_slotAlbumUpdate (const QString &, const SmugMug::Attributes &)));

   // Images
   connect (
      &_impl.smugmug, SIGNAL (imageUpdate (const QString &, const SmugMug::Attributes &)),
      this, SLOT (_slotImageUpdate (const QString &, const SmugMug::Attributes &)));

   connect (
      &_impl.smugmug, SIGNAL (imageGetAllFinished (bool)),
      this, SLOT (_slotImageGetAllFinished (bool)));

   connect (
      &_impl.smugmug, SIGNAL (imageGetInfoFinished (bool)),
      this, SLOT (_slotImageGetInfoFinished (bool)));

   connect (
      &_impl.smugmug, SIGNAL (imageDownloadFinished (const QPixmap &)),
      this, SLOT (_slotImageDownloadFinished (const QPixmap &)));

   // Categories
   connect (
      &_impl.smugmug, SIGNAL (categoryUpdate (const QString &, const QString &)),
      this, SLOT (_slotCategoryUpdate (const QString &, const QString &)));

//   connect (
//      &_impl.smugmug, SIGNAL (categoryGetAllFinished (bool)),
//      this, SLOT (_slotCategoryGetAllFinished (bool)));

//   connect (
//      &_impl.smugmug, SIGNAL (categoryUpdate (SmugMug::Album *)),
//      this, SLOT (_slotCategoryUpdate (SmugMug::Album *)));

   // SubCategories
   connect (
      &_impl.smugmug, SIGNAL (subCategoryUpadte (const QString &, const QString &, const QString &)),
      this, SLOT (_slotSubCategoryUpdate (const QString &, const QString &, const QString &)));

   QIcon navIcon;
   navIcon.addPixmap (QPixmap (":/smugup/images/nav.png"), QIcon::Normal, QIcon::Off);
   navIcon.addPixmap (QPixmap (":/smugup/images/nav_blue.png"), QIcon::Normal, QIcon::On);
   navIcon.addPixmap (QPixmap (":/smugup/images/nav.png"), QIcon::Active, QIcon::Off);
   navIcon.addPixmap (QPixmap (":/smugup/images/nav_blue.png"), QIcon::Active, QIcon::On);

   _ui.navCategoryButton->setIcon (navIcon);
   _ui.navSubCategoryButton->setIcon (navIcon);
   _ui.navAlbumButton->setIcon (navIcon);

   _ui.navCategoryButton->installEventFilter (this);

   _impl.catIcon.addPixmap (QPixmap (":/smugup/images/category.png"), QIcon::Normal, QIcon::Off);
   _impl.subCatIcon.addPixmap (QPixmap (":/smugup/images/subcategory.png"), QIcon::Normal, QIcon::Off);
   _impl.albumIcon.addPixmap (QPixmap (":/smugup/images/album.png"), QIcon::Normal, QIcon::Off);

   _ui.categoryButton->setIcon (_impl.catIcon);
   _ui.subCategoryButton->setIcon (_impl.subCatIcon);

   _impl.navCatMenu = new QMenu (this);

   connect (
      _impl.navCatMenu, SIGNAL (triggered (QAction *)),
      this, SLOT (_slotNavCatMenuTriggered (QAction *)));

   _impl.navSubCatMenu = new QMenu (this);

   connect (
      _impl.navSubCatMenu, SIGNAL (triggered (QAction *)),
      this, SLOT (_slotNavSubCatMenuTriggered (QAction *)));

   _impl.navAlbumMenu = new QMenu (this);

   connect (
      _impl.navAlbumMenu, SIGNAL (triggered (QAction *)),
      this, SLOT (_slotNavAlbumMenuTriggered (QAction *)));

   _ui.navCategoryButton->setMenu (_impl.navCatMenu);
   _ui.navSubCategoryButton->setMenu (_impl.navSubCatMenu);
   _ui.navAlbumButton->setMenu (_impl.navAlbumMenu);
   
   _impl.connectionStateLabel = new QLabel ("", statusBar ());

   statusBar ()->insertPermanentWidget (0, _impl.connectionStateLabel);

   _impl.statusProgressBar = new QProgressBar (statusBar ());
   _impl.statusProgressBar->setTextVisible (false);

   statusBar ()->insertPermanentWidget (0, _impl.statusProgressBar);

   _navHome ();
   _impl.smugmug.loginWithHash ();
   
   _impl.scene = new QGraphicsScene (this);
//   _ui.graphicsView->setScene (_impl.scene);
}


MainWindow::~MainWindow () {

   _settingsSave ();

    delete &_impl;
}


bool
MainWindow::eventFilter (QObject *watched, QEvent *event) {

   bool retVal = false;
   const QMimeData *mimeData = 0;
   if (event->type() == QEvent::DragEnter) {

      QDragEnterEvent *dragEvent = static_cast<QDragEnterEvent *>(event);
      mimeData = dragEvent->mimeData ();
   }
   else if (event->type() == QEvent::DragMove) {

      QDragMoveEvent *dragEvent = static_cast<QDragMoveEvent *>(event);
      mimeData = dragEvent->mimeData ();
   }
   else if (event->type() == QEvent::Drop) {

      QDropEvent *dragEvent = static_cast<QDropEvent *>(event);
      dropEvent (dragEvent);
      retVal = true;
   }

   if (_mimeDataContainsdImageFileName (mimeData)) {

      event->accept();
      retVal = true;
   }

   return retVal;
}


void 
MainWindow::dragEnterEvent (QDragEnterEvent *event) {

   if (_mimeDataContainsdImageFileName (event->mimeData ())) {

      event->acceptProposedAction();
   }
}


void
MainWindow::dropEvent (QDropEvent * event) {

   QList<QUrl> urlList = event->mimeData ()->urls ();

   foreach (QUrl url, urlList) {

      QFileInfo fi (url.toLocalFile ());

qDebug () << "file: " << fi.filePath ();
   }
}


void
MainWindow::on_loginButton_clicked () {

   if (_impl.smugmug.loggedIn ()) {

      _impl.smugmug.logout ();
   }
   else {

      LoginDialog dialog (_impl.smugmug, this);
      dialog.exec ();
   }
}


void
MainWindow::on_sendRequestButton_clicked () {

//   _impl.smugmug.albumGetAll (false);
//   _impl.smugmug.categoryGetAll ();

//   QString albumId ("2160061");
//   _impl.smugmug.albumGetInfo (albumId);
}


void
MainWindow::on_imageGetInfoButton_clicked () {

   QString imageId ("112136707");
   _impl.smugmug.imageGetInfo (imageId);

   QString msg (tr ("ImageGetInfo..."));
   statusBar ()->showMessage (msg);
   _ui.logTextEdit->setText (msg);
}


void
MainWindow::on_imageUploadButton_clicked () {

   QString albumId ("2160061");
   QString comment ("SmugMug Rocks!!!");
   QString fileName ("c:/devel/code/test.jpg");
//   QString fileName ("/Users/scott/devel/smugup/bin/kids.jpg");
   
   _impl.smugmug.imageUpload (albumId, fileName, comment);
   _ui.logTextEdit->setText (QString ("uploading ") + fileName);
}


void
MainWindow::on_userGetTreeButton_clicked () {

   _impl.smugmug.userGetTree (false);

   QString msg (tr ("UserGetTree ..."));
   statusBar ()->showMessage (msg);
   _ui.logTextEdit->setText (msg);
}


void
MainWindow::on_homeButton_clicked () {

   _navHome ();
}


void
MainWindow::on_categoryButton_clicked () {

   _navToCat ();
}


void
MainWindow::on_subCategoryButton_clicked () {

   _navToSubCat ();
}


void
MainWindow::on_syncButton_clicked () {
   
   _impl.catTable.clear ();
   _impl.subCatTable.clear ();
   _impl.albumTable.clear ();
   _impl.imageTable.clear ();
   
   _impl.doingSync = true;

   _impl.smugmug.userGetTree ();
}


void
MainWindow::on_imageListWidget_itemDoubleClicked (QListWidgetItem *item) {

   if (item) {
      
      Image *image = _impl.imageItemTable.value (item);
      if (image) {
         
         if (!image->thumbUrl.isEmpty ()) {
            
            _ui.thumbLabel->setText (image->id);
            
//             QUrl url (image->thumbUrl);

//             _impl.http->setHost (url.host (), url.port (80));
//qDebug () << "host" << url.host ();             

//             _impl.buffer.open (QIODevice::WriteOnly);
//             _impl.downloadId = _impl.http->get (url.path (), &_impl.buffer);

//qDebug () << "http->get" << _impl.downloadId << url.path ();             
             
            _impl.smugmug.imageDownload (image->thumbUrl);
         }
      }

/*
      Category *cat = _impl.itemCatTable.value (item);
      if (cat) {
         
         if (_impl.navCat) {

            _impl.navSubCat = cat;
            _navToSubCat ();
         }
         else {
            
            _impl.navCat = cat;
            _navToCat ();
         }
      }

      Album *album = _impl.itemAlbumTable.value (item);
      if (album) {
         
         if (_impl.navSubCat) {
            
            _impl.navSubCatAlbum = album;
            _navToSubCatAlbum ();
         }
         else {
            
            _impl.navCatAlbum = album;
            _navToCatAlbum ();
         }
      }
*/
   }
}


void
MainWindow::_setupNavCatMenu () {

   _impl.navCatMenu->clear ();

   QHashIterator<QString, Category *> it (_impl.catTable);

   while (it.hasNext ()) {

      it.next();

      if (it.value ()->albumCount ()) {

         QAction *action = new QAction (_impl.catIcon, it.value ()->title, this);
         action->setData (it.value ()->id);

         _impl.navCatMenu->addAction (action);
      }
   }
}


void
MainWindow::_setupNavSubCatMenu (Category *cat) {

   _impl.navSubCatMenu->clear ();

   if (cat) {

      QHashIterator<QString, Category *> it (cat->subCatTable);

      while (it.hasNext ()) {

         it.next();

         if (it.value ()->albumCount ()) {

            QAction *action = new QAction (_impl.subCatIcon, it.value ()->title, this);
            action->setData (it.value ()->id);

            _impl.navSubCatMenu->addAction (action);
         }
      }

      _impl.navSubCatMenu->addSeparator ();

      QHashIterator<QString, Album *> albumIt (cat->albumTable);

      while (albumIt.hasNext ()) {

         albumIt.next();

         QAction *action = new QAction (_impl.albumIcon, albumIt.value ()->title, this);
         action->setData (albumIt.value ()->id);

         _impl.navSubCatMenu->addAction (action);
      }
   }
}


void
MainWindow::_setupNavAlbumMenu (Category *cat) {

   _impl.navAlbumMenu->clear ();

   if (cat) {

      QHashIterator<QString, Album *> it (cat->albumTable);

      while (it.hasNext ()) {

         it.next();

         QAction *action = new QAction (_impl.albumIcon, it.value ()->title, this);
         action->setData (it.value ()->id);

         _impl.navAlbumMenu->addAction (action);
      }
   }
}

/*
void
MainWindow::_updateNavView () {

   _impl.itemCatTable.clear ();
   _impl.itemAlbumTable.clear ();
   _impl.itemImageTable.clear ();
   _ui.imageListWidget->clear ();

   QHashIterator<QString, Category *> it (_impl.catTable);

   while (it.hasNext ()) {

      it.next();

      if (it.value ()->albumCount ()) {

         QListWidgetItem *item =
            new QListWidgetItem (it.value ()->title, _ui.imageListWidget);

         item->setIcon (_impl.catIcon);
         _impl.itemCatTable.insert (item, it.value ());
      }
   }
}

void
MainWindow::_updateNavView (Category *cat) {

   _impl.itemCatTable.clear ();
   _impl.itemAlbumTable.clear ();
   _impl.itemImageTable.clear ();
   _ui.imageListWidget->clear ();

   if (cat) {

      QHashIterator<QString, Category *> it (cat->subCatTable);

      while (it.hasNext ()) {

         it.next();

         if (it.value ()->albumCount ()) {

            QListWidgetItem *item =
               new QListWidgetItem (it.value ()->title, _ui.imageListWidget);

            item->setIcon (_impl.subCatIcon);
            _impl.itemCatTable.insert (item, it.value ());
         }
      }

      QHashIterator<QString, Album *> albumIt (cat->albumTable);

      while (albumIt.hasNext ()) {

         albumIt.next();

//         if (albumIt.value ()->imageCount ()) {

            QListWidgetItem *item =
               new QListWidgetItem (albumIt.value ()->title, _ui.imageListWidget);

            item->setIcon (_impl.albumIcon);
            _impl.itemAlbumTable.insert (item, albumIt.value ());
//         }
      }
   }
}
*/


void
MainWindow::_updateNavView (Album *album) {

   _impl.imageItemTable.clear ();
   _ui.imageListWidget->clear ();

   if (album) {

      QHashIterator<QString, Image *> it (album->imageTable);

      while (it.hasNext ()) {

         it.next();

         Image *image = it.value ();
qDebug () << image->fileName << image->thumbUrl;      

         QListWidgetItem *item = new QListWidgetItem (image->fileName, _ui.imageListWidget);

//         item->setIcon (_impl.subCatIcon);

         _impl.imageItemTable.insert (item, image);
      }
   }
}


void
MainWindow::_navHome () {

   _impl.navCat = 0;
   _impl.navCatAlbum = 0;
   _impl.navSubCat = 0;
   _impl.navSubCatAlbum = 0;

   if (_impl.catTable.count ()) { _ui.navCategoryButton->show (); }
   else { _ui.navCategoryButton->hide (); }

   _ui.categoryButton->hide ();
   _ui.navSubCategoryButton->hide ();
   _ui.subCategoryButton->hide ();
   _ui.navAlbumButton->hide ();
   _ui.albumIconLabel->hide ();
   _ui.albumLabel->hide ();

   _setupNavCatMenu ();
   _updateNavView (0);
}


void
MainWindow::_navToCat () {

   _impl.navCatAlbum = 0;
   _impl.navSubCat = 0;
   _impl.navSubCatAlbum = 0;

   _ui.categoryButton->show ();
   _ui.categoryButton->setText (_impl.navCat->title);
   _ui.navSubCategoryButton->show ();
   _ui.subCategoryButton->hide ();
   _ui.navAlbumButton->hide ();
   _ui.albumIconLabel->hide ();
   _ui.albumLabel->hide ();

   _setupNavSubCatMenu (_impl.navCat);
//   _updateNavView (_impl.navCat);
   _updateNavView (0);
}


void
MainWindow::_navToCatAlbum () {

   _impl.navSubCat = 0;
   _impl.navSubCatAlbum = 0;

   _ui.categoryButton->show ();
   _ui.categoryButton->setText (_impl.navCat->title);
   _ui.navSubCategoryButton->show ();
   _ui.subCategoryButton->hide ();
   _ui.navAlbumButton->hide ();
   _ui.albumIconLabel->show ();
   _ui.albumLabel->show ();
   _ui.albumLabel->setText (_impl.navCatAlbum->title);

   _updateNavView (_impl.navCatAlbum);
}


void
MainWindow::_navToSubCat () {

   _impl.navCatAlbum = 0;
   _impl.navSubCatAlbum = 0;

   _ui.categoryButton->show ();
   _ui.categoryButton->setText (_impl.navCat->title);
   _ui.navSubCategoryButton->show ();
   _ui.subCategoryButton->show ();
   _ui.subCategoryButton->setText (_impl.navSubCat->title);
   _ui.navAlbumButton->show ();
   _ui.albumIconLabel->hide ();
   _ui.albumLabel->hide ();

   _setupNavAlbumMenu (_impl.navSubCat);
//   _updateNavView (_impl.navSubCat);
   _updateNavView (0);
}


void
MainWindow::_navToSubCatAlbum () {

   _impl.navCatAlbum = 0;

   _ui.categoryButton->show ();
   _ui.categoryButton->setText (_impl.navCat->title);
   _ui.navSubCategoryButton->show ();
   _ui.subCategoryButton->show ();
   _ui.subCategoryButton->setText (_impl.navSubCat->title);
   _ui.navAlbumButton->show ();
   _ui.albumIconLabel->show ();
   _ui.albumLabel->show ();
   _ui.albumLabel->setText (_impl.navSubCatAlbum->title);

   _updateNavView (_impl.navSubCatAlbum);
}


void
MainWindow::_slotDataReadProgress (int done, int total) {

   if (total > 1000) {

      _impl.statusProgressBar->show ();
      _impl.statusProgressBar->setRange (0, total);
      _impl.statusProgressBar->setValue (done);
   }
}


void
MainWindow::_slotDataSendProgress (int done, int total) {

   if (total > 1000) {

      _impl.statusProgressBar->show ();
      _impl.statusProgressBar->setRange (0, total);
      _impl.statusProgressBar->setValue (done);
   }
}


void
MainWindow::_slotDataAvailable (const QByteArray &Buffer) {

   QString msg (Buffer);
   _ui.logTextEdit->setText (msg);
}


void
MainWindow::_slotActionStarted () {

}


void
MainWindow::_slotActionFinished () {

   _impl.statusProgressBar->setValue (0);
   _impl.statusProgressBar->hide ();
}


void
MainWindow::_slotLoginStarted () {

   QString msg = "Logging in ...";
   statusBar ()->showMessage (msg);
}


void
MainWindow::_slotLoginFinished (bool error) {

   QString msg;

   if (error) {

      msg = "Failed to login. Please try again";
      _ui.loginButton->setText (QLatin1String("Login"));
      QTimer::singleShot (0, this, SLOT (on_loginButton_clicked ()));
   }
   else {

      msg = tr ("Logged in as ") + _impl.smugmug.userName ();
      _ui.loginButton->setText (QLatin1String("Logout"));
      
      if (_impl.mode == impl::mInit) {

         _ui.homeButton->setIcon (QIcon (":/smugup/images/sync-icon.png"));
         _ui.homeButton->setText ("Retrieving Tree. Please wait...");
         _impl.smugmug.userGetTree ();
      }
   }

   statusBar ()->showMessage (msg, 2000);
}


void
MainWindow::_slotLogoutFinished () {

   _ui.loginButton->setText (QLatin1String("Login"));
}


void
MainWindow::_slotCommectionStateChanged (const QString &State) {

   if (_impl.connectionStateLabel) {

      _impl.connectionStateLabel->setText (State);
   }
}


void
MainWindow::_slotImageGetAllFinished (bool error) {

   if (!_impl.imageQueue.isEmpty ()) {

      QString imageId = _impl.imageQueue.dequeue ();
      _impl.smugmug.imageGetInfo (imageId);
   }     
}


void
MainWindow::_slotImageGetInfoFinished (bool error) {

   if (!_impl.imageQueue.isEmpty ()) {

      QString imageId = _impl.imageQueue.dequeue ();
      _impl.smugmug.imageGetInfo (imageId);
   }
}


void
MainWindow::_slotImageDownloadFinished (const QPixmap &Image) {

//   _impl.scene->addPixmap (Image);
   _ui.thumbLabel->setPixmap (Image);
}


void
MainWindow::_slotUserGetTreeFinished (bool error) {

   if (_impl.mode == impl::mInit) {

      _ui.homeButton->setIcon (QIcon (":/smugup/images/home.png"));
      _ui.homeButton->setText ("Home");
      _navHome ();

      QSettings settings;
      settings.beginGroup (QLatin1String ("MainWindow"));

      QString id = settings.value("navCat", "").toString ();
      Category *cat = _impl.catTable.value (id);

      id = settings.value("navCatAlbum", "").toString ();
      Album *catAlbum = _impl.albumTable.value (id);

      id = settings.value("navSubCat", "").toString ();
      Category *subCat = _impl.subCatTable.value (id);

      id = settings.value("navSubCatAlbum", "").toString ();
      Album *subCatAlbum = _impl.albumTable.value (id);

      if (cat) {

         _impl.navCat = cat;
         _navToCat ();

         if (catAlbum) {

_impl.smugmug.imageGetAll (catAlbum->id);
            _impl.navCatAlbum = catAlbum;
            _navToCatAlbum ();
         }
         else if (subCat) {

            _impl.navSubCat = subCat;
            _navToSubCat ();

            if (subCatAlbum) {

_impl.smugmug.imageGetAll (subCatAlbum->id);
               _impl.navSubCat = subCat;
               _impl.navSubCatAlbum = subCatAlbum;
               _navToSubCatAlbum ();
            }
         }
      }

      settings.endGroup ();

      _impl.mode = impl::mNormal;
   }
}


void
MainWindow::_slotAlbumGetAllFinished (bool error) {

}


void
MainWindow::_slotAlbumUpdate (
      const QString &AlbumId,
      const SmugMug::Attributes &Attr) {

   Album *album = _impl.albumTable.value (AlbumId);
   if (!album) {

      album = new Album;
      album->id = AlbumId;

      _impl.albumTable.insert (AlbumId, album);

      Category *cat = _impl.subCatTable.value (Attr["SubCategoryId"]);
      if (!cat) { cat = _impl.catTable.value (Attr["CategoryId"]); }
      album->setCategory (cat);
   }

   if (album) {

      album->title = Attr["Title"];
      album->description = Attr["Description"];
      album->imgCount = Attr["ImageCount"];
      album->keywords = Attr["Keywords"];
      
//      if (!_impl.albumQueue.contains (album->id)) {
//         
//         _impl.albumQueue.enqueue (album->id);
//qDebug () << "queue album" << _impl.albumQueue.count () << album->id << album->title;  
//      }
   }
}


void
MainWindow::_slotImageUpdate (
      const QString &ImageId,
      const SmugMug::Attributes &Attr) {

   Image *image = _impl.imageTable.value (ImageId);
   if (!image) {

      image = new Image;
      image->id = ImageId;

      _impl.imageTable.insert (ImageId, image);

      if (!_impl.imageQueue.contains (image->id)) {

         _impl.imageQueue.enqueue (image->id);
qDebug () << "queue image" << _impl.imageQueue.count () << image->fileName << image->thumbUrl;  
      }
   }

   if (image) {

      Album *album = _impl.albumTable.value (Attr["AlbumId"]);
      if (album) { album->addImage (image); }

      if (image->fileName.isEmpty ()) { image->fileName = Attr["FileName"]; }
      if (image->caption.isEmpty ()) { image->caption = Attr["Caption"]; }
      if (image->keywords.isEmpty ()) { image->keywords = Attr["Keywords"]; }
      if (image->date.isEmpty ()) { image->date = Attr["Date"]; }
      if (image->format.isEmpty ()) { image->format = Attr["Format"]; }
      if (image->size.isEmpty ()) { image->size = Attr["Size"]; }
      if (image->width.isEmpty ()) { image->width = Attr["Width"]; }
      if (image->height.isEmpty ()) { image->height = Attr["Height"]; }
      if (image->md5sum.isEmpty ()) { image->md5sum = Attr["MD5Sum"]; }
      if (image->lastUpdated.isEmpty ()) { image->lastUpdated = Attr["LastUpdated"]; }
      if (image->originalUrl.isEmpty ()) { image->originalUrl = Attr["OriginalUrl"]; }
      if (image->largeUrl.isEmpty ()) { image->largeUrl = Attr["LargeUrl"]; }
      if (image->mediumUrl.isEmpty ()) { image->mediumUrl = Attr["MediumUrl"]; }
      if (image->smallUrl.isEmpty ()) { image->smallUrl = Attr["SmallUrl"]; }
      if (image->thumbUrl.isEmpty ()) { image->thumbUrl = Attr["ThumbUrl"]; }
      if (image->albumUrl.isEmpty ()) { image->albumUrl = Attr["AlbumUrl"]; }
   }
}


void
MainWindow::_slotCategoryUpdate (
      const QString &CategoryId,
      const QString &CategoryName) {

   if (!_impl.catTable.contains (CategoryId)) {

      Category *cat = new Category;
      cat->id = CategoryId;
      cat->title = CategoryName;

      _impl.catTable.insert (CategoryId, cat);
   }
}


void
MainWindow::_slotSubCategoryUpdate (
      const QString &CategoryID,
      const QString &SubCategoryId,
      const QString &SubCategoryName) {

   if (!_impl.subCatTable.contains (SubCategoryId)) {

      Category *cat = _impl.catTable.value (CategoryID);
      if (cat) {

         Category *subCat = new Category;
         subCat->id = SubCategoryId;
         subCat->title = SubCategoryName;

         cat->addSubCategory (subCat);

         _impl.subCatTable.insert (SubCategoryId, subCat);
      }
   }
}


void
MainWindow::_slotNavCatMenuTriggered (QAction *action) {

   if (action) {

      Category *category = _impl.catTable.value (action->data ().toString ());
      if (category) {

         _impl.navCat = category;
         _navToCat ();
      }
   }
}


void
MainWindow::_slotNavSubCatMenuTriggered (QAction *action) {

   if (action) {

      Category *category = _impl.subCatTable.value (action->data ().toString ());
      if (category) {

         _impl.navSubCat = category;
         _navToSubCat ();
      }
      else {

         Album *album = _impl.albumTable.value (action->data ().toString ());
         if (album) {

            _impl.navCatAlbum = album;
_impl.smugmug.imageGetAll (album->id);

            _navToCatAlbum ();
         }
      }
   }
}


void
MainWindow::_slotNavAlbumMenuTriggered (QAction *action) {

   if (action) {

      Album *album = _impl.albumTable.value (action->data ().toString ());
      if (album) {

         _impl.navSubCatAlbum = album;
_impl.smugmug.imageGetAll (album->id);

         _navToSubCatAlbum ();
      }
   }
}


void
MainWindow::_settingsLoad () { 

   QSettings settings;
}


void
MainWindow::_settingsSave () {

   QSettings settings;
   settings.beginGroup (QLatin1String ("MainWindow"));

   if (_impl.navCat) { settings.setValue (QLatin1String ("navCat"), _impl.navCat->id); }
   else { settings.remove ("navCat"); }

   if (_impl.navCatAlbum) { settings.setValue (QLatin1String ("navCatAlbum"), _impl.navCatAlbum->id); }
   else { settings.remove ("navCatAlbum"); }

   if (_impl.navSubCat) { settings.setValue (QLatin1String ("navSubCat"), _impl.navSubCat->id); }
   else { settings.remove ("navSubCat"); }

   if (_impl.navSubCatAlbum) { settings.setValue (QLatin1String ("navSubCatAlbum"), _impl.navSubCatAlbum->id); }
   else { settings.remove ("navSubCatAlbum"); }
   
   settings.endGroup ();
}


bool
MainWindow::_mimeDataContainsdImageFileName (const QMimeData *mimeData) {

   bool retVal = false;

   if (mimeData && mimeData->hasUrls ()) {

      QList<QUrl> urlList = mimeData->urls ();

      foreach (QUrl url, urlList) {

         QFileInfo fi (url.toLocalFile ());

         if (fi.isFile () && (fi.suffix ().toLower () == "jpg")) {

            retVal = true;
            break;
         }
      }
   }

   return retVal;
}


void
Album::setCategory (Category  *cat) {

   if (cat != category) {

      if (category) { category->removeAlbum (this); }

      category = cat;

      if (category) { category->addAlbum (this); }
   }
}


void
Album::addImage (Image *image) {

   if (image) {
      
      if (!imageTable.contains (image->id)) {
         
         imageTable.insert (image->id, image);
      }
   }
}


void
Album::removeImage (Image *image) {
   
   if (image) {

      if (imageTable.contains (image->id)) {

         imageTable.remove (image->id);
      }
   }   
}


void
Image::setAlbum (Album *theAlbum) {
   
   if (album != theAlbum) {
      
      if (album) { album->removeImage (this); }
      
      album = theAlbum;
      
      if (album) { album->addImage (this); }
   }
}


void
Category::setParent (Category *cat) {

   if (cat) { parent = cat; }
}


void
Category::addSubCategory (Category *cat) {

   if (cat) {

      if (!subCatTable.contains (cat->id)) {

         subCatTable.insert (cat->id, cat);
         cat->setParent (this);
      }
   }
}


void
Category::removeSubCategory (Category *cat) {

   if (cat) {

      if (subCatTable.contains (cat->id)) {

         subCatTable.remove (cat->id);
         cat->setParent (0);
      }
   }
}


void
Category::addAlbum (Album *album) {

   if (album) {

      if (!albumTable.contains (album->id)) {

         albumTable.insert (album->id, album);
//         album->setCategory (this);
      }
   }
}


void
Category::removeAlbum (Album *album) {

   if (album) {

      if (albumTable.contains (album->id)) {

         albumTable.remove (album->id);
//         album->setCategory (0);
      }
   }
}


int
Category::albumCount () {

   int retVal = 0;

   QHashIterator<QString, Category *> it (subCatTable);

   while (it.hasNext()) {

      it.next();

      Category *cat = it.value ();
      retVal += cat->albumTable.count ();
   }

   retVal += albumTable.count ();

   return retVal;
}
