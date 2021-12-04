#ifndef SINGLETON_H_
#define SINGLETON_H_

/* 单例的模板加宏实现方法
 * 参考资料:https://qtdebug.com/qtbook-singleton-5-template-macro/
 */
#include <QMutex>
#include <QScopedPointer>

template <typename T>
class Singleton {
public:
    static T& getInstance();                                                                            /* 获取单例唯一实例对象 */

    Singleton(const Singleton &other) = delete;
    Singleton<T>& operator=(const Singleton &other) = delete;
private:
    static QMutex mutex;
    static QScopedPointer<T> instance;                                                                  /* 智能指针 */
};

template <typename T> QMutex Singleton<T>::mutex;
template <typename T> QScopedPointer<T> Singleton<T>::instance;

template <typename T>
T& Singleton<T>::getInstance()
{
    if (instance.isNull())
    {
        mutex.lock();
        if (instance.isNull())                                                                          /* 两次判断对象是否为空,防止多线程时多次new */
        {
            instance.reset(new T());                                                                    /* 此指针会在全局变量作用域结束时自动 delete (main 函数返回后) */
        }
        mutex.unlock();
    }
    return *instance.data();
}

#define SINGLETON(Class)                           \
private:                                           \
    Class();                                       \
    ~Class();                                      \
    Class(const Class &other) = delete;            \
    Class& operator=(const Class &other) = delete; \
    friend class Singleton<Class>;                 \
    friend struct QScopedPointerDeleter<Class>;    \
public:                                            \
    static Class& instance()                       \
    {                                              \
        return Singleton<Class>::getInstance();    \
    }

#endif // SINGLETON_H_

