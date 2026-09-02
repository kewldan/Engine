#include "Texture.h"

#include "Engine.h"
#include "io/Filesystem.h"
#include <cstdio>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace {
    // Mirrors the debug/release split used by Shader: files on disk in debug builds,
    // RT_RCDATA resources in release builds (plain files off Windows).
    unsigned char *loadPixels(const char *path, int *w, int *h, int *channels, int desiredChannels) {
#ifndef NDEBUG
        return stbi_load(path, w, h, channels, desiredChannels);
#else
        int size = 0;
        char *raw = Engine::Filesystem::readResourceFile(path, &size);
        if (raw == nullptr || size <= 0) {
            delete[] raw;
            return nullptr;
        }
        unsigned char *pixels = stbi_load_from_memory(reinterpret_cast<const unsigned char *>(raw), size,
                                                      w, h, channels, desiredChannels);
        delete[] raw;
        return pixels;
#endif
    }

    GLenum formatForChannels(int channels) {
        switch (channels) {
            case 1:
                return GL_RED;
            case 2:
                return GL_RG;
            case 3:
                return GL_RGB;
            default:
                return GL_RGBA;
        }
    }
}

Engine::Texture::Texture(const char *filename) {
    ASSERT("Name is nullptr", filename != nullptr);
    glGenTextures(1, &texture);
    ASSERT("Texture invalid", texture > 0);

    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    const std::string path = std::string("data/textures/") + filename;
    unsigned char *data = loadPixels(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        // Rows are tightly packed by stb; the GL default (4-byte row alignment) would skew RGB
        // images whose row size is not a multiple of 4.
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        const GLenum format = formatForChannels(nrChannels);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint) format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    } else {
        PLOGE << "Failed to load texture [" << filename << "]";
        PLOGE << stbi_failure_reason();
    }
    stbi_image_free(data);
}

Engine::Texture::~Texture() {
    glDeleteTextures(1, &texture);
}

void Engine::Texture::nearest() const {
    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Engine::Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texture);
}

unsigned char *Engine::Texture::loadImage(const char *path, int *w, int *h) {
    ASSERT("Path is nullptr", path != nullptr);
    return loadPixels(path, w, h, nullptr, 4);
}

unsigned int Engine::Texture::getTexture() const {
    return texture;
}
