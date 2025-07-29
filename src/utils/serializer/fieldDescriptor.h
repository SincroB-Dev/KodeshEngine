#pragma once

#include <string>
#include <type_traits>

struct FieldDescriptor
{
    std::string name;
    void *ptr;
    const std::type_info &type;

    FieldDescriptor(const std::string &n, void *p, const std::type_info &t)
        : name(n), ptr(p), type(t) {}
};