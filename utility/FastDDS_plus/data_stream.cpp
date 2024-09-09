#include "data_stream.h"  
using namespace std;


void data_stream::show()
{
    int size = m_buf.size();
    std::cout<<"data size = " <<size<<std::endl;
    int i = 0;
    while(i < size )
    {
        switch ((DataType)m_buf[i])
        {
        case DataType::CHAR:
            std::cout<<m_buf[++i]<<std::endl;
            ++i;
            break;
        case DataType::BOOL:
            if((int)m_buf[++i] == 0)
            {
                std::cout<<"false"<<std::endl;
            }else{
                std::cout<<"true"<<std::endl;
            }
            break;
        case DataType::INT32://打印int类型，找到这个int的地址&m_buf，然后把它转换成int32_t类型的指针，再把指针解引用
            std::cout<<*((int32_t*)(&m_buf[++i]))<<std::endl;
            i+=4;
            break;
        case DataType::INT64:
            std::cout<<*((int64_t*)(&m_buf[++i]))<<std::endl;
            i+=8;
            break;
        case DataType::UINT32:
            std::cout<<*((uint32_t*)(&m_buf[++i]))<<std::endl;
            i+=4;
            break;
        case DataType::UINT64:
            std::cout<<*((uint64_t*)(&m_buf[++i]))<<std::endl;
            i+=8;
            break;
        case DataType::DOUBLE:
            std::cout<<*((double*)(&m_buf[++i]))<<std::endl;
            i+=8;
            break;
        case DataType::FLOAT:
            std::cout<<*((float*)(&m_buf[++i]))<<std::endl;
            i+=4;
            break;
        case DataType::STRING://对于字符串来说，他存进去的第二位里面是这个字符串的长度len，而且用的是int32类型保存的len，只需要判断这个就OK了
               if((DataType)m_buf[++i] == DataType::INT32)
               {
                
                int len = *((int32_t *)(&m_buf[++i]));
                i += 4;
                // std::cout<<"string"<<std::endl;
                std::cout<<string(&m_buf[i],len)<<std::endl;
                i += len;
               }else
                {
                    throw std::logic_error("parse string error");
                }
            break;
        
        default:
            break;
        }





    }


}
void data_stream::reserve(int len)
{
    int size = m_buf.size();
    int cap = m_buf.capacity();
    if(size + len > cap)
    {
        while(size + len  > cap)
            {
                if (cap == 0)
                {
                    cap = 1;
                }
                else
                {
                    cap *= 2;
                }
            }
    }
    m_buf.reserve(cap);
}

void data_stream::write(const char* data, int len )
{
    reserve(len);
    int size = m_buf.size();
    m_buf.resize(size + len);
    std::memcpy(&m_buf[size],data,len);
}
void data_stream::write(char value)
{
    char type = DataType::CHAR;
    write(reinterpret_cast<char*>(&type),sizeof(char));
    write(reinterpret_cast<char*>(&value),sizeof(char));
}

void data_stream::write(bool value)
{
    char type = DataType::BOOL;
    // 这里要进行指针类型的转换，&type指向的是bool类型的指针，函数要的是一个指向char类型的指针
    write(reinterpret_cast<char*>(&type),sizeof(char));
    write(reinterpret_cast<char*>(&value),sizeof(char));
}
void data_stream::write(int32_t value)
{
    char type = DataType::INT32;
    // 这里要进行指针类型的转换，&type指向的是bool类型的指针，函数要的是一个指向char类型的指针
    write(reinterpret_cast<char*>(&type),sizeof(char));
    write(reinterpret_cast<char*>(&value),sizeof(int32_t));
}

void data_stream::write(int64_t value)
{
    char type = DataType::INT64;
    // 这里要进行指针类型的转换，&type指向的是bool类型的指针，函数要的是一个指向char类型的指针
    write(reinterpret_cast<char*>(&type),sizeof(char));
    write(reinterpret_cast<char*>(&value),sizeof(int64_t));
}

void data_stream::write(float value)
{
    char type = DataType::FLOAT;
    // 这里要进行指针类型的转换，&type指向的是bool类型的指针，函数要的是一个指向char类型的指针
    write(reinterpret_cast<char*>(&type),sizeof(char));
    write(reinterpret_cast<char*>(&value),sizeof(float));
}

void data_stream::write(double value)
{
    char type = DataType::DOUBLE;
    // 这里要进行指针类型的转换，&type指向的是bool类型的指针，函数要的是一个指向char类型的指针
    write(reinterpret_cast<char*>(&type),sizeof(char));
    write(reinterpret_cast<char*>(&value),sizeof(double));
}

void data_stream::write(uint32_t value)
{
    char type = DataType::UINT32;
    // 这里要进行指针类型的转换，&type指向的是bool类型的指针，函数要的是一个指向char类型的指针
    write(reinterpret_cast<char*>(&type),sizeof(char));
    write(reinterpret_cast<char*>(&value),sizeof(uint32_t));
}

