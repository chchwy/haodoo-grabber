
#pragma once
#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class HaodooGrabber : public QObject
{
    Q_OBJECT
public:
    HaodooGrabber();
    ~HaodooGrabber();

    void grabListFromCategory();

public slots:
    void networkFinished(QNetworkReply*);

private:
    QNetworkAccessManager* mNetworkManager = nullptr;
};

