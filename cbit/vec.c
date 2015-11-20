#include "vec.h"
#include <stdio.h>
#include <stdlib.h>

void vec_realloc_internal(struct vec_internal *vi, size_t new_capacity,
                          size_t esize) {
    if (new_capacity == 0) {
        if (vi->els != vi->storage)
            free(vi->els);
        vi->els = NULL;
        vi->capacity = 0;
        return;
    }
    size_t new_size = safe_mul(new_capacity, esize);
    if (vi->els == vi->storage) {
        void *new_buf = malloc(new_size);
        size_t min_cap = cbit_min(new_capacity, vi->capacity);
        memcpy(new_buf, vi->els, min_cap * esize);
        vi->els = new_buf;
    } else {
        vi->els = realloc(vi->els, new_size);
    }
    vi->capacity = new_capacity;
}
void vec_realloc_internal_as_necessary(struct vec_internal *vi,
                                       size_t min_capacity,
                                       size_t esize) {
    if (min_capacity > vi->capacity)
        vec_realloc_internal(vi,
            cbit_max(min_capacity, safe_mul(vi->capacity, 2)),
            esize);
    else if (min_capacity < vi->capacity / 3)
        vec_realloc_internal(vi, vi->capacity / 3, esize);
}
