#ifndef STDAFX_H
#define STDAFX_H

#include <QtCore/qglobal.h>

// 判断是否在构建库时，设置导出或导入宏
#ifdef TAOTOOLS_LIBRARY
/* 导出宏，用于在生成共享库时导出符号 */
#define TAO_EXPORT Q_DECL_EXPORT
#else
/* 导入宏，用于在引用共享库时导入符号 */
#define TAO_EXPORT Q_DECL_IMPORT
#endif

/* 用于快速创建属性，并生成相关的 getter、setter 和 signal（信号） */
#define Q_PROPERTY_CREATE(TYPE, M)                                \
/* 定义一个属性 p##M，类型为 TYPE，使用 _p##M 存储值，属性变化时触发 p##M##Changed 信号 */ \
    Q_PROPERTY(TYPE p##M MEMBER _p##M NOTIFY p##M##Changed)       \
    public:                                                           \
    /* 属性变化时触发的信号 */                                      \
    Q_SIGNAL void p##M##Changed();                                 \
    /* 设置属性值，并触发属性变化的信号 */                           \
    void set##M(TYPE M)                                           \
{                                                             \
        _p##M = M;                                                \
        Q_EMIT p##M##Changed();                                    \
}                                                             \
    /* 获取属性值 */                                               \
    TYPE get##M() const                                           \
{                                                             \
        return _p##M;                                             \
}                                                             \
                                                                  \
    private:                                                          \
    /* 存储属性值的私有成员变量 */                                   \
    TYPE _p##M;

/* 用于快速创建带 getter 和 setter 的属性（没有内部变量） */
#define Q_PROPERTY_CREATE_Q_H(TYPE, M)                                \
/* 声明一个名为 p##M 的属性，类型为 TYPE，提供 getter 和 setter 方法，没有内部变量存储值 */ \
    Q_PROPERTY(TYPE p##M READ get##M WRITE set##M NOTIFY p##M##Changed) \
    public:                                                               \
    /* 属性变化时触发的信号 */                                          \
    Q_SIGNAL void p##M##Changed();                                      \
    /* 设置属性值 */                                                    \
    void set##M(TYPE M);                                                \
    /* 获取属性值 */                                                    \
    TYPE get##M() const;

/* 用于快速创建带 getter 和 setter 的指针类型成员变量（没有信号） */
#define Q_PRIVATE_CREATE_Q_H(TYPE, M) \
public:                               \
    /* 设置指针类型成员变量 M 的值 */  \
    void set##M(TYPE M);              \
    /* 获取指针类型成员变量 M 的值 */  \
    TYPE get##M() const;

/* 用于实现 `set` 和 `get` 方法的 C++ 代码，实现 PImpl（指针到实现）模式 */
#define Q_PROPERTY_CREATE_Q_CPP(CLASS, TYPE, M)                       \
/* 在 C++ 类中实现 set##M 和 get##M 方法，    \
       使用 PImpl 模式，直接操作私有实现对象，并在设置值时触发信号 */  \
    void CLASS::set##M(TYPE M)                                          \
{                                                                   \
        Q_D(CLASS);                                                     /* 获取类的私有实现对象 */ \
        d->_p##M = M;                                                   /* 设置私有数据成员的值 */ \
        Q_EMIT p##M##Changed();                                         /* 设置值后触发信号 */ \
}                                                                   \
    /* 获取私有成员 M 的值 */                                             \
    TYPE CLASS::get##M() const                                          \
{                                                                   \
        return d_ptr->_p##M;                                            /* 获取私有数据成员的值 */ \
}

/* 用于实现 `set` 和 `get` 方法的 C++ 代码，不触发 `Changed` 信号 */
#define Q_PRIVATE_CREATE_Q_CPP(CLASS, TYPE, M)                       \
/* 在 C++ 类中实现 set##M 和 get##M 方法，采用 PImpl 模式，但不触发 `Changed` 信号 */ \
    void CLASS::set##M(TYPE M)                                          \
{                                                                   \
        Q_D(CLASS);                                                     /* 获取类的私有实现对象 */ \
        d->_p##M = M;                                                   /* 设置私有数据成员的值 */ \
}                                                                   \
    /* 获取私有成员 M 的值 */                                             \
    TYPE CLASS::get##M() const                                          \
{                                                                   \
        return d_ptr->_p##M;                                            /* 获取私有数据成员的值 */ \
}

/* 定义私有成员变量 `_p##M` */
#define Q_PROPERTY_CREATE_D(TYPE, M) \
private:                             \
    /* 存储属性值的私有成员变量 */  \
    TYPE _p##M;

/* 用于 PImpl 模式，声明私有成员变量 `_p##M` */
#define Q_PRIVATE_CREATE_D(TYPE, M) \
private:                            \
    /* 存储属性值的私有成员变量 */  \
    TYPE _p##M;

/* 用于快速创建属性的 getter 和 setter 方法 */
#define Q_PRIVATE_CREATE(TYPE, M) \
public:                           \
    /* 设置属性值 */               \
    void set##M(TYPE M)           \
{                             \
        _p##M = M;                \
}                             \
    /* 获取属性值 */               \
    TYPE get##M() const           \
{                             \
        return _p##M;             \
}                             \
                                  \
    private:                          \
    /* 存储属性值的私有成员变量 */  \
    TYPE _p##M;

/* 定义类的 PImpl 模式接口，包含私有数据指针和析构函数 */
#define Q_Q_CREATE(CLASS)                                                \
protected:                                                               \
    /* 构造函数，传入私有数据对象引用 */                                   \
    explicit CLASS(CLASS##Private& dd, CLASS* parent = nullptr);          \
    /* 使用智能指针管理私有数据对象 */                                     \
    QScopedPointer<CLASS##Private> d_ptr;                                \
                                                                       \
    private:                                                                 \
    /* 禁止类的复制操作 */                                               \
    Q_DISABLE_COPY(CLASS)                                                \
    /* 声明私有数据类 */                                                   \
    Q_DECLARE_PRIVATE(CLASS);

/* 定义类的公共接口指针和 PImpl 模式 */
#define Q_D_CREATE(CLASS) \
protected:                \
    /* 定义公共接口指针 */  \
    CLASS* q_ptr;         \
                          \
    private:                  \
    /* 声明公共接口类 */    \
    Q_DECLARE_PUBLIC(CLASS);

#endif // STDAFX_H
