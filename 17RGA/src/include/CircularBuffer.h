// CircularBuffer.h
#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <QReadWriteLock>
#include <QVector>
#include <QDebug>

template <typename T>
class CircularBuffer
{
public:
    explicit CircularBuffer(int capacity)
        : m_capacity(capacity), m_size(0), m_readIndex(0), m_writeIndex(0)
    {
        m_buffer.resize(capacity);
    }

    bool write(const T& data)
    {
        m_writeLock.lockForWrite();
        bool success = false;
        if (m_size < m_capacity) {
            m_buffer[m_writeIndex] = data;
            m_writeIndex = (m_writeIndex + 1) % m_capacity;
            m_size++;
            success = true;
        } else {
            qDebug() << "Buffer is full!";
        }
        m_writeLock.unlock();
        return success;
    }

    bool read(T& data)
    {
        m_readLock.lockForRead();
        bool success = false;
        if (m_size > 0) {
            data = m_buffer[m_readIndex];
            m_readIndex = (m_readIndex + 1) % m_capacity;
            m_size--;
            success = true;
        } else {
            // qDebug() << "Buffer is empty!";
        }
        m_readLock.unlock();
        return success;
    }

    bool isEmpty() const { return m_size == 0; }
    bool isFull() const { return m_size == m_capacity; }
    int size() const { return m_size; }

private:
    QVector<T> m_buffer;
    int m_capacity;
    int m_size;
    int m_readIndex;
    int m_writeIndex;
    mutable QReadWriteLock m_readLock;  // Read-write lock for concurrent access
    mutable QReadWriteLock m_writeLock;
};

#endif // CIRCULARBUFFER_H
