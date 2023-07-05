#pragma once

class Serializable {
public:
    virtual void write(char* buffer) = 0;
    virtual void read(const char* buffer) = 0;
};