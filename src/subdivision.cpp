#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <string>
#include <tuple>
#include <map>

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
tuple<vector<vector<int>>, vector<vec3>> get_edges_faces(vector<vec3> input_points, vector<vector<int>> input_faces)
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
    }
    

    // merge edges with 2 adjacent faces
    sort(edges.begin(), edges.end());

    // merge edges with 2adjacent faces
    //[pointnum_1,pointnum_2,facenum_1,facenum_2] or
    //[pointnum_1,pointnum_2,facenum_1,None]

    int num_edges = edges.size();
    int eindex = 0;
    vector<vector<int>> merged_edges;

    while (eindex < num_edges)
    {
        vector<int> e1 = edges[eindex];
        vector<int>temp;
        //check if it is last edge 
        if (eindex < num_edges - 1)
        {
            vector<int>e2 = edges[eindex + 1];

            if (e1[0] == e2[0] and e1[1] == e2[1])
            {
                temp.insert(temp.end(), { e1[0], e1[1], e1[2], e2[2] });
                merged_edges.push_back(temp);
                eindex += 2;
                temp.clear();
            }

            else
            {
                temp.insert(temp.end(), { e1[0], e1[1], e1[2] });
                merged_edges.push_back(temp);
                eindex += 1;
                temp.clear();
            }
        }

        else
        {
            temp.insert(temp.end(), { e1[0], e1[1], e1[2] });
            merged_edges.push_back(temp);
            eindex += 1;
            temp.clear();
        }
    }

        //add edge centers

    vector<vec3> edges_centers;

    for(vector<int> me : edges)
    {
        
        vec3 p1 = input_points[me[0]];
        vec3 p2 = input_points[me[1]];
        vec3 cp = center_point(p1, p2);
    }

    tuple< vector<vector<int>>, vector<vec3>> result = { edges, edges_centers };
    
    return result;
}


//for each edge, an edge point is created which is the average
//between the center of the edge and the center of the segment made
//with the face points of the two adjacent faces.

vector<vec3> get_edge_points(vector<vec3>input_points, vector <vector<int>> edges_faces, vector<vec3> edges_centers, vector<vec3>face_points)
{
    vector<vec3> edge_points;

    for (int i =0; i< edges_faces.size();i++)
    {
       
        // get center of edge
        vec3 cp = edges_centers[i];

        // get center of two facepoints
        vec3 fp1 = face_points[edges_faces[i][2]];

        // if not two faces just use one facepoint
        // should not happen for solid like a cube

        vec3 fp2;

        if (edges_faces[i].size() == 3)
        {
            fp2 = fp1;
        }

        else
        {
            fp2 = face_points[edges_faces[i][3]];
        }

        vec3 cfp = center_point(fp1, fp2);

        // get average between center of edge and center of facepoints

        vec3 edge_point = center_point(cp, cfp);
        edge_points.push_back(edge_point);
    }

    return edge_points;

}


vector<vec3> get_avg_face_points(vector<vec3>input_points, vector<vector<int>> input_faces, vector<vec3>face_points)
{
    // for each point calculate
    //the average of the face points of the faces the point belongs to(avg_face_points)
    //create a list of lists of two numbers[facepoint_sum, num_points] by going through the
    //points in all the faces.
    //then create the avg_face_points list of point by dividing point_sum(x, y, z) by num_points


    int number_points = input_points.size();

    vector<vec3> temp_points;
    vector<int> temp_points_num;

    // initialize 
    for (int i = 0;i < number_points;i++)
    {
        temp_points.push_back({ 0.0,0.0,0.0 });
        temp_points_num.push_back(0);
    }

    // loop through faces updating temp_points

    for (int facenum = 0; facenum < input_faces.size();facenum++)
    {
        vec3 fp = face_points[facenum];
        for (int pointnum : input_faces[facenum])
        {
            temp_points[facenum] = sum_point(temp_points[facenum], fp);
            temp_points_num[facenum] += 1;
        }
    }

    // count how many faces the point is belong to, and sum all the point, next step we caculate the average
    // divide to create avg_face_points

    vector<vec3> avg_face_points;

    for (int i=0;i< temp_points.size();i++)
    {
        vec3 afp = div_point(temp_points[i], temp_points_num[i]);
        avg_face_points.push_back(afp);
    }

    return avg_face_points;
}


vector<vec3> get_avg_mid_edges(vector<vec3>input_points, vector <vector<int>> edges_faces, vector<vec3>edges_centers)
{
    //the average of the centers of edges the point belongs to (avg_mid_edges)
    //create list with entry for each point
    //each entry has two elements. one is a point that is the sum of the centers of the edges
    //and the other is the number of edges. after going through all edges divide by
    //number of edges.


    // initialize list
    int num_points = input_points.size();

    vector<vec3> temp_points;
    vector<int> temp_points_num;

    for (int i = 0; i < num_points;i++)
    {
        temp_points.push_back({ 0.0,0.0,0.0 });
        temp_points_num.push_back(0);
    }

    // go through edges_faces using center updating each point

    for (int i = 0;i < edges_faces.size();i++)
    {
        vec3 cp = edges_centers[i];
        vec3 tp = temp_points[edges_faces[i][0]];

        temp_points[edges_faces[i][0]] = sum_point(tp, cp);
        temp_points_num[i] += 1;

        tp = temp_points[edges_faces[i][1]];
        temp_points[edges_faces[i][0]] = sum_point(tp, temp_points[edges_faces[i][0]]);
        temp_points_num[i] += 1;
    }

    // divide out number of points to get average

    vector<vec3> avg_mid_edges;

    for (int i = 1; i < temp_points_num.size();i++)
    {
        vec3 ame = div_point(temp_points[i], temp_points_num[i]);
        avg_mid_edges.push_back(ame);
    }

    return avg_mid_edges;
}

