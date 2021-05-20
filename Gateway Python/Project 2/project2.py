import math
import VTK_mesh_proc

class Surface:
    def __init__(self,filename):
        [self.faceList, self.vertexList, self.vtkData] = VTK_mesh_proc.read_VTK_withScales(filename)


    def display(self, scalers=[], scalarsOnFaces=True):
        """
        Displays surface
        """
        if scalers != []:
            vtkData = VTK_mesh_proc.addScalers(self.vtkData, self.triangleAreaList(), scalarsOnFaces)
        else:
            vtkData = self.vtkData
        VTK_mesh_proc.render(vtkData)
        
    def numVertices(self):
        # Write your code here
        return len(self.vertexList) #return the length of the list
        pass
    
    def numFaces(self):
        # Write your code here
        return len(self.faceList) #return the length of the list
        pass
    
        
    def triangleArea(self, vertices):    
        # Write your code here
        a = math.sqrt(math.pow(vertices[1][0] - vertices[0][0],2) + math.pow(vertices[1][1] - vertices[0][1],2) + math.pow(vertices[1][2] - vertices[0][2],2)) #Define length of a using distance forumla with math module 
        b = math.sqrt(math.pow(vertices[2][0] - vertices[1][0],2) + math.pow(vertices[2][1] - vertices[1][1],2) + math.pow(vertices[2][2] - vertices[1][2],2)) #Define length of b using distance forumla with math module 
        c = math.sqrt(math.pow(vertices[2][0] - vertices[0][0],2) + math.pow(vertices[2][1] - vertices[0][1],2) + math.pow(vertices[2][2] - vertices[0][2],2)) #Define length of c using distance forumla with math module 
        d = (a + b + c) / 2 #define d using a b and c
        return math.sqrt(d * (d - a) * (d - b) * (d - c)) #return the final value
        
        
        pass

    def triangleVolume(self, vertices):
        # Write your code here
        v1x = vertices[0][0] #Define vertices with repspective coordinate values
        v1y = vertices[0][1]
        v1z = vertices[0][2]
        v2x = vertices[1][0]
        v2y = vertices[1][1]
        v2z = vertices[1][2]
        v3x = vertices[2][0]
        v3y = vertices[2][1]
        v3z = vertices[2][2]
        
        a = ((v1y * v2z) - (v1z*v2y)) * v3x #define first chunk of formula
        b = ((v1z * v2x) - (v1x*v2z)) * v3y #define second chunk of formula
        c = ((v1x * v2y) - (v1y*v2x)) * v3z #define third chunk of formula
        
        return (1/6) * (a + b + c) #return the sum of the three chunks of the formula for volume multiplied by 1/6 to complete formula
        pass

    def triangleAreaList(self):
        # Write your code here
        l = []
        for i in range(self.numFaces()): #Loop through all possible faces
            triangle = [self.vertexList[self.faceList[i][0]], self.vertexList[self.faceList[i][1]], self.vertexList[self.faceList[i][2]]] #create a triagnle with a list of vertices
            l.append(self.triangleArea(triangle)) #calculate area of triangle and append it to the list
        return l
        pass
        
    def area(self):
        # Write your code here
        total = 0
        tlist = self.triangleAreaList() #define triangle area list
        
        for i in range(len(tlist)): #loop through every value of the triangle area list
            total += tlist[i] #add current value of area to the total of area
        return total #return the total area
        pass
    
    def volume(self):
        # Write your code here
        total = 0
        for i in range(self.numFaces()):
            triangle = [self.vertexList[self.faceList[i][0]], self.vertexList[self.faceList[i][1]], self.vertexList[self.faceList[i][2]]] #create triangle with vertices
            total += self.triangleVolume(triangle) #get volume of triangle and add it to the total volume
        return total
        
        
        pass

def main():
    name = 'rightHippocampus.vtk'   
    S = Surface(name)
    print('The surface area and volume are {0:.2f} mm^2 and {1:0.2f} mm^3 respectively'.format(S.area(), S.volume())) #Print total values
    S.display()
    
    
if __name__ == "__main__": main()    