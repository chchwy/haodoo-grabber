#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "haodoograbber.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->best100Button, &QPushButton::clicked, this, &MainWindow::best100ButtonClicked);
    connect(ui->wisdomButton, &QPushButton::clicked, this, &MainWindow::wisdomButtonClicked);

    mGrabber = new HaodooGrabber;
    connect(mGrabber, &HaodooGrabber::bookDownloaded, this, &MainWindow::oneBookDownloaded);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mGrabber;
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
    for (int i = 3; i <= 4; ++i)
    {
        urlList.append(QString("http://www.haodoo.net/?M=hd&P=wisdom-%1").arg(i));
    }
    mGrabber->grabBooksFromCategory(urlList);
}

void MainWindow::oneBookDownloaded(QString bookName)
{
    ui->logWidget->addItem("下載: " + bookName);
}
