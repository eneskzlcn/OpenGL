#include <GL/glew.h> // include this lib, before include the other opengl sources to not take an error.
#include <GLFW/glfw3.h>
#include <iostream>
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window); // we must call glewInit() after a valid opengl context created << what this func. provides.

    if (glewInit() != GLEW_OK)
        std::cout << "Eroooor!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6]{
        -0.5f,-0.5f, // 1th vertex pos,
         0.0f, 0.5f, // 2th vertex pos,
         0.5f,-0.5f, // 3th vertex pos
    };
    unsigned int buffer;
    glGenBuffers(1, &buffer); // tell the gpu , generate a buffer with id of 1 .
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW) // look at to docs.gl adress to take more info.

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE, sizeof(float) * 2, 0); 
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

    glEnableVertexAttribArray(0); // we enable the vertex attribute. we need to do that.
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3); // start from 0th element and read 3 element. Thought like
                                          // Every ordered position couple is a one vertex position element.
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}