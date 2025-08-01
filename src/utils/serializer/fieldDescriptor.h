#pragma once

#include <typeinfo>
#include <string>
#include <type_traits>

struct FieldDescriptor
{
    std::string name;
    void *ptr;
    const std::type_info &type;
    size_t size;

    FieldDescriptor(const std::string &n, void *p, const std::type_info &t, size_t s)
        : name(n), ptr(p), type(t), size(s) {}
};