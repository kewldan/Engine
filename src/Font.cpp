#include "Font.h"

Engine::Font::Font(unsigned int VAO, Engine::Shader *shader, const char *path) : shader(shader),
                                                                                 VAO(VAO) {
    texture = new Texture("font.png");
    glm::vec2 textureSize = glm::vec2(texture->width, texture->height);

    scale = 1.f;

    const char *fnt = Engine::File::readString("data/fonts/font.fnt");

    const char *info_ptr = strstr(fnt, "info ");
    fontSize = strtol(strstr(info_ptr, "size=") + 5, nullptr, 10);

    lineHeight = 0;

    const char *char_ptr = strstr(fnt, "char ");
    while (char_ptr) {
        int id = strtol(strstr(char_ptr, "id=") + 3, nullptr, 10);
        int x = strtol(strstr(char_ptr, "x=") + 2, nullptr, 10);
        int y = strtol(strstr(char_ptr, "y=") + 2, nullptr, 10);
        int width = strtol(strstr(char_ptr, "width=") + 6, nullptr, 10);
        int height = strtol(strstr(char_ptr, "height=") + 7, nullptr, 10);
        int advance = strtol(strstr(char_ptr, "xadvance=") + 9, nullptr, 10);
        int xoffset = strtol(strstr(char_ptr, "xoffset=") + 8, nullptr, 10);
        int yoffset = strtol(strstr(char_ptr, "yoffset=") + 8, nullptr, 10);

        if (height > lineHeight) {
            lineHeight = height;
        }

        Characters[(char) id] = Character(glm::vec2(x, y) / textureSize, glm::vec2(width, height) / textureSize,
                                          glm::vec2(width, height),
                                          glm::vec2(xoffset, yoffset),
                                          advance);

        char_ptr++;
        char_ptr = strstr(char_ptr, "char ");
    }
}

char *Engine::Font::format(const char *fmt, ...) {
    static char *buf = new char[1024];
    va_list args;
            va_start(args, fmt);
    vsnprintf(buf, 1024, fmt, args);
    buf[1023] = 0;
            va_end(args);
    return buf;
}

void Engine::Font::text(Engine::Camera *camera, float x, float y, const char *text, glm::vec4 color) {
    glBindVertexArray(VAO);

    shader->bind();
    shader->upload("color", color);
    shader->upload("proj", camera->getOrthographic());
    glActiveTexture(GL_TEXTURE0);
    shader->upload("tex", 0);
    texture->bind();

    float offsetX = x, offsetY = y;
    for (int i = 0; i < strlen(text); i++) {
        char c = text[i];
        if (c == '\n') {
            offsetY += (lineHeight + 3) * scale;
            offsetX = x;
        } else if (Characters.contains(c)) {
            Character ch = Characters[c];
            glm::mat4 mvp = glm::translate(glm::mat4(1), glm::vec3(offsetX + ch.offset.x * scale, offsetY + ch.offset.y * scale, -0.1f));
            mvp = glm::scale(mvp, glm::vec3(ch.size * scale, 1));
            shader->upload("uv", ch.uv);
            shader->upload("size", ch.uvSize);
            shader->upload("mvp", mvp);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            offsetX += (float) ch.advance * scale;
        }
    }
}

void Engine::Font::setSize(int size) {
    scale = (float) size / (float) fontSize;
}
