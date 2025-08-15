#ifndef REGISTER_H
#define REGISTER_H

#include "serializable.h"

#include <type_traits>
#include <cstring>
#include "../../ui/forms/logger.h"

template<typename TClass, typename TField>
void RegisterField(TClass *obj, TField &field, const char *name, size_t size)
{
    static_assert(std::is_base_of<Serializable, TClass>::value, "Classe deve herdar de Serializable para usar o REGISTER_FIELD");

    core::ui::LogWindow::Log({"Registrando campo '", name, "' na Classe"}, core::ui::LogType::INFO);

    obj->AddField(&field, name, typeid(TField), size);
}

#endif
