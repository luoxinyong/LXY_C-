#pragma once
// 写入的形式是（类型名）+（数据）+（类型名）+（数据）。。。。
#include <vector>
#include <iostream>
#include <cstring>
#include <list>
#include <map>
#include <set>
#include "serializable.h"
using namespace std;

class data_stream
{
private:
    // 序列化后的数据以二进制的形式保存，这里把二进制后的数据按照字符放到缓冲区里面
    vector<char> m_buf;
    //读取数据的时候需要一个记录位置的符号
    int m_pos;

public:
 
    data_stream():m_pos(0){}
    ~data_stream();
    // 初始定义基础数据类型的排序,相比较YAZI要多无符号int32和64，加上一个ENUM

    enum DataType
    {
    BOOL = 0,
    CHAR,
    INT32,
    INT64,
    UINT32,
    UINT64,
    ENUM,
    FLOAT,
    DOUBLE,
    STRING,
    VECTOR,
    LIST,
    MAP,
    SET,
    CUSTOM
    };
    void show();
    //扩容函数
    void reserve(int cap);
    // 序列化操作的基本函数write，将数据写入到序列化的m_buf中，会根据数据类型的不一样有多个重载，但这个是最最基本的
    //data表示的写入数据的地址，len表示这个data的长度,读取是要对这个value进行修改的，所以value不能式const修饰的
    void write(const char* data , int len);
    void write(bool value);
    void write(char value);
    void write(int32_t value);
    void write(int64_t value);
    void write(double value);
    void write(float value);
    void write(uint32_t value);
    void write(uint64_t value);
    //对C/C++同时进行支持的话,C的string是char*类型的，C++，string是一个独立类型
    void write( const  char* value);
    void write( const string& value);//引用传递，传入引用减小内存使用
    // void write(string value);//这个属于是值传递，传入的是一个数据的副本,进行了一次拷贝
    void write(const Serializable& value);

        template <typename T>
    void write(const std::vector<T> & value);

    template <typename T>
    void write(const std::list<T> & value);

    template <typename K, typename V>
    void write(const std::map<K, V> & value);

    template <typename T>
    void write(const std::set<T> & value);
    template <typename T, typename ...Args>
    void write_args(const T & head, const Args&... args)
    {
        write(head);
        write_args(args...);
    }
    void write_args(){}//空函数，用来处理参数为空的情况
    //
    //read函数实现从区中拿数据出来，要先判断数据类型是否正确，然后取出后面的数据进行赋值
    //用bool来确认函数是否赋值成功
    bool read( char* data , int len);
    bool read(bool &value);
    bool read(char &value);
    bool read(int32_t &value);
    bool read(int64_t &value);
    bool read(uint32_t &value);
    bool read(uint64_t &value);
    bool read(float &value);
    bool read(double &value);
    // bool read( char*  &value);
    bool read(Serializable &value);
    bool read( string &value);
    template <typename T>
    bool read(std::vector<T> & value);

    template <typename T>
    bool read(std::list<T> & value);

    template <typename K, typename V>
    bool read(std::map<K, V> & value);

    template <typename T>
    bool read(std::set<T> & value);

    template <typename T, typename ...Args>
    bool read_args( T & value,  Args&... args)
    {
        read(value);
        return read_args(args...);
    }
    bool read_args(){return true;}//空函数，用来处理参数为空的情况

    //除了使用write这种显式函数调用的方法实现，还完成一种<<流加载类型的输入写入，为此要重载运算符<<
    data_stream & operator <<(bool value);
    data_stream & operator <<(int32_t value);
    data_stream & operator <<(char value);
    data_stream & operator <<(int64_t value);
    data_stream & operator <<(uint32_t value);
    data_stream & operator <<(uint64_t value);
    data_stream & operator <<(float value);
    data_stream & operator <<(double value);
    data_stream & operator <<(const string value);
    data_stream & operator <<(const char* value);
    data_stream & operator <<(const Serializable &value);
    template <typename T>
    data_stream & operator << (const std::vector<T> & value);

    template <typename T>
    data_stream & operator << (const std::list<T> & value);

    template <typename K, typename V>
    data_stream & operator << (const std::map<K, V> & value);

    template <typename T>
    data_stream & operator << (const std::set<T> & value);
    
    data_stream & operator >> (bool &value);
    data_stream & operator >>(int32_t &value);
    data_stream & operator >>(char &value);
    data_stream & operator >>(int64_t& value);
    data_stream & operator >>(uint32_t &value);
    data_stream & operator >>(uint64_t &value);
    data_stream & operator >>(float &value);
    data_stream & operator >>(double &value);
    data_stream & operator >>( string &value);
        template <typename T>
    data_stream & operator >> (std::vector<T> & value);

    template <typename T>
    data_stream & operator >> (std::list<T> & value);

    template <typename K, typename V>
    data_stream & operator >> (std::map<K, V> & value);

    template <typename T>
    data_stream & operator >> (std::set<T> & value);
    data_stream & operator >> (Serializable & value);

};

