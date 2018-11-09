#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class HaodooGrabber;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void goBookButtonClicked();
private:
    Ui::MainWindow *ui;

    HaodooGrabber* mGrabber = nullptr;
};

#endif // MAINWINDOW_H
