#ifndef VECTOR_H
#define VECTOR_H

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 

// vector type
typedef struct
{
	unsigned int size;
	unsigned int current;
	unsigned int* contents;
} Vector;

void init(Vector *vec, int data);

void destroy(Vector *vec);

void clear(Vector* vec);

void add(Vector* vec, int val);

int get(Vector* vec, int index);

void set(Vector* vec, int index, int val);

int next(Vector* vec);

int size(Vector* vec);

#endif // VECTOR_H