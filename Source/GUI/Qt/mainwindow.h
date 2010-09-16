#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "Common/Core.h"
#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtGui/QTreeWidget>
#include <QtGui/QProgressDialog>
#include <QtCore/QTimer>
#include "views.h"
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#ifdef NEW_VERSION
    #include <QtNetwork/QNetworkAccessManager.h>
    #include <QtNetwork/QNetworkReply.h>
    #include <QtCore/QTemporaryFile.h>
#endif //NEW_VERSION

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

    static QString shortName(QDir d, QString name);
    static QString shortName(Core*C, QString name);
    static QDir getCommonDir(Core*C);

    void checkForNewVersion();


protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    void defaultSettings();
    void applySettings();
    QString  mediaInfoTest(QString filename);
    void refreshDisplay();
    void openFiles(QStringList fileNames);
    void openDir(QString dirName);
    QTreeWidget* showTreeView(bool completeDisplay);


    Ui::MainWindow *ui;
    //Non-GUI Elements
#ifdef NEW_VERSION
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QTemporaryFile *file;
#endif
    Core* C;
    ViewMode view;
    QSettings* settings;
    QProgressDialog* progressDialog;
    QTimer* timer;
    void openTimerInit ();

private slots:
    void updateProgressBar ();
    void on_actionClose_All_triggered();
    void on_actionAdvanced_Mode_toggled(bool );
    void on_actionExport_triggered();
    void on_actionPreferences_triggered();
    void on_actionKnown_parameters_triggered();
    void actionView_toggled(QAction* view);
    void on_actionKnown_codecs_triggered();
    void on_actionKnown_formats_triggered();
    void on_actionAbout_triggered();
    void on_actionOpen_Folder_triggered();
    void on_actionQuit_triggered();
    void on_actionOpen_triggered();
    void toolBarOptions(QPoint);
    void httpFinished();
    void httpReadyRead();
};

#endif // MAINWINDOW_H
