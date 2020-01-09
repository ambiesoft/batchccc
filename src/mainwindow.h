#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class QThreadPool;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_btnBrowseInput_clicked();

    void on_btnBrowseOutput_clicked();

    void on_btnStart_clicked();

private:
    Ui::MainWindow *ui;

    QThreadPool* pTaskPool_ = nullptr;
    QThreadPool* getTaskPool();
};

#endif // MAINWINDOW_H
