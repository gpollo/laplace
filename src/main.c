#include <stdio.h>
#include <pthread.h>

#include "lattice.h"
#include "worker.h"

struct bound* boundaries(struct bound* bound, struct rect* pos) {
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

/*    if(x <= 16 && y >= 48) {
        bound->value =  150;
        bound->cond  = DIRICHLET;
    } else if(44 <= x && x <= 64 && y >= 48) {
        bound->value = -150;
        bound->cond  = DIRICHLET;
	}*/

	if(16 <  x && x < 48 && (y < 12 || y >= 36)) {
        bound->value = 0;
        bound->cond  = NEUMANN;
    }

    return bound;
}

int main() {
    struct rect size = {64, 48};
    struct point dim = {320, 240};
    struct config conf = {8, 10, 0.0001};
    struct lattice* lattice = lattice_new(&size, &dim, &boundaries);
    struct worker* worker;

    //printf("%d\n", lattice_compute(lattice, 0.01));
    printf("%d\n", lattice_compute_threaded(lattice, &conf));

    lattice_print(lattice);
    lattice_delete(lattice);

	return 0;
}

