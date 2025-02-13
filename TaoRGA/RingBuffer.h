#pragma once
#include <QMutexLocker>
#include <QMutex>
#include <QList>
#include <QString>

struct RGAparseData
{
    QString data;
    QString name;
    QString origin;

};




class RingBuffer {
public:
    RingBuffer(int size) : m_size(size), m_readIndex(0), m_writeIndex(0) {
        m_buffer.resize(size);

    bool write(const RGAparseData &data) {
        QMutexLocker locker(&m_mutex);
        if ((m_writeIndex + 1) % m_size == m_readIndex) {
            return false; // 缓冲区已满
        }
        m_buffer[m_writeIndex] = data;
        m_writeIndex = (m_writeIndex + 1) % m_size;
        return true;
    }

    bool read(RGAparseData &data) {
        QMutexLocker locker(&m_mutex);
        if (m_readIndex == m_writeIndex) {
            return false; // 缓冲区为空
        }
        data = m_buffer[m_readIndex];
        m_readIndex = (m_readIndex + 1) % m_size;
        return true;
    }

private:
    QList<RGAparseData> m_buffer;
    int m_size;
    int m_readIndex;
    int m_writeIndex;
    mutable QMutex m_mutex;
};
