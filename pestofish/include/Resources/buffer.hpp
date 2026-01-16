//
// Created by Kiril on 1/16/2026.
//

#pragma once
#include <iosfwd>
#include <streambuf>

struct MemBuf : std::streambuf {
    MemBuf(const unsigned char* base, size_t size) {
        char* p(const_cast<char*>(reinterpret_cast<const char*>(base)));
        this->setg(p, p, p + size);
    }
};