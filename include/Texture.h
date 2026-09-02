#pragma once

#include "glad/glad.h"
#include "plog/Log.h"

namespace Engine {
    class Texture {
        unsigned int texture{};
    public:
        int width{}, height{}, nrChannels{};

        explicit Texture(const char *filename);

        ~Texture();

        Texture(const Texture &) = delete;

        Texture &operator=(const Texture &) = delete;

        void nearest() const;

        void bind() const;

        [[nodiscard]] unsigned int getTexture() const;

        // Decodes an image to RGBA8; the result must be released with stbi_image_free().
        // Returns nullptr on failure.
        static unsigned char *loadImage(const char *path, int *w, int *h);
    };
}
