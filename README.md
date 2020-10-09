# Sphere Histogram
## Brief description
_Sphere Histogram_ is a graphical tool to depict a distribution of points in 3D space on a sphere approximated by congruent triangles. The triangles' colors indicate the density of points lying in the respective direction.
Points can be read in from .npy files with shape (n, 3).

## Dependencies
- qt5-default
- zlib1g-dev
- libglm-dev

## How to deploy
0. Make sure to have the __dependencies__ installed on your system

1. Clone this Git repository
`$ git clone https://jugit.fz-juelich.de/a.roethenbacher/sphere-histo.git`

2. Change into the project's source directory
`$ cd sphere-histo/cpp-sphere-histo`

3. Use qmake to generate Makefile
`$ qmake .`

4. Use make to install
`$ make`

5. Start application from the same directory
`$ ./sphere-histo`

## How to use

- Inside the application window click File->Open to open a .npy file
- Use the slider below the sphere to change the number of triangles, that approximate the sphere
- Use the selection box next to the slider to choose your preferred color map
- To turn the sphere, press the left mouse button while moving the mouse
- To move the sphere, press the right mouse button while moving the mouse
- Use the mouse wheel to zoom
