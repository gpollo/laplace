#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>

#include "lattice.h"
#include "worker.h"

/**
 * This function setups each of the cell in the lattice.
 */
void lattice_set_size(struct lattice* lattice, struct rect* size);

/**
 * This function applies the boundary function to each of the cell.
 */
void lattice_apply_bound(struct lattice* lattice, bound_t func);

/**
 * This function generates the function for each of the cell.
 */
void lattice_generate_function(struct lattice* lattice);

/**
 * This function applies one sequential iteration.
 */
double lattice_iterate(struct lattice* lattice);

struct lattice* lattice_new(struct rect* size, struct point* dim, bound_t func) {
    struct cell* cells;
    struct lattice* lattice;
    double (**update)(struct lattice*, struct cell*);

    /* make sure the dimension is useful */
    if(dim->x == 0 || dim->y == 0)
        return NULL;

    /* add two rows and two columns */
    dim->x += 2;
    dim->y += 2;

    /* compute the number of cell */
    uint32_t m = dim->x*dim->y;

    /* allocate memory for the cells */
    cells = malloc(m*sizeof(struct cell));
    if(cells == NULL) goto ERROR;

    /* allocate memory for the functions */
    update = malloc(m*sizeof(double (*)(struct lattice*, struct cell*)));
    if(update == NULL) goto ERROR;

    /* allocate the memory for the lattice structure */
    lattice = malloc(sizeof(struct lattice));
    if(lattice == NULL) goto ERROR;

    /* initialise the lattice structure */
    lattice->dim.x = dim->x;
    lattice->dim.y = dim->y;
    lattice->cells = cells;
    lattice->update = update;

    /* apply all the steps for finishing the lattice */
    lattice_set_size(lattice, size);
    lattice_apply_bound(lattice, func);
    lattice_generate_function(lattice);

    return lattice;

ERROR:
    if(cells   != NULL) free(cells);
    if(update  != NULL) free(update);
    if(lattice != NULL) free(lattice);

    return NULL;
}

void lattice_delete(struct lattice* lattice) {
    /* free all the allocated memory */
    free(lattice->cells);
    free(lattice->update);
    free(lattice);
}

void lattice_print(struct lattice* lattice) {
    /* extract the dimension of the lattice */
    uint32_t w = lattice->dim.x;
    uint32_t h = lattice->dim.y;

    for(uint32_t j = 0; j < h; j++) {
        for(uint32_t i = 0; i < w; i++) {
            /* extract the pointer to the specified cell */
            struct cell* cell = &lattice->cells[i+j*w];

            /* don't print cells if they contain neumann condition */
            if(cell->cond == NEUMANN)
                fprintf(stderr, "       ,");
            else
                fprintf(stderr, "% 07.5f,", cell->value);
        }
        fprintf(stderr, "\n");
    }
}

void lattice_set_size(struct lattice* lattice, struct rect* size) {
    /* extract the dimension of the lattice */
    uint32_t w = lattice->dim.x;
    uint32_t h = lattice->dim.y;

    /* compute the subdivisions */
    double dx = size->x/(w-2);
    double dy = size->y/(h-2);

    for(int32_t j = -1; j+1 < h; j++) {
        for(int32_t i = -1; i+1 < w; i++) {
            /* extract the pointer to the specified cell */
            struct cell* cell = &lattice->cells[(i+1)+(j+1)*w];

            /* initialise the cell */
            cell->pos.x = i*dx;
            cell->pos.y = j*dy;
            cell->index.x = i+1;
            cell->index.y = j+1;
            cell->value = 0;

            /* the limits of the lattice are Neumann conditions */
            if(i == -1 || j == -1 || i == w-2 || j == h-2) {
                /* we don't need the adjacent cells */
                cell->param[0] = 0;
                cell->param[1] = 0;
                cell->param[2] = 0;
                cell->param[3] = 0;

                cell->cond = NEUMANN;
            } else {
                /* compute the index of the adjacent cells */
                cell->param[0] = (i+1)+(j-0)*w;
                cell->param[1] = (i+1)+(j+2)*w;
                cell->param[2] = (i-0)+(j+1)*w;
                cell->param[3] = (i+2)+(j+1)*w;

                cell->cond = UNSET;
            }
        }
    }
}

void lattice_apply_bound(struct lattice* lattice, bound_t func) {
    /* extract the dimension of the lattice */
    uint32_t w = lattice->dim.x;
    uint32_t h = lattice->dim.y;

    /* used for returning data from the boundary function */
    struct bound bound = {NONE, 0};

    /* apply the boundary function to each cell */
    for(int32_t j = 0; j < h; j++) {
        for(int32_t i = 0; i < w; i++) {
            /* extract the pointer to the specified cell */
            struct cell* cell = &lattice->cells[i+j*w];

            /* make sure the cell isn't already set */
            if(cell->cond != UNSET)
                continue;

            /* apply the boundary function */
            if(func(&bound, &cell->pos) == NULL)
                continue;

            /* update the cell */
            cell->value = bound.value;
            cell->cond  = bound.cond;
        }
    }
}

/* definition of the supported configurations */
#define GETFORMULA40 (v1+v2+v3+v4)/4
#define GETFORMULA31 (2*v2+v3+v4)/4
#define GETFORMULA32 (2*v1+v3+v4)/4
#define GETFORMULA33 (2*v4+v1+v2)/4
#define GETFORMULA34 (2*v3+v1+v2)/4
#define GETFORMULA21 (v1+v4)/2
#define GETFORMULA22 (v2+v4)/2
#define GETFORMULA23 (v2+v3)/2
#define GETFORMULA24 (v1+v3)/2

