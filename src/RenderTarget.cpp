#include "RenderTarget.h"

Engine::RenderTarget::RenderTarget(int w, int h) : w(w), h(h) {
    glGenFramebuffers(1, &FBO);
    bind();


}

void Engine::RenderTarget::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}
