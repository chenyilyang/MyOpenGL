#include "common.hpp"
#include "shaderutils.hpp"
#define ERROR_CREATE_WINDOW -1
#define ERROR_LOADING_GLAD -2
#define ERROR_OPENGL -3

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
//light object position
const glm::vec3 lightObjectPosition(1.2f, 1.0f, 2.0f);
//cube position
const glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -15.0f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f),
};
float mixValue = 0.2f;
void combineTextureProcessInput(GLFWwindow * window) {
    common_process_input(window);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        mixValue += 0.001f;
        if (mixValue >= 1.0f)
        {
            mixValue = 1.0f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        mixValue -= 0.001f;
        if (mixValue <= 0.0f)
        {
            mixValue = 0.0f;
        }
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
    }
}

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
    lastX = SCR_WIDTH / 2.0f;
    lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;
    //timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return ERROR_LOADING_GLAD;
    }

    //build and compile our shader program
    //------------------------------------
    Shader lightCubeShader("../shaders/cube.vert", "../shaders/cube.frag");
    Shader lightObjectShader("../shaders/light_object.vert", "../shaders/light_object.frag");
    //set up vertex data (and buffers) and config vertex attributes
    //-------------------------------------------------------------
    float vertices[] = {
        //front face
        //position        //color             //texture coords  //normal vector
        .5f, .5f, .5f,    1.0f, 0.5f, 0.2f,   1.0f, 1.0f,       0.0f, 0.0f, 1.0f,//top right
        .5f, -.5f, .5f,   0.2f, 1.0f, 0.5f,   1.0f, 0.0f,       0.0f, 0.0f, 1.0f,//bottom right
        -.5f, -.5f, .5f,  0.5f, 0.2f, 1.0f,   0.0f, 0.0f,       0.0f, 0.0f, 1.0f,//bottom left
        -.5f, .5f, .5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,       0.0f, 0.0f, 1.0f,//top left
        //back face
        -.5f, .5f, -.5f,    1.0f, 0.5f, 0.2f,   1.0f, 1.0f,     0.0f, 0.0f, -1.0f,//top right
        .5f, .5f, -.5f,   0.2f, 1.0f, 0.5f,   0.0f, 1.0f,       0.0f, 0.0f, -1.0f,//bottom right
        -.5f, -.5f, -.5f,  0.5f, 0.2f, 1.0f,   1.0f, 0.0f,      0.0f, 0.0f, -1.0f,//bottom left
        .5f, -.5f, -.5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,      0.0f, 0.0f, -1.0f,//top left
        //left face
        -.5f, .5f, .5f,    1.0f, 0.5f, 0.2f,   1.0f, 1.0f,      -1.0f, 0.0f, 0.0f,//top right
        -.5f, .5f, -.5f,   0.2f, 1.0f, 0.5f,   0.0f, 1.0f,      -1.0f, 0.0f, 0.0f,//bottom right
        -.5f, -.5f, -.5f,  0.5f, 0.2f, 1.0f,   0.0f, 0.0f,      -1.0f, 0.0f, 0.0f,//bottom left
        -.5f, -.5f, .5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,      -1.0f, 0.0f, 0.0f,//top left
        //right face
        .5f, .5f, .5f,    1.0f, 0.5f, 0.2f,   0.0f, 1.0f,       1.0f, 0.0f, 0.0f,//top right
        .5f, .5f, -.5f,   0.2f, 1.0f, 0.5f,   1.0f, 1.0f,       1.0f, 0.0f, 0.0f,//bottom right
        .5f, -.5f, -.5f,  0.5f, 0.2f, 1.0f,   1.0f, 0.0f,       1.0f, 0.0f, 0.0f,//bottom left
        .5f, -.5f, .5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,       1.0f, 0.0f, 0.0f,//top left
        //top face
        -.5f, .5f, -.5f,    1.0f, 0.5f, 0.2f,   0.0f, 1.0f,     0.0f, 1.0f, 0.0f,//top right
        .5f, .5f, -.5f,   0.2f, 1.0f, 0.5f,   1.0f, 1.0f,       0.0f, 1.0f, 0.0f,//bottom right
        .5f, .5f, .5f,  0.5f, 0.2f, 1.0f,   1.0f, 0.0f,         0.0f, 1.0f, 0.0f,//bottom left
        -.5f, .5f, .5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,       0.0f, 1.0f, 0.0f//top left
        //bottom face
        -.5f, -.5f, -.5f,    1.0f, 0.5f, 0.2f,   0.0f, 1.0f,    0.0f, -1.0f, 0.0f,//top right
        .5f, -.5f, -.5f,   0.2f, 1.0f, 0.5f,   1.0f, 1.0f,      0.0f, -1.0f, 0.0f,//bottom right
        .5f, -.5f, .5f,  0.5f, 0.2f, 1.0f,   1.0f, 0.0f,        0.0f, -1.0f, 0.0f,//bottom left
        -.5f, -.5f, .5f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f,      0.0f, -1.0f, 0.0f//top left
    };
    unsigned int indices [] = {//note that we start from 0
        0, 1, 3,//first triangle
        1, 2, 3,//second triangle
        4, 5, 6,
        5, 6, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        16, 17, 19,
        17, 18, 19,
        20, 21, 23,
        21, 22, 23
    };
    float lightobjvertices[] = {
        //position        
        .5f, .5f, .5f,    
        .5f, -.5f, .5f,   
        -.5f, -.5f, .5f,  
        -.5f, .5f, .5f,   
        //back face
        -.5f, .5f, -.5f,  
        .5f, .5f, -.5f,   
        -.5f, -.5f, -.5f, 
        .5f, -.5f, -.5f,  
        //left face
        -.5f, .5f, .5f,   
        -.5f, .5f, -.5f,  
        -.5f, -.5f, -.5f, 
        -.5f, -.5f, .5f,  
        //right face
        .5f, .5f, .5f,    
        .5f, .5f, -.5f,   
        .5f, -.5f, -.5f,  
        .5f, -.5f, .5f,   
        //top face
        -.5f, .5f, -.5f,  
        .5f, .5f, -.5f,   
        .5f, .5f, .5f,  
        -.5f, .5f, .5f, 
        //bottom face
        -.5f, -.5f, -.5f, 
        .5f, -.5f, -.5f,  
        .5f, -.5f, .5f, 
        -.5f, -.5f, .5f,  
    };
    unsigned int lightObjectIndices [] = {
        0, 1, 3,//first triangle
        1, 2, 3,//second triangle
        4, 5, 6,
        5, 6, 7,
        8, 9, 11,
        9, 10, 11,
        12, 13, 15,
        13, 14, 15,
        16, 17, 19,
        17, 18, 19,
        20, 21, 23,
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

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //color attribute
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    //texture coord attribute
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);
    //normal vector
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void *)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
    //note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    //remeber: do not unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO, keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    //VAO requires a call to glBindVertexArray anyways so we generally don't unbind VAOS (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    unsigned int lightObjectVAO, lightVBO, lightEBO;
    glGenVertexArrays(1, &lightObjectVAO);
    glBindVertexArray(lightObjectVAO);
    glGenBuffers(1, &lightVBO);
    glGenBuffers(1, &lightEBO);

    //We only need to bind to the VBO(to link it with glVertexAttribPointer), no need to fill it,the VBO's data alread contains all we need
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightobjvertices), lightobjvertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightObjectIndices), lightObjectIndices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //load and create a texture
    //-------------------------
    //uncomment this call to draw in wireframe polygons
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    // cube light
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
    //loop rendering
    while (!glfwWindowShouldClose(window))
    {
        fps();
        // std::cout << "fps:" << fps() << std::endl;
        //inpuglViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        processInput(window);
        //prerendering
        glClearColor(.1f, .1f, .1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //enable Z buffer
        glEnable(GL_DEPTH_TEST);
    
        //draw cube
        lightCubeShader.use();
        //either set it manually like so:
        glBindVertexArray(VAO);//seeing as we only hava 1 VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //view matrix
        unsigned int viewLoc = glGetUniformLocation(lightCubeShader.ID, "view");
        //projection matrix
        unsigned int projectionLoc = glGetUniformLocation(lightCubeShader.ID, "projection");
        glm::mat4 projection(1.0);
        projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
        // projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
        //camera view transformation
        glm::mat4 view = camera.getViewMatrix();
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setVec3("objectColor", objectColor);
        lightCubeShader.setVec3("lightColor", lightColor);
        lightCubeShader.setVec3("lightPos", lightObjectPosition);
        //change color as animation
        // combineBoxShader.setFloat("anim_time", (sin(glfwGetTime() / 2.0f) + 0.5f));
        // combineBoxShader.setFloat("mixValue", mixValue);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        for(unsigned int i = 0; i < 10; i++) {
            //model matrix
            unsigned int modelLoc = glGetUniformLocation(lightCubeShader.ID, "model");
            glm::mat4 model(1.0);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(-25.0f * (i+1) * static_cast<float>(sin(glfwGetTime() / 2.0f) + 0.5f)), glm::vec3(0.5f, 1.0f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        // draw light object    
        lightObjectShader.use();
        glBindVertexArray(lightObjectVAO);
        lightObjectShader.setMat4("projection", projection);
        lightObjectShader.setMat4("view", view);
        glm::mat4 lightObjectModel(1.0f);
        lightObjectModel = glm::translate(lightObjectModel, lightObjectPosition);
        lightObjectModel = glm::scale(lightObjectModel, glm::vec3(0.2f));
        lightObjectShader.setMat4("model", lightObjectModel);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);//no need to unbind it every time
        //rendering
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //optional:de-allocate all resources once they've outlived there purpose:
    //-----------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightObjectVAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &lightEBO);
    glDeleteProgram(lightCubeShader.ID);
    //glfw: terminate, clearing all previously allocated GLFW resources.
    //------------------------------------------------------------------
    glfwTerminate();
    return 0;
}