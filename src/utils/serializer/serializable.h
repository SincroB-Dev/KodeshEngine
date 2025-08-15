#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include "fieldDescriptor.h"

#include <vector>
#include <type_traits>
#include <iostream>
#include "../../ui/forms/logger.h"

class Serializable
{
protected:
    std::vector<FieldDescriptor> _fields;

public:
    const std::vector<FieldDescriptor> &GetFields() const;
    void AddField(void *ptr, const char *name, const std::type_info &type, size_t size);

    virtual ~Serializable() = default;

    inline virtual void Register() {
        core::ui::LogWindow::Log("<NoN Registers>", core::ui::LogType::INFO);
    }
};

#endif
