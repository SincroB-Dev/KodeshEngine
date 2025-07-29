#pragma once

#include "serializer/fieldDescriptor.h"
#include "serializer/register.h"

#define GET_MACRO(_1, _2, NAME, ...) NAME

#define REGISTER_FIELD_1(field) RegisterField(this, field, #field)
#define REGISTER_FIELD_2(field, title) RegisterField(this, field, title)

#define REGISTER_FIELD(...) GET_MACRO(__VA_ARGS__, REGISTER_FIELD_2, REGISTER_FIELD_1)(__VA_ARGS__)