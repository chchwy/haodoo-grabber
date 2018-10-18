#include "haodoograbber.h"
#include <QUrlQuery>
#include <QFile>

HaodooGrabber::HaodooGrabber() : QObject()
{
    mNetworkManager = new QNetworkAccessManager(this);

    connect(mNetworkManager, &QNetworkAccessManager::finished, this, &HaodooGrabber::networkFinished);
    connect(mNetworkManager, &QNetworkAccessManager::networkAccessibleChanged, this, &HaodooGrabber::networkAccessibleChanged);
}

HaodooGrabber::~HaodooGrabber()
{
    delete mNetworkManager;
}

void HaodooGrabber::grabListFromCategory()
{
    
    QUrl url("http://www.haodoo.net/?M=hd&P=100-1");
    //QUrl url("http://chchwy.github.io/");
    QNetworkRequest req;
    req.setUrl(url);
    req.setRawHeader("User-Agent", "Mozilla Firefox");
    
    QNetworkReply* reply = mNetworkManager->get(req);
    
    auto networkError = static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error);
    connect(reply, &QNetworkReply::readyRead, this, &HaodooGrabber::requestReadyRead);
    connect(reply, networkError, this, &HaodooGrabber::requestError);
    connect(reply, &QNetworkReply::sslErrors, this, &HaodooGrabber::requestSSLErrors);
}

void HaodooGrabber::networkFinished(QNetworkReply* reply)
{
    auto content = QString::fromUtf8(reply->readAll());
    qDebug() << content;

    QFile f("ok.html");
    if (f.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream sout(&f);
        sout.setCodec("UTF-8");
        sout << content;
    }
    f.close();
}

void HaodooGrabber::networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible)
{
    qDebug() << "Accessible changed";
}

void HaodooGrabber::requestReadyRead()
{
    qDebug() << "Request ready";
}

void HaodooGrabber::requestError(QNetworkReply::NetworkError)
{
    qDebug() << "Request Error";
}

void HaodooGrabber::requestSSLErrors(QList<QSslError>)
{
    qDebug() << "Request SSL Errors";
}
