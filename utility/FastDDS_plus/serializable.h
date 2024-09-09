#pragma once

// 实现一个自定义类的序列化，类似于SMW，只需要对于每个自定义类添加一个序列化和反序列化的函数实现
class data_stream;
class Serializable{
public:
// 其中的参数，就是需要序列化的数据
virtual void serialize(data_stream data) const = 0;
virtual bool unserialize(data_stream &data) = 0;
};

// 同时为了避免
#define SERIALIZE(...)                              \
                                                    \
    void serialize(data_stream & stream) const       \
    {                                               \
        char type = data_stream::CUSTOM;             \
        stream.write((char *)&type, sizeof(char));  \
        stream.write_args(__VA_ARGS__);             \
    }                                               \
                                                    \
    bool unserialize(data_stream & stream)           \
    {                                               \
        char type;                                  \
        stream.read(&type, sizeof(char));           \
        if (type != data_stream::CUSTOM)             \
        {                                           \
            return false;                           \
        }                                           \
        stream.read_args(__VA_ARGS__);              \
        return true;                                \
    }
