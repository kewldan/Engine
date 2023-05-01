#pragma once

#include "glm/ext.hpp"
#include "File.h"
#include "Texture.h"
#include <string>
#include <sstream>

namespace Engine {
    class Font {
    private:
        Texture* texture;
    public:
        explicit Font(const char* name, bool from_file = false);
        char* format(const char* fmt, ...);
        void text(float x, float y, const char* text, glm::vec3 color = glm::vec3(0));
    };
}