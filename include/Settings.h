#pragma once

#include <Shlobj.h>
#include <comdef.h>
#include "json.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "File.h"

namespace Engine {
    class Settings {
        char *path;
    public:
        nlohmann::json json;

        explicit Settings(const char *name);

        void save();

        void load();

        bool exists();

        template<class T>
        T get(const nlohmann::json::json_pointer &ptr, T defaultValue);
    };
}