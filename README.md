# MyOpenGL examples on Linux Paltform

### Install Linux OpenGL desktop enviroment dependences:
```bash
sudo apt-get update
sudo apt-get install cmake pkg-config
sudo apt-get install mesa-utils libglu1-mesa-dev freeglut3-dev mesa-common-dev
sudo apt-get install libglew-dev libglfw3-dev libglm-dev
sudo apt-get install libao-dev libmpg123-dev
```
### Compile and Install GLFW Library
```bash
git clone https://github.com/glfw/glfw.git
cd glfw
mkdir build
cd build
cmake ..
make
sudo make install
```
### Copy Glad headers to /usr/include/
```bash
cd glad/include
sudo cp -R * /usr/include/
```
### Compile and run your example
```bash
g++ ./src/triangle.cpp ./glad/glad.c -I./include -ldl -lglfw -o triangle
./triangle
```