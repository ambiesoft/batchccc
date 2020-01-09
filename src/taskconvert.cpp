#include <string>

#include <QRegularExpression>
#include <QDirIterator>
#include <QDebug>

#include "../../lsMisc/stdQt/stdQt.h"

#include "helper.h"
#include "global.h"

#include "taskconvert.h"

using namespace AmbiesoftQt;
using namespace std;

void TaskConvert::run()
{
    runstuff(args_.dir);
}
void TaskConvert::runstuff(const QString& dir)
{
    if(gStop)
        return;
    while(gPaused)
        QThread::sleep(5);
    if(gStop)
        return;

    {
        QDirIterator itDir(args_.dir, QDir::NoDotAndDotDot|QDir::Dirs);
        while(itDir.hasNext())
        {
            runstuff(itDir.next());
        }
    }

    {
        QStringList files;
        QDirIterator itFile(dir, QDir::NoDotAndDotDot|QDir::Files);
        while(itFile.hasNext())
        {
            if(gStop)
                return;
            itFile.next();
            Q_ASSERT(itFile.fileInfo().isFile());

            QRegularExpression regex("(\\.cpp$)|(\\.h$)", QRegularExpression::CaseInsensitiveOption);
            qDebug() << itFile.fileName() << __FUNCTION__;
            QRegularExpressionMatch match = regex.match(itFile.fileName());
            if(match.hasMatch())
            {
                runFile(itFile.fileInfo());
            }
        }
        emit fileProcessed(args_.loopid,
                           itFile.fileInfo().absoluteFilePath(),
                           false,
                           true);
    }

}

namespace {
QIODevice::OpenModeFlag bbb()
{
    // if 0 returns, text is saved LF
    // if TEXT return text is saved CRLF
    return QFile::Text;
}
}

bool getByteArrayFromFile(QFile& file,
                          QByteArray& qba,
                          qint64 maxsize)
{
    qint64 fileSize = file.size();
    if(maxsize != -1 && fileSize > maxsize)
    {
        //        QMessageBox::warning(this,
        //                             qAppName(),
        //                             tr("File size is too large."));
        return false;
    }

    qba = file.peek(fileSize);
    return true;
}
void TaskConvert::runFile(const QFileInfo& fileInfo)
{
    int32_t buffsize=0;
    unique_ptr<char[]> p;
    try
    {
        QFile file(fileInfo.absoluteFilePath());
        if (Q_UNLIKELY(!file.open(QFile::ReadOnly| bbb())))
            throw file.errorString();


        QByteArray allBytes;
        if(!getByteArrayFromFile(file, allBytes, 10 * 1024 * 1024))
            throw tr("File size");

        string charcode = GetDetectedCodecICU(allBytes);

        p = (ConvertToUTF8(charcode, allBytes, &buffsize));
        if(!p)
            throw tr("Failed to convert");
    }
    catch (QString& error)
    {
        emit fileFailed(loopid(),
                        fileInfo,
                        error);
        return;

    }

    QString newFilePath = fileInfo.absoluteFilePath() + ".new";
    try {

        QFile newFile(newFilePath);
        if(!newFile.open(QFile::WriteOnly | QFile::NewOnly))
            throw newFile.errorString();

        if(true)
        {
            unsigned char bom[] = {0xEF, 0xBB, 0xBF};
            if(sizeof(bom) != newFile.write((char*)bom, sizeof(bom)))
                throw newFile.errorString();

        }
        if(buffsize != newFile.write(p.get(), buffsize))
            throw newFile.errorString();
    }
    catch (QString& error)
    {
        emit fileFailed(loopid(),
                        newFilePath,
                        error);
        return;
    }

    bool backup=false;
    QString backFilePath = backup ?
                fileInfo.absoluteFilePath() + ".bk" + thistick() :
                QString();

    QString error;
    if(!Move3Files(fileInfo.absoluteFilePath(),
               newFilePath,
               backFilePath,
               &error))
    {
        emit fileFailed(loopid(),
                        fileInfo.absoluteFilePath(),
                        error);
        return;
    }

    emit fileProcessed(loopid(),
                       fileInfo,
                       false,
                       true);
}
