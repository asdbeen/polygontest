#include <stdafx.h>
#include <subdivision.h>
#include <random>


using namespace std;
using namespace glm;



vector<vec3> points{ {-0.5f, -0.5f,0},{-0.5f, 0.5f,0},{0.5f, 0.5f,0},{0.5f, -0.5f,0} };
vector <vector<int>> faces{ {0,1,2,3} };





//suddvision control
//vector<vec3> points{ {-0.5f, 0,0},{-0.25, 0.25,0},{0.25, 0.25,0},{0.5f, 0,0}, {0.25f,-0.25f,0} ,{-0.25,-0.25,0} };
//vector <vector<int>> faces{ {0,1,2,3,4,5} };



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        tuple<vector<vec3>, vector<vector<int>>>result = cmc_subdiv(points, faces);
        points = get<0>(result);
        faces = get<1>(result);
        cout << "Sudivision" << endl;

        for (int i = 0; i < faces.size();i++)
        {
            for (int j = 0; j < 4;j++)
            {
                cout << faces[i][j];
                cout << ",";
            }

            cout <<" " << endl;
        }

       
    }
}


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



   

   

    double num;
    double down = 0.0, top = 2.4;
    double a[3];
   


 

    /* Loop until the user closes the window */
   /* glfwSetKeyCallback(window, key_callback);*/

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        for (int facenum = 0;facenum < faces.size();facenum++)
        {
            srand((signed int)facenum);
            for (int i = 0;i < 3;i++)
            {
                num = (double)rand() / RAND_MAX;
                a[i] = num;
            }

            glBegin(GL_POLYGON);   

            glColor3f(a[0], a[1], a[2]);

            for (int j = 0;j < faces[facenum].size();j++)
            {
                glVertex2f(points[faces[facenum][j]][0], points[faces[facenum][j]][1]);
            }
            

            glEnd();

            
        }
        glfwSetKeyCallback(window, key_callback);







        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


//int main(void)
//{
//	cout << "a";
//
//
//	vector<vec3> points{ {-0.5f, -0.5f,0},{-0.5f, 0.5f,0},{0.5f, 0.5f,0},{0.5f, -0.5f,0} };
//	vector <vector<int>> faces{ {0,1,2,3} };
//
//	cout << "a";
//
//	tuple<vector<vec3>, vector<vector<int>>>  result = cmc_subdiv(points, faces);
//
//	vector<vec3> pointsA = get<0>(result);
//
//	
//	cout << "a";
//	cout << "B";
//
//}