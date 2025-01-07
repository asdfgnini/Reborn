#pragma once

#include <QList>
#include <QMutex>
#include <QWaitCondition>

struct Post {
    QString data;
    QString name;
    QString origin;
};


class RingBuffer {
public:
    RingBuffer(int size) : m_size(size), m_readIndex(0), m_writeIndex(0) {
        m_buffer.resize(size);
    }

    bool write(const Post &post) {
        QMutexLocker locker(&m_mutex);
        if ((m_writeIndex + 1) % m_size == m_readIndex) {
            return false; // 缓冲区已满
        }
        m_buffer[m_writeIndex] = post;
        m_writeIndex = (m_writeIndex + 1) % m_size;
        m_condition.wakeOne(); // 通知读取线程
        return true;
    }

    bool read(Post &post) {
        QMutexLocker locker(&m_mutex);
        if (m_readIndex == m_writeIndex) {
            return false; // 缓冲区为空
        }
        post = m_buffer[m_readIndex];
        m_readIndex = (m_readIndex + 1) % m_size;
        return true;
    }

    bool isEmpty() const {
        return m_readIndex == m_writeIndex;
    }

private:
    QList<Post> m_buffer;
    int m_size;
    int m_readIndex;
    int m_writeIndex;
    mutable QMutex m_mutex;
    QWaitCondition m_condition;
};
