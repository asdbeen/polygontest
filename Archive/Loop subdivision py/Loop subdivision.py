def center_point(p1,p2):
    #return a point in the center of the segment ended by points p1 and p2
    
    cp=[]
    for i in range(3):
        cp.append((p1[i]+p2[i])/2)
    
    return cp
    
def sum_point(p1,p2):
    #adds points p1 and p2
    sp = []
    
    for i in range(3):
        sp.append(p1[i]+p2[i])
        
    return sp
    
def sub_point(p1,p2):
    
    sp=[]
    for i in range(3):
        sp.append(p1[1]-p2[2])
        
    return sp
    
def div_point(p,d):
    #divide point p by d
    sp=[]
    for i in range(3):
        sp.append(p[i]/d)
    return sp
    
def mul_point(p,m):
    #multply point p by m
    sp=[]
    for i in range(3):
        sp.append(p[i]*m)
        
    return sp
    
def get_edges_faces(input_points,input_faces):
    # get list of egdes and the one or two adjacent faces in a list
    # also get center point of edge
    # each edge would be [pointnum_1,pointnum_2,facenum_1,facenum_2,center]
    
    #will have[pointnum_1,pointnum_2,facenum]
    
    edges=[]
    
    # get edges from each face
    face0 = input_faces[0]

    for facenum in range(len(input_faces)):
        face = input_faces[facenum]
        num_points = len(face)
        # loop over index into face
        for pointindex in range(num_points):
            # if not last point then edge is curr point and next point
            if pointindex < num_points -1:
                pointnum_1 = face[pointindex]
                pointnum_2 = face[pointindex+1]
                
            
            else:
                # for last point edge is curr point and first point
                pointnum_1 = face[pointindex]
                pointnum_2 = face[0]
        
        
        # order points in edge by lowest point number
        if pointnum_1 > pointnum_2:
            temp = pointnum_1
            pointnum_1 = pointnum_2
            pointnum_2 = temp
            
        edges.append([pointnum_1,pointnum_2,facenum])
        edges=sorted(edges)
        num_edges = len(edges)
        eindex=0
        merged_edges = []
        
        while eindex < num_edges:
            e1 = edges[eindex]
            # check if not last edge
            if eindex < num_edges -1:
                e2 = edges[eindex-1]
                if e1[0] == e2[0] and e1[1] == e2[1]:
                    merged_edges.append([e1[0],e1[1],e1[2],e2[2]])
                    eindex +=2
                else:
                    merged_edges.append([e1[0],e1[1],e1[2],None])
                    eindex +=1
            
            else:
                merged_edges.append([e1[0],e1[1],e1[2],None])
                eindex +=1

        # add edge centers

        edges_centers = []

        for me in merged_edges:
            p1 = input_points[me[0]]
            p2 = input_points[me[1]]
            cp = center_point(p1,p2)
            edges_centers.append(me+[cp])

    return edges_centers


def get_faces_points(input_points,input_faces,edges_faces):
    # get list of edges and the one or two adjacent faces in a list
    # also get center point of edge
    # each edge would be [pointnum_1,pointnum_2,facenum_1,facenum_2,center]

    num_edges = len(edges_faces)
    faces_points=[]

    for edge in edges_faces:
        if edge[3] != None:
            faces_point=[0.0,0.0,0.0]
            for face_num in [edge[2],edge[3]]:
                for point_num in input_faces[face_num]:
                    if point_num !=edge[0] and point_num != edge[1]:
                        faces_point = sum_point(faces_point,mul_point(input_points[point_num],0.25))
                    else:
                        faces_point = sum_point(faces_point,mul_point(input_points[point_num],0.125))
            faces_points.append(faces_point)

        else:
            faces_point=[0.0,0.0,0.0]
            faces_points.append(faces_point)   
    return faces_points  

input_faces=[[0,3,1],[0,3,2],[0,2,1],[3,2,1]]
input_points=[[-1.0,1.0,1.0],[1.0,-1.0,1.0],[1.0,1.0,-1.0],[-1.0,-1.0,-1.0]]

a=get_edges_faces(input_points,input_faces)
print (a)
print("\n")
b=get_faces_points(input_points,input_faces,a)
print (b)
