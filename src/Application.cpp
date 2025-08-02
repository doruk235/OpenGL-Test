#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <fstream>
#include <string>
#include <sstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "glUtilities.h"
#include "Texture.h"

#include "test/test.h"
#include "test/TestClearColor.h"




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
    
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    /*Initialize glew*/
    if (glewInit() != GLEW_OK) {
        std::cout << "Glew Init Error" << std::endl;
        return -1;
    }

    glCall(glEnable(GL_BLEND));
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;
        
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init((char*)glGetString(460));

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;


    testMenu->RegisterTest<test::TestClearColor>("Clear Color");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
        {
            glCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f)); 
            renderer.Clear();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest) {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("-<")) {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }
            {
                ImGui::Begin("Control Panel");

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
           
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
    }

    if (currentTest != testMenu)
        delete testMenu;
    delete currentTest;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}