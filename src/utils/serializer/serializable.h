#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "fieldDescriptor.h"

#include <vector>
#include <type_traits>
#include <iostream>

class Serializable
{
protected:
    std::vector<FieldDescriptor> _fields;

public:
    const std::vector<FieldDescriptor> &GetFields() const;
    void AddField(void *ptr, const char *name, const std::type_info &type);

    virtual ~Serializable() = default;

    inline virtual void Register() {
        std::cout << "<NoN Registers>" << std::endl;
    }
};

#endif