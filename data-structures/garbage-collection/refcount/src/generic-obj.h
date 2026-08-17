#include <stdbool.h>
#include <stddef.h>

typedef struct GenericObject generic_object_t;

int genobj_length(generic_object_t *obj);
generic_object_t *genobj_add(generic_object_t *a, generic_object_t *b);

typedef struct {
  size_t size;
  generic_object_t **elements;
} generic_array_t;

typedef struct {
  generic_object_t *x;
  generic_object_t *y;
  generic_object_t *z;
} generic_vector_t;

typedef enum GenericObjectKind {
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY,
} generic_object_kind_t;

typedef union GenericObjectData {
  int v_int;
  float v_float;
  char *v_string;
  generic_vector_t v_vector3;
  generic_array_t v_array;
} generic_object_data_t;

typedef struct GenericObject {
  int refcount;
  generic_object_kind_t kind;
  generic_object_data_t data;
} generic_object_t;

generic_object_t *new_generic_integer(int value);
generic_object_t *new_generic_float(float value);
generic_object_t *new_generic_string(char *value);
generic_object_t *new_generic_vector3(generic_object_t *x, generic_object_t *y,
                                generic_object_t *z);

generic_object_t *new_generic_array(size_t size);

void refcount_inc(generic_object_t *obj);
void refcount_dec(generic_object_t *obj);
void refcount_free(generic_object_t *obj);

bool generic_array_set(generic_object_t *array, size_t index, generic_object_t *value);
generic_object_t *generic_array_get(generic_object_t *array, size_t index);
