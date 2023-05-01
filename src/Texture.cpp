#include "Texture.h"

Engine::Texture::Texture(const char *filename, bool from_file) {
    ASSERT("Name is nullptr", filename != nullptr);
    glGenTextures(1, &texture);
    ASSERT("Texture invalid", texture > 0);

    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    static char *f = new char[128];
    strcpy_s(f, 128, "data/textures/");
    strcat_s(f, 128, filename);

    unsigned char *data;
    if (from_file) {
        data = stbi_load(f, &width, &height, &nrChannels, 0);
    } else {
        int size = 0;
        auto *raw = reinterpret_cast<unsigned char *>(Engine::File::readResourceFile(f, &size));
        data = stbi_load_from_memory(raw, size, &width, &height, &nrChannels, 0);
    }
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nrChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
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
    int size = 0;
    auto *raw = reinterpret_cast<unsigned char *>(Engine::File::readResourceFile(path, &size));
    return stbi_load_from_memory(raw, size, w, h, nullptr, 4);
}
