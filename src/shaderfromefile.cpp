#include "common.hpp"
#include "shaderutils.hpp"

#define ERROR_CREATE_WINDOW -1
#define ERROR_LOADING_GLAD -2
#define ERROR_OPENGL -3

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main(void) {
    //init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//for macos
#endif

    //create window
    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return ERROR_CREATE_WINDOW;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return ERROR_LOADING_GLAD;
    }

    //build and compile our shader program
    //------------------------------------
    Shader triangleShader("shaders/shaderfromfile.vert", "shaders/shaderfromfile.frag");
    
    //set up vertex data (and buffers) and config vertex attributes
    //-------------------------------------------------------------
    float vertices[] = {
        //position      //color
        .0f, .5f, .0f,  1.0f, 0.0f, 0.0f,//top right
        .5f, -.5f, .0f, 0.0f, 1.0f, 0.0f,//bottom right
        -.5f, -.5f, .0f,0.0f, 0.0f, 1.0f//bottom left
    };
    unsigned int indices [] = {//note that we start from 0
        0, 1, 2,//first triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    //bind the vertex array objects first, then bind and set vertex buffers, and then configure vertex attributes.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //remeber: do not unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO, keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    //VAO requires a call to glBindVertexArray anyways so we generally don't unbind VAOS (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    //uncomment this call to draw in wireframe polygons
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    //draw square
    triangleShader.use();
    glBindVertexArray(VAO);//seeing as we only hava 1 VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //loop rendering
    while (!glfwWindowShouldClose(window))
    {
        //input
        common_process_input(window);
        //prerendering
        glClearColor(.2f, .3f, .4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //change color as animation
        triangleShader.setFloat("anim_time", (sin(glfwGetTime() / 2.0f) + 0.5f));
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);//no need to unbind it every time

        //rendering
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //optional:de-allocate all resources once they've outlived there purpose:
    //-----------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(triangleShader.ID);
    //glfw: terminate, clearing all previously allocated GLFW resources.
    //------------------------------------------------------------------
    glfwTerminate();
    return 0;
}