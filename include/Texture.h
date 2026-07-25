#pragma once

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include "glad/glad.h"
#endif
#include "plog/Log.h"

namespace Engine {
    class Texture {
        unsigned int texture{};
    public:
        int width{}, height{}, nrChannels{};

        explicit Texture(const char *filename);

        ~Texture();

        void nearest() const;

        void bind() const;

        [[nodiscard]] unsigned int getTexture() const;

        static unsigned char *loadImage(const char *path, int *w, int *h);
    };
}
