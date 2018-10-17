#include "haodoograbber.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

HaodooGrabber::HaodooGrabber() : QObject()
{
    mNetworkManager = new QNetworkAccessManager(this);

    connect(mNetworkManager, &QNetworkAccessManager::finished, this, &HaodooGrabber::networkFinished);
}

HaodooGrabber::~HaodooGrabber()
{

}

void HaodooGrabber::grabListFromCategory()
{
    QUrl url("http://www.haodoo.net/?M=hd&P=100");
    mNetworkManager->get(QNetworkRequest(url));
}

void HaodooGrabber::networkFinished(QNetworkReply* reply)
{
    QString content = reply->readAll();
    qDebug() << content;
}
