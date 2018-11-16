#ifndef CATEGORYHTMLPARSER_H
#define CATEGORYHTMLPARSER_H

#include <QString>
#include <QStringList>

struct Book
{
    QString id;
    QString title;
    QString prcLink;
    QString epubLink;

    bool valid()
    {
        return (!prcLink.isEmpty() && !epubLink.isEmpty());
    }
};



class HtmlParser
{
public:
    HtmlParser(QString content);
    
    QStringList linksToBookPage();
    Book linksToEbookFiles();

private:
    void extractBodyTag();
    void extractHyperLinks();
    void extractBookTitle();
    bool extractEBookFileLinks();

    void invalidBook();
    QString stripHtmlTags(const QString& str);


    QString mContent;
    QStringList mLinks;
    Book mBook;
};


#endif // CATEGORYHTMLPARSER_H
