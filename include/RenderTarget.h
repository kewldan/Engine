#pragma once

#include "Shader.h"

namespace Engine {
    class RenderTarget {
    private:
        unsigned int FBO{};
        unsigned int *attachments;
    public:
        int w, h;

        RenderTarget(int w, int h);

        void bind();
    };
}