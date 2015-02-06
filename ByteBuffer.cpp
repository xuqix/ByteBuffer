//
//  ByteBuffer.cpp
//
//  2015-1-22.
//
//

#include "ByteBuffer.h"

ByteBuffer::ByteBuffer(uint32_t size)
{
    _buf.reserve(size);
    clear();
}


ByteBuffer::ByteBuffer(uint8_t* data, uint32_t size)
{
    if(data == NULL)
    {
        _buf.reserve(size);
        clear();
    }
    else
    {
        _buf.reserve(size);
        clear();
        append(data, size);
    }
}

ByteBuffer::~ByteBuffer()
{
    
}

void ByteBuffer::append(const std::string &str)
{
    append((uint32_t)str.size());
    for(auto &ch:str)
        append(ch);
}

void ByteBuffer::append(const char *data, uint32_t size)
{
    for(uint32_t i=0; i < size; i++)
        append(data[i]);
}

void ByteBuffer::append(const uint8_t *data, uint32_t size)
{
    for(uint32_t i=0; i < size; i++)
        append(data[i]);
}

void ByteBuffer::append(const ByteBuffer &buffer)
{
    clear();
    for(uint32_t i=0; i < buffer.size(); i++)
        append(buffer.read<uint8_t>(i));
}



uint32_t ByteBuffer::bytesRemaining()
{
    return size()-_rpos;
}


void ByteBuffer::clear()
{
    _rpos = 0;
    _wpos = 0;
    _buf.clear();
}

std::vector<uint8_t> ByteBuffer::getBuffer()
{
    return std::move(_buf);
}

bool ByteBuffer::equals(const ByteBuffer &other)
{
    if(size() != other.size())
        return false;

    uint32_t len = size();
    for(uint32_t i = 0; i < len; i++)
        if(read<uint8_t>(i) != other.read<uint8_t>(i))
            return false;
    return true;
}

void ByteBuffer::resize(uint32_t newSize)
{
    _buf.resize(newSize);
    _rpos = 0;
    _wpos = 0;
}

uint32_t ByteBuffer::size() const
{
    return _buf.size();
}

#ifdef BB_UTILITY
void ByteBuffer::setName(std::string n) {
    name = n;
}

std::string ByteBuffer::getName() {
    return name;
}

void ByteBuffer::printInfo() {
    uint32_t length = _buf.size();
    std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << ". Info Print" << std::endl;
}

void ByteBuffer::printAH() {
    uint32_t length = _buf.size();
    std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << ". ASCII & Hex Print" << std::endl;
    for(uint32_t i = 0; i < length; i++) {
        printf("0x%02x ", _buf[i]);
    }
    printf("\n");
    for(uint32_t i = 0; i < length; i++) {
        printf("%c ", _buf[i]);
    }
    printf("\n");
}

void ByteBuffer::printAscii() {
    uint32_t length = _buf.size();
    std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << ". ASCII Print" << std::endl;
    for(uint32_t i = 0; i < length; i++) {
        printf("%c ", _buf[i]);
    }
    printf("\n");
}

void ByteBuffer::printHex() {
    uint32_t length = _buf.size();
    std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << ". Hex Print" << std::endl;
    for(uint32_t i = 0; i < length; i++) {
        printf("0x%02x ", _buf[i]);
    }
    printf("\n");
}

void ByteBuffer::printPosition() {
    uint32_t length = _buf.size();
    std::cout << "ByteBuffer " << name.c_str() << " Length: " << length << " Read Pos: " << _rpos << ". Write Pos: " << _wpos << std::endl;
}

#endif

ByteBuffer & ByteBuffer::operator<<(bool value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(uint8_t value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(uint16_t value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(uint32_t value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(uint64_t value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(int8_t value)
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(int16_t value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(int32_t value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(int64_t value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(float value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(double value) 
{
    append(value);
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(const std::string & value) 
{
    append((uint32_t)value.size());
    append(value.c_str(), value.size());
    return *this;
}

ByteBuffer & ByteBuffer::operator<<(const char * str) 
{
    append((uint32_t)strlen(str));
    append(str, strlen(str));
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(bool &value)
{
    value = read<bool>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(uint8_t &value)
{
    value = read<uint8_t>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(uint16_t &value)
{
    value = read<uint16_t>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(uint32_t &value)
{
    value = read<uint32_t>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(uint64_t &value)
{
    value = read<uint64_t>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(int8_t &value)
{
    value = read<int8_t>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(int16_t &value)
{
    value = read<int16_t>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(int32_t &value)
{
    value = read<int32_t>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(int64_t &value)
{
    value = read<int64_t>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(float &value)
{
    value = read<float>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(double &value)
{
    value = read<double>();
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(std::string & value) 
{
    value.clear();
    
    uint32_t size = read<uint32_t>();
    for(uint32_t i = 0; i < size; i++)
        value.push_back(read<char>());
    return *this;
}

ByteBuffer & ByteBuffer::operator>>(char &value)
{
    value = read<char>();
    return *this;
}

uint8_t ByteBuffer::operator[](uint32_t pos)
{
    return read<uint8_t>(pos);
}

