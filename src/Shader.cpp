#include "Shader.h"
#include <cstdio>
#include <string>

#define SHADER_PART_VERTEX 1
#define SHADER_PART_GEOMETRY 2
#define SHADER_PART_FRAGMENT 4

Engine::Shader::Shader(const char *filename) : filename(filename) {
    ASSERT("Filename is nullptr", filename != nullptr);
    shaderParts = 0;
    program = glCreateProgram();
    ASSERT("Program invalid", program > 0);

    uniforms = new Uniforms();

    vertex = -1;
    geometry = -1;
    fragment = -1;
    blockIndex = 0;

    char path[128];
    std::snprintf(path, sizeof(path), "data/shaders/%s.vert", filename);

#ifndef NDEBUG
    if (Engine::Filesystem::exists(path)) {
        vertex = loadShader(path, GL_VERTEX_SHADER, SHADER_PART_VERTEX);
    }

    path[strlen(path) - 5] = 0;
    strncat(path, ".frag", sizeof(path) - strlen(path) - 1);
    if (Engine::Filesystem::exists(path)) {
        fragment = loadShader(path, GL_FRAGMENT_SHADER, SHADER_PART_FRAGMENT);
    }

    path[strlen(path) - 5] = 0;
    strncat(path, ".geom", sizeof(path) - strlen(path) - 1);
    if (Engine::Filesystem::exists(path)) {
        geometry = loadShader(path, GL_GEOMETRY_SHADER, SHADER_PART_GEOMETRY);
    }
#else
    if (Engine::Filesystem::resourceExists(path)) {
        vertex = loadShader(path, GL_VERTEX_SHADER, SHADER_PART_VERTEX);
    }

    path[strlen(path) - 5] = 0;
    strncat(path, ".frag", sizeof(path) - strlen(path) - 1);
    if (Engine::Filesystem::resourceExists(path)) {
        fragment = loadShader(path, GL_FRAGMENT_SHADER, SHADER_PART_FRAGMENT);
    }

    path[strlen(path) - 5] = 0;
    strncat(path, ".geom", sizeof(path) - strlen(path) - 1);
    if (Engine::Filesystem::resourceExists(path)) {
        geometry = loadShader(path, GL_GEOMETRY_SHADER, SHADER_PART_GEOMETRY);
    }
#endif
    path[strlen(path) - 5] = 0;

    glLinkProgram(program);

    {
        int linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        int logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if (linked != GL_TRUE || logLength > 0) {
            char *log = new char[logLength > 0 ? logLength : 1];
            glGetProgramInfoLog(program, logLength, nullptr, log);
            printf("[program %s] link=%d log:\n%s\n", filename, linked, log);
            delete[] log;
        }
    }

    if (shaderParts == 0) {
        PLOGW << "Empty shader [" << filename << "] linked";
    } else {
        PLOGI << "Shader [" << filename << "] linked ["
              << ((shaderParts & SHADER_PART_VERTEX) != 0 ? 'V' : '\0')
              << ((shaderParts & SHADER_PART_GEOMETRY) != 0 ? 'G' : '\0')
              << ((shaderParts & SHADER_PART_FRAGMENT) != 0 ? 'F' : '\0')
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

int Engine::Shader::loadShader(const char *path, int type, const char bitshift) {
    ASSERT("Path is nullptr", path != nullptr);
    int shader = glCreateShader(type);
    ASSERT("Shader invalid", shader > 0);
    const char *shader_source;
#ifndef NDEBUG
    shader_source = Engine::Filesystem::readString(path);
#else
    shader_source = Engine::Filesystem::readResourceString(path);
#endif
    if (shader_source == nullptr) {
        PLOGE << "Shader source [" << path << "] could not be read";
        return shader;
    }
#ifdef __EMSCRIPTEN__
    std::string source = shader_source;
    const std::size_t versionEnd = source.find('\n');
    if (source.rfind("#version", 0) == 0 && versionEnd != std::string::npos) {
        source = "#version 300 es\nprecision highp float;\nprecision highp int;\n"
                 + source.substr(versionEnd + 1);
    }
    const char *patched_source = source.c_str();
    glShaderSource(shader, 1, &patched_source, nullptr);
#else
    glShaderSource(shader, 1, &shader_source, nullptr);
#endif
    glCompileShader(shader);
    delete[] shader_source;

    int length = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        char *log = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, log);
        PLOG_WARNING << "Shader log:\n" << log;
        printf("[shader %s]\n%s\n", path, log);
        delete[] log;
    }

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE) {
        glAttachShader(program, shader);
        shaderParts += bitshift;
    } else {
        PLOG_WARNING << "Shader found, but not attached";
        printf("[shader %s] FAILED TO COMPILE\n", path);
    }
    return shader;
}

GLint Engine::Shader::getUniformLocation(const char *name) const {
    ASSERT("Name is nullptr", name != nullptr);
    if (uniforms->contains(name)) {
        return (*uniforms)[name];
    }
    GLint value = glGetUniformLocation(program, name);
    if (value == -1) {
        PLOGE << "Uniform location in shader [" << filename << "] not found > " << name;
    }
    (*uniforms)[name] = value;
    return value;
}

void Engine::Shader::bind() const {
    glUseProgram(program);
}

Engine::Shader::~Shader() {
    PLOGW << "Shader [" << filename << "] was destroyed";
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
    delete uniforms;
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

void Engine::Shader::upload(const char *name, glm::mat4 value) const {
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

void Engine::Shader::uploadMat4(const char *name, float *value) const {
    ASSERT("Name is nullptr", name != nullptr);
    ASSERT("Value is nullptr", value != nullptr);
    glUniformMatrix4fv(getUniformLocation(name), 1, false, value);
}

void Engine::Shader::bindUniformBlock(const char *name) {
    ASSERT("Name is nullptr", name != nullptr);
    unsigned int bindingPoint = glGetUniformBlockIndex(program, name);
    glUniformBlockBinding(program, bindingPoint, blockIndex++);
}

Engine::UniformBlock::UniformBlock(unsigned int size) {
    ASSERT("Size must be > 0", size > 0);
    glGenBuffers(1, &block);
    glBindBuffer(GL_UNIFORM_BUFFER, block);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, block, 0, size);
    glBindBuffer(GL_UNIFORM_BUFFER, block);
    offset = 0;
}

void Engine::UniformBlock::add(unsigned int size, void *value) {
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, value);
    offset += size;
}
