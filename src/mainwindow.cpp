#include <QFileDialog>
#include <QThreadPool>

#include "global.h"
#include "taskconvert.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifndef NDEBUG
    ui->lineInput->setText(QFileInfo("../src/testdata").absoluteFilePath());
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowseInput_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                      tr("Choose Input Directory"));

    ui->lineInput->setText(dir);
}

void MainWindow::on_btnBrowseOutput_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                      tr("Choose Output Directory"));

    ui->lineInput->setText(dir);
}

QThreadPool* MainWindow::getTaskPool()
{
    if(!pTaskPool_)
    {
        pTaskPool_ = new QThreadPool;
        pTaskPool_->setExpiryTimeout(-1);
        // Q_ASSERT(optionThreadcountGetDir_ > 0);
        pTaskPool_->setMaxThreadCount(10);
    }
    return pTaskPool_;
}

void MainWindow::on_btnStart_clicked()
{
    // todo:dir check
    gLoopId++;
    TaskConvert::TaskConvertArgs args;
    args.loopid = gLoopId;
    args.dir = ui->lineInput->text();
    TaskConvert* task = new TaskConvert(args);

    task->setAutoDelete(true);

    getTaskPool()->start(task);
}
