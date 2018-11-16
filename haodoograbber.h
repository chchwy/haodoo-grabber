
#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>
#include <QStringList>
#include <QTimer>
#include <mutex>
#include <QMap>
#include "htmlparser.h"


class HaodooGrabber : public QObject
{
    Q_OBJECT
public:
    HaodooGrabber();
    ~HaodooGrabber();

    //
    void grab100best();
    void sendWebRequest(QString linkUrl);
    
    QStringList parseCategoryHtml(QString htmlFile);
    Book parseBookPageHtml(QString htmlFile);

signals:
    void bookDownloaded(QString);

public slots:
    void networkFinished(QNetworkReply*);
    void networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);

    void requestReadyRead();
    void requestError(QNetworkReply::NetworkError);
    void requestSSLErrors(QList<QSslError>);

    void timerTick();

    QString getBookFileNameFromLink(QString link);

private:
    QNetworkAccessManager* mNetworkManager = nullptr;

    QTimer* mTimer = nullptr;
    std::mutex mtx;

    QStringList mCategoryLinks;
    QStringList mBookPageLinks;

    QList<Book> mBooks;
    QList<Book> mDownloadedBook;
};

