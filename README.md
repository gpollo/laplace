Numerical solver of Laplace's equation
======================================
This repository contains a small C code that implements a
finite difference solver of Laplace's equation. It also provides
a multi-threaded version of the algorithm. To define a boundary
conditions, one must create a function like the one showed below.

Running
-------
To run the code, simply clone this repository and edit the `main.c`
file to match the desired problem.

    $ git clone https://github.com/TheNiceGuy/laplace
    $ mkdir -p laplace/build && cd laplace/build
    $ cmake .. && make
    $ ./laplace

The matrix will be written in `stderr`, thus you can save it to a file
using simple IO redirection.

    $ ./laplace 2> data.csv
    
A simple script is provided in the `scripts` folder that can plot the
data using [matplotlib](http://matplotlib.org/). The following command
will create an image file.

    $ ./graph.py data.csv
    
The following command will show the data directly.
    
    $ ./graph.py data.csv --show

Examples
--------
    struct bound* boundaries(struct bound* bound, struct rect* pos) {
        double x = pos->x;
        double y = pos->y;

        if(x < 16 && y <= 0) {
            bound->value =  150;
            bound->cond  = DIRICHLET;
        } else if(48 <= x && x <= 64 && y <= 0) {
            bound->value = -150;
            bound->cond  = DIRICHLET;
        } else if(16 <= x && x< 48 && (y < 12 || y >= 36)) {
            bound->value = 0;
            bound->cond  = NEUMANN;
        } else {
            bound->value = 0;
            bound->cond  = NONE;
        }

        return bound;
    }

![JFET](https://github.com/TheNiceGuy/laplace/raw/master/img/jfet.png)

    struct bound* boundaries(struct bound* bound, struct rect* pos) {
        double x = pos->x;
        double y = pos->y;

        if(x < 16 && y <= 0) {
            bound->value =  150;
            bound->cond  = DIRICHLET;
        } else if(48 <= x && x <= 64 && y <= 0) {
            bound->value = -150;
            bound->cond  = DIRICHLET;
        } else if(16 <= x && x< 48 && (y < 12 || y >= 36)) {
            bound->value = 0;
            bound->cond  = NEUMANN;
        } else if(10 <= x && x < 20 && 25 <= y && y < 30) {
            bound->value = 0;
            bound->cond  = NEUMANN;
        } else if(30 <= x && x < 55 && 15 <= y && y < 18) {
            bound->value = 60;
            bound->cond  = DIRICHLET;
        } else {
            bound->value = 0;
            bound->cond  = NONE;
        }

        return bound;
    }

![JFET with holes](https://github.com/TheNiceGuy/laplace/raw/master/img/jfet_holes.png)
