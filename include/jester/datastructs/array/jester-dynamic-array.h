//
// Created by Case on 10/25/2025.
//

#ifndef JESTER_STDLIB_JESTER_DYNAMICARRAY_H
#define JESTER_STDLIB_JESTER_DYNAMICARRAY_H
#include <stdbool.h>
#include <stddef.h>

typedef struct DynamicArray
{
    void* data;
    size_t count;
    size_t capacity;
    size_t element_size;
} DynamicArray_t;

struct DynamicArray create_dynamic_array(size_t element_size, size_t capacity);
void push_dynamic_array(DynamicArray_t* a, const void* data);
void* get_dynamic_array_element(const DynamicArray_t* a, size_t i);
void clear_dynamic_array(DynamicArray_t* a);
void free_dynamic_array(DynamicArray_t* a);

void reserve_dynamic_array(DynamicArray_t* a, size_t new_capacity);
void shrink_dynamic_array(DynamicArray_t* a);
bool pop_dynamic_array(DynamicArray_t* a, void* dst);
void copy_dynamic_array(const DynamicArray_t* src, DynamicArray_t* dst);

#endif // JESTER_STDLIB_JESTER_DYNAMICARRAY_H
