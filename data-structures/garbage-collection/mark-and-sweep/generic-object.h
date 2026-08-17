#include "stack.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct GenericObject generic_object_t;

typedef struct {
  size_t size;
  generic_object_t **elements;
} generic_array_t;

typedef struct {
  generic_object_t *x;
  generic_object_t *y;
  generic_object_t *z;
} generic_vector_t;

typedef enum genericObjectKind {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY,
} generic_object_kind_t;

typedef union genericObjectData {
  int v_int;
  float v_float;
  char *v_string;
  generic_vector_t v_vector3;
  generic_array_t v_array;
} generic_object_data_t;

typedef struct genericObject {
  bool is_marked;

  generic_object_kind_t kind;
  generic_object_data_t data;
} generic_object_t;

void generic_object_free(generic_object_t *obj);

bool generic_array_set(generic_object_t *array, size_t index, generic_object_t *value);
generic_object_t *generic_array_get(generic_object_t *array, size_t index);
