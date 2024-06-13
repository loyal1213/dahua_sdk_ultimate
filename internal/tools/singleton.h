#pragma once

template<typename T>
class Singleton {
public:
    static T *instance() {
        if (m_instance == nullptr)
            m_instance = new T();
        return m_instance;
    }

private:
    Singleton() {}

    Singleton(const Singleton<T> &);

    Singleton<T> &operator=(const Singleton<T> &);

    ~Singleton() {}

private:
    static T *m_instance;
};

template<typename T>
T *Singleton<T>::m_instance = nullptr;

