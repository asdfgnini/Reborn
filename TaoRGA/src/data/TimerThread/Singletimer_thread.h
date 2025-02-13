#ifndef SINGLETIMER_THREAD_H
#define SINGLETIMER_THREAD_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
#include <QMutex>
#include <QCoreApplication>
#include <QDebug>  // 用于输出调试信息

// 定义回调类型，回调函数在定时器超时时被调用
typedef void (*MY_QT_CALLBACK1)(QObject* sender, uint32_t code, void* userData);  // 定时器1的回调函数

class SingleMY_Timer;
class SingleMY_Timer : public QObject
{
    Q_OBJECT
private:
    MY_QT_CALLBACK1 pfCallback;  // 定时器1的回调函数
    uint32_t Time_Out;  // 定时器超时时间（单位：毫秒）
    bool Thread_Flag;  // 是否启用线程
    QThread Thread;  // 用于线程管理
    bool Timer_Flag;  // 定时器是否正在运行的标志

public slots:
    /**
     * @brief 定时器超时的槽函数。
     * 
     * 该函数在定时器超时时被调用，并且会调用回调函数（pfCallback）。
     */
    void timerTimeOut(void)
    {
        pfCallback(this, 0, NULL);  // 调用回调函数，传递定时器标识符0
    }

    /**
     * @brief 停止定时器并执行清理工作。
     * 
     * 该函数用于停止定时器工作，并在有线程时退出线程。
     */
    void Judg_doWork(void)
    {
        this->Timer.stop();  // 停止定时器
        Timer_Flag = false;  // 更新定时器状态标志
    }

signals:
    /**
     * @brief 启动定时器工作的信号。
     * 
     * 该信号会触发定时器清理工作。
     */
    void startWork(void);

public:
    QTimer Timer;  // 定义定时器对象

    /**
     * @brief 获取当前定时器的超时时间。
     * 
     * @return uint32_t 定时器超时时间（单位：毫秒）。
     */
    uint32_t Get_Time_Out(void)
    {
        return this->Time_Out;  // 返回定时器的超时时间
    }

    /**
     * @brief 更改定时器的超时时间。
     * 
     * @param time_out 新的超时时间（单位：毫秒）。
     * 
     * 该函数更改定时器的超时时间，并重新设置定时器的间隔。
     */
    void Change_Time_Out(uint32_t time_out)
    {
        Stop_Timer();  // 停止当前定时器
        this->Time_Out = time_out;  // 更新定时器的超时时间
        this->Timer.setInterval(this->Time_Out);  // 设置新的超时时间
        this->Start_Timer();
    }

    /**
     * @brief 停止定时器的工作。
     * 
     * 该函数用于停止定时器的工作。如果定时器在运行并启用了线程，它将退出线程。
     */
    void Stop_Timer(void)
    {
        if (Timer_Flag)  // 如果定时器正在运行
        {
            if (this->Thread_Flag)  // 如果启用了线程
            {
                emit this->startWork();  // 发出工作信号，执行清理工作
                QElapsedTimer t;
                t.start();
                while (t.elapsed() < 5000 && Timer_Flag) {
                    QCoreApplication::processEvents();  // 处理事件，防止阻塞应用
                }
                this->Thread.quit();  // 退出线程
            }
            else  // 如果没有启用线程
            {
                Timer_Flag = false;  // 更新定时器状态标志
                this->Timer.stop();  // 停止定时器
            }
        }
    }

    /**
     * @brief 启动定时器。
     * 
     * 该函数用于启动定时器。如果启用了线程，它会启动线程，否则直接启动定时器。
     */
    void Start_Timer(void)
    {
        if (!Timer_Flag)  // 如果定时器没有启动
        {
            Timer_Flag = true;  // 设置定时器状态为启动
            if (this->Thread_Flag)  // 如果启用了线程
            {
                this->Thread.start();  // 启动线程
            }
            else
            {
                this->Timer.start();  // 启动定时器
            }
        }
    }

    /**
     * @brief 构造函数，初始化定时器和回调函数。
     * 
     * @param pfunc 定时器超时后调用的回调函数。
     * @param time_out 定时器超时时间（单位：毫秒）。
     * @param Thread_EnableNotDisable 是否启用线程（默认值为 `false`，不启用线程）。
     * 
     * 该函数用于设置定时器的超时时间，选择是否启用线程，并初始化回调函数。
     */
    SingleMY_Timer(MY_QT_CALLBACK1 const pfunc, uint32_t time_out, bool Thread_EnableNotDisable = false)
    {
        this->Timer_Flag = false;  // 初始化定时器状态为未启动
        pfCallback = pfunc;  // 设置回调函数
        this->Time_Out = time_out;  // 设置定时器超时时间
        this->Timer.setInterval(this->Time_Out);  // 设置定时器间隔时间
        this->Thread_Flag = Thread_EnableNotDisable;  // 设置是否启用线程

        if (this->Thread_Flag)  // 如果启用了线程
        {
            // 将定时器移到该线程中运行
            this->Timer.moveToThread(&this->Thread);
            this->moveToThread(&this->Thread);  // 当前对象也移到线程中
            connect(&this->Thread, SIGNAL(started()), &this->Timer, SLOT(start()));  // 连接线程启动信号和定时器启动槽
            connect(this, SIGNAL(startWork()), this, SLOT(Judg_doWork()));  // 连接工作信号和清理工作槽
            connect(&this->Timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()), Qt::DirectConnection);  // 连接定时器超时信号和槽
        }
        else  // 如果没有启用线程
        {
            connect(&this->Timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));  // 直接连接定时器超时信号和槽
        }
    }

    /**
     * @brief 析构函数，清理定时器和线程资源。
     * 
     * 在对象销毁时停止定时器并清理线程。
     */
    ~SingleMY_Timer()
    {
        Stop_Timer();  // 停止定时器
        this->Thread.deleteLater();  // 删除线程资源
    }
};

#endif // MY_TIMER_H
