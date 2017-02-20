#include <stdio.h>
#include <pthread.h>

#include "lattice.h"
#include "worker.h"

struct bound* boundaries(struct bound* bound, struct rect* pos) {
    double x = pos->x;
    double y = pos->y;

    if(x < 16 && y <= 0) {
        bound->value =  150;
        bound->cond  = DIRICHLET;
    } else if(48 <= x && x <= 64 && y <= 0) {
        bound->value = -150;
        bound->cond  = DIRICHLET;
    } else if(16 <= x && x< 48) {
        if(12 < y && y <= 36) {
            bound->value = 0;
            bound->cond  = NONE;
        } else {
            bound->value = 0;
            bound->cond  = NEUMANN;
        }
//    } else if((x-10)*(x-10)+(y-20)*(y-30)<16) {
//            bound->value = 0;
//            bound->cond  = NEUMANN;
//    } else if((x-50)*(x-50)+(y-25)*(y-25) < 9) {
//            bound->value = -150;
//            bound->cond  = DIRICHLET;
    } else {
        bound->value = 0;
        bound->cond  = NONE;
    }

    return bound;
}

int main() {
    struct rect size = {64, 48};
    struct point dim = {160, 120};
    struct config conf = {8, 50, 0.5};
    struct lattice* lattice = lattice_new(&size, &dim, &boundaries);
    struct worker* worker;

//    printf("%d\n", lattice_compute(lattice, 0.005));
   printf("%d\n", lattice_compute_threaded(lattice, &conf));
 //   printf("%d\n", lattice_compute(lattice, 0.01));

   // worker = worker_new(NULL, lattice);
   // if(worker != NULL) pthread_join(worker->thread, NULL);

 //   lattice_print(lattice);
    lattice_delete(lattice);

	return 0;
}

