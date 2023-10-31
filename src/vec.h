#ifndef _AR_VEC_H
#define _AR_VEC_H

struct _arvec {
    void* values;
    int size;
    int cap;
    int type_size;
};

#define Vec(T) struct _arvec*

#define arvec_create(T, cap) arvec_create_m(cap, sizeof(T))
struct _arvec* arvec_create_m(int cap, int type_size);

void arvec_free(struct _arvec* arvec);

/// Copies the value to the vec
void arvec_append(struct _arvec*, void* value);

void arvec_assureMinCap(struct _arvec*, int);

/// Invalidates all elements, but keeps the allocated memory
void arvec_clear(struct _arvec* arvec);

#endif
