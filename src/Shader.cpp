#include "Shader.h"
#include <cstdio>
#include <string>

#define SHADER_PART_VERTEX 1
#define SHADER_PART_GEOMETRY 2
#define SHADER_PART_FRAGMENT 4

namespace {
    bool shaderSourceExists(const std::string &path) {
#ifndef NDEBUG
        return Engine::Filesystem::exists(path.c_str());
#else
        return Engine::Filesystem::resourceExists(path.c_str());
#endif
    }
}

Engine::Shader::Shader(const char *filename) : filename(filename ? filename : "") {
    ASSERT("Filename is nullptr", filename != nullptr);
    program = glCreateProgram();
    ASSERT("Program invalid", program > 0);

    const std::string base = "data/shaders/" + this->filename;

    std::string path = base + ".vert";
    if (shaderSourceExists(path)) {
        vertex = loadShader(path.c_str(), GL_VERTEX_SHADER, SHADER_PART_VERTEX);
    }

    path = base + ".frag";
    if (shaderSourceExists(path)) {
        fragment = loadShader(path.c_str(), GL_FRAGMENT_SHADER, SHADER_PART_FRAGMENT);
    }

    path = base + ".geom";
    if (shaderSourceExists(path)) {
        geometry = loadShader(path.c_str(), GL_GEOMETRY_SHADER, SHADER_PART_GEOMETRY);
    }

    glLinkProgram(program);

    int linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::string log(length > 0 ? length : 0, '\0');
        if (length > 0) {
            glGetProgramInfoLog(program, length, nullptr, log.data());
        }
        PLOGE << "Shader [" << this->filename << "] failed to link:\n" << log.c_str();
    }

    if (shaderParts == 0) {
        PLOGW << "Empty shader [" << this->filename << "] linked";
    } else {
        PLOGI << "Shader [" << this->filename << "] linked ["
              << ((shaderParts & SHADER_PART_VERTEX) != 0 ? "V" : "")
              << ((shaderParts & SHADER_PART_GEOMETRY) != 0 ? "G" : "")
              << ((shaderParts & SHADER_PART_FRAGMENT) != 0 ? "F" : "")
              << ']';
    }
}

GLuint Engine::Shader::getProgramId() const {
    return program;
}

GLint Engine::Shader::getAttribLocation(const char *name) const {
    ASSERT("Name is nullptr", name != nullptr);
    GLint value = glGetAttribLocation(program, name);
    if (value == -1) {
        PLOGE << "Attrib location in shader not found > " << name;
    }
    return value;
}

// Returns the shader object on success, 0 when the source is missing or fails to compile.
GLuint Engine::Shader::loadShader(const char *path, int type, const char bitshift) {
    ASSERT("Path is nullptr", path != nullptr);
    GLuint shader = glCreateShader(type);
    ASSERT("Shader invalid", shader > 0);
    const char *shader_source;
#ifndef NDEBUG
    shader_source = Engine::Filesystem::readString(path);
#else
    shader_source = Engine::Filesystem::readResourceString(path);
#endif
    if (shader_source == nullptr) {
        PLOGE << "Shader source [" << path << "] could not be read";
        glDeleteShader(shader);
        return 0;
    }
    glShaderSource(shader, 1, &shader_source, nullptr);
    glCompileShader(shader);
    delete[] shader_source;

    int length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length > 1) { // 1 == just the terminating NUL
        std::string log(length, '\0');
        glGetShaderInfoLog(shader, length, nullptr, log.data());
        PLOG_WARNING << "Shader log:\n" << log.c_str();
    }

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE) {
        glAttachShader(program, shader);
        shaderParts |= bitshift;
        return shader;
    }
    PLOG_WARNING << "Shader [" << path << "] found, but not attached";
    glDeleteShader(shader);
    return 0;
}

