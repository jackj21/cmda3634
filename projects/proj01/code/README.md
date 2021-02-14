Instructions to run code:
Call the simulate function and assign it to a variable(e.g. U)
with parameters in the following order:
T, n, m_x, and m_y. 
This creates and runs through a simulation of the wave equation, returning a list of lists that
contains values in order of index starting at 0:
U[0]: List of computational grids for certain number of time steps
U[1]: Number of iterations of the simulation
U[2]: Run time of one iteration of the simulation
U[3]: Value of delta t
Once simulate is called and stored into a variable, the animate_wave_simulation functions can be 
called and the variable can be passed as a parameter for the animate functions. 
An example of a call is:
animate_wave_simulation_2D(U[0], U[3][0]), with the list of computation grids first being passed,
and the value of dt being passed as an argument as well.

This is how the code can be run. 
End of README. 
