#include "jester/datastructs/array/jester-dynamic-array.h"

#include <stdlib.h>
#include <string.h>

struct DynamicArray create_dynamic_array(const size_t element_size, const size_t capacity)
{
    DynamicArray_t dynamic_array = {};

    const size_t total_byte = element_size * capacity;
    dynamic_array.data = malloc(total_byte);
    if (dynamic_array.data == NULL) return dynamic_array;

    dynamic_array.element_size = element_size;
    dynamic_array.count = 0;
    dynamic_array.capacity = capacity;

    return dynamic_array;
}

void push_dynamic_array(DynamicArray_t* a, const void* data)
{
    if (a->count == a->capacity)
    {
        const size_t new_capacity = a->capacity * 2;
        const size_t new_size = a->element_size * new_capacity;
        void* temp_ptr = realloc(a->data, new_size);
        if (temp_ptr)
        {
            a->data = temp_ptr;
            a->capacity = new_capacity;
        }
        else
        {
            return;
        }
    }

    char* new_destination = (char*)a->data + (a->count * a->element_size);
    memcpy(new_destination, data, a->element_size);

    a->count++;
}

void* get_dynamic_array_element(const DynamicArray_t* a, const size_t i)
{
    if (i < a->count)
    {
        return (char*)a->data + (a->element_size * i);
    }
    else
    {
        return NULL;
    }
}

void clear_dynamic_array(DynamicArray_t* a)
{
    a->count = 0;
}

void free_dynamic_array(DynamicArray_t* a)
{
    if (a->data != NULL)
    {
        free(a->data);
    }
    a->data = NULL;
    a->count = 0;
    a->capacity = 0;
    a->element_size = 0;
}

void reserve_dynamic_array(DynamicArray_t* a, size_t new_capacity)
{
    if (a->capacity < new_capacity)
    {
        const size_t new_size = new_capacity * a->element_size;
        void* temp_ptr = realloc(a->data, new_size);
        if (temp_ptr)
        {
            a->data = temp_ptr;
            a->capacity = new_capacity;
        }
        else
        {
            return;
        }
    }
}

void shrink_dynamic_array(DynamicArray_t* a)
{
    if (a->count == 0)
    {
        free(a->data);
        a->data = NULL;
        a->capacity = 0;
        a->element_size = 0;
        return;
    }

    const size_t new_size = a->element_size * a->count;
    void* temp_ptr = realloc(a->data, new_size);
    if (temp_ptr)
    {
        a->data = temp_ptr;
        a->capacity = a->count;
    }
}

bool pop_dynamic_array(DynamicArray_t* a, void* dst)
{
    if (a->count == 0)
    {
        return false;
    }
    a->count--;
    const char* src = (char*)a->data + (a->element_size * a->count);

    if (dst)
    {
        memcpy(dst, src, a->element_size);
    }
    return true;
}

void copy_dynamic_array(const DynamicArray_t* src, DynamicArray_t* dst)
{
    dst->count = src->count;
    dst->element_size = src->element_size;
    dst->capacity = src->capacity;
    dst->data = malloc(src->element_size * src->capacity);

    if (dst->data == NULL)
    {
        dst->count = 0;
        dst->element_size = 0;
        dst->capacity = 0;
        return;
    }

    if (src->data)
    {
        memcpy(dst->data, src->data, src->element_size * src->count);
    }
}