GLint Engine::Shader::lookupUniform(const char *name, bool logMissing) const {
    ASSERT("Name is nullptr", name != nullptr);
    auto it = uniforms.find(std::string_view(name));
    if (it != uniforms.end()) {
        return it->second;
    }
    GLint value = glGetUniformLocation(program, name);
    if (value == -1 && logMissing) {
        PLOGE << "Uniform location in shader [" << filename << "] not found > " << name;
    }
    uniforms.emplace(name, value);
    return value;
}

GLint Engine::Shader::getUniformLocation(const char *name) const {
    return lookupUniform(name, true);
}

bool Engine::Shader::hasUniform(const char *name) const {
    return lookupUniform(name, false) != -1;
}

void Engine::Shader::bind() const {
    glUseProgram(program);
}

Engine::Shader::~Shader() {
    PLOGD << "Shader [" << filename << "] was destroyed";
    if ((shaderParts & SHADER_PART_VERTEX) != 0) {
        glDetachShader(program, vertex);
        glDeleteShader(vertex);
    }
    if ((shaderParts & SHADER_PART_FRAGMENT) != 0) {
        glDetachShader(program, fragment);
        glDeleteShader(fragment);
    }
    if ((shaderParts & SHADER_PART_GEOMETRY) != 0) {
        glDetachShader(program, geometry);
        glDeleteShader(geometry);
    }
    glDeleteProgram(program);
}

void Engine::Shader::upload(const char *name, int value) const {
    ASSERT("Name is nullptr", name != nullptr);
    glUniform1i(getUniformLocation(name), value);
}

void Engine::Shader::upload(const char *name, float value) const {
    ASSERT("Name is nullptr", name != nullptr);
    glUniform1f(getUniformLocation(name), value);
}

void Engine::Shader::upload(const char *name, glm::vec2 value) const {
    ASSERT("Name is nullptr", name != nullptr);
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Engine::Shader::upload(const char *name, glm::vec3 value) const {
    ASSERT("Name is nullptr", name != nullptr);
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Engine::Shader::upload(const char *name, glm::vec4 value) const {
    ASSERT("Name is nullptr", name != nullptr);
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Engine::Shader::upload(const char *name, const glm::mat4 &value) const {
    ASSERT("Name is nullptr", name != nullptr);
    glUniformMatrix4fv(getUniformLocation(name), 1, false, glm::value_ptr(value));
}

char *Engine::Shader::getElementName(const char *name, int index) {
    ASSERT("Name is nullptr", name != nullptr);
    ASSERT("Index must be >= 0", index >= 0);
    static char n[96];
    std::snprintf(n, sizeof(n), "%s[%d]", name, index);
    return n;
}

void Engine::Shader::uploadMat4(const char *name, const float *value) const {
    ASSERT("Name is nullptr", name != nullptr);
    ASSERT("Value is nullptr", value != nullptr);
    glUniformMatrix4fv(getUniformLocation(name), 1, false, value);
}

void Engine::Shader::bindUniformBlock(const char *name) {
    ASSERT("Name is nullptr", name != nullptr);
    unsigned int blockIdx = glGetUniformBlockIndex(program, name);
    if (blockIdx == GL_INVALID_INDEX) {
        PLOGE << "Uniform block in shader [" << filename << "] not found > " << name;
        return;
    }
    glUniformBlockBinding(program, blockIdx, blockIndex++);
}

Engine::UniformBlock::UniformBlock(unsigned int size, unsigned int bindingPoint) {
    ASSERT("Size must be > 0", size > 0);
    glGenBuffers(1, &block);
    glBindBuffer(GL_UNIFORM_BUFFER, block);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, block, 0, size);
}

Engine::UniformBlock::~UniformBlock() {
    glDeleteBuffers(1, &block);
}

void Engine::UniformBlock::add(unsigned int size, const void *value) {
    ASSERT("Value is nullptr", value != nullptr);
    // Re-bind: another buffer may have been bound to GL_UNIFORM_BUFFER since the constructor ran.
    glBindBuffer(GL_UNIFORM_BUFFER, block);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, value);
    offset += size;
}
