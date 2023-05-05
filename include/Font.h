#pragma once

#include "glm/ext.hpp"
#include "File.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera2D.h"
#include <string>
#include <sstream>
#include <map>
#include "stb_truetype.h"

namespace Engine {
    struct Character {
        glm::vec2 uv;
        glm::vec2 uvSize;
        glm::vec2 size;
        glm::vec2 offset;
        int advance;
    };

    class Font {
    private:
        Texture *texture;
        int fontSize, lineHeight;
        float scale;
        std::map<char, Character> Characters;
        Engine::Shader *shader;
        unsigned int VAO;
    public:
        explicit Font(unsigned int VAO, Engine::Shader *shader, const char *path);

        static char *format(const char *fmt, ...);

        void text(Engine::Camera2D *camera, float x, float y, const char *text, glm::vec4 color = glm::vec4(1));

        void setSize(int size);
    };
}