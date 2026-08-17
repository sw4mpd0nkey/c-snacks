#include "generic-obj-new.h"
#include "generic-object.h"
#include "vm.h"

#include <stdlib.h>
#include <string.h>

generic_object_t *_new_generic_object(vm_t *vm) {
  generic_object_t *obj = calloc(1, sizeof(generic_object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->is_marked = false;

  vm_track_object(vm, obj);

  return obj;
}

generic_object_t *new_generic_array(vm_t *vm, size_t size) {
  generic_object_t *obj = _new_generic_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  generic_object_t **elements = calloc(size, sizeof(generic_object_t *));
  if (elements == NULL) {
    free(obj);
    return NULL;
  }

  obj->kind = ARRAY;
  obj->data.v_array = (generic_array_t){.size = size, .elements = elements};

  return obj;
}

generic_object_t *new_generic_vector3(vm_t *vm, generic_object_t *x, generic_object_t *y,
                                generic_object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  generic_object_t *obj = _new_generic_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = VECTOR3;
  obj->data.v_vector3 = (generic_vector_t){.x = x, .y = y, .z = z};

  return obj;
}

generic_object_t *new_generic_integer(vm_t *vm, int value) {
  generic_object_t *obj = _new_generic_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = INTEGER;
  obj->data.v_int = value;

  return obj;
}

generic_object_t *new_generic_float(vm_t *vm, float value) {
  generic_object_t *obj = _new_generic_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = FLOAT;
  obj->data.v_float = value;
  return obj;
}

generic_object_t *new_generic_string(vm_t *vm, char *value) {
  generic_object_t *obj = _new_generic_object(vm);
  if (obj == NULL) {
    return NULL;
  }

  int len = strlen(value);
  char *dst = malloc(len + 1);
  if (dst == NULL) {
    free(obj);
    return NULL;
  }

  strcpy(dst, value);

  obj->kind = STRING;
  obj->data.v_string = dst;
  return obj;
}