void data_stream::write(uint64_t value)
{
    char type = DataType::UINT64;
    // 这里要进行指针类型的转换，&type指向的是bool类型的指针，函数要的是一个指向char类型的指针
    write(reinterpret_cast<char*>(&type),sizeof(char));
    write(reinterpret_cast<char*>(&value),sizeof(uint64_t));
}

void data_stream::write(const char* value)
{
    char type = DataType::STRING;
    write(reinterpret_cast<char*>(&type),sizeof(char));
    int len = strlen(value);
    write(len);
    write(value,len);
}

void data_stream::write(const  string& value)
{
    char type = DataType::STRING;
    write(reinterpret_cast<char*>(&type),sizeof(char));
    int len = value.size();
    write(len);
    write(value.data(),len);
}

void data_stream::write(const Serializable & value)
{
    value.serialize(*this);
}

template <typename T>
void data_stream::write(const std::vector<T> & value)
{
    char type = DataType::VECTOR;
    write((char *)&type, sizeof(char));
    int len = value.size();
    write(len);
    for (int i = 0; i < len; i++)
    {
        write(value[i]);
    }
}

template <typename T>
void data_stream::write(const std::list<T> & value)
{
    char type = DataType::LIST;
    write((char *)&type, sizeof(char));
    int len = value.size();
    write(len);
    for (auto it = value.begin(); it != value.end(); it++)
    {
        write((*it));
    }
}

template <typename K, typename V>
void data_stream::write(const std::map<K, V> & value)
{
    char type = DataType::MAP;
    write((char *)&type, sizeof(char));
    int len = value.size();
    write(len);
    for (auto it = value.begin(); it != value.end(); it++)
    {
        write(it->first);
        write(it->second);
    }
}

template <typename T>
void data_stream::write(const std::set<T> & value)
{
    char type = DataType::SET;
    write((char *)&type, sizeof(char));
    int len = value.size();
    write(len);
    for (auto it = value.begin(); it != value.end(); it++)
    {
        write(*it);
    }
}
data_stream & data_stream::operator << (bool value)
{
    write(value);
    return *this;
}
data_stream & data_stream::operator << (char value)
{
    write(value);
    return *this;

}
data_stream & data_stream::operator << (int32_t value)
{
    write(value);
    return *this;

}
data_stream & data_stream::operator << (int64_t value)
{
    write(value);
    return *this;

}
data_stream & data_stream::operator << (uint32_t value)
{
    write(value);
    return *this;

}
data_stream & data_stream::operator << (uint64_t value)
{
    write(value);
    return *this;

}
data_stream & data_stream::operator << (float value)
{
    write(value);
    return *this;

}
data_stream & data_stream::operator << (double value)
{
    write(value);
    return *this;

}
data_stream & data_stream::operator << (const char* value)
{
    write(value);
    return *this;

}
data_stream & data_stream::operator << (const string value)
{
    write(value);
    return *this;

}
data_stream & data_stream::operator << (const Serializable &value)
{
    write(value);
    return *this;
}

bool data_stream::read(bool &value)
{
    if(m_buf[m_pos] != DataType::BOOL)
    {
        return false;
    }
    m_pos++;
    value =  m_buf[m_pos];
    m_pos++;
    return true;
}
bool data_stream::read(char &value)
{
     if(m_buf[m_pos] != DataType::CHAR)
    {
        return false;
    }
    m_pos++;
    value =  m_buf[m_pos];
    m_pos++;
    return true;
}
bool data_stream::read(int32_t &value)
{
     if(m_buf[m_pos] != DataType::INT32)
    {
        return false;
    }
    m_pos++;
    value = *((int32_t *)(&m_buf[m_pos]));
    m_pos+=4;
    return true;
}
bool data_stream::read(int64_t &value)
{
     if(m_buf[m_pos] != DataType::INT64)
    {
        return false;
    }
    m_pos++;
    value = *((int64_t *)(&m_buf[m_pos]));
    m_pos+=8;
    return true;
}
bool data_stream::read(uint32_t &value)
{
     if(m_buf[m_pos] != DataType::UINT32)
    {
        return false;
    }
    m_pos++;
    value = *((uint32_t *)(&m_buf[m_pos]));
    m_pos+=4;
    return true;
}
bool data_stream::read(uint64_t &value)
{
     if(m_buf[m_pos] != DataType::UINT64)
    {
        return false;
    }
    m_pos++;
    value = *((uint64_t *)(&m_buf[m_pos]));
    m_pos+=8;
    return true;
}
bool data_stream::read(float &value)
{
     if(m_buf[m_pos] != DataType::FLOAT)
    {
        return false;
    }
    m_pos++;
    value = *((float *)(&m_buf[m_pos]));
    m_pos+=4;
    return true;
}
bool data_stream::read(double &value)
{
     if(m_buf[m_pos] != DataType::DOUBLE)
    {
        return false;
    }
    m_pos++;
    value = *((double *)(&m_buf[m_pos]));
    m_pos+=8;
    return true;
}
bool data_stream::read(Serializable &value)
{
     return value.unserialize(*this);
}

