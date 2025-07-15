#include "Renderer.h"
#include <iostream>
#include <GL/glew.h>



void GLClearError() {

    while (glGetError() != GL_NO_ERROR);

}

bool GLLogCall(const char* function, const char* file, const int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGl Error]: " << error << " in file " << file << " in function " << function << " line " << line << std::endl;
        return false;
    }
    return true;
}