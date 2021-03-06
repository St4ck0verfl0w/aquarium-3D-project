# Aquarium 3D project

This project is part of an assignment in the course 'INF443 - Informatique Graphique 3D'.

Everything required for compilation is already there.


<p align="center"><img width="50%" src="https://github.com/St4ck0verfl0w/aquarium-3D-project/tree/main/imgs/overview.jpeg" /></p>


## Introduction

The main idea of this project was to model a seabed in which a user would be able to move using the keyboard and the mouse, just like a first-person game.

In order to get a realistic representation of the sea, where the player would not be always outside the intended bounds, my objective was to create an infinite ground, where the user could evolve without any limit.
The next objective was to reproduce physical phenomena that can be found underwater, like the specific lightning that can be observed, the fading horizon, water effects, as well as gravity and friction loss.

Also, the objective of this project was to implement boids, and different animals, in order to make the scene more alive.

At last, the scene contains several textures and billboards to decorate the scene.

The whole project was coded using as many properties of object-oriented programming as possible, making the implementation and execution slower, but a code much more resilient and easier to use.


## Creating an infinite world

First, I created a 'Chunk', a finite world generated using perlin noise with some texture. The exact representation is detailed in the file Chunk.cpp, and a chunk needs a scaling vector as an argument.
This scaling vector will scale the chunk in any direction by any factor, but with only be used for symmetry. Thanks to symmetrical properties, consecutive occurrence of the same chunk can be continuous if some of them are mirrored along the correct axis.

With this technique, we can have a central chunk without symmetry, surrounded by chunks with symmetry along the axis x or y, and so on, creating a 4*4 grid of mirrored chunks. The file Terrain.cpp contains all the information regarding this grid.

| X-MIRROR  | NO-MIRROR | X-MIRROR  | NO-MIRROR |
|-----------|-----------|-----------|-----------|
| XY-MIRROR | Y-MIRROR  | XY-MIRROR | Y-MIRROR  |
| X-MIRROR  | NO-MIRROR | X-MIRROR  | NO-MIRROR |
| XY-MIRROR | Y-MIRROR  | XY-MIRROR | Y-MIRROR  |


Such grid is periodic, and it is possible to move the farthest chunks from the players in order to ensure that they are always at the center of the grid. From their point of view, it will always be possible to move in any direction, without limit, and 16 blocks around them will constantly be displayed. The transition always happens far away and with some fog, the users cannot see the end of the map.

At last, once the keyboard is detected and implemented, we have to make sure that the players are never underground (by ensuring f(x,y)<= z) where Z=f(X,Y) is the equation of the chunk, with X=x%L and Y=y%L where L is the size of a chunk.


Followed is documentation about VCL,



# VCL

