#ifndef MAIN_WINDOW_DOT_H
#define MAIN_WINDOW_DOT_H

#include "ui_MainWindow.h"
#include <QMainWindow>

class QHttpResponseHeader;
struct Category;
struct Album;

namespace SmugMug {

   typedef QHash<QString, QString> Attributes;
}


class MainWindow : public QMainWindow {
	Q_OBJECT

	public:
    	MainWindow ();
		~MainWindow ();
		
      bool eventFilter (QObject * watched, QEvent *event);

	protected slots:
      void on_loginButton_clicked ();
      void on_sendRequestButton_clicked ();

      void on_imageGetInfoButton_clicked ();
      void on_imageUploadButton_clicked ();
      void on_userGetTreeButton_clicked ();

      void on_homeButton_clicked ();
      void on_categoryButton_clicked ();
      void on_subCategoryButton_clicked ();
      
      void on_syncButton_clicked ();

      void on_imageListWidget_itemDoubleClicked (QListWidgetItem *);

      void _slotDataReadProgress (int done, int total);
      void _slotDataSendProgress (int done, int total);

      void _slotDataAvailable (const QByteArray &);

      void _slotActionStarted ();
      void _slotActionFinished ();

      void _slotLoginStarted ();
      void _slotLoginFinished (bool error);
      
      void _slotLogoutFinished ();

      void _slotCommectionStateChanged (const QString &);

      void _slotImageGetAllFinished (bool error);
      void _slotImageGetInfoFinished (bool error);
      void _slotImageDownloadFinished (const QPixmap &Image);
      void _slotUserGetTreeFinished (bool error);
      void _slotAlbumGetAllFinished (bool error);
         
      void _slotAlbumUpdate (
         const QString &AlbumId,
         const SmugMug::Attributes &Attr);
         
      void _slotImageUpdate (
         const QString &ImageId,
         const SmugMug::Attributes &Attr);
             
      void _slotCategoryUpdate (
         const QString &CategoryId,
         const QString &CategoryName);

      void _slotSubCategoryUpdate (
         const QString &CategoryID,
         const QString &SubCategoryId,
         const QString &SubCategoryName);

      void _slotNavCatMenuTriggered (QAction *);
      void _slotNavSubCatMenuTriggered (QAction *);
      void _slotNavAlbumMenuTriggered (QAction *);

   protected:
      virtual void dragEnterEvent(QDragEnterEvent *event);
      virtual void dropEvent (QDropEvent * event);

//      void _updateNavView ();
//      void _updateNavView (Category *cat);
      void _updateNavView (Album *album);

      void _setupNavCatMenu ();
      void _setupNavSubCatMenu (Category *cat);
      void _setupNavAlbumMenu (Category *cat);

      void _navHome ();
      void _navToCat ();
      void _navToCatAlbum ();
      void _navToSubCat ();
      void _navToSubCatAlbum ();

      void _settingsLoad ();
      void _settingsSave ();

      bool _mimeDataContainsdImageFileName (const QMimeData *mimeData);

	protected:
		Ui::MainWindow _ui;
		
		struct impl;
		impl &_impl;
};

#endif // MAIN_WINDOW_DOT_H
