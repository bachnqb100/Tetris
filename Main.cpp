#include <iostream>
#include <cmath>
#include <vector>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


#include "Display.h"
#include "Shader.h"
#include "IO.h"
#include "Tetris.h"

using namespace std;


const int DELAY_TIME_INPUT = 100;


Tetris t;
static int counter = 0;

string mvs =
"#version 330 core                       \n"
"layout(location = 0) in vec3 aPos;      \n"
"layout(location = 1) in vec3 aColor;    \n"
"                                        \n"
"out vec3 ourColor;                      \n"
"                                        \n"
"void main()                             \n"
"{                                       \n"
"    gl_Position = vec4(aPos, 1.0);      \n"
"    ourColor = aColor;                  \n"
"}                                       \n";

string mfs =
"#version 330 core                      \n"
"out vec4 FragColor;                    \n"
"                                       \n"
"in vec3 ourColor;                      \n"
"                                       \n"
"void main()                            \n"
"{                                      \n"
"    FragColor = vec4(ourColor, 1.0f);  \n"
"}                                      \n";

inline int vindex(int row, int column) {
    return row * COL + column;
}

void define_vertices(vector<float>& vertices) {

    for (int i = 0; i < 4 * ROW * COL; i++) {
        int indeks = 6 * i;

        //vertices[indeks + 3] = 0.5 + (1 + std::sin(i / 10)) / 2;//red
        //vertices[indeks + 4] = 0.5 + (1 + std::sin(i / 10 + 45)) / 2;//green
        //vertices[indeks + 5] = 0.5 + (1 + std::sin(i / 10 + 90)) / 2;//blue
        
        vertices[indeks + 3] = 0.8;//red
        vertices[indeks + 4] = 0.5;//green
        vertices[indeks + 5] = 0.2;//blue

    }

    float x_scale = 2.0 / 5.0;
    float y_scale = 0.9;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            int indeks = (i * COL + j) * 4 * 6;

            vertices[indeks + 2] = 0; //z
            vertices[indeks + 1] = (-1 + 0.1 * i + 0.01) * y_scale; //y
            vertices[indeks] = (-1 + 0.2 * j + 0.01) * x_scale; //x

        }
    }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            int indeks = ((i * COL + j) * 4 + 1) * 6;

            vertices[indeks + 2] = 0; //z
            vertices[indeks + 1] = (-1 + 0.1 * i + 0.01) * y_scale; //y
            vertices[indeks] = (-1 + 0.2 * j + 0.2 - 0.01) * x_scale; //x

        }
    }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            int indeks = ((i * COL + j) * 4 + 2) * 6;

            vertices[indeks + 2] = 0; //z
            vertices[indeks + 1] = (-1 + 0.1 * (i + 1) - 0.01) * y_scale; //y
            vertices[indeks] = (-1 + 0.2 * j + 0.2 - 0.01) * x_scale; //x

        }
    }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            int indeks = ((i * COL + j) * 4 + 3) * 6;

            vertices[indeks + 2] = 0; //z
            vertices[indeks + 1] = (-1 + 0.1 * (i + 1) - 0.01) * y_scale; //y
            vertices[indeks] = (-1 + 0.2 * j + 0.01) * x_scale; //x

        }
    }


}


void define_vertices_table(vector<float>& vertices) {

    //for (int i = 0; i < 4 * 200; i++) {
    //    int indeks = 6 * i;

    //    vertices[indeks + 3] = 0.5 + (1 + std::sin(i / 10)) / 4;//red
    //    vertices[indeks + 4] = 0.5 + (1 + std::sin(i / 10 + 45)) / 4;//green
    //    vertices[indeks + 5] = 0.5 + (1 + std::sin(i / 10 + 90)) / 4;//blue

    //}

    float x_scale = 2.0 / 5.0;
    float y_scale = 0.9;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            int indeks = (i * COL + j) * 4 * 6;

            vertices[indeks + 2] = 0; //z
            vertices[indeks + 1] = (-1 + 0.1 * i + 0.01) * y_scale; //y
            vertices[indeks] = (-1 + 0.2 * j + 0.01) * x_scale; //x

        }
    }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            int indeks = ((i * COL + j) * 4 + 1) * 6;

            vertices[indeks + 2] = 0; //z
            vertices[indeks + 1] = (-1 + 0.1 * i + 0.01) * y_scale; //y
            vertices[indeks] = (-1 + 0.2 * j + 0.2 - 0.01) * x_scale; //x

        }
    }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            int indeks = ((i * COL + j) * 4 + 2) * 6;

            vertices[indeks + 2] = 0; //z
            vertices[indeks + 1] = (-1 + 0.1 * (i + 1) - 0.01) * y_scale; //y
            vertices[indeks] = (-1 + 0.2 * j + 0.2 - 0.01) * x_scale; //x

        }
    }
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {

            int indeks = ((i * COL + j) * 4 + 3) * 6;

            vertices[indeks + 2] = 0; //z
            vertices[indeks + 1] = (-1 + 0.1 * (i + 1) - 0.01) * y_scale; //y
            vertices[indeks] = (-1 + 0.2 * j + 0.01) * x_scale; //x

        }
    }


}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {       
        if (t.to_left()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_TIME_INPUT));
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (t.to_right()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_TIME_INPUT));
        }
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_TIME_INPUT));
        t.turn_clockwise();
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY_TIME_INPUT));
        counter = 0;
        t.to_bottom();
    }
}


int main()
{
    Display d(800, 800, "OpenGL TETRIS");


    // build and compile our shader program
    // ------------------------------------
    Shader ourShader(mvs, mfs); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    vector<float> vertices(4 * ROW * COL * 6);
    define_vertices(vertices);  
    



    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    //glLineWidth(2);


    //table base

    vector<float> vertices_table(4 * ROW * COL * 6);
    define_vertices_table(vertices_table);

    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, vertices_table.size() * sizeof(vertices_table[0]), vertices_table.data(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    //glLineWidth(2);






    // render the triangle
    ourShader.use();


    while (!d.should_window_close()) {
        processInput(d.get_window());

        glClearColor(0.72f, 0.72f, 0.72, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glBindVertexArray(VAO1);
        auto v_temp = t.print_game_table();
        for (int i = 0; i < v_temp.size(); i++)
            for (int j = 0; j < v_temp[i].size(); j++)
                //if (v_temp[i][j] == 1)
                    glDrawArrays(GL_LINE_LOOP, (ROW * COL - 1 - vindex(i, j)) * 4, 4);


        glBindVertexArray(VAO);
        auto v_temp1 = t.print_game_table();
        for (int i = 0; i < v_temp1.size(); i++)
            for (int j = 0; j < v_temp1[i].size(); j++)
                if (v_temp1[i][j] == 1) {
                    glDrawArrays(GL_TRIANGLE_FAN, (ROW * COL - 1 - vindex(i, j)) * 4, 4);
                }



        if (counter == 10) {
            counter = 0;
            t.to_bottom();
        }
        counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        d.Update();
    }


    return 0;
}