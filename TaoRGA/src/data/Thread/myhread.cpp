#include "mythread.h"
#include <QDebug>
#include <QCoreApplication>

MyThread::MyThread(QObject *parent)
    : QThread(parent)
    , m_state(ThreadState::Stopped)
    , m_isRunning(false)
    , m_isPaused(false)
{
    
}

MyThread::~MyThread()
{
    stopThread();
    quit();
    wait();
}

void MyThread::startThread()
{
    QMutexLocker locker(&m_mutex);
    qDebug() << "startThread called, current state:" << static_cast<int>(m_state);
    
    if (m_state == ThreadState::Stopped) {
        m_isRunning = true;
        m_isPaused = false;
        setState(ThreadState::Running);
        
        qDebug() << "Starting new thread...";
        start();
    } 
    else if (m_state == ThreadState::Paused) {
        qDebug() << "Resuming from pause...";
        resumeThread();
    }
}

void MyThread::pauseThread()
{
    QMutexLocker locker(&m_mutex);
    if (m_state == ThreadState::Running) {
        qDebug() << "Pausing thread...";
        m_isPaused = true;
        setState(ThreadState::Paused);
    }
}

void MyThread::resumeThread()
{
    QMutexLocker locker(&m_mutex);
    if (m_state == ThreadState::Paused) {
        qDebug() << "Resuming thread...";
        m_isPaused = false;
        setState(ThreadState::Running);
        m_pauseCondition.wakeAll();
    }
}

void MyThread::stopThread()
{
    {
        QMutexLocker locker(&m_mutex);
        if (m_state != ThreadState::Stopped) {
            qDebug() << "Stopping thread...";
            m_isRunning = false;
            m_isPaused = false;
            setState(ThreadState::Stopped);
            m_pauseCondition.wakeAll();
        }
    }
    
    if (isRunning()) {
        qDebug() << "Waiting for thread to finish...";
        wait();
        qDebug() << "Thread stopped successfully";
    }
}

MyThread::ThreadState MyThread::getState() const
{
    QMutexLocker locker(&m_mutex);
    return m_state;
}

bool MyThread::isRunning() const
{
    QMutexLocker locker(&m_mutex);
    return m_isRunning;
}

bool MyThread::isPaused() const
{
    QMutexLocker locker(&m_mutex);
    return m_isPaused;
}

void MyThread::run()
{
    qDebug() << "Thread started running";
    try {
        while (m_isRunning) {
            {
                QMutexLocker locker(&m_mutex);
                if (m_isPaused) {
                    qDebug() << "Thread pausing...";
                    m_pauseCondition.wait(&m_mutex);
                    qDebug() << "Thread resumed from pause";
                    continue;
                }
            }
            
            processTask();
            
        }
    }
    catch (const std::exception& e) {
        setError(QString("Exception in thread: %1").arg(e.what()));
    }
    catch (...) {
        setError("Unknown error in thread");
    }
    
    qDebug() << "Thread finished running";
    emit finished();
}

void MyThread::setError(const QString& error)
{
    QMutexLocker locker(&m_mutex);
    m_lastError = error;
    setState(ThreadState::Error);
    qDebug() << "Thread error:" << error;
}

bool MyThread::checkPaused()
{
    QMutexLocker locker(&m_mutex);
    if (m_isPaused) {
        qDebug() << "Thread pausing in checkPaused...";
        m_pauseCondition.wait(&m_mutex);
        qDebug() << "Thread resumed in checkPaused";
        return true;
    }
    return false;
}

bool MyThread::shouldContinue() const
{
    QMutexLocker locker(&m_mutex);
    return m_isRunning && !m_isPaused;
}

void MyThread::setState(ThreadState newState)
{
    if (m_state != newState) {
        m_state = newState;
        qDebug() << "Thread state changed to:" << static_cast<int>(newState);
        emit stateChanged(newState);
    }
}