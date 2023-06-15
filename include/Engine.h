#pragma once

#define ASSERT(text, expression) assert(((void) text, expression))
#define GET_DEFAULT(value, key, type, def) value.contains(key) ? value[key].get<type>() : def
#ifndef NDEBUG
#define CHECK_DATA {PLOGF_IF(!Engine::Filesystem::exists("data")) << "Working directory is " << Engine::Filesystem::getWorkingPath();}
#else
#define CHECK_DATA
#endif
