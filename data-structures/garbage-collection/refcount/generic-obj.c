#include "generic-obj.h"
#include <stdlib.h>
#include <string.h>

generic_object_t *generic_add(generic_object_t *a, generic_object_t *b) {
  if (a == NULL || b == NULL) {
    return NULL;
  }

  switch (a->kind) {
    case INTEGER:
      if (b->kind == INTEGER) {
        return new_generic_integer(a->data.v_int + b->data.v_int);
      }
      if (b->kind == FLOAT) {
        return new_generic_float( ((float)a->data.v_int) + b->data.v_float);
      }
      return NULL;
    case FLOAT:
      if (b->kind == INTEGER) {
        return new_generic_integer(((int)a->data.v_float) + b->data.v_int);
      }
      if (b->kind == FLOAT) {
        return new_generic_float(a->data.v_float + b->data.v_float);
      }
      return NULL;
    case STRING:
      if (b != NULL && b->kind != STRING) {
        return NULL;
      } else {
        int new_length = strlen(b->data.v_string) + strlen(b->data.v_string) +2;
        char *new_str = calloc(new_length, sizeof(char));
        strcat(new_str,a->data.v_string);
        strcat(new_str, b->data.v_string);
        generic_object_t *to_return = new_generic_string(new_str);
        free(new_str);
        return to_return;
      }
      
      return NULL;
    case VECTOR3:
      if (b != NULL && b->kind != VECTOR3) {
        return NULL;
      } else {
        return new_generic_vector3(
          generic_add(a->data.v_vector3.x,b->data.v_vector3.x),
          generic_add(a->data.v_vector3.y,b->data.v_vector3.y),
          generic_add(a->data.v_vector3.z,b->data.v_vector3.z)
        );
      }
      
      return NULL;
    case ARRAY:
      if (b != NULL && b->kind != ARRAY) {
        return NULL;
      } else {
        size_t new_size = a->data.v_array.size + b->data.v_array.size;
        generic_object_t *new_array = new_generic_array(new_size);
        int index = 0;
        
        for (int i = 0; i < a->data.v_array.size; i++) {
          generic_array_set(new_array, index, generic_array_get(a,i));
          index++;
        }
        for (int i = 0; i < b->data.v_array.size; i++) {
          generic_array_set(new_array, index, generic_array_get(b,i));
          index++;
        }

        return new_array;
      }
      
      return NULL;
    default:
      return NULL;
  }
}

int generic_length(generic_object_t *obj) {
  if (obj == NULL) {
    return -1;
  }

  switch (obj->kind) {
  case INTEGER:
    return 1;
  case FLOAT:
    return 1;
  case STRING:
    return strlen(obj->data.v_string);
  case VECTOR3:
    return 3;
  case ARRAY:
    return obj->data.v_array.size;
  default:
    return -1;
  }
}

generic_object_t *_new_generic_object() {
  generic_object_t *obj = calloc(1, sizeof(generic_object_t));
  if (obj == NULL) {
    return NULL;
  }

  obj->refcount = 1;

  return obj;
}

generic_object_t *new_generic_array(size_t size) {
  generic_object_t *obj = _new_generic_object();
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

bool generic_array_set(generic_object_t *array, size_t index, generic_object_t *value) {
  if (array == NULL || value == NULL) {
    return false;
  }

  if (array->kind != ARRAY) {
    return false;
  }

  if (index >= array->data.v_array.size) {
    return false;
  }

  refcount_inc(value);
  if (array->data.v_array.elements[index] != NULL) {
    refcount_dec(array->data.v_array.elements[index]);
  }

  array->data.v_array.elements[index] = value;
  return true;
}

generic_object_t *generic_array_get(generic_object_t *array, size_t index) {
  if (array == NULL) {
    return NULL;
  }

  if (array->kind != ARRAY) {
    return NULL;
  }

  if (index >= array->data.v_array.size) {
    return NULL;
  }

  return array->data.v_array.elements[index];
}

generic_object_t *new_generic_vector3(generic_object_t *x, generic_object_t *y,
                                generic_object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  generic_object_t *obj = _new_generic_object()
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = VECTOR3;
  obj->data.v_vector3 = (generic_vector_t){.x = x, .y = y, .z = z};
  refcount_inc(x);
  refcount_inc(y);
  refcount_inc(z);
  return obj;
}

generic_object_t *new_generic_integer(int value) {
  generic_object_t *obj = _new_generic_object();
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = INTEGER;
  obj->data.v_int = value;
  return obj;
}

generic_object_t *new_generic_float(float value) {
  generic_object_t *obj = _new_generic_object();
  if (obj == NULL) {
    return NULL;
  }

  obj->kind = FLOAT;
  obj->data.v_float = value;
  return obj;
}

generic_object_t *new_generic_string(char *value) {
  generic_object_t *obj = _new_generic_object();
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

void refcount_inc(generic_object_t *obj) {
  if (obj == NULL) {
    return;
  }

  obj->refcount++;
  return;
}

void refcount_dec(generic_object_t *obj) {
  if (obj == NULL) {
    return;
  }
  obj->refcount--;
  if (obj->refcount == 0) {
    return refcount_free(obj);
  }
  return;
}

void refcount_free(generic_object_t *obj) {
  switch (obj->kind) {
  case INTEGER:
  case FLOAT:
    break;
  case STRING:
    free(obj->data.v_string);
    break;
  case VECTOR3: 
    generic_vector_t vec = obj->data.v_vector3;
    refcount_dec(vec.x);
    refcount_dec(vec.y);
    refcount_dec(vec.z);
    break;
  case ARRAY:
    for (int i = 0; i <= obj->data.v_array.size - 1; i++) {
      refcount_dec(obj->data.v_array.elements[i]);
    }
    free(obj->data.v_array.elements);
    break;
  default:
    assert(false);
  }
  free(obj);
}