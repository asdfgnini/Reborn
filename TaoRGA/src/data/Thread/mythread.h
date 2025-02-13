#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QDateTime>

class MyThread : public QThread
{
    Q_OBJECT

public:
    enum class ThreadState {
        Stopped,
        Running,
        Paused,
        Error
    };

    explicit MyThread(QObject *parent = nullptr);
    virtual ~MyThread();

    // 线程控制函数
    void startThread();
    void pauseThread();
    void resumeThread();
    void stopThread();
    
    // 状态查询函数
    ThreadState getState() const;
    bool isRunning() const;
    bool isPaused() const;

signals:
    void stateChanged(ThreadState newState);
    void errorOccurred(const QString& error);
    void finished();
protected:
    virtual void run() override final;  // 标记为final，防止子类重写
    virtual void processTask() = 0;     // 纯虚函数，子类必须实现具体任务

    void setError(const QString& error);
    bool checkPaused();                 // 检查是否需要暂停
    bool shouldContinue() const;        // 检查是否应该继续运行

private:
    void setState(ThreadState newState);

    mutable QMutex m_mutex;            // 用于保护状态访问
    QWaitCondition m_pauseCondition;   // 用于实现暂停功能
    ThreadState m_state;               // 线程状态
    bool m_isRunning;                  // 运行标志
    bool m_isPaused;                   // 暂停标志
    QString m_lastError;               // 最后的错误信息
};

#endif // MYTHREAD_H