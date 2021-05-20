import numpy as np
import matplotlib.pyplot as plt
import math as Math
      

def main():    
    
    
    I = Image2D('mri85.png') #test code required to geenrate images and test functions
    sharp = I.getSharp()
    smooth = I.getSmooth(5,2)
    smooth.write('smooth.png')
    sharp.write('sharp.png')
    I.show([smooth, sharp]) #Displays images side by side
    
    
    
    
    """
    Write your code here
    """
    
class Image2D:
    
    def __init__(self, arg):
        """
        Constructor for Image2D class
        
        input: 
            arg : file name or image data as a numpy array
        """

        if type(arg) is np.ndarray:
            self.pixels = arg
        else:
            self.read(arg)
    
    def read(self, fileName):
        """
        Reads image from file

        input:
            fileName : file name of image
        """
        self.pixels = plt.imread(fileName)
    
    def write(self, fileName):
        """
        Writes image to file.
        
        input:
            fileName : file name of image
        """
        plt.imsave(fileName, np.clip(self.pixels,0,None) , cmap=plt.cm.gray)
        
        
    def show(self, others = []):    
        """
        Displays two images side by side in a new figure assuming color map is gray.
        Input: a second 2D image object.
               axis is set to off.
               
               input: 
                   titleSelf : A string showing image display title for image 1 (self)
                   titleOther : A string showing image display title for image 2
        """
        
        
        plt.figure()
        n = 1 + len(others) # number of images
        # Show first image
        plt.subplot(1,n,1)
        plt.imshow(np.clip(self.pixels,0,None),cmap=plt.cm.gray)
        plt.axis('off')
        
        # Show other images
        for i in range(1,n):
            plt.subplot(1,n,i+1)
            plt.imshow(np.clip(others[i-1].pixels,0,None),cmap=plt.cm.gray)
            plt.axis('off')
    
        plt.show()        

    def getWidth(self):
        """
        returns image width
        """
        return self.pixels.shape[1]
    
    def getHeight(self):
        """
        returns image height
        """
        return self.pixels.shape[0]

    def getFiltered(self, kernel):
        """
        Write your code here
        """
        I = self.pixels #defines I as original image
        
        R = kernel.shape[0] // 2 #Defines R constant as center of kernel indices
        J = np.zeros((self.getHeight(), self.getWidth())) #Defines empty array the same size as the original to begin to add new values after being filtered
        H = kernel
        sumOf = 0
        for u in range(self.getWidth()): #first two for loops go through the entirety of the original image
            for v in range(self.getHeight()):
                for i in range(kernel.shape[1]): #these two loops go through surroudings and sum them for every single pixel in I because they are nested
                    for j in range(kernel.shape[0]):
                        if (u + i - R) >= 0 and (u + i - R) < self.getWidth() and (v + j - R) >= 0 and (v + j - R) < self.getHeight(): #check to make sure indices arent out ofounds before trying to access them
                            sumOf += H[i, j] * I[v + j - R, u + i - R] # sums the values around the pixel by multiplying them by the value overlayed on it by the kernel and adds it to total value
                            
                        
                J[v, u] = sumOf #sets the current pixel equal to the combined surroundings after being kernel filtered
                sumOf = 0 #resets sum for nest iteration of the loop
            
        return Image2D(J) #returns 2D image of the array
        
        
    def getSharp(self):
        """
        Write your code here
        """
        kernel = np.array([[0, -1, 0], #creates kernel that was specified in directions
              [-1, 5, -1],
              [0, -1, 0]])
        return self.getFiltered(kernel) #returns the filtered version of the current image with the kernel specified above
        
    def makeGaussKernel(self, size, sigma):
        """
        Write your code here
        """
        S = size #defines S as size for simpilcity in variables
        
        R = (S-1) / 2 # Defines R as mid point for array for simplicity
        
        O = sigma #defines O as sigma for simpilcity in variables
        
        H = np.zeros((S, S)) #Defines an array of zeros to begin to add values too in loops
        
        for i in range(S): #First two loops go through every pixel of the array
            for j in range(S): #the formula is broken down into several variables for simplictity
                exponentnum = Math.pow(i - R, 2) + Math.pow(j - R, 2) #Defines the numerator of the exponent of e in the formula specified in the instructions
                exponentdenom = 2 * Math.pow(O, 2) #define the denomenator of the exponent for e in the formula specified in the instructions
                numerator = Math.pow(Math.e, -(exponentnum/exponentdenom)) #defines the numerator as e to the power of the negative numerator/denomenator
                sums1 = 0 #defines sum variables to be used for sum operation in next two loops
                sums2 = 0
                for m in range(S): #two loops used to execute the sum functions in the denominator
                    for n in range(S):
                        expnum = Math.pow(m - R, 2) + Math.pow(n - R, 2) #defines the exponent numerator in the denomenator of the function
                        expdenom = 2 * Math.pow(O, 2) #defines the exponent denomenator in the denomenator of the function
                        value = Math.pow(Math.e, -(expnum/expdenom)) #generates the value to be summed
                        sums1 += value #Adds the value to the sum
                    sums2 += sums1 #adds the sum of the previous sum operation
                    sums1 = 0 #resets the original sum for the next iteration
                H[i, j] = numerator/sums2 #Sets the current value at H to the numerator divided by the sums generated in the denominator
        return H #Returns H after the oeprations are complete
        
        
        
        
        
        
        
        
        
    def getSmooth(self, size=3, sigma=1):
        """
        Write your code here
        """ 
        kernel = self.makeGaussKernel(size, sigma) #generates gaussian kernel with the specifications from the functiojn
        return self.getFiltered(kernel) #returns the filtered version of the image using the gaussian kernel generated above
        
        
        
        
        
        
        
        
if __name__ == "__main__": 
    from IPython import get_ipython
    get_ipython().run_line_magic('matplotlib', 'qt')        
    main()