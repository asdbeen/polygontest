#include <stdafx.h>


using namespace std;
using namespace glm;



    vec3 center_point(vec3 p1, vec3 p2)
    {
        vec3 cp;

        for (int i = 0; i < 3;i++)
        {
            cp[i] = ((p1[i] + p2[i]) / 2);
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

        for (int i = 0; i < 3;i++)
        {
            sp[i] = (p[i] * m);
        }

        return sp;

    }

    vec3 div_point(vec3 p, int d)
    {
        vec3 sp;

        for (int i = 0; i < 3;i++)
        {
            sp[i] = (p[i] / d);
        }

        return sp;
    }

    //for each face, a face point is created which is the average of all the points of the face
    vector<vec3> get_face_points(vector<vec3> input_points, vector<vector<int>> input_faces)
    {

        int NUM_DIMENSIONS = 3;
        // face_points will have one point for each face
        vector<vec3> face_points;

        for (int num=0; num <input_faces.size(); num++)
        {
            vec3 face_point{ 0.0, 0.0, 0.0 };
            for (int index=0; index< input_faces[num].size();index++)
            {
                vec3 curr_point = input_points[input_faces[num][index]];
                // add curr_point to face_point
                // will divide later
                for (int i = 0; i < 3;i++)
                {
                    face_point[i] += curr_point[i];
                }
            }

            // divide by number of points for average
            int num_points = input_faces[num].size();

            for (int i = 0; i < NUM_DIMENSIONS;i++)
            {
                face_point[i] /= num_points;
            }

            face_points.push_back(face_point);
        }
        return face_points;
    }

    //Get list of edgesand the one or two adjacent faces in a list.
    //also get center point of edge
    //Each edge would be[pointnum_1, pointnum_2, facenum_1, facenum_2, center]
    tuple<vector<vector<int>>, vector<vec3>> get_edges_faces(vector<vec3> input_points, vector<vector<int>> input_faces)
    {
        vector<vector<int>> edges;
        for (int facenum = 0; facenum < input_faces.size();facenum++)
        {
            vector<int> face = input_faces[facenum];
            int num_points = face.size();

            for (int pointindex = 0; pointindex < num_points;pointindex++)
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

        for (vector<int> me : merged_edges)
        {

            vec3 p1 = input_points[me[0]];
            vec3 p2 = input_points[me[1]];
            vec3 cp = center_point(p1, p2);

            edges_centers.push_back(cp);
        }
        
        tuple< vector<vector<int>>, vector<vec3>> result = { merged_edges, edges_centers };

        return result;
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

        vector<vector<int>> edges_faces = get<0>(get_edges_faces(input_points, input_faces));
        vector<vec3> edges_centers = get <1>(get_edges_faces(input_points, input_faces));

        

        // add face points to new_points

        vector<int>face_point_nums;

        // point num after next append to new_points

        int next_pointnum = input_points.size();

        for (vec3 face_point : face_points)
        {
            input_points.push_back(face_point);
            face_point_nums.push_back(next_pointnum);
            next_pointnum += 1;
        }

        // add edge points to new_points

        map<tuple<int, int>, int> edge_point_nums;

        for (int edgenum = 0; edgenum < edges_faces.size();edgenum++)
        {
            int pointnum_1 = edges_faces[edgenum][0];
            int pointnum_2 = edges_faces[edgenum][1];
            vec3 edge_point = edges_centers[edgenum];

            input_points.push_back(edge_point);
            

            tuple<int, int> temptuple = { pointnum_1, pointnum_2 };

            edge_point_nums[temptuple] = next_pointnum;

            next_pointnum += 1;
        }



        // new_points now has the points to output. Need new faces

        vector<vector<int>> new_faces;

        for (int oldfacenum = 0;oldfacenum < input_faces.size();oldfacenum++)
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

                int edge_point_ab = edge_point_nums[switch_nums({ a,b })];
                int edge_point_da = edge_point_nums[switch_nums({ d,a })];
                int edge_point_bc = edge_point_nums[switch_nums({ b,c })];
                int edge_point_cd = edge_point_nums[switch_nums({ c,d })];

                new_faces.push_back({ a, edge_point_ab, face_point_abcd, edge_point_da });
                new_faces.push_back({ b, edge_point_bc, face_point_abcd, edge_point_ab });
                new_faces.push_back({ c, edge_point_cd, face_point_abcd, edge_point_bc });
                new_faces.push_back({ d, edge_point_da, face_point_abcd, edge_point_cd });
            }

            //for 6 point face
            if (oldface.size() == 6)
            {
                int a = oldface[0];
                int b = oldface[1];
                int c = oldface[2];
                int d = oldface[3];
                int e = oldface[4];
                int f = oldface[5];
                

                int face_point = face_point_nums[oldfacenum];

                int edge_point_ab = edge_point_nums[switch_nums({ a,b })];
                int edge_point_bc = edge_point_nums[switch_nums({ b,c })];
                int edge_point_cd = edge_point_nums[switch_nums({ c,d })];
                int edge_point_de = edge_point_nums[switch_nums({ d,e })];
                int edge_point_ef = edge_point_nums[switch_nums({ e,f })];
                int edge_point_fa = edge_point_nums[switch_nums({ f,a })];

                new_faces.push_back({ a, edge_point_ab, face_point, edge_point_fa });
                new_faces.push_back({ b, edge_point_bc ,face_point, edge_point_ab });
                new_faces.push_back({ c, edge_point_cd, face_point,edge_point_bc });
                new_faces.push_back({ d, edge_point_de, face_point, edge_point_cd });
                new_faces.push_back({ e, edge_point_ef, face_point, edge_point_de });
                new_faces.push_back({ f, edge_point_fa, face_point, edge_point_ef });


            }


        }

        tuple<vector<vec3>, vector<vector<int>>> result = { input_points,new_faces };

        return result;
    }

