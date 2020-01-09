#ifndef TASKCONVERT_H
#define TASKCONVERT_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QDateTime>

class QFileInfo;
class TaskConvert : public QObject, public QRunnable
{
    Q_OBJECT

    QString thistick_;
protected:
    int loopid() const {
        return args_.loopid;
    }
    const QString& thistick() const {
        return thistick_;
    }
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
        args_(args){
            thistick_ = QString::number(QDateTime::currentDateTimeUtc().toMSecsSinceEpoch());
    };

    void run() override;
    void runstuff(const QString& dir);
    void runFile(const QFileInfo& file);

signals:
    void dirProcessed(int loopid, const QString& dir,
                      bool skipped);
    void fileProcessed(int loopid, const QFileInfo& file,
                       bool skipped,
                       bool converted);
    void fileFailed(int loopid,
                    const QFileInfo& file,
                    const QString& error);
};

#endif // TASKCONVERT_H
