#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QSettings>
#include "haodoograbber.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->openFolderButton, &QPushButton::clicked, this, &MainWindow::browseDestFolder);
    connect(ui->best100Button, &QPushButton::clicked, this, &MainWindow::best100ButtonClicked);
    connect(ui->wisdomButton, &QPushButton::clicked, this, &MainWindow::wisdomButtonClicked);
    connect(ui->historyButton, &QPushButton::clicked, this, &MainWindow::historyButtonClicked);
    connect(ui->wushaButton, &QPushButton::clicked, this, &MainWindow::wushaButtonClicked);

    mGrabber = new HaodooGrabber;
    connect(mGrabber, &HaodooGrabber::bookDownloaded, this, &MainWindow::oneBookDownloaded);

    QSettings settings("haodoo-gragger.ini", QSettings::IniFormat);
    QString lastDestFolder = settings.value("DestFolder").toString();
    ui->destLineEdit->setText(lastDestFolder);
    mGrabber->setDestFolder(lastDestFolder);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mGrabber;
}

void MainWindow::browseDestFolder()
{
    QString s = QFileDialog::getExistingDirectory(this, "下載目錄");
    if (!s.isEmpty())
    {
        ui->destLineEdit->setText(s);
        mGrabber->setDestFolder(s);

        QSettings settings("haodoo-gragger.ini", QSettings::IniFormat);
        settings.setValue("DestFolder", s);
    }
}

void MainWindow::best100ButtonClicked()
{
    ui->logWidget->addItem(QString::fromUtf8("開始解析: 世紀百強"));
    QStringList best100 =
    {
        "http://www.haodoo.net/?M=hd&P=100-1",
        "http://www.haodoo.net/?M=hd&P=100-2",
        "http://www.haodoo.net/?M=hd&P=100-3",
        "http://www.haodoo.net/?M=hd&P=100-4",
        "http://www.haodoo.net/?M=hd&P=100-5",
    };
    mGrabber->grabBooksFromCategory(best100);
}

void MainWindow::wisdomButtonClicked()
{
    ui->logWidget->addItem(QString::fromUtf8("開始解析: 隨身智囊"));
    
    QStringList urlList;
    for (int i = 1; i <= 6; ++i)
    {
        urlList.append(QString("http://www.haodoo.net/?M=hd&P=wisdom-%1").arg(i));
    }
    mGrabber->grabBooksFromCategory(urlList);
}

void MainWindow::historyButtonClicked()
{
    ui->logWidget->addItem(QString::fromUtf8("開始解析: 歷史煙雲"));
    QStringList urlList;
    for (int i = 1; i <= 3; ++i)
    {
        urlList.append(QString("http://www.haodoo.net/?M=hd&P=history-%1").arg(i));
    }
    mGrabber->grabBooksFromCategory(urlList);
}

void MainWindow::wushaButtonClicked()
{
    ui->logWidget->addItem(QString::fromUtf8("開始解析: 武俠小說"));
    QStringList urlList;
    for (int i = 1; i <= 10; ++i)
    {
        urlList.append(QString("http://www.haodoo.net/?M=hd&P=martial-%1").arg(i));
    }
    mGrabber->grabBooksFromCategory(urlList);
}

void MainWindow::oneBookDownloaded(QString bookName)
{
    ui->logWidget->addItem("下載: " + bookName);
}
