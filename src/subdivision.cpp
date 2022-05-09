#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

using namespace std;
using namespace glm;


vec3 center_point(vec3 p1, vec3 p2)
{
    vec3 cp;

    for (int i = 0; i < 3;i++)
    {
        cp[i] = ((p1[i] + p1[i])/2);
    }

    return cp;
}

vec3 sum_point(vec3 p1, vec3 p2)
{
    vec3 sp;

    for (int i = 0; i < 3;i++)
    {
        sp[i] = (p1[i] + p2[i]);
    }

    return sp;
}

vec3 mul_point(vec3 p, int m)
{
    vec3 sp;
    
    for (int i=0; i < 3;i++)
    {
        sp[i]=(p[i] * m);
    }
        
    return sp;

}

vec3 div_point(vec3 p, int d)
{
    vec3 sp;

    for (int i = 0; i < 3;i++)
    {
        sp[i] = (p[i] /d);
    }
}
        
//for each face, a face point is created which is the average of all the points of the face
vector<vec3> get_face_points(vector<vec3> input_points, vector<vector<int>> input_faces)
{
    
    int NUM_DIMENSIONS = 3;
    // face_points will have one point for each face
    vector<vec3> face_points;

    for (vector<int> curr_face : input_faces)
    {
        vec3 face_point = vec3(0.0, 0.0, 0.0);
        for (int curr_point_index : curr_face)
        {
            vec3 curr_point = input_points[curr_point_index];
            // add curr_point to face_point
            // will divide later
            for (int i = 0; i < 3;i++)
            {
                face_points[i] += curr_point[i];
            }
        }

        // divide by number of points for average
        int num_points = curr_face.size();

        for (int i = 0; i < NUM_DIMENSIONS;i++)
        {
            face_point[i] /= num_points;
        }

        face_points.push_back(face_point);

        return face_points;
    }
}

//Get list of edgesand the one or two adjacent faces in a list.
//also get center point of edge

//Each edge would be[pointnum_1, pointnum_2, facenum_1, facenum_2, center]
vector<vector<int>> get_edges_faces(vector<vec3> input_points, vector<vector<int>> input_faces)
{
    vector<vector<int>> edges;
    for (int facenum =0; facenum < input_faces.size();facenum++)
    {
        vector<int> face = input_faces[facenum];
        int num_points = face.size();

        for (int pointindex = 0; pointindex < num_points;pointindex)
        {
            int pointnum_1;
            int pointnum_2;
            int temp;
            vector<int> edge(3);
            if (pointindex < num_points - 1)
            {
                pointnum_1 = face[pointindex];
                pointnum_2 = face[pointindex + 1];
            }

            else
            {
                pointnum_1 = face[pointindex];
                pointnum_2 = face[0];
            }

            if (pointnum_1 > pointnum_2)
            {
                temp = pointnum_1;
                pointnum_1 = pointnum_2;
                pointnum_2 = temp;
            }
            
            edge[0] = pointnum_1;
            edge[1] = pointnum_2;
            edge[2] = facenum;
            edges.push_back(edge); 
        }

        sort(edges.begin(), edges.end());
    }


}
