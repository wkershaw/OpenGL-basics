#include "OPENGL.h"

void GLClearError() { //Clear all OpenGL error flags
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError())
    {
        std::cout << "OpenGL error " << function <<
            " in " << file << " on line "
            << line << " : "
            << error << std::endl;
        return false;
    }
    return true;
}