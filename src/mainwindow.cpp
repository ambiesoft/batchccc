#include <QFileDialog>
#include <QThreadPool>
#include <QDebug>
#include <QComboBox>

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

    // ui->lineInput->setText(QFileInfo("../src/testdata").absoluteFilePath());
#endif

    // Toolbar
    // ComboBox
    cmbFind_ = new QComboBox(ui->mainToolBar);
    cmbFind_->setMinimumWidth(100);
    cmbFind_->setMaximumWidth(100);
    cmbFind_->setEditable(true);
    // QStringList findtexts = settings_.valueStringList(KEY_COMBO_FINDTEXTS);
    // cmbFind_->addItems(findtexts);
    cmbFind_->setEditText("");
//    connect(cmbFind_, &FindComboBox::on_EnterPressed,
//            this, &MainWindow::OnFindComboEnterPressed);
    ui->mainToolBar->insertWidget(ui->placeHolder_ComboPath, cmbFind_);
    ui->mainToolBar->removeAction(ui->placeHolder_ComboPath);


    // Prepare treeFolder
    // QString sPath = "F:/";
    // tree view
    dirModel_ = new QFileSystemModel(this);
    ui->treeFolder->setModel(dirModel_);
    // first column is the name
    for (int i = 1; i < ui->treeFolder->header()->length(); ++i)
        ui->treeFolder->hideColumn(i);

    dirModel_->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Drives);
    QString sPath;
#ifndef NDEBUG
    sPath = QFileInfo("../src/testdata").absoluteFilePath();
#endif
    dirModel_->setRootPath(sPath);
    ui->treeFolder->expand(dirModel_->index(sPath));

    // ui->treeFolder->setRootIndex(dirModel_->setRootPath(sPath));


    // file view
    fileModel_ = new QFileSystemModel(this);
    ui->treeFile->setModel(fileModel_);
    fileModel_->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel_->setRootPath(sPath);
    ui->treeFile->setRootIndex(fileModel_->setRootPath(sPath));

    // Ini
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
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_btnBrowseInput_clicked()
//{
//    QString dir = QFileDialog::getExistingDirectory(this,
//                                      tr("Choose Input Directory"));

//    ui->lineInput->setText(dir);
//}

//void MainWindow::on_btnBrowseOutput_clicked()
//{
//    QString dir = QFileDialog::getExistingDirectory(this,
//                                      tr("Choose Output Directory"));

//    ui->lineInput->setText(dir);
//}

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
    args.dir = dirModel_->fileInfo(ui->treeFolder->currentIndex()).absoluteFilePath();
    TaskConvert* task = new TaskConvert(args);

    task->setAutoDelete(true);

    getTaskPool()->start(task);
}

// TODO: move to header
#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION __FILE__ " : " S2(__LINE__)
void MainWindow::on_treeFolder_clicked(const QModelIndex &index)
{
    QString sPath = dirModel_->fileInfo(index).absoluteFilePath();
    qDebug() << sPath << LOCATION;
    ui->treeFile->setRootIndex(fileModel_->setRootPath(sPath));
    //fileModel_->setRootPath(sPath);
}
