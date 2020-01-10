#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "../../lsMisc/stdQt/inisettings.h"

namespace Ui {
class MainWindow;
}
class QThreadPool;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(AmbiesoftQt::IniSettings& settings);
    ~MainWindow();


private slots:
    void on_btnBrowseInput_clicked();

    void on_btnBrowseOutput_clicked();

    void on_btnStart_clicked();

private:
    Ui::MainWindow *ui;
    AmbiesoftQt::IniSettings& settings_;
    QThreadPool* pTaskPool_ = nullptr;
    QThreadPool* getTaskPool();
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