/**
 * This macro generates function for the supported configurations.
 * Even though not each function needs all the adjacent point, the
 * compiler should optimize away points that are not needed.
 */
#define MAKE_FUNCPOINTS(NUM,IDX) \
double func##NUM##points##IDX (struct lattice* lattice, struct cell* cell) {\
    uint32_t i1 = cell->param[0];         \
    uint32_t i2 = cell->param[1];         \
    uint32_t i3 = cell->param[2];         \
    uint32_t i4 = cell->param[3];         \
                                          \
    double v1 = lattice->cells[i1].value; \
    double v2 = lattice->cells[i2].value; \
    double v3 = lattice->cells[i3].value; \
    double v4 = lattice->cells[i4].value; \
                                          \
    return GETFORMULA##NUM##IDX ;         \
}

/* we generate the function for each configuration */
MAKE_FUNCPOINTS(4,0)
MAKE_FUNCPOINTS(3,1)
MAKE_FUNCPOINTS(3,2)
MAKE_FUNCPOINTS(3,3)
MAKE_FUNCPOINTS(3,4)
MAKE_FUNCPOINTS(2,1)
MAKE_FUNCPOINTS(2,2)
MAKE_FUNCPOINTS(2,3)
MAKE_FUNCPOINTS(2,4)

void lattice_generate_function(struct lattice* lattice) {
    /* extract the dimension of the lattice */
    uint32_t w = lattice->dim.x;
    uint32_t h = lattice->dim.y;

    for(int32_t j = 0; j < h; j++) {
        for(int32_t i = 0; i < w; i++) {
            /* compute the index of the cell */
            uint32_t index = i+j*w;

            /* extract the pointer to the specified cell */
            struct cell* cell = &lattice->cells[index];

            /* we ignore neumann or dirichlet conditions */
            if(cell->cond == NEUMANN || cell->cond == DIRICHLET) {
                lattice->update[index] = NULL;

                continue;
            }

            /* extracts each adjacent cell */
            struct cell* c1 = &lattice->cells[cell->param[0]];
            struct cell* c2 = &lattice->cells[cell->param[1]];
            struct cell* c3 = &lattice->cells[cell->param[2]];
            struct cell* c4 = &lattice->cells[cell->param[3]];

            /* check if the adjacent cells are neumann boundary */
            int b1 = (c1->cond == NEUMANN) ? 1 : 0;
            int b2 = (c2->cond == NEUMANN) ? 1 : 0;
            int b3 = (c3->cond == NEUMANN) ? 1 : 0;
            int b4 = (c4->cond == NEUMANN) ? 1 : 0;

            /* generate a function the supported configurations */
            #define f lattice->update[index]
            if     (!b1 && !b2 && !b3 && !b4) f = &func4points0;
            else if( b1 && !b2 && !b3 && !b4) f = &func3points1;
            else if(!b1 &&  b2 && !b3 && !b4) f = &func3points2;
            else if(!b1 && !b2 &&  b3 && !b4) f = &func3points3;
            else if(!b1 && !b2 && !b3 &&  b4) f = &func3points4;
            else if( b1 && !b2 && !b3 &&  b4) f = &func2points1;
            else if(!b1 &&  b2 && !b3 &&  b4) f = &func2points2;
            else if(!b1 &&  b2 &&  b3 && !b4) f = &func2points3;
            else if( b1 && !b2 &&  b3 && !b4) f = &func2points4;
            else f = &func4points0;
            #undef f
        }
    }
}

uint32_t lattice_compute(struct lattice* lattice, double threshold) {
    uint32_t iterations = 0;

    /* apply iterations until the threshold is bigger */
    while(lattice_iterate(lattice) > threshold)
        iterations++;

    return iterations;
}

double lattice_iterate(struct lattice* lattice) {
    /* extract the dimension of the lattice */
    uint32_t w = lattice->dim.x;
    uint32_t h = lattice->dim.y;

    /* the largest difference */
    double diff = 0;

    for(int32_t j = 0; j < h; j++) {
        for(int32_t i = 0; i < w; i++) {
            /* compute the index of the cell */
            uint32_t index = i+j*w;
            double value, check;

            /* extract the pointer to the specified cell */
            struct cell* cell = &lattice->cells[index];

            /* make sure the cell can be updated */
            if(lattice->update[index] == NULL)
                continue;

            /* compute the new value */
            value = (*lattice->update[index])(lattice, cell);
            check = fabs(value-cell->value);
            if(check > diff) diff = check;

            /* update the cell */
            cell->value = value;
        }
    }

    return diff;
}

uint32_t lattice_compute_threaded(struct lattice* lattice, struct config* conf) {
    struct worker* worker;
    struct worker* previous;
    struct worker* next;
    uint32_t iterations = 0;

    /* create the first worker and wait */
    worker = worker_new(NULL, lattice, conf);
    pthread_join(worker->thread, NULL);

    /* traver the linked list */
    next = worker->next;
    worker->next = NULL;
    while(next != NULL) {
        /* count the total number of iterations */
        iterations += next->iterations;

        /* free the workers */
        next = next->next;
        if(next != NULL)
           worker_delete(next->previous);
    }

    /* free the last worker */
    worker_delete(worker);

    return iterations;
}

