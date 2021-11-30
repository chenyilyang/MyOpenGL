#include "common.hpp"
#include "shaderutils.hpp"

#define ERROR_CREATE_WINDOW -1
#define ERROR_LOADING_GLAD -2
#define ERROR_OPENGL -3

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
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
    Shader combineBoxShader("../shaders/combinetexture.vert", "../shaders/combinetexture.frag");
    //set up vertex data (and buffers) and config vertex attributes
    //-------------------------------------------------------------
    float vertices[] = {
        //position          //color             //texture coords
        .5f, .5f, .0f,    1.0f, 0.5f, 0.2f,   1.0f, 1.0f,//top right
        .5f, -.5f, .0f,   0.2f, 1.0f, 0.5f,   1.0f, 0.0f,//bottom right
        -.5f, -.5f, .0f,  0.5f, 0.2f, 1.0f,   0.0f, 0.0f,//bottom left
        -.5f, .5f, .0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f//top left
    };
    unsigned int indices [] = {//note that we start from 0
        0, 1, 3,//first triangle
        1, 2, 3//second triangle
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //remeber: do not unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO, keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    //VAO requires a call to glBindVertexArray anyways so we generally don't unbind VAOS (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    //load and create a texture
    //-------------------------
    unsigned int textures[2];
    glGenTextures(2, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);//all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    //set the texture wrapping  parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image,create texture and generate mipmaps
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true);
    unsigned char * data = stbi_load("../resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, textures[1]);//all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    //set the texture wrapping  parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image,create texture and generate mipmaps
    data = stbi_load("../resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //uncomment this call to draw in wireframe polygons
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    
    //draw square
    combineBoxShader.use();
    //either set it manually like so:
    glUniform1i(glGetUniformLocation(combineBoxShader.ID, "texture1"), 0);
    //or set it via the texture class
    combineBoxShader.setInt("texture2", 1);
    //bind texture
    // glBindTexture(GL_TEXTURE_2D, textures[0]);
    //bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    
    glBindVertexArray(VAO);//seeing as we only hava 1 VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //loop rendering
    while (!glfwWindowShouldClose(window))
    {
        //input
        combineTextureProcessInput(window);
        //prerendering
        glClearColor(.2f, .3f, .4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //change color as animation
        unsigned int transformLoc = glGetUniformLocation(combineBoxShader.ID, "transform");
        glm::mat4 trans(1.0);
        trans = glm::rotate(trans, glm::radians(90.0f * static_cast<float>(sin(glfwGetTime() / 2.0f) + 0.5f)), glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        combineBoxShader.setFloat("anim_time", (sin(glfwGetTime() / 2.0f) + 0.5f));
        combineBoxShader.setFloat("mixValue", mixValue);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
    glDeleteProgram(combineBoxShader.ID);
    //glfw: terminate, clearing all previously allocated GLFW resources.
    //------------------------------------------------------------------
    glfwTerminate();
    return 0;
}