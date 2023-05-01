#pragma once

#define ASSERT(text, expression) assert(((void) text, expression))
#define GET_DEFAULT(value, key, type, def) value.contains(key) ? value[key].get<type>() : def