vector<int> get_points_faces(vector<vec3> input_points, vector<vector<int>> input_faces)
{
    int num_points = input_points.size();

    vector<int> points_faces;

    for (int i = 0; i < num_points;i++)
    {
        points_faces.push_back(0);
    }

    // loop through faces updating points_faces

    for (int facenum = 0;facenum < input_faces.size();facenum++)
    {
        for (int pointnum : input_faces[facenum])
        {
            points_faces[pointnum] += 1;
        }
    }

    // counts how many faces each point is belong to

    return points_faces;

}


vector<vec3> get_new_points(vector<vec3> input_points, vector<int> points_faces, vector<vec3> avg_face_points, vector<vec3> avg_mid_edges)
{
    vector<vec3> new_points;

for (int pointnum = 0; pointnum < input_points.size();pointnum++)
{
    int n = points_faces[pointnum];
    int m1 = (n - 3.0) / n;
    int m2 = 1.0 / n;
    int m3 = 2.0 / n;
    vec3 old_coords = input_points[pointnum];
    vec3 p1 = mul_point(old_coords, m1);
    vec3 afp = avg_face_points[pointnum];
    vec3 p2 = mul_point(afp, m2);
    vec3 ame = avg_mid_edges[pointnum];
    vec3 p3 = mul_point(ame, m3);
    vec3 p4 = sum_point(p1, p2);
    vec3 new_coords = sum_point(p4, p3);

    new_points.push_back(new_coords);
}

return new_points;
}

tuple<int, int> switch_nums(tuple<int, int> point_nums)
{
    // return tuple of point numbers
    // sorted least to most

    if (get<0>(point_nums) < get<1>(point_nums))
    {
        return point_nums;
    }

    else
    {
        tuple<int, int> result = { get<1>(point_nums),get<0>(point_nums) };
        return result;
    }
}

tuple<vector<vec3>, vector<vector<int>>> cmc_subdiv(vector<vec3>input_points, vector <vector<int>> input_faces)
{
    vector<vec3>face_points = get_face_points(input_points, input_faces);

    vector<vector<int>> edges_faces = get <0>(get_edges_faces(input_points, input_faces));
    vector<vec3> edges_centers = get <1>(get_edges_faces(input_points, input_faces));

    vector<vec3>edge_points = get_edge_points(input_points, edges_faces, edges_centers, face_points);

    vector<vec3> avg_face_points = get_avg_face_points(input_points, input_faces, face_points);

    vector<vec3>avg_mid_edges = get_avg_mid_edges(input_points, edges_faces, edges_centers);

    vector<int>points_faces = get_points_faces(input_points, input_faces);

    vector<vec3>new_points = get_new_points(input_points, points_faces, avg_face_points, avg_mid_edges);

    // add face points to new_points

    vector<int>face_point_nums;

    // point num after next append to new_points

    int next_pointnum = new_points.size();

    for (vec3 face_point : face_points)
    {
        new_points.push_back(face_point);
        face_point_nums.push_back(next_pointnum);
        next_pointnum += 1;
    }

    // add edge points to new_points

    map<tuple<int, int>, int> edge_point_nums;

    for (int edgenum = 0; edgenum < edges_faces.size();edgenum++)
    {
        int pointnum_1 = edges_faces[edgenum][0];
        int pointnum_2 = edges_faces[edgenum][1];
        vec3 edge_point = edge_points[edgenum];

        new_points.push_back(edge_point);
        map<tuple<int, int>, int> edge_point_nums;

        edge_point_nums[{pointnum_1, pointnum_2}] = next_pointnum;
        next_pointnum += 1;
    }



    // new_points now has the points to output. Need new faces

    vector<vector<int>> new_faces;

    for (int oldfacenum = 0;oldfacenum < input_faces.size();)
    {
        vector<int> oldface = input_faces[oldfacenum];

        //for 4 point face
        if (oldface.size() == 4)
        {
            int a = oldface[0];
            int b = oldface[1];
            int c = oldface[2];
            int d = oldface[3];


            int face_point_abcd = face_point_nums[oldfacenum];

            int edge_point_ab = edge_point_nums[switch_nums({a,b})];
            int edge_point_da = edge_point_nums[switch_nums({d,a})];
            int edge_point_bc = edge_point_nums[switch_nums({b,c})];
            int edge_point_cd = edge_point_nums[switch_nums({c,d})];

            new_faces.push_back({ a, edge_point_ab, face_point_abcd, edge_point_da });
            new_faces.push_back({ b, edge_point_bc, face_point_abcd, edge_point_ab });
            new_faces.push_back({ c, edge_point_cd, face_point_abcd, edge_point_bc });
            new_faces.push_back({ d, edge_point_da, face_point_abcd, edge_point_cd }); 
        }
    }

    tuple<vector<vec3>, vector<vector<int>>> result = { new_points,new_faces };

    return result;
}