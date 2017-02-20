Numerical solver of Laplace's equation
======================================
This repository contains a small C code that implements a
finite difference solver of Laplace's equation. It also provides
a multi-threaded version of the algorithm. To define a boundary
conditions, one must create a function like the one showed below.

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
