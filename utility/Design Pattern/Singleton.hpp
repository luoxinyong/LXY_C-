#include <atomic>
#include <mutex>

template <typename T>
class Singleton
{
public:
    Singleton(const Singleton* obj) = delete;
    Singleton* operator= (Singleton* obj) = delete;
    static T* getInstance(){
        T* obj_ = instance.load();
        if( obj_ == nullptr)
        {
            std::lock_guard<std::mutex> locker(m_mutex);
            obj_ = instance.load();
            if(obj_ == nullptr)
            {
                obj_ = new T();
                instance.store(obj_);
            }
        }
        return obj_;
    }
    ~Singleton();
private:
    Singleton() = default;
    static std::mutex m_mutex;
    static std::atomic<T*> instance;
};

template <typename T>
std::atomic<T*> Singleton<T>::instance{nullptr};

template <typename T>
std::mutex Singleton<T>::m_mutex;