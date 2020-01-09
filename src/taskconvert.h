#ifndef TASKCONVERT_H
#define TASKCONVERT_H

#include <QObject>
#include <QRunnable>
#include <QThread>

class QFileInfo;
class TaskConvert : public QObject, public QRunnable
{
    Q_OBJECT


public:
    class TaskConvertArgs
    {
      public:
        int loopid;
        QString dir;
        QString includeFilterDirectory;
        QString includeFilterFile;
    };
    const TaskConvertArgs args_;
    explicit TaskConvert(const TaskConvertArgs& args) :
        QObject(nullptr),
        args_(args){};

    void run() override;
    void runstuff(const QString& dir);
    void runFile(const QFileInfo& file);

signals:
    void dirProcessed(int loopid, const QString& dir,
                      bool skipped);
    void fileProcessed(int loopid, const QString& file,
                       bool skipped,
                       bool converted);
};

#endif // TASKCONVERT_H