- [Introduction](#Introduction)
- [Compile the library](#Compilation)
- [Basic usage of VCL structures](#Usage)
  - [Vec](#vec)
  - [Mat](#mat)
  - [Transformations](#Transformations)
  - [Buffers](#Buffers)

<a name="Introduction"></a>
## Introduction

VCL - Visual Computing Library - is a simple lightweight library on top of OpenGL provided to ease learning of 3D programming while avoiding re-coding everything from scratch (matrices, mesh structures, etc.). The library provides several helper structure and functions to set up 3D scene with interactive and/or animated elements.


The objective of the library is to be simple as simple as possible to read and use.
The majority of VCL structures and functions are aimed to be minimalistic without hidden states. The code remains fully compatible with direct raw OpenGL calls and user-defined shaders. The animation loop itself is entirely defined by the user.


The code contains two main parts:
* The VCL library itself in `library/vcl/` directory - contains the helper functions and structures
* The pre-made 3D scenes in `scenes/` directory. Each scene is fully defined in its subdirectory and correspond to an independant project and executable.

<br>


<a name="Compilation"></a>

## Compile the library

* A **CMakeLists.txt** is provided in each scene directory.

The library requires one external dependency: [GLFW](https://www.glfw.org/) which can be installed through standard packages in Linux/MacOS (see the provided detailed tutorials). Pre-compiled dll are provided for Windows/Visual Studio 2019.

* Link to [detailed tutorials for the different systems (Linux, Windows, MacOS)](https://imagecomputing.net/damien.rohmer/teaching/2020_2021/s2/inf585-computer_animation/compilation_vcl/index.html).

<a name="Usage"></a>
## Basic usage of VCL structures

<a name="vec"></a>
### vec

Basic structure and functions associated to 2/3/4D vectors are provided as `vec2`, `vec3` and `vec4` (following mostly GLSL naming convention).

`vec3` (and similarily with `vec2` and `vec4`) are lightweight structure storing 3 floating values (x,y,z). `vec3` are used throughout the scenes to define typically 3D vectors and positions coordinates.

```c++
// Declaring 2D/3D/4D vectors
vec2 p0 = {1.1f, 2.0f};
vec3 p1 = {1.1f, 2.0f, -2.5f};
vec4 p2 = {1.1f, 2.0f, -2.5f, 8.1f};

vec3 p3; // Default values initialized at (0,0,0)

// alternative declaration: constructor call
vec3 p4 = vec3(1.1f, 2.0f, -2.5f);
```

Components can be accessed via named (.x, .y, .z) or indexed ([0], [1], [2]) syntax.

```c++
vec3 p = {1.1f, 2.5f, -2.0f};
p.x = 0.5f;   // Access component as .x, .y, .z
p[1] = -2.5f; // Access component as [0]/[1]/[2]

// Check equality between two vec3 using is_equal(vec3,vec3) function
assert( is_equal(p, {0.5f, -2.5f, -2.0f}) );
```

vec3 (and all other structures) can be safely copied (no pointer or references involved).

```c++
vec3 p0 = {1.1f, 2.5f, -2.0f};
vec3 p1 = {5.0f, 1.1f,  3.0f};

// Copy p1.x/y/z = p0.x/y/z
p1 = p0;

p1.x = -12.0f;

// modifying p1 doesn't impact p0
assert( is_equal(p0, {  1.1f, 2.5f, -2.0f}) );
assert( is_equal(p1, {-12.0f, 2.5f, -2.0f}) );
```

Standard vector operators +-*/ are directly available

```c++
vec3 p = {1.0f, -1.5f, 2.0f};
p = 4 * p;           // p = {4,-6,8}
p = p / 2;           // p = {2,-3,4}
p = p + vec3(1,1,1); // p = {3,-2,5}
p = -p;              // p = {-3,2,-5}

// Operators can be chained
vec3 p2 = ( 2*p + vec3(1,0,2)/2.0f ) / 1.2f;

// Display the coordinates
std::cout<< p << std::endl;
```

As well as helping mathematical functions

```c++
vec3 a = {1,2,3};
vec3 b = {1,2,-1};

vec3  c = a * b;        // pointwise multiplication c = {1,4,-3}
float d = dot(a, b);    // dot product a.b = 2
vec3  e = cross(a,b);   // cross product a x b = {-8,4,0}
float f = norm(a);      // norm ||a|| = sqrt(dot(a,a))
vec3  g = normalize(a); // return the unit norm vector a / ||a||
```

<a name="mat"></a>
### mat

Similarily to vectors, the library provide `mat2`, `mat3` and `mat4` structure (also following GLSL naming convention). 

```c++
// Direct initialization
mat3 A = { 1.1f, 2.5f, 2.0f,
          -2.1f, 4.1f, 1.5f,
            3.0f, 1.0f, 3.5f};

// Display matrix components
std::cout<< A <<std::endl;

// Matrix-vector product
vec3 x = {1,2,3};
vec3 y = A * x;   // = {12.1, 10.6, 15.5}


// Matrix component access
A(0,0) =  2.0f;
A(1,0) =  3.0f;
A(0,1) = -1.0f;
// A = { 2.0f, -1.0f, 2.0f,
//       3.0f,  4.1f, 1.5f,
//       3.0f,  1.0f, 3.5f};

mat3 B; // default initialization as matrix identity

// Matrix product
mat3 C = A * B;
```

As well as several helping function

```c++
// Helper function
mat3 At = transpose(A); // matrix transpose
mat3 iA = inverse(A);   // matrix inverse: A*iA = identity
float d = det(A);       // matrix determinant = 13.1

// Access to row and column
vec3 c0 = A.col(0); // first column = {2,3,3}
vec3 r1 = A.row(1); // second row   = {3, 4.1, 1.5}
// Indexing matrix as a contiguous vector
// A[0] == 2.0f
// A[1] == -1.0f
// ...
// A[8] == 3.5f
```

<a name="Transformations"></a>
### Transformations

`mat3` can be used to store linear transformation (such as rotation), while `mat4` can store general affine transform.

`Rotations` are have a dedicated structure easing their usage. Rotation stores internally the rotational data as a quaternion, but allow to manipulate the variable like a matrix.

```c++
// create a rotation described by an axis and an angle
rotation(const vec3& axis, float angle); 
// create a rotation R such that R a = b
rotation_between_vector(const vec3& a, const vec3& b);
```

Example of usage
```c++
// Rotation of pi/4 around the y-axis
rotation R1 = rotation({0,1,0}, 3.14f/4);
// Rotation of pi/6 around some arbitrary axis (1,5,-2)/||(1,5,-2)||
rotation R2 = rotation({1,5,-2}, 3.14f/6);

// Two arbitrary axis (scaled to have a unit norm)
vec3 a = normalize(vec3{1,5,-2});
vec3 b = normalize(vec3{-2,1.5,3});
// Rotation transforming a into b: R3 a = b
rotation R3 = rotation_between_vector(a, b);

// Converting the rotation to its 3x3 matrix
mat3 matrix = R1.matrix();
```

`affine_rt` (for rotation and translation) and `affine_rts` (for rotation, translation and scaling) are helper structures to handle affine transformation. It stores linear, translation and scaling component independendly, while allowing a use similar to a 4x4 matrix

```c++
// A 3x3 matrix
rotation R = rotation({0,1,0}, 3.14f/4);
// A translation vector
vec3 t = {1,-2,3};
// Handle the transform acting like the matrix T such that
// T = (         |    )
//     (    R    | t  )
//     ( ________|____)
//     ( 0  0  0 | 1  )
affine_rt T = affine_rt(R, t);
```



<a name="Buffers"></a>
### Buffers

#### Dynamic 1D buffer

The structure `buffer<type>` is proposed at your convenience to store arbitrary number and numerical data contiguously in memory. `buffer` is only a special case of `std::vector` associated to an extra set of convenient functionalities for numerical vectors such as operators +-*/, std::cout, as well as strong bound checking.

```c++
// Buffer initialization
buffer<float> a = {1.1f, 2.5f, 8.0f, 4.0f};

// default initialization as an empty vector
buffer<float> b;

// Buffer can be resized at any time
b.resize(45); // b.size()==45
b.fill(5.0f); // fill all 45 elements with the number 5

// Add an element at the back
a.push_back(2.4f);

// Check equality
assert( is_equal(a, {1.1f, 2.5f, 8.0f, 4.0f, 2.4f}) );

// Display all elements
std::cout<< a <<std::endl;

// Getter/Setter similar to std::vector
a[2] = a[0] + 1;
// Calling a[5] would generate a run time error

// Apply operators on the entire buffer
a = 2.0f * a;
a = a + 2.5f; // add 2.5f to all elements of a
float avg = average(a); // get averaged value of the buffer
```

`buffer` can be used with `vec3` to conveniently store coordinates. Note that all floating values of the buffer are guaranteed to be contiguous in memory.

```c++
buffer<vec3> a = { {1,0,0}, {2,1,3} };
a.push_back({0,1,2});

a[0] = {0,0,1}; // Accessing an element as a vec3
a[0].x = 1.1f;  // x coordinate of the first element

// Display all coordinates
std::cout<< a <<std::endl;

// numerical operations on the entire buffer
a = 2.0f*a;
a = a + vec3(1,1,0); // (1,1,0) is added to every element

buffer<vec3> b;
b.resize(3);
b.fill({0,1,0});

// componentwise sum: for all i, a[i] = a[i] + b[i]
a = a + b;
// componentwise product: for all i, a[i] = a[i] + b[i]
a = a * b;
```

#### 2D/3D grid

Buffers organized as 2D / 3D grid are also provided for convenience as `grid_2D` and `grid_3D`.

These buffer are internally similar to 1D one, and ensure contiguity of elements, but provide accessor using (x,y) notations and handle bounds checking.

Example of a grid_2D containing vec3 - example of application: grid of positions.

```c++
// Initialize a grid of size 2x3
// Each element of the grid contains a vec3
grid_2D<vec3> grid;
grid.resize(2,3);

// Fill the entire grid with (1,1,0)
grid.fill({1,1,0});

// Set grid each grid element using (x,y) indexed notation
grid(0,0)={0,0,0}; grid(1,0)={0,0,1};
grid(0,1)={0,1,0}; grid(1,1)={0,1,1};
grid(0,2)={1,0,0}; grid(1,2)={1,0,1};

// Grid element can also be indexed using its offset in the buffer
grid[0] = {0,0,0};
grid[1] = {0,0,1};
grid[2] = {0,1,0};
grid[3] = {0,1,1};
grid[4] = {1,0,0};
grid[5] = {1,0,1};

// Display all elements
std::cout<< grid <<std::endl;


// Compatible with operators
grid_2D<vec3> grid2 = 0.5f * grid;

grid = 2.0f * grid;
grid_2D<vec3> g0 = grid + grid2; // g0(i,j) = grid(i,j) + grid2(i,j)
grid_2D<vec3> g1 = grid * grid2; // g1(i,j) = grid(i,j) * grid2(i,j)
grid_2D<vec3> g2 = grid + vec3(0,1,0); // g2(i,j) = grid(i,j) + (0,1,0)
```

_Note: You cannot `push_back` one element in a buffer2D or buffer3D as it would not be compatible with grid-like structure._


#### Fixed size buffer

The structure `buffer_stack` is similar the `buffer` (contiguous data) but for fixed size container (elements stored on stack memory).
While `buffer` are only convenient wrap around `std::vector`, `buffer_stack` is a convenient wrap around `std::array` for numerical data.

You will probably not have to create yourself directly `buffer_stack` object, but be aware that `vec` (2/3/4) are specific cases of the more general `buffer_stack`.
