import matplotlib.pyplot as plt
import matplotlib.animation as animation
import mpl_toolkits.mplot3d.axes3d as p3
import numpy as np


def animate_wave_simulation_2D(U):
    """
    Creates a 2D animation of a wave simulation.

    Automatically selects color-scale.  In the event of instability or
    a wild range of values, some components might not be easily seen.
    Consider tweaking this scale if you need to.

    Parameters
    ----------
    U: list-like
       A list of 2D wavefields to animate.

    Returns
    -------
    Matplotlib animation class instance.

    """

    fig = plt.figure()

    cmin, cmax = U[0].min(), U[0].max()
    for u in U:
        cmin = min(cmin, u.min())
        cmax = max(cmax, u.max())

    cmin = max(-1, cmin)
    cmax = min(1, cmax)

    im = plt.imshow(U[0], clim=(cmin,cmax), cmap='gray')
    def animate(i):
        im.set_data(U[i])
        return (im,)

    ani = animation.FuncAnimation(fig, animate, interval=50, repeat_delay=1000)

    return ani


def animate_wave_simulation_3D(U):
    """
    Creates a 3D surface animation of a wave simulation.

    Automatically selects color-scale.  In the event of instability or
    a wild range of values, some components might not be easily seen.
    Consider tweaking this scale if you need to.

    Adapted from https://stackoverflow.com/a/45713451 under the 
    CC BY-SA 3.0 license.

    Parameters
    ----------
    U: list-like
       A list of 2D wavefields to animate.

    Returns
    -------
    Matplotlib animation class instance.

    """
    
    grid_y = np.linspace(0, 1, U[0].shape[0])
    grid_x = np.linspace(0, 1, U[0].shape[1])
    YY, XX = np.meshgrid(grid_y, grid_x)

    fig = plt.figure()
    ax = p3.Axes3D(fig)


    cmin, cmax = U[0].min(), U[0].max()
    for u in U:
        cmin = min(cmin, u.min())
        cmax = max(cmax, u.max())

    cmin = max(-1, cmin)
    cmax = min(1, cmax)

    surf = [ax.plot_surface(YY, XX, U[0], cmap="viridis", clim=(cmin, cmax))]
    ax.set_zlim(1.1*cmin, 1.1*cmax)

    def animate(i, U, surf):
        surf[0].remove()
        surf[0] = ax.plot_surface(YY, XX, U[i], cmap="viridis", clim=(-1,1))

    ani = animation.FuncAnimation(fig, animate, fargs=(U, surf), interval=50, repeat_delay=1000)

    return ani