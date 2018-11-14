#include "htmlparser.h"
#include <QDebug>

HtmlParser::HtmlParser(QString content)
{
    mContent = content;
    extractBodyTag();   
}

QStringList HtmlParser::linksToBookPage()
{
    extractHyperLinks();

    QStringList actualBookLinks;
    for (const QString& s : mLinks)
    {
        if (s.contains("M=book"))
        {
            actualBookLinks.append("http://www.haodoo.net/" + s);
        }
    }
    mLinks = actualBookLinks;
    return mLinks;
}

Book HtmlParser::linksToEbookFiles()
{
    extractBookTitle();
    extractEBookFileLinks();
    return mBook;
}

void HtmlParser::extractBodyTag()
{
    int b1 = mContent.indexOf("<body");
    int b2 = mContent.indexOf("</body>");

    QString body = mContent.mid(b1, b2 - b1);
    //qDebug() << body;

    mContent = body;
}

void HtmlParser::extractHyperLinks()
{
    int offset = 0;
    int length = mContent.length();

    const QString aStartTag = "<a ";
    const QString aEndStag = "</a>";

    QStringList links01;

    while (offset < length)
    {
        int aPos = mContent.indexOf(aStartTag, offset);
        int aEnd = mContent.indexOf(aEndStag, aPos);

        if (aPos == -1) break;

        QString aTagContent = mContent.mid(aPos, aEnd - aPos);
        //qDebug() << aTagContent;

        links01.append(aTagContent);

        offset = aEnd;
    }

    QString href("href=\"");

    QStringList links02;
    for (const QString& s : links01)
    {
        int begin = s.indexOf(href);
        int end = s.indexOf('"', begin + href.length());

        if (begin < 0 || end < 0)
            continue;

        begin += href.length();

        QString s2 = s.mid(begin, end - begin);
        //qDebug() << s2;
        links02.append(s2);
    }
    links01.clear();
    
    mLinks = links02;
}

void HtmlParser::extractBookTitle()
{
    QString setTitle("SetTitle(\"");
    int begin = mContent.indexOf(setTitle);
    begin += setTitle.length();

    int end = mContent.indexOf('"', begin);

    QString title = mContent.mid(begin, end - begin).trimmed();
    //qDebug() << title;

    mBook.title = title;
}

void HtmlParser::extractEBookFileLinks()
{
    const QString pattern = "http://www.haodoo.net/?M=d&P=%1.%2";

    QString downloadPrc("DownloadPrc('");
    int begin = mContent.indexOf(downloadPrc);
    begin += downloadPrc.length();

    QString endQuote(')');
    int end = mContent.indexOf(endQuote, begin);

    QString bookID = mContent.mid(begin, end - begin - 1);
    //qDebug() << "PRC:" << bookID;

    QString PRCLink = QString(pattern).arg(bookID).arg("prc");
    //qDebug() << PRCLink;

    QString EPUBLink = QString(pattern).arg(bookID).arg("epub");
    //qDebug()

    mBook.epubLink = EPUBLink;
    mBook.prcLink = PRCLink;
}