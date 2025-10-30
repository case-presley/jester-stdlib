/**
 * @file      jester-dynamic-array.c
 * @brief     Implementation of the generic dynamic array for the Jester stdlib.
 *
 * @details   Provides internal logic for creation, resizing, and management of
 *            dynamically allocated, type-agnostic arrays. This implementation
 *            supports push, pop, clear, shrink, reserve, copy, and free operations.
 *            All allocation is currently handled via malloc/realloc/free, but will
 *            later be replaced by Jester’s custom memory subsystem.
 *
 * @copyright GPL-3.0
 * @author    Case Presley
 * @date      10-30-2025
 *
 * @note      Assertions and error handling will be integrated once
 *            jester-utils is complete.
 */

//-------------------- INCLUDE FILES -------------------------┑
#include "jester/data_structures/array/jester_dynamic_array.h" // |
#include <stdlib.h>                                        // |
#include <string.h>                                        // |
//------------------------------------------------------------┙

//-----------------------------------------------------┑
// TODO: Add assertions when utils is integrated.      |
// For now, return booleans for simple success/failure |
//-----------------------------------------------------┙
struct DynamicArray create_dynamic_array(const size_t element_size, const size_t capacity)
{
    DynamicArray_t dynamic_array = {};  // initialize to defaults

    // --- allocate initial buffer ---
    const size_t total_byte = element_size * capacity;
    dynamic_array.data      = malloc(total_byte);
    if (dynamic_array.data == NULL) return dynamic_array;

    // --- initialize fields ---
    dynamic_array.element_size = element_size;
    dynamic_array.count        = 0;
    dynamic_array.capacity     = capacity;

    return dynamic_array;
}

//-----------------------------------------------------┑
// TODO: Add assertions when utils is integrated.      |
// For now, return booleans for simple success/failure |
//-----------------------------------------------------┙
bool push_dynamic_array(DynamicArray_t* a, const void* data)
{
    // --- grow capacity if full ---
    if (a->count == a->capacity)
    {
        const size_t new_capacity = a->capacity * 2;                 // double capacity
        const size_t new_size     = a->element_size * new_capacity;  //
        void* temp_ptr            = realloc(a->data, new_size);      // attempt to grow the buffer

        if (temp_ptr)
        {
            a->data     = temp_ptr;
            a->capacity = new_capacity;
        }
        else
        {
            return false;  // reallocation failed
        }
    }

    // --- copy new element into array ---
    char* new_destination = (char*)a->data + (a->count * a->element_size);
    memcpy(new_destination, data, a->element_size);
    a->count++;

    return true;
}

//-----------------------------------------------------┑
// TODO: Add assertions when utils is integrated.      |
// For now, return NULL when index is out of bounds    |
//-----------------------------------------------------┙
void* get_dynamic_array_element(const DynamicArray_t* a, const size_t i)
{
    // --- validate index ---
    if (i >= a->count)
    {
        return NULL;  // invalid index, nothing to return
    }

    // --- compute and return the elements address ---
    return (char*)a->data + (a->element_size * i);
}

//-----------------------------------------------------┑
// TODO: Add assertions when utils is integrated.      |
// For now, return booleans for simple success/failure |
//-----------------------------------------------------┙
bool clear_dynamic_array(DynamicArray_t* a)
{
    // --- mark array as empty (reuse existing capacity) ---
    a->count = 0;
    return true;
}

//-----------------------------------------------------┑
// TODO: Add assertions when utils is integrated.      |
// For now, return booleans for simple success/failure |
//-----------------------------------------------------┙
bool free_dynamic_array(DynamicArray_t* a)
{
    // --- free the allocated memory, if any ---
    if (a->data != NULL)
    {
        free(a->data);
        a->data = NULL;
    }
    else
    {
        return false;  // nothing to free
    }

    // --- reset the metadata fields to safe defaults ---
    a->count        = 0;
    a->capacity     = 0;
    a->element_size = 0;

    return true;
}

//-----------------------------------------------------┑
// TODO: Add assertions when utils is integrated.      |
// For now, return booleans for simple success/failure |
//-----------------------------------------------------┙
bool reserve_dynamic_array(DynamicArray_t* a, size_t new_capacity)
{
    // --- check if the current capacity is sufficient ---
    if (a->capacity >= new_capacity)
    {
        return true;  // already large enough
    }

    // --- attempt to reallocate to the new capacity ---
    const size_t new_size = new_capacity * a->element_size;
    void* temp_ptr        = realloc(a->data, new_size);

    if (temp_ptr)
    {
        a->data     = temp_ptr;
        a->capacity = new_capacity;
        return true;  // successfully reallocated
    }

    // --- allocation failed ---
    return false;
}

//-----------------------------------------------------┑
// TODO: Add assertions when utils is integrated.      |
// For now, return booleans for simple success/failure |
//-----------------------------------------------------┙
bool shrink_dynamic_array(DynamicArray_t* a)
{
    // --- handle empty array: free all memory and reset fields ---
    if (a->count == 0)
    {
        free(a->data);
        a->data         = NULL;
        a->capacity     = 0;
        a->element_size = 0;
        return false;  // nothing to shrink
    }

    // --- shrink buffer to match current element count ---
    const size_t new_size = a->element_size * a->count;
    void* temp_ptr = realloc(a->data, new_size);

    if (temp_ptr)
    {
        a->data     = temp_ptr;
        a->capacity = a->count;
        return true;  // successfully shrunk
    }

    // --- reallocation failed ---
    return false;
}

//-----------------------------------------------------┑
// TODO: Add assertions when utils is integrated.      |
// For now, return booleans for simple success/failure |
//-----------------------------------------------------┙
bool pop_dynamic_array(DynamicArray_t* a, void* dst)
{
    // --- ensure array is not empty ---
    if (a->count == 0)
    {
        return false;  // nothing to pop
    }

    // --- locate the last element ---
    a->count--;
    const char* src = (char*)a->data + (a->element_size * a->count);

    // --- optionally copy element into user buffer ---
    if (dst)
    {
        memcpy(dst, src, a->element_size);
    }

    return true;  // element successfully removed
}

//-----------------------------------------------------┑
// TODO: Add assertions when utils is integrated.      |
// For now, return booleans for simple success/failure |
//-----------------------------------------------------┙
bool copy_dynamic_array(const DynamicArray_t* src, DynamicArray_t* dst)
{
    // --- copy metadata fields ---
    dst->count        = src->count;
    dst->element_size = src->element_size;
    dst->capacity     = src->capacity;

    // --- allocate new buffer for destination ---
    dst->data = malloc(src->element_size * src->capacity);
    if (dst->data == NULL)
    {
        // --- allocation failed: reset destination to safe defaults ---
        dst->count        = 0;
        dst->element_size = 0;
        dst->capacity     = 0;
        return false;
    }

    // --- perform deep copy if source has data ---
    if (src->data)
    {
        memcpy(dst->data, src->data, src->element_size * src->count);
        return true;  // successfully copied contents
    }

    // --- source had no data to copy ---
    return false;
}
