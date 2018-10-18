#include "haodoograbber.h"
#include <QUrlQuery>
#include <QFile>
#include <QDomDocument>

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

void HaodooGrabber::grab100best()
{
    grabBookListFromCategory("http://www.haodoo.net/?M=hd&P=100-1");
}

void HaodooGrabber::grabBookListFromCategory(QString linkUrl)
{
    mIsSendingNetworkRequest = true;

    QUrl url(linkUrl);

    QNetworkRequest req;
    req.setUrl(url);
    req.setRawHeader("User-Agent", "Mozilla Firefox");
    
    QNetworkReply* reply = mNetworkManager->get(req);
    
    auto networkError = static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error);
    connect(reply, &QNetworkReply::readyRead, this, &HaodooGrabber::requestReadyRead);
    connect(reply, networkError, this, &HaodooGrabber::requestError);
    connect(reply, &QNetworkReply::sslErrors, this, &HaodooGrabber::requestSSLErrors);
}

void HaodooGrabber::parseCategoryHtml(QString htmlFile)
{
    QFile fin(htmlFile);
    if (!fin.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Cannot open the file";
    }

    QString rawContent = QString::fromUtf8(fin.readAll());

    QVector<QString> bookLines;

    bool inScriptTag = false;
    QTextStream sin(&rawContent);
    while (!sin.atEnd())
    {
        QString line = sin.readLine();
        //qDebug() << "Line:" << line;

        if (line.startsWith("<!DOCTYPE")) { continue; }
        if (line.startsWith("<script")) { inScriptTag = true; continue; }
        if (line.startsWith("</script")) { inScriptTag = false; continue; }

        if (inScriptTag) continue;

        if (line.contains("?M=book"))
            bookLines.append(line);
    }

    for (QString s : bookLines)
    {
        int iStart = s.indexOf("<a");
        int iEnd = s.indexOf("</a>");

        QString sub = s.mid(iStart, iEnd - iStart + 4);
        sub.replace("class=s", "");
        
        iStart = sub.indexOf("href=");
        QString sub2 = sub.mid(iStart);
        qDebug() << sub2;

        int firstQuote = sub2.indexOf("\"");
        int secondQuote = sub2.indexOf("\"", firstQuote + 1);

        QString sub3 = sub2.mid(firstQuote + 1, secondQuote - firstQuote - 1);
        qDebug() << sub3;
    }
}

void HaodooGrabber::networkFinished(QNetworkReply* reply)
{
    auto content = QString::fromUtf8(reply->readAll());
    //qDebug() << content;

    QFile f("catetory.html");
    if (f.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream sout(&f);
        sout.setCodec("UTF-8");
        sout << content;
    }
    f.close();

    reply->deleteLater();

    parseCategoryHtml("catetory.html");
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
