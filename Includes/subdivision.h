#pragma once
#include <stdafx.h>

using namespace std;
using namespace glm;

vec3 center_point(vec3 p1, vec3 p2);
vec3 sum_point(vec3 p1, vec3 p2);
vec3 mul_point(vec3 p, int m);
vec3 div_point(vec3 p, int d);
vector<vec3> get_face_points(vector<vec3> input_points, vector<vector<int>> input_faces);
tuple<vector<vector<int>>, vector<vec3>> get_edges_faces(vector<vec3> input_points, vector<vector<int>> input_faces);
vector<vec3> get_edge_points(vector<vec3>input_points, vector <vector<int>> edges_faces, vector<vec3> edges_centers, vector<vec3>face_points);
vector<vec3> get_avg_face_points(vector<vec3>input_points, vector<vector<int>> input_faces, vector<vec3>face_points);
vector<vec3> get_avg_mid_edges(vector<vec3>input_points, vector <vector<int>> edges_faces, vector<vec3>edges_centers);
vector<int> get_points_faces(vector<vec3> input_points, vector<vector<int>> input_faces);
vector<vec3> get_new_points(vector<vec3> input_points, vector<int> points_faces, vector<vec3> avg_face_points, vector<vec3> avg_mid_edges);
tuple<int, int> switch_nums(tuple<int, int> point_nums);
tuple<vector<vec3>, vector<vector<int>>> cmc_subdiv(vector<vec3>input_points, vector <vector<int>> input_faces);