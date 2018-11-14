#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "haodoograbber.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->goBookButton, &QPushButton::clicked, this, &MainWindow::goBookButtonClicked);
    mGrabber = new HaodooGrabber;
    mGrabber->grab100best();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mGrabber;
}

void MainWindow::goBookButtonClicked()
{
    //mGrabber->parseLinks();
}
