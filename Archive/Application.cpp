#include <stdafx.h>
#include <subdivision.h>
#include <random>

using namespace std;
using namespace glm;

vector<vec3> points{ {-0.5f, -0.5f,0},{-0.5f, 0.5f,0},{0.5f, 0.5f,0},{0.5f, -0.5f,0} };
vector <vector<int>> faces{ {0,1,2,3} };


//suddvision control


//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    if (key == GLFW_KEY_C && action == GLFW_PRESS)
//    {
//        glClear(GL_COLOR_BUFFER_BIT);
//        tuple<vector<vec3>, vector<vector<int>>> result = cmc_subdiv(points, faces);
//        
//        vector<vec3> points = get<0>(result);
//        vector <vector<int>> faces = get<1>(result);
//
//        cout << "subdivision!" << endl;
//    }
//}

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
    glfwMakeContextCurrent(window);

    

    
    tuple<vector<vec3>, vector<vector<int>>> result = cmc_subdiv(points, faces);

    vector<vec3>pointsA = get<0>(result);
    vector <vector<int>>facesA = get<1>(result);

    /* Loop until the user closes the window */
   /* glfwSetKeyCallback(window, key_callback);*/

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        for (int facenum = 0;facenum < faces.size();facenum++)
        {
            glBegin(GL_POLYGON);

            float r = 0.2;

            glColor3f(r, r, r);

            glVertex2f(points[faces[facenum][0]][0], points[faces[facenum][0]][1]);
            
            glVertex2f(points[faces[facenum][1]][0], points[faces[facenum][1]][1]);

            glVertex2f(points[faces[facenum][2]][0], points[faces[facenum][2]][1]);

            glVertex2f(points[faces[facenum][3]][0], points[faces[facenum][3]][1]);
        
            glEnd();
            
            r += 0.5;
        }

        

        

       

       
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}