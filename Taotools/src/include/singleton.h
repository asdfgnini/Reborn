#ifndef SINGLETON_H
#define SINGLETON_H

#include <QMutex>  // 引入 QMutex，用于线程安全

/**
 * Singleton 模板类
 * 该模板类提供了一个全局唯一的实例，并保证线程安全。
 * 任何类型 T 都可以作为参数使用，从而创建一个相应类型的单例。
 */
template <typename T>
class Singleton
{
public:
    /**
     * 获取单例实例的静态方法
     * 该方法确保只有一个实例被创建，并在多线程环境下是线程安全的。
     * @return T* 返回类 T 的唯一实例。
     */
    static T* getInstance();

private:
    /**
     * 禁止拷贝构造函数和赋值操作符。
     * 确保单例模式不被复制或赋值，保持单例的唯一性。
     */
    Q_DISABLE_COPY(Singleton)
};

/**
 * Singleton 类模板的静态方法实现
 * 该方法返回单例实例，如果实例不存在，则创建一个新的实例。
 */
template <typename T>
T* Singleton<T>::getInstance()
{
    /**
     * 创建一个静态互斥锁对象，保证线程安全。
     * 只有一个线程可以在同一时刻访问这个方法。
     */
    static QMutex mutex;

    /**
     * 使用 QMutexLocker 在当前作用域内自动加锁，离开作用域时自动解锁。
     * 这是一个RAII模式的使用，避免了手动加锁和解锁的麻烦。
     */
    QMutexLocker locker(&mutex);

    /**
     * 静态成员变量 instance，用于保存单例实例。
     * 它只会在首次调用时被创建一次，后续的调用都会返回这个已创建的实例。
     */
    static T* instance = nullptr;

    /**
     * 如果实例为空，说明第一次调用，需要创建一个新的实例。
     */
    if (instance == nullptr)
    {
        instance = new T();
    }

    /**
     * 返回唯一的实例
     */
    return instance;
}

/**
 * 宏定义：用于创建一个类的单例
 * 它提供了一个静态方法 `getInstance()` 来获取类的唯一实例。
 * 宏的作用是简化单例模式的实现，避免重复代码。
 */
#define Q_SINGLETON_CREATE(Class)                                \
private:                                                         \
    friend class Singleton<Class>;                               \
                                                                 \
    public:                                                          \
    /**                                                         \
     * 获取单例实例的静态方法                                      \
     * 通过 Singleton 模板类的 `getInstance` 方法获取单例实例  \
     * @return Class* 返回该类的唯一实例                        \
     */                                                         \
    static Class* getInstance()                                  \
{                                                            \
        return Singleton<Class>::getInstance();                  \
}

/**
 * 宏定义：类头文件中的单例声明
 * 该宏定义在头文件中声明了单例实例和 `getInstance()` 方法。
 * `_instance` 是静态成员变量，用于存储类的唯一实例。
 */
#define Q_SINGLETON_CREATE_H(Class)                               \
private:                                                         \
    /** 静态成员变量，用于存储类的唯一实例 */                 \
    static Class* _instance;                                     \
                                                                 \
    public:                                                          \
    /** 获取单例实例的声明 */                                    \
    static Class* getInstance();

/**
 * 宏定义：类源文件中的单例实现
 * 该宏定义在源文件中实现了 `getInstance()` 方法，并确保只创建一个单例实例。
 * `_instance` 是该类的静态成员变量，用于持有单例实例。
 */
#define Q_SINGLETON_CREATE_CPP(Class)                             \
/** 初始化单例实例为空 */                                      \
    Class* Class::_instance = nullptr;                            \
                                                                 \
    /** 获取单例实例的实现 */                                     \
    Class* Class::getInstance()                                   \
{                                                             \
        static QMutex mutex;                                      \
        /** 使用 QMutexLocker 进行加锁 */                          \
        QMutexLocker locker(&mutex);                              \
        /** 如果实例为空，创建一个新的实例 */                      \
        if (_instance == nullptr)                                 \
    {                                                         \
            _instance = new Class();                              \
    }                                                         \
        /** 返回唯一的单例实例 */                                  \
        return _instance;                                         \
}

#endif // SINGLETON_H
