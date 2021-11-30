#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

int main() {
    glm::vec4 v1(1.0f, 0.0f, 0.0f, 1.0f);
    //if you use glm v0.9.9.9 or above, you should initial the matrix with value 1
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
    v1 = trans * v1;
    std::cout << v1.x << " " << v1.y << " " << v1.z <<std::endl;
}