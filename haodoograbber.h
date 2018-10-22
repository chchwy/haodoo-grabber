
#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>
#include <QStringList>


class HaodooLinks
{
    static QVector<QString> get100best()
    {
        return QVector<QString>
        {
            "http://www.haodoo.net/?M=hd&P=100-1",
            "http://www.haodoo.net/?M=hd&P=100-2",
            "http://www.haodoo.net/?M=hd&P=100-3",
            "http://www.haodoo.net/?M=hd&P=100-4",
            "http://www.haodoo.net/?M=hd&P=100-5",
        };
    }
};

class HaodooGrabber : public QObject
{
    Q_OBJECT
public:
    HaodooGrabber();
    ~HaodooGrabber();

    //
    void grab100best();

    void grabBookListFromCategory(QString linkUrl);
    QStringList parseCategoryHtml(QString htmlFile);

public slots:
    void networkFinished(QNetworkReply*);
    void networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);

    void requestReadyRead();
    void requestError(QNetworkReply::NetworkError);
    void requestSSLErrors(QList<QSslError>);


private:
    QNetworkAccessManager* mNetworkManager = nullptr;
    bool mIsSendingNetworkRequest = false;

    QStringList mLinks;
};

