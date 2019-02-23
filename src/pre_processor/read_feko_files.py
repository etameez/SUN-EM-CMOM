from data_structures import *
from log import log

import math

class FEKOFileReader:

    def __init__(self, file_name):
        self.file_name = file_name
        self.nodes = []
        self.triangles = []
        self.edges = []
        self.const = {}
    
    def readFEKOOutFile(self):
        
        self.const["srcFile"] = self.file_name
        self.const["numFreq"] = 0
        self.const["freq"] = []

        incident_flag = True
        field_strength_flag = True

        try:
            file = open(self.file_name, "r")
            log(self.file_name + " opened successfully")
        except IOError as e:
            log(e)
            return False

        line = file.readline()

        # Loop through file for the first time to get number of triangles, edges and frequencies 
        while line:

            # Get number of triangles
            if "Number of metallic triangles:" in line:
                content = line.split()
                number_of_triangles = int(content[9])
                
            # Get number of edges
            if "Number of metallic edges (MoM):" in line:
                content = line.split()
                number_of_edges = int(content[13])

 
            # get frequency
            if "Frequency in Hz:" in line:
                self.const["numFreq"] = self.const["numFreq"] + 1
                content = line.split()
                self.const["freq"].append(float(content[5]))

            # get excitation angle
            if "Direction of incidence:" in line:
                if incident_flag:
                    content = line.split()
                    self.const['phi'] = content[8]
                    self.const['theta'] = content[5]
                    incident_flag = False
            
            # get magnitude of excitation
            if "Field strength in V/m:" in line:
                if field_strength_flag:
                    content = line.split()
                    e_x = float(content[6])

                    line = file.readline()
                    content = line.split()
                    e_y = float(content[5])

                    line = file.readline()
                    content = line.split()
                    e_z = float(content[2])

                    self.const["emag"] = math.sqrt(pow(e_x, 2) + pow(e_y, 2) + pow(e_z, 2))

            # Get next line
            line = file.readline() 

        # Close the file
        file.close()
        self.const["numEdges"] = number_of_edges

        # Second pass through file to read edge, triangle and source data
        file = open(self.file_name, "r")
        line = file.readline()

        while line:
            
            # Get the triangle data
            if "DATA OF THE METALLIC TRIANGLES" in line:
                line = file.readline() # empty line
                line = file.readline() # first header
                line = file.readline() # second header
                line = file.readline() # third header
                line = file.readline() # fourth header
                
                for i in range(number_of_triangles):
                    line = file.readline() # row no label x1 y1 z1 edges 
                    content = line.split()

                    label = content[1]

                    current_nodes = []
                    current_nodes.append(Node(float(content[2]), float(content[3]), float(content[4])))

                    line = file.readline() # row x2 y2 z2
                    content = line.split()
                    current_nodes.append(Node(float(content[2]), float(content[3]), float(content[4])))

                    line = file.readline() # row x3 y3 z3
                    content = line.split()
                    current_nodes.append(Node(float(content[2]), float(content[3]), float(content[4])))

                    line = file.readline() # row nx ny nz area
                    content = line.split()
                    area = float(content[3])

                    current_vertices = []
                    for i in range(3):
                        if current_nodes[i] in self.nodes:
                            current_vertices.append(self.nodes.index(current_nodes[i]))
                        else:
                            self.nodes.append(current_nodes[i])
                            current_vertices.append(self.nodes.index(current_nodes[i]))
                    
                    self.triangles.append(Triangle(current_vertices[0],
                                                   current_vertices[1],
                                                   current_vertices[2],
                                                   triangleCentreCalculator(current_nodes),
                                                   area,
                                                   label))

                            



            if "DATA OF THE METALLIC EDGES" in line:
                line = file.readline() # empty line
                line = file.readline() # first header
                line = file.readline() # second header

                for i in range(number_of_edges):
                    line = file.readline() # no type length KORP KORM
                    content = line.split()

                    length = float(content[2])
                    korp = int(content[6]) - 1 # plus triangle
                    korm = int(content[7]) - 1 # minus triangle

                    edge_vertices = getEdgeVertices(self.triangles[korp], self.triangles[korm])
                    

                    centre = edgeCentreCalculator(self.nodes[edge_vertices[0]], self.nodes[edge_vertices[1]])

                    plus_free_vertex = egdeGetFreeVertex(self.triangles[korp], edge_vertices)
                    minus_free_vertex = egdeGetFreeVertex(self.triangles[korm], edge_vertices)

                    rho_c_plus = edgeRhoPlusMinusCalculator(self.triangles[korp], self.nodes[plus_free_vertex], 0)
                    rho_c_minus = edgeRhoPlusMinusCalculator(self.triangles[korm], self.nodes[minus_free_vertex], 1)

                    self.edges.append(Edge(edge_vertices[0],
                                           edge_vertices[1],
                                           centre,
                                           length,
                                           korm,
                                           korp,
                                           minus_free_vertex,
                                           plus_free_vertex,
                                           rho_c_minus,
                                           rho_c_plus))

            
            line = file.readline()
        
        return True


# freader = FEKOFileReader("C:\\Users\\Tameez\\Documents\\git\\SUN-EM\\examples\\two_plate_array\\pec_plate.out")
# freader.readFEKOOutFile()
# print("hello")