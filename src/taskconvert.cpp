#include <string>

#include <QRegularExpression>
#include <QDirIterator>

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

            QRegularExpression regex("\\.cpp$", QRegularExpression::CaseInsensitiveOption);
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
    QFile file(fileInfo.absoluteFilePath());
    if (Q_UNLIKELY(!file.open(QFile::ReadOnly| bbb())))
    {
        Alert(nullptr,
              tr("Cannot read file %1:\n%2.")
              .arg(QDir::toNativeSeparators(fileInfo.absoluteFilePath()), file.errorString()));
        return;
    }

    QByteArray allBytes;
    if(!getByteArrayFromFile(file, allBytes, 10 * 1024 * 1024))
        return;

    string charcode = GetDetectedCodecICU(allBytes);

    int32_t buffsize=0;
    unique_ptr<char[]> p(ConvertToUTF8(charcode, allBytes, &buffsize));
    if(!p)
    {
        Alert(nullptr,"fail");
        return;
    }

    QString newFilePath = fileInfo.absoluteFilePath() + ".new";
    QFile newFile(newFilePath);
    if(!newFile.open(QFile::WriteOnly | QFile::NewOnly))
    {
        Alert(nullptr,"ng");
        return;
    }
    if(buffsize != newFile.write(p.get(), buffsize))
    {
        Alert(nullptr,"ng");
        return;
    }
}
