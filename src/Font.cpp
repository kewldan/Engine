#include "Font.h"

Engine::Font::Font(const char *name, bool from_file) {
    static char *path = new char[128];
    strcpy_s(path, 128, name);
    strcat_s(path, 128, ".png");

    texture = new Texture(path, from_file);
    path[strlen(path) - 4] = 0;
    strcat_s(path, 128, ".fnt");
    const char* font = Engine::File::readString(path);

    const char* info_str = strstr(font, "info ");
    int size = strtol(strstr(info_str, "size=") + 5, nullptr, 10);
    int stretchH = strtol(strstr(info_str, "stretchH=") + 9, nullptr, 10);
    int smooth = strtol(strstr(info_str, "smooth=") + 7, nullptr, 10);
    int aa = strtol(strstr(info_str, "aa=") + 3, nullptr, 10);
    int paddingL = strtol(strstr(info_str, "padding=") + 8, nullptr, 10);
    int paddingR = strtol(strstr(info_str, ",") + 1, nullptr, 10);
    int paddingTop = strtol(strstr(info_str, ",") + 1, nullptr, 10);
    int paddingBottom = strtol(strstr(info_str, ",") + 1, nullptr, 10);
    int spacingX = strtol(strstr(info_str, "spacing=") + 8, nullptr, 10);
    int spacingY = strtol(strstr(info_str, ",") + 1, nullptr, 10);
}

char *Engine::Font::format(const char *fmt, ...) {
    return nullptr;
}

void Engine::Font::text(float x, float y, const char *text, glm::vec3 color) {
    //
}
