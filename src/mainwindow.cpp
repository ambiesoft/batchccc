#include <QFileDialog>
#include <QThreadPool>

#include "global.h"
#include "helper.h"
#include "consts.h"
#include "taskconvert.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace AmbiesoftQt;
using namespace Consts;

MainWindow::MainWindow(IniSettings& settings) :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow),
    settings_(settings)
{
    ui->setupUi(this);

#ifndef NDEBUG
    QString str;
//    char* p = ConvertToUTF8_obsolete("euc-jp");
//    str = QString::fromUtf8(p);
//    ui->editLog->setPlainText(str);
//    delete[] p;


    const char src[]     = "\xC6\xFC\xCB\xDC"; // "日本"のeuc-jp表現
      QByteArray ar(src, sizeof(src)-1);
//      unsigned char utf8[] = {0xE6,0x97,0xA5,0xE6,0x9C,0xAC,0x0};
//      QByteArray ar((char*)utf8, sizeof(utf8));
    int32_t buffsize=0;
    auto pp = ConvertToUTF8("euc-jp", ar, &buffsize);
    str= QString::fromUtf8(pp.get(), buffsize);
    ui->editLog->setPlainText(str);

    ui->lineInput->setText(QFileInfo("../src/testdata").absoluteFilePath());
#endif


    restoreGeometry(settings.value(KEY_GEOMETRY).toByteArray());

    if(!restoreState(settings.value(KEY_WINDOWSTATE).toByteArray()))
    {
        // this must be called to make docking windows sizable properly
        settings_.setValue(KEY_WINDOWSTATE, saveState());
        restoreState(settings.value(KEY_WINDOWSTATE).toByteArray());
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event)

    gPaused = false;
    // WaitingCursor wc;


    settings_.setValue(KEY_GEOMETRY, saveGeometry());
    settings_.setValue(KEY_WINDOWSTATE, saveState());

    settings_.sync();
    __super::closeEvent(event);
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
