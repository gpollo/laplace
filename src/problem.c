#include <stdlib.h>

#include "problem.h"
#include "lattice.h"

struct bound* prob_boundaries(struct bound* bound, struct rect* pos) {
    double x = pos->x;
    double y = pos->y;

    if(x <= 16 && y <= 0) {
        bound->value =  150;
        bound->cond  = DIRICHLET;
    } else if(48 <= x && x <= 64 && y <= 0) {
        bound->value = -150;
        bound->cond  = DIRICHLET;
    } else {
        bound->value = 0;
        bound->cond  = NONE;
    }

	if(16 <  x && x < 48 && (y < 12 || y > 36)) {
        bound->value = 0;
        bound->cond  = NEUMANN;
    }

    return bound;
}

struct problem* get_problem() {
    /* dimensions */
    struct rect* size = malloc(sizeof(struct rect));
    if(size == NULL) goto ERROR_ALLOC1;
    size->x = 64;
    size->y = 48;

    /* lattice resolution */
    struct point* dim = malloc(sizeof(struct point));
    if(dim == NULL) goto ERROR_ALLOC2;
    dim->x = 320;
    dim->y = 240;

    /* the lattice itself */
    struct lattice* lattice = lattice_new(size, dim, &prob_boundaries);
    if(lattice == NULL) goto ERROR_ALLOC3;

    /* the problem */
    struct problem* problem = malloc(sizeof(struct problem));
    if(problem == NULL) goto ERROR_ALLOC4;
    problem->lattice = (void*) lattice;
    problem->resolution = 0.001;

    return problem;

ERROR_ALLOC4:
    free(lattice);
ERROR_ALLOC3:
    free(dim);
ERROR_ALLOC2:
    free(size);
ERROR_ALLOC1:
    return NULL;
}

void solve_problem(struct problem* problem) {
    lattice_compute((struct lattice*) problem->lattice, problem->resolution);
}

void delete_problem(struct problem* problem) {
    lattice_delete((struct lattice*) problem->lattice);
    free(problem);
}
