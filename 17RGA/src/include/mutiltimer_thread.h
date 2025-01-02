#ifndef MULTIMY_TIMER_H
#define MULTIMY_TIMER_H

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QThreadPool>
#include <QRunnable>
#include <QDebug>
#include <QReadWriteLock>
#include <functional>

class MultiMY_Timer : public QObject
{
    Q_OBJECT

private:
    QMap<int, QTimer*> timers;  // 定时器容器
    QMap<int, uint32_t> timeoutValues;  // 定时器超时时间
    QMap<int, bool> timerFlags;  // 定时器状态
    QThreadPool threadPool;  // 线程池
    QReadWriteLock lock;  // 锁，保证线程安全

public:
    explicit MultiMY_Timer(QObject* parent = nullptr) : QObject(parent) {}

    // 添加定时器，使用模板来绑定成员函数
    template <typename T>
    void addTimer(int timerId, T* receiver, void (T::*memberFunction)(), uint32_t timeout, bool useThread = false)
    {
        if (!timers.contains(timerId)) {
            QTimer* newTimer = new QTimer();
            connect(newTimer, &QTimer::timeout, this, [=]() {
                // 使用线程池执行任务
                if (useThread) {
                    std::function<void()> func = std::bind(memberFunction, receiver);  // 绑定成员函数
                    TimerTask* task = new TimerTask(receiver, func);
                    threadPool.start(task);  // 提交任务到线程池
                } else {
                    QMetaObject::invokeMethod(receiver, memberFunction);  // 直接调用成员函数
                }
            });
            timers[timerId] = newTimer;
            timeoutValues[timerId] = timeout;
            timerFlags[timerId] = false;
            newTimer->setInterval(timeout);
        }
    }

    // 启动定时器
    void startTimer(int timerId)
    {
        if (timers.contains(timerId) && !timerFlags[timerId]) {
            timers[timerId]->start();
            timerFlags[timerId] = true;
            qDebug() << "Timer" << timerId << "started.";
        }
    }

    // 停止定时器
    void stopTimer(int timerId)
    {
        if (timers.contains(timerId) && timerFlags[timerId]) {
            timers[timerId]->stop();
            timerFlags[timerId] = false;
            qDebug() << "Timer" << timerId << "stopped.";
        }
    }

    ~MultiMY_Timer()
    {
        for (auto timer : timers) {
            timer->stop();
            delete timer;
        }
        threadPool.waitForDone();  // 等待线程池中的所有任务完成
    }

private:
    // 定时器任务类，用于执行定时任务
    class TimerTask : public QRunnable {
    public:
        // 使用函数指针和对象传递来调用成员函数
        TimerTask(QObject* receiver, std::function<void()> function)
            : m_receiver(receiver), m_function(function) {}

        void run() override {
            if (m_function) {
                m_function();  // 执行绑定的函数
            }
        }

    private:
        QObject* m_receiver;
        std::function<void()> m_function;  // 用 std::function 来存储函数
    };
};

#endif // MULTIMY_TIMER_H
