#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include "../../lsMisc/stdQt/inisettings.h"

namespace Ui {
class MainWindow;
}
class QThreadPool;
class QComboBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(AmbiesoftQt::IniSettings& settings);
    ~MainWindow();


private slots:

    void on_btnStart_clicked();

    void on_treeFolder_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    AmbiesoftQt::IniSettings& settings_;
    QThreadPool* pTaskPool_ = nullptr;
    QThreadPool* getTaskPool();

    QFileSystemModel* dirModel_ = nullptr;
    QFileSystemModel* fileModel_ = nullptr;

    QComboBox* cmbFind_ = nullptr;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
