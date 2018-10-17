#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "haodoograbber.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    HaodooGrabber haodoo;
    haodoo.grabListFromCategory();
}

MainWindow::~MainWindow()
{
    delete ui;
}
