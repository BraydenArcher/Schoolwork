from scipy.integrate import solve_ivp
import numpy as np
import matplotlib.pyplot as plt

def f(t,y):
    [B, V, S, I, R] = y #Assign appropriate values from original function values
    
    return np.array([0, 0, -B * S * I, B * S * I - V * I, V * I]) #Create and return an array with the two constant derivates, and the derivative of S, I, and R respectively based on the original function values specified in the project guide
    
    
def f_wash(t,y):
    [B, V, S, I, R] = y #Assign appropriate values from original function values
    if t > 12: #After day 12, the B value is re-assigned to its new value for the rest of the plot
        B = 0.0001
    return np.array([0, 0, -B * S * I, B * S * I - V * I, V * I])#Create and return an array with the two constant derivates, and the derivative of S, I, and R respectively based on the original function values specified in the project guide

def f_washMed(t,y):
    [B, V, S, I, R] = y #Assign appropriate values from original function values
    
    if t > 12: #After day 12, the B value is re-assigned to its new value for the rest of the plot
        B = 0.0001
    
    if t > 30: #After day 30, the V value is re-assigned to its new value for the rest of the plot
        V = 0.4
    
    return np.array([0, 0, -B * S * I, B * S * I - V * I, V * I])#Create and return an array with the two constant derivates, and the derivative of S, I, and R respectively based on the original function values specified in the project guide

def calcSIR(f):
    B0 = 0.0005 #Initial
    V0 = 0.1    #conditions
    S0 = 1500   #for
    I0 = 1      #simulation
    R0 = 0      #defined
    
    
    Y0 = np.array([B0, V0, S0, I0, R0]) #Create original Y array with all initial conditions
    
    t_start = 0 #Starting value for t
    t_end = 60 #Ending value for T
    N = 121 #Number of steps
    
    t = np.linspace(t_start, t_end, N) #Create an array from t_start to t_end with N steps inbetween
    
    result = solve_ivp(f, [t_start, t_end], Y0, t_eval = t) #Solve the equations using the 4th-order Runge-Kutta method
    
    S = result.y[2] #Retrieve values of S from result variable
    I = result.y[3] #Retrieve values of I from result variable
    R = result.y[4] #Retrieve values of R from result variable
    
    
    
    return (t,S,I,R) #Return the arrays of value for each variable

def plotSIR(t,S,I,R):
    """
    Write your code here.
    """
    plt.figure() #Create the plot
    plt.plot(t, S, 'b') #Add the Susceptable values to the plot
    plt.plot(t, I, 'r') #Add the Infected values to the plot
    plt.plot(t, R, 'g') #Add the Recovered values to the plot
    plt.xlabel('Day') #Add X-Axis label
    plt.ylabel('Number') #Add Y-Axis label
    plt.legend(["Susceptable","Infected","Recovered"]) #Create legend with each respective label
    
    

def main():
    plot = calcSIR(f) #Generate values for first simulation
    plotWash = calcSIR(f_wash) #Generate values for second simulation
    plotWashMed = calcSIR(f_washMed) #Generate values for third simulation
    
    plotSIR(plot[0], plot[1], plot[2], plot[3]) #Plot values for first simulation
    plotSIR(plotWash[0], plotWash[1], plotWash[2], plotWash[3]) #Plot values for second simulation
    plotSIR(plotWashMed[0], plotWashMed[1], plotWashMed[2], plotWashMed[3]) #Plot values for third simulation
    
    
    
    
if __name__== "__main__":
    from IPython import get_ipython
    get_ipython().run_line_magic('matplotlib', 'auto') 
    plt.close('all')
    main()