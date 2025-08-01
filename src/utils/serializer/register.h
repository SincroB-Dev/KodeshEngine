#ifndef REGISTER_H
#define REGISTER_H

#include "serializable.h"

#include <type_traits>

template<typename TClass, typename TField>
void RegisterField(TClass *obj, TField &field, const char *name, size_t size)
{
    static_assert(std::is_base_of<Serializable, TClass>::value, "Classe deve herdar de Serializable para usar o REGISTER_FIELD");

    std::cout << "Registrando campo '" << name << "' na Classe" << std::endl;

    obj->AddField(&field, name, typeid(TField), size);
}

#endif