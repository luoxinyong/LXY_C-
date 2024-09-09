#include "./Singleton.hpp"
#include <iostream>
// using namespace std;
#if 0
    class Singleton
    {
    public:
        Singleton(const Singleton* obj) = delete; //把拷贝构造函数ban了，
        Singleton& operator=(const Singleton& obj) = delete;//把它拷贝重载也ban了
        static Singleton* getInstance(){//获取实例化对象的函数
            Singleton* obj_ = obj.load();
            if(obj_ == nullptr ) 
            {
                std::lock_guard<std::mutex> locker(m_tex);
                obj_ = obj.load();
                if(obj_ == nullptr ){
                    obj_ = new Singleton;
                    obj.store(obj_);
                }
            } 
            return obj_;
        }
    private:
        //不要忘记把类的构造函数给他干成私有的
        Singleton(){}
        // static Singleton* obj;
        static std::atomic<Singleton*> obj;
        static std::mutex m_tex;
    };
    // Singleton* Singleton::obj = nullptr;
    std::atomic<Singleton*> Singleton::obj{nullptr};
    std::mutex Singleton::m_tex;
#endif

class test_singleton
{
private:
    int m_value;
    std::string m_name;
public:
    test_singleton(int value, std::string name): m_value(value), m_name(name)
    {

    };
    test_singleton(){};
    ~test_singleton(){};
};




int main()
{
    test_singleton* instance1 = Singleton<test_singleton>::getInstance();
    test_singleton* instance2 = Singleton<test_singleton>::getInstance();

    if(instance1 == instance2)
    {
        std::cout<<"这俩是一个玩意"<<std::endl;
    }

return 0;
}