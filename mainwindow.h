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

    void browseDestFolder();
    void best100ButtonClicked();
    void wisdomButtonClicked();
    void historyButtonClicked();
    void wushaButtonClicked();
    void mysteryButtonClicked();
    void romanceButtonClicked();
    void scifiButtonClicked();
    void novelButtonClicked();

    void oneBookDownloaded(QString bookName);
    void onErrorThrow(QString errorStr);

    bool checkDestFolderExists();

private:
    Ui::MainWindow *ui;

    HaodooGrabber* mGrabber = nullptr;
};

#endif // MAINWINDOW_H
