#include "haodoograbber.h"
#include <QFile>
#include <QDir>

HaodooGrabber::HaodooGrabber() : QObject()
{
    mNetworkManager = new QNetworkAccessManager(this);
    connect(mNetworkManager, &QNetworkAccessManager::finished, this, &HaodooGrabber::networkFinished);
    connect(mNetworkManager, &QNetworkAccessManager::networkAccessibleChanged, this, &HaodooGrabber::networkAccessibleChanged);

    mTimer = new QTimer(this);
    mTimer->setTimerType(Qt::VeryCoarseTimer);
    connect(mTimer, &QTimer::timeout, this, &HaodooGrabber::timerTick);
    mTimer->start(1000);

    // for debugging
    //mBookPageLinks.append("http://www.haodoo.net/?M=book&P=1132");
}

HaodooGrabber::~HaodooGrabber()
{
    delete mNetworkManager;
}

void HaodooGrabber::grabBooksFromCategory(const QStringList& urls)
{
    for (const QString& s : urls)
    {
        mCategoryLinks.append(s);
    }
}

void HaodooGrabber::sendWebRequest(QString linkUrl)
{
    if (linkUrl.isEmpty()) return;

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

QStringList HaodooGrabber::parseCategoryHtml(QString htmlFile)
{
    const QString& rawContent = htmlFile;
    QVector<QString> bookLines;

    HtmlParser htmlParser(rawContent);
    return htmlParser.linksToBookPage();
}

Book HaodooGrabber::parseBookPageHtml(QString htmlFile)
{
    const QString& rawContent = htmlFile;
    QVector<QString> bookLines;

    HtmlParser htmlParser(rawContent);
    return htmlParser.linksToEbookFiles();
}

void HaodooGrabber::networkFinished(QNetworkReply* reply)
{
    QString urlString = reply->url().toString();
    
    if (urlString.contains("M=hd"))
    {
        auto content = QString::fromUtf8(reply->readAll());
        qDebug() << "It's a category! [" << urlString << "]";
        QStringList newLinks = parseCategoryHtml(content);

        mtx.lock();
        mBookPageLinks.append(newLinks);
        mtx.unlock();
    }
    else if (urlString.contains("M=book") || urlString.contains("M=Share"))
    {
        auto content = QString::fromUtf8(reply->readAll());
        qDebug() << "It's a book [" << urlString << "]";
        Book book = parseBookPageHtml(content);

        if (book.valid())
        {
            mtx.lock();
            mBooks.append(book);
            mtx.unlock();
        }
        //qDebug() << book.title << book.epubLink << book.prcLink;
        
    }
    else if (urlString.contains("?M=d&P="))
    {
        auto content = reply->readAll();

        QString fileName = getBookFileNameFromLink(urlString);

        QString fullPath = QDir(mDestFolder).filePath(fileName);
        QFile file(fullPath);
        if (file.open(QIODevice::WriteOnly))
        {
            file.write(content);
        }
        file.close();
        qDebug() << "Downloaded [" << fileName << "] from " << urlString;
        emit bookDownloaded(fileName);
    }
    else 
    {
        qDebug() << "Can't recognize the url:" << urlString;
    }

    reply->deleteLater();
}

void HaodooGrabber::networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)
{
    qDebug() << "Accessible changed";
}

void HaodooGrabber::requestReadyRead()
{
    //qDebug() << "Request ready";
}

void HaodooGrabber::requestError(QNetworkReply::NetworkError)
{
    qDebug() << "Request Error";
}

void HaodooGrabber::requestSSLErrors(QList<QSslError>)
{
    qDebug() << "Request SSL Errors";
}

void HaodooGrabber::timerTick()
{
    mtx.lock();
    if (mBooks.isEmpty() == false)
    {
        Book b = mBooks.takeFirst();
        sendWebRequest(b.prcLink);
        sendWebRequest(b.epubLink);
        mDownloadedBook.append(b);
        mtx.unlock();
        return;
    }
    mtx.unlock();

    mtx.lock();
    if (mBookPageLinks.isEmpty() == false)
    {
        QString url = mBookPageLinks.takeFirst();
        sendWebRequest(url);
        mtx.unlock();
        return;
    }
    mtx.unlock();

    mtx.lock();
    if (mCategoryLinks.isEmpty() == false)
    {
        QString url = mCategoryLinks.takeFirst();
        sendWebRequest(url);
        mtx.unlock();
        return;
    }
    mtx.unlock();
}

QString HaodooGrabber::getBookFileNameFromLink(QString link)
{
    //mtx.lock();
    for (const Book& b : mDownloadedBook)
    {
        if (link == b.epubLink)
            return b.title + ".epub";
        if (link == b.prcLink)
            return b.title + ".prc";
    }
    //mtx.unlock();
    qDebug() << "Couldn't find the book";
    return "";
}
