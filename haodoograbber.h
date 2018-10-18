
#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class HaodooGrabber : public QObject
{
    Q_OBJECT
public:
    HaodooGrabber();
    ~HaodooGrabber();

    void grabListFromCategory();

public slots:
    void networkFinished(QNetworkReply*);
    void networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);

    void requestReadyRead();
    void requestError(QNetworkReply::NetworkError);
    void requestSSLErrors(QList<QSslError>);

private:
    QNetworkAccessManager* mNetworkManager = nullptr;
};

