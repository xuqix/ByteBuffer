//
//  ByteBuffer.h
//
//  2015-1-22.
//
//

#ifndef __ByteBuffer__
#define __ByteBuffer__


// If defined, utility functions within the class are enabled
#define BB_UTILITY

#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <vector>
#include <list>
#include <map>

//字节流缓存,用于打包、序列化各种数据类型
class ByteBuffer
{
public:
    static const uint32_t DEFAULT_SIZE = 0x1000;
    
    ByteBuffer(uint32_t size = DEFAULT_SIZE);
    
    ByteBuffer(uint8_t* data, uint32_t size);
    
    ~ByteBuffer();
    
    template <typename T>
    T get()
    {
        return read<T>();
    }

    template <typename T>
    void put(T data)
    {
        append(data);
    }
    
    template <typename T>
    T read()
    {
        T data = read<T>(_rpos);
        _rpos += sizeof(T);
        return data;
    }
    
    template <typename T>
    T read(uint32_t index) const
    {
        T data;
        if(index + sizeof(T) <= _buf.size())
        {
            memcpy(&data, (uint8_t*)&_buf[index], sizeof(T));
            return data;
        }
        return 0;
    }
    
    template <typename T>
    void append(T data)
    {
        uint32_t s = sizeof(data);
        
        if (size() < (_wpos + s))
            _buf.resize(_wpos + s);
        memcpy(&_buf[_wpos], (uint8_t*)&data, s);
        _wpos += s;
    }
    
    template <typename T>
    void insert(T data, uint32_t index)
    {
        if((index + sizeof(data)) > size())
            return;
        memcpy(&_buf[index], (uint8_t*)&data, sizeof(data));
        _wpos = index+sizeof(data);
    }
    
    void append(const std::string &str);
    
    void append(const char *data, uint32_t size);
    
    void append(const uint8_t *data, uint32_t size);
    
    void append(const ByteBuffer &buffer);
    
    //将多个数据打包
    template <typename... Args>
    ByteBuffer& package(Args &...args)
    {
        return _package(args...);
    }
    
    //解包数据包指定类型
    template <typename... Args>
    ByteBuffer& unpacked(Args &...args)
    {
        return _unpacked(args...);
    }
    
private:
    template <typename V, typename... Args>
    ByteBuffer& _package(V &v, Args &...args);
    template <typename V, typename... Args>
    ByteBuffer& _unpacked(V &v, Args &...args);
    //变参模板终止操作
    template <typename V>
    ByteBuffer& _package(V &v);
    template <typename V>
    ByteBuffer& _unpacked(V &v);
    
public:
    
    uint32_t bytesRemaining();
    
    void clear();
    
    bool equals(const ByteBuffer &other);
    
    void resize(uint32_t newSize);
    
    uint32_t size() const;
    
    void setReadPos(uint32_t r)
    {
        _rpos = r;
    }
    
    uint32_t getReadPos()
    {
        return _rpos;
    }
    
    void setWritePos(uint32_t w)
    {
        _wpos = w;
    }
    
    uint32_t getWritePos()
    {
        return _wpos;
    }
    
    //流操作写数据
    ByteBuffer & operator<<(bool value);
    ByteBuffer & operator<<(uint8_t value);
    ByteBuffer & operator<<(uint16_t value);
    ByteBuffer & operator<<(uint32_t value);
    ByteBuffer & operator<<(uint64_t value);
    ByteBuffer & operator<<(int8_t value);
    ByteBuffer & operator<<(int16_t value);
    ByteBuffer & operator<<(int32_t value);
    ByteBuffer & operator<<(int64_t value);
    ByteBuffer & operator<<(float value);
    ByteBuffer & operator<<(double value);
    ByteBuffer & operator<<(const std::string &value);
    ByteBuffer & operator<<(const char* str);
    //流操作读数据
    ByteBuffer & operator>>(bool  &value);
    ByteBuffer & operator>>(uint8_t  &value);
    ByteBuffer & operator>>(uint16_t &value);
    ByteBuffer & operator>>(uint32_t &value);
    ByteBuffer & operator>>(uint64_t &value);
    ByteBuffer & operator>>(int8_t &value);
    ByteBuffer & operator>>(int16_t &value);
    ByteBuffer & operator>>(int32_t &value);
    ByteBuffer & operator>>(int64_t &value);
    ByteBuffer & operator>>(float &value);
    ByteBuffer & operator>>(double &value);
    ByteBuffer & operator>>(std::string &value);
    ByteBuffer & operator>>(char &value);
    
    //下标操作读数据
    uint8_t operator[](uint32_t pos);
    
    std::vector<uint8_t> getBuffer();
    
private:
    uint32_t _rpos = 0;
    uint32_t _wpos = 0;
    std::vector<uint8_t> _buf;
    
#ifdef ARM_PLATFORM
    //避免arm平台内存地址对齐异常
    void* memcpy(void *dst, const void *src, size_t len) const
    {
        char *_dst = (char*)dst;
        const char *_src = (const char*)src;
        while(len--) *_dst++ = *_src++;
        return dst;
    }
#endif
    
#ifdef BB_UTILITY
    std::string name = "";
#endif
    
public:
#ifdef BB_UTILITY
    void setName(std::string n);
    std::string getName();
    void printInfo();
    void printAH();
    void printAscii();
    void printHex();
    void printPosition();
#endif
};


template<typename T>
inline ByteBuffer& operator<<(ByteBuffer & b, std::vector<T> v)
{
    b << (uint32_t) v.size();
    for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); i++)
    {
        b << *i;
    }
    return b;
}
template<typename T>
inline ByteBuffer & operator>>(ByteBuffer & b, std::vector<T> &v)
{
    uint32_t vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}
template<typename T>
inline ByteBuffer & operator<<(ByteBuffer & b, std::list<T> v)
{
    b << (uint32_t) v.size();
    for (typename std::list<T>::iterator i = v.begin(); i != v.end(); i++)
    {
        b << *i;
    }
    return b;
}
template<typename T>
inline ByteBuffer & operator>>(ByteBuffer & b, std::list<T> &v)
{
    uint32_t vsize;
    b >> vsize;
    v.clear();
    while (vsize--)
    {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}
template<typename K, typename V>
inline ByteBuffer & operator<<(ByteBuffer & b, std::map<K, V> &m)
{
    b << (uint32_t) m.size();
    for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); i++)
    {
        b << i->first << i->second;
    }
    return b;
}
template<typename K, typename V>
inline ByteBuffer & operator>>(ByteBuffer & b, std::map<K, V> &m)
{
    uint32_t msize;
    b >> msize;
    m.clear();
    while (msize--)
    {
        K k;
        V v;
        b >> k >> v;
        m.insert(make_pair(k, v));
    }
    return b;
}

template <typename V, typename... Args>
ByteBuffer& ByteBuffer::_package(V &v, Args &...args)
{
    *this << v;
    return package(args...);
}

template <typename V, typename... Args>
ByteBuffer& ByteBuffer::_unpacked(V &v, Args &...args)
{
    *this >> v;
    return unpacked(args...);
}

template <typename V>
ByteBuffer& ByteBuffer::_package(V &v)
{
    *this << v;
    return *this;
}

template <typename V>
ByteBuffer& ByteBuffer::_unpacked(V &v)
{
    *this >> v;
    return *this;
}

#endif /* defined(__ByteBuffer__) */
