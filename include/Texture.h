#pragma once

#include "Windows.h"
#include "glad/glad.h"
#include "plog/Log.h"
#include "io/Filesystem.h"
#include "stb_image.h"

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
