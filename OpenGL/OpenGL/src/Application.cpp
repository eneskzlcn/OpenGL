#include <GL/glew.h> // include this lib, before include the other opengl sources to not take an error.
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

// ERROR HANLING AND DEBUGGING
#define ASSERT(x) if(!(x)) __debugbreak();

#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__ , __LINE__))

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall(const char* function,const char * file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error!] (" << error << "): "<< function <<
            " " << file << ": " << line  << std::endl;
        return false;
    }
    return true;
}

//this function takes a source and type as argument and compiles a shader from this source by given type.
struct ShaderSource
{
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSource ParseShader(const std::string& filepath)
{
    std::fstream stream(filepath);

    std::string line;

    std::stringstream ss[2];

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT =1
    };
    ShaderType currentShader = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                currentShader = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                currentShader = ShaderType::FRAGMENT;
                
            }
        }
        else
        {
            ss[(int)currentShader] << line << '\n';
        }
    }
    return { ss[0].str(),ss[1].str() };
}
static unsigned int CompileShader(const std::string& source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr); // add source code to tha shader source
    glCompileShader(id); // compile sourced shader program


    /* debugging and error checking stuffs.*/
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(sizeof(char) * length);//alloca gives a memory from stack dynamically :))
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compiled" << ((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    /* debugging and error checking stuffs finished.*/
    return id;
}

static unsigned int CreateShader(const std::string& vertexSource, const std::string& fragmentSource)
{
    unsigned int program = glCreateProgram(); // request a free program from gpu returns the id of the program

    unsigned int vs = CompileShader(vertexSource, GL_VERTEX_SHADER);  //returns compiled shader programs id
    unsigned int fs = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs); // attach shaders to free program
    glAttachShader(program, fs);

    glLinkProgram(program); // link the objects of program

    glValidateProgram(program); // validate the program for gpu

    glDeleteShader(vs); // delete intermediate files...
    glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window); // we must call glewInit() after a valid opengl context created << what this func. provides.

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Eroooor!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[]{
        -0.5f,-0.5f, // 0th index for square
        -0.5f, 0.5f, // 1th index for square
         0.5f, 0.5f, // 2th index for square
         0.5f,-0.5f, // 3th index for square
    };
    
    unsigned int indices[]
    {
         0, 1 , 2, // needed position on indices for first triangle to draw square
         2, 3, 0 // needed position on indices for second triangle to draw square
    };
    
    unsigned int vao;
    GLCALL(glGenVertexArrays(1, &vao));
    GLCALL(glBindVertexArray(vao));
    //adding positions as attiribute or sth.
    unsigned int buffer;
    GLCALL(glGenBuffers(1, &buffer)); // tell the gpu , generate a buffer with id of 1 .
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCALL(glBufferData(GL_ARRAY_BUFFER, 4 *2 * sizeof(float), positions, GL_STATIC_DRAW)); // look at to docs.gl adress to take more info.
    GLCALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); // detailly told in below.
    GLCALL(glEnableVertexAttribArray(0)); // we enable the vertex attribute. we need to do that.

    unsigned int ibo; //index buffer object
    GLCALL(glGenBuffers(1, &ibo)); // tell the gpu , generate a buffer with id of 1 .
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * sizeof(unsigned int), indices, GL_STATIC_DRAW)); // look at to docs.gl adress to take more info.

    //glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, sizeof(float) * 2, 0);  Expression of this func. below -->
    // detailly  told in http://docs.gl/gl4/glVertexAttribPointer
    /*first parameter : index--> which attribute gonna modify.We have just three vertex positions JUST position attribute
        so, we just in the 0th atribute.
    */

    /* second parameter : size --> it is just how many elements each component has.An ex. for vertex position attribute
           there is two float for x and y there. So this parameter gonna be 2.
    */

    /*
    *  third parameter : type --> the type of the data we gonna use. In here it is float. So the parameter wil be GL_FLOAT
    */
    /*
        fourth parameter: is normalized--> if that is true, it says gpu hey normalize this values (convert them to 0...1 range)    if not, there will be no normalization. In here we already have floats which have normalized so it gonna be GL_FALSE
    */
    /*
        fifth parameter: stride --> that is the distance between each vertex in size of bytes.
         example there is a vertex has 3 attribiute pos (floatx, float y) , normal( float x, float y , float z), texture_coord(float x, float y) ...then the other vertex attributes starts... In this ex. stride will be sizeof(pos)+ sizeof(normal)+sizeof(texture_coord) which is sizeof(float) * 7 --> 28 is the stride. Stride says the other vertex attrib. will be start after 28 bytes which is the same with each vertex is 28 bytes.
            In the example above is there is 3 vertex and each one just have one attribute which is vertex_position that has
            2 floats values. So the distance between vertices is sizeof(float) *2 --> 8;
    */
    /*
        sixth parameter:poiner --> that is mean where the attribute is starts in bytes. Show in example
            thought that we have the array positions[8] ={ (pos) 0.5f,0.5f,   (texture) 0.5f, 0.5f ,   (2nd vertex pos) 0.5f,0.5f ...). if we modify the texture attiribute start from first vertex, the pointer gonna be 0 + sizeof(pos)-->
                which is 8. But in our real example above we working on position attribute which is we just have so starting from 0
    */

    
    
    //writing and adding shader
    
    ShaderSource source = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource); // create shader from a string shader sources..
    
    /* Loop until the user closes the window */
    GLCALL(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);
    float r = 0.2f;
    float increment = 0.05f;

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

     
        GLCALL(glUseProgram(shader)); // tell gpu use this program as shader.
        //for animated color, do not forget that uniforms are works per draw. so before every draw you need to init them and then draw
        GLCALL(glUniform4f(location, r, 0.2, 0.3, 0.5));
        GLCALL(glBindVertexArray(vao));
        GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
         //  //2th param --> count of indices to be rendered.
         // 
         // error handled draw
        GLCALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
        // for animated colors.
        if (r >= 1.0f)
            increment = -0.05f;
        else if (r <= 0.0f)
            increment = 0.05f;
        
        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader); // delete shader from gpu after you finish your job.

    glfwTerminate();
    return 0;
}