// bool data_stream::read(const char*  &value)
// {
//      if(m_buf[m_pos] != DataType::STRING)
//     {
//         return false;
//     }
//     m_pos++;
//     int len ;
//     read(len);
//     if ( len < 0 )
//     {
//         return false;
//     }
//     value.assign((char* )&(m_buf[m_pos]),len);
//     m_pos += len;
//     return true;
// }
bool data_stream::read( string &value)
{
     if(m_buf[m_pos] != DataType::STRING)
    {
        return false;
    }
    m_pos++;
    int len ;
    read(len);
    if (len < 0)
    {
        return false;
    }
    value.assign((char* )&(m_buf[m_pos]),len);
    m_pos += len;
    return true;
}
template <typename T>
bool data_stream::read(std::vector<T> & value)
{
    value.clear();
    if (m_buf[m_pos] != DataType::VECTOR)
    {
        return false;
    }
    ++m_pos;
    int len;
    read(len);
    for (int i = 0; i < len; i++)
    {
        T v;
        read(v);
        value.push_back(v);
    }
    return true;
}

template <typename T>
bool data_stream::read(std::list<T> & value)
{
    value.clear();
    if (m_buf[m_pos] != DataType::LIST)
    {
        return false;
    }
    ++m_pos;
    int len;
    read(len);
    for (int i = 0; i < len; i++)
    {
        T v;
        read(v);
        value.push_back(v);
    }
    return true;
}

template <typename K, typename V>
bool data_stream::read(std::map<K, V> & value)
{
    value.clear();
    if (m_buf[m_pos] != DataType::MAP)
    {
        return false;
    }
    ++m_pos;
    int len;
    read(len);
    for (int i = 0; i < len; i++)
    {
        K k;
        read(k);

        V v;
        read(v);
        value[k] = v;
    }
    return true;
}

template <typename T>
bool data_stream::read(std::set<T> & value)
{
    value.clear();
    if (m_buf[m_pos] != DataType::SET)
    {
        return false;
    }
    ++m_pos;
    int len;
    read(len);
    for (int i = 0; i < len; i++)
    {
        T v;
        read(v);
        value.insert(v);
    }
    return true;
}

bool data_stream::read( char* data , int len){
    std::memcpy(data, (char *)&m_buf[m_pos], len);
    m_pos += len;
    return true;
}
data_stream & data_stream::operator >> (bool &value)
{
    read(value);
    return *this;
}
data_stream & data_stream::operator >> (char &value)
{
    read(value);
    return *this;

}
data_stream & data_stream::operator >> (int32_t &value)
{
    read(value);
    return *this;

}
data_stream & data_stream::operator >> (int64_t &value)
{
    read(value);
    return *this;

}
data_stream & data_stream::operator >> (uint32_t &value)
{
    read(value);
    return *this;

}
data_stream & data_stream::operator >> (uint64_t &value)
{
    read(value);
    return *this;

}
data_stream & data_stream::operator >> (float &value)
{
    read(value);
    return *this;

}
data_stream & data_stream::operator >> (double &value)
{
    read(value);
    return *this;

}

data_stream & data_stream::operator >> ( string &value)
{
    read(value);
    return *this;

}
template <typename T>
data_stream & data_stream::operator << (const std::vector<T> & value)
{
    write(value);
    return *this;
}
template <typename T>
data_stream & data_stream::operator << (const std::list<T> & value)
{
    write(value);
    return *this;
}

template <typename K, typename V>
data_stream & data_stream::operator << (const std::map<K, V> & value)
{
    write(value);
    return *this;
}

template <typename T>
data_stream & data_stream::operator << (const std::set<T> & value)
{
    write(value);
    return *this;
}

template <typename T>
data_stream & data_stream::operator >> (std::vector<T> & value)
{
    read(value);
    return *this;
}

template <typename T>
data_stream & data_stream::operator >> (std::list<T> & value)
{
    read(value);
    return *this;
}

template <typename K, typename V>
data_stream & data_stream::operator >> (std::map<K, V> & value)
{
    read(value);
    return *this;
}

template <typename T> 
data_stream & data_stream::operator >> (std::set<T> & value)
{
    read(value);
    return *this;
}
data_stream & data_stream::operator >> (Serializable & value)
{
    read(value);
    return *this;
}
data_stream::~data_stream()
{
}

