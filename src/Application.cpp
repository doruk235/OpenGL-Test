#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "glUtilities.h"

int main(void) {

    GLFWwindow* window;

    /* Initialize glfw */
    if (!glfwInit()) {
        std::cout << "Glfw Init Error" << std::endl;
        return -1;
    }


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 960, "OpenGL Test", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        std::cout << "GLFW Init Error" << std::endl;
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

    {
        float positions[] = {
           -0.5f,-0.5f,
            0.5f,-0.5f,
            0.5f, 0.5f,
           -0.5f, 0.5f,
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };


        glCall(glBindVertexArray(0));
        glCall(glUseProgram(0));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));



        VertexArray vertexArray;

        VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;

        layout.Push<float>(2);

        vertexArray.AddBuffer(vertexBuffer, layout);

        IndexBuffer indexBuffer(indices, 6);
        

        double r = 0.4f;
        double g = 0.2f;
        double b = 0.8f;
        double a = 1.0f;

        double rInc = 0.005f;
        double gInc = 0.002f;
        double bInc = 0.01f;
        double aInc = 0;

        bool incrementing = true;

        Shader shader("res/Shaders/basic.shader");
        Renderer renderer;

        renderer.Draw(vertexArray, indexBuffer, shader);
        shader.SetUniforms4f("u_Color", r, g, b, a);


        vertexArray.UnBind();
        shader.UnBind();
        vertexBuffer.UnBind();
        indexBuffer.UnBind();




        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();
            renderer.Draw(vertexArray, indexBuffer, shader);
            shader.SetUniforms4f("u_Color", r, g, b, a);


            //Epilepsy Crisis Mode
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
    }

    glfwTerminate();
    return 0;
}