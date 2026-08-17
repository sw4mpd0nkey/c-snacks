#pragma once

#include "genericobject.h"
#include "vm.h"

generic_object_t *new_generic_integer(vm_t *vm, int value);
generic_object_t *new_generic_float(vm_t *vm, float value);
generic_object_t *new_generic_string(vm_t *vm, char *value);
generic_object_t *new_generic_vector3(vm_t *vm, generic_object_t *x, generic_object_t *y,
                                generic_object_t *z);
generic_object_t *new_generic_array(vm_t *vm, size_t size);