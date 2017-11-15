#ifndef GLCH_CPP
#define GLCH_CPP

#include "../include/glch.h"
#include <fstream>

void fb_size_callback(GLFWwindow * window, int width, int height);

Glch::Glch( int width, int height, char * title, int objectNum):vao(objectNum)
{
    _width = width;
    _height = height;
    _title = title;
    _start = nullptr;
    _end = nullptr;
    mainLoop = nullptr;
    // Init glfw
    glfwInit();
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Create a window
    _window = glfwCreateWindow( _width,_height, _title, nullptr, nullptr);
    if (_window == nullptr)
    {
        std::cout << "failed to create window " << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent( _window);

    // Load glad
    if (!gladLoadGLLoader( ( GLADloadproc) glfwGetProcAddress) )
    {
        std::cout << "failed to init GLAD" << std::endl;
        glfwTerminate();
    }

    // Config view and sizecallback
    glViewport( 0, 0, _width, _height);
    glfwSetFramebufferSizeCallback(_window, fb_size_callback);
}

// Deal with ending
Glch::~Glch()
{
    object * now = _start;
    object * next;
    if (mainLoop != nullptr && mainLoop ->joinable())
        mainLoop ->join();
    for ( auto a : _shader)
        a.~Shader();
    vao.~glVAO();
    
    while (now != nullptr)
    {
        next = now -> next;
        delete now;
        now = next;
    }
    glfwTerminate();
}

// Return window
GLFWwindow * Glch::win()
{
    return _window;
}

// Create shader program
int Glch::addPro( const char *vsp, const char *fsp )
{
    _shader.push_back(Shader(vsp, fsp));
    return _shader.size() - 1;
}

// Using this shader program
void Glch::usePro(int n )
{
    _shader[n].use();
}

// Return shader program id
unsigned int Glch::id(int n)
{
    return _shader[n].id();
}

// Window size changing call back
void fb_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
    window = nullptr;
}

// Create OpenGL object
void Glch::addVAO( char * dataPath)
{
    vao.add(dataPath);
}

// Bind OpenGL object
void Glch::useVAO(int index, bool flag)
{
    vao.bind(index,flag);
}

void Glch::addObj(void (* draw)(), int prgIndex, int vaoIndex)
{
    if (_start == nullptr)
    {
    std::cout << __LINE__ << std::endl;
        _end = _start = new object;
    }else{
    std::cout << __LINE__ << std::endl;
        _end->next = new object;
        _end = _end->next;
    }
    _end->draw = draw;
    _end->prgIndex = prgIndex;
    _end->vaoIndex = vaoIndex;
}

void drawObj(Glch &t)
{
    while ( glfwWindowShouldClose(t._window))
    {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        object * now = t._start;
        while (now != nullptr)
        {
            t.usePro(now -> prgIndex);
            t.useVAO(now -> vaoIndex,true);
            (now -> draw)();
            now = now -> next;
        }
        glfwSwapBuffers(t._window);
        glfwPollEvents();
    }
}

void Glch::startMainLoop()
{
    mainLoop = new std::thread(drawObj,std::ref(*this));
}


#endif
