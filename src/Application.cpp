#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <fstream>
#include <string>
#include <sstream>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "glUtilities.h"
#include "Texture.h"




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
        
    int projectRes[] = {1920,1080};

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(projectRes[0], projectRes[1], "OpenGL Test", nullptr, NULL);

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
            500.0f, 500.0f, 0.0f, 0.0f,
            1000.0f, 500.0f, 1.0f, 0.0f,
            1000.0f, 1000.0f, 1.0f, 1.0f,
            500.0f, 1000.0f, 0.0f, 1.0f,
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };


        glCall(glEnable(GL_BLEND));
        glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        glCall(glBindVertexArray(0));
        glCall(glUseProgram(0));
        glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));



        VertexArray vertexArray;

        VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;

        layout.Push<float>(2);
        layout.Push<float>(2);

        vertexArray.AddBuffer(vertexBuffer, layout);

        IndexBuffer indexBuffer(indices, 6);
       

        glm::mat4 proj = glm::ortho(0.0f, (float) projectRes[0], 0.0f, (float)projectRes[1], -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(100, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(100, -200, 0));

        glm::mat4 mvp = proj * view * model;

        double r = 0.4f;
        double g = 0.2f;
        double b = 0.8f;
        double a = 0.0f;

        double rInc = 0.005f;
        double gInc = 0.002f;
        double bInc = 0.01f;
        double aInc = 0;

        bool incrementing = true;

        Texture texture("Res/Textures/99.png");
        short textureSlot = 0;
        texture.Bind(textureSlot);

        Shader shader("res/Shaders/basic.shader");
        Renderer renderer;

        renderer.Draw(vertexArray, indexBuffer, shader);
        shader.SetUniforms4f("u_Color", r, g, b, a);
        shader.SetUniforms4m("u_MVP", mvp);


        shader.SetUniforms1i("u_Texture", textureSlot);

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