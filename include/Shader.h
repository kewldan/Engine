#pragma once

#include "glad/glad.h"
#include "glm/ext.hpp"
#include <cstdint>
#include <functional>
#include <plog/Log.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include "io/Filesystem.h"

namespace Engine {
    class UniformBlock {
        unsigned int offset{};
    public:
        unsigned int block{};

        // Creates a GL_UNIFORM_BUFFER of `size` bytes and binds it to `bindingPoint`
        // (the index handed out by Shader::bindUniformBlock, in call order starting at 0).
        explicit UniformBlock(unsigned int size, unsigned int bindingPoint = 0);

        ~UniformBlock();

        UniformBlock(const UniformBlock &) = delete;

        UniformBlock &operator=(const UniformBlock &) = delete;

        void add(unsigned int size, const void *value);
    };

    // Lets the uniform cache be queried with a `const char *` without building a std::string per lookup.
    struct TransparentStringHash {
        using is_transparent = void;

        std::size_t operator()(std::string_view s) const noexcept { return std::hash<std::string_view>{}(s); }
    };

    typedef std::unordered_map<std::string, int, TransparentStringHash, std::equal_to<>> Uniforms;

    class Shader {
    private:
        unsigned int vertex{}, fragment{}, geometry{}, program{}, blockIndex{};
        mutable Uniforms uniforms;
        int8_t shaderParts{};
        std::string filename;

        // Cached uniform lookup; logs an error for a missing uniform only when `logMissing` is set.
        int lookupUniform(const char *name, bool logMissing) const;

        int getUniformLocation(const char *name) const;

        int getAttribLocation(const char *name) const;

        unsigned int loadShader(const char *path, int type, char bitshift);

    public:
        explicit Shader(const char *filename);

        ~Shader();

        Shader(const Shader &) = delete;

        Shader &operator=(const Shader &) = delete;

        [[nodiscard]] unsigned int getProgramId() const;

        void bind() const;

        // True if the linked program has an active uniform called `name` (uses the same cache as upload()).
        [[nodiscard]] bool hasUniform(const char *name) const;

        void upload(const char *name, int value) const;

        void upload(const char *name, float value) const;

        void upload(const char *name, glm::vec2 value) const;

        void upload(const char *name, glm::vec3 value) const;

        void upload(const char *name, glm::vec4 value) const;

        void upload(const char *name, const glm::mat4 &value) const;

        // Returns a pointer to a static buffer: not thread-safe, overwritten by the next call.
        static char *getElementName(const char *name, int index);

        void uploadMat4(const char *name, const float *value) const;

        void bindUniformBlock(const char *name);
    };
}
