#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError() {

    while (glGetError() != GL_NO_ERROR);

}

static bool GLLogCall(const char* function, const char* file, const int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGl Error]: " << error << " in file " << file << " in function " << function<< " line "<< line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource{
    std::string VertexSource;
    std::string FragmentSource;
};



static ShaderProgramSource parseShader(const std::string& filepath) {
    
    std::ifstream stream(filepath);
    
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    
    std::string line;
    std::stringstream sStream[2];
    ShaderType type = ShaderType::NONE;

    
    while ( getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {

            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;

            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;

            }
            else {
                type = ShaderType::NONE;
            }
       
        }
        else {

            sStream[(int)type] << line << '\n';
        }
    
    }
    return{ sStream[0].str(),sStream[1].str() };


}

static unsigned int compileShader(unsigned int type, const std::string& source) {

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);


    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS,&result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) _malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 5;
    }

    return id;

}

static int createShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
     
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void) {

    GLFWwindow* window;

    /* Initialize glfw */
    if (!glfwInit()) {
        std::cout << "Glfw Init Error" << std::endl;
        return -1;
    }
        
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 960, "OpenGL Test", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /*Initialize glew*/
    if (glewInit() != GLEW_OK) {
        std::cout << "Glew Init Error" << std::endl;
        return -1;
    }

    
    float positions[] = {
       -0.5f,-0.5f,
        0.5f,-0.5f,
        0.5f, 0.5f,
       -0.5f, 0.5f,
    };

    const int positionNumber = 2;
    const int vertexSize = 2;

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };


    


    //Vertex Buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * vertexSize * sizeof(float), positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * vertexSize), 0);


    //Index Buffer
    unsigned int indexBufferObject;
    glGenBuffers(1, &indexBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    


    ShaderProgramSource source = parseShader("res/Shaders/basic.shader");


    unsigned int shader = createShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);


    double r = 0.4f;
    double g = 0.2f;
    double b = 0.8f;
    double a = 1.0f;

    double rInc = 0.005f;
    double gInc = 0.002f;
    double bInc = 0.01f;
    double aInc = 0;

    bool incrementing = true;
    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);




    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        

        glUniform4f(location, r, g, b, a);
        glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
         
        if (incrementing) {
            if (r > 1.0f || r < 0.0f) {
                rInc = -rInc;
            }

            r += rInc;

            if (g > 1.0f || g < 0.0f) {
                gInc = -gInc;
            }

            g += gInc;

            if (b > 1.0f || b < 0.0f) {
                bInc = -bInc;
            }

            b += bInc;

            if (a > 1.0f || a < 0.0f) {
                aInc = -aInc;
            }

            a += aInc;


        }


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }


    //glDeleteProgram(shader);


    glfwTerminate();
    return 0;
}