/**
 * @headerfile jester_dynamic_array.h
 * @brief      Generic dynamic array implementation for the Jester stdlib.
 *
 * @details    Provides type-agnostic creation, push, get, clear, free,
 *             reserve, shrink, pop, and copy operations, somewhat
 *             similar to std::vector but implemented in plain C.
 *
 * @copyright  GPL-3.0
 * @author     Case Presley
 * @date       10-30-2025
 */

#ifndef JESTER_STDLIB_JESTER_DYNAMICARRAY_H
#define JESTER_STDLIB_JESTER_DYNAMICARRAY_H

//-------------------- INCLUDE FILES -------------------------┑
#include <stdlib.h>                                        // |
#include <stdbool.h>                                       // |
//------------------------------------------------------------┙

// ---------------------------------------------------------------------------------------------------------------

/**
 * @struct DynamicArray
 * @brief  Represents a resizable, generic array in the Jester stdlib.
 *
 * @var    DynamicArray::data
 *         Pointer to the raw memory storing the elements.
 *
 * @var    DynamicArray::count
 *         Current number of elements in use.
 *
 * @var    DynamicArray::capacity
 *         Total number of elements that can fit before the array is resized.
 *
 * @var    DynamicArray::element_size
 *         Size of each element within the array in bytes.
 */
typedef struct DynamicArray
{
    void*  data;
    size_t count;
    size_t capacity;
    size_t element_size;
} DynamicArray_t;

// ---------------------------------------------------------------------------------------------------------------

/**
 * @brief   Creates a dynamic array for elements of a fixed size.
 *
 * @details Allocates an initial buffer on the heap capable of holding
 *          the specified number of elements. The array can later grow
 *          automatically when new elements are pushed beyond its current capacity.
 *
 * @param   element_size  Size of each element in bytes (usually use sizeof(T)).
 * @param   capacity      Initial number of elements to allocate space for.
 *
 * @return  A DynamicArray_t instance with allocated storage. If allocation fails,
 *          the returned struct will have data = NULL and capacity = 0.
 *
 * @note    The array MUST be freed later using free_dynamic_array().
 * @see     push_dynamic_array(), free_dynamic_array()
 */
struct DynamicArray create_dynamic_array(size_t element_size, size_t capacity);

// ---------------------------------------------------------------------------------------------------------------

/**
 * @brief   Pushes a new element into a dynamic array
 *
 * @details Appends an element to the end of the given DynamicArray_t.
 *          If the array is full, its internal buffer is automatically
 *          reallocated to double its current capacity before insertion.
 *
 * @param   dynamic_array  Pointer to the target DynamicArray_t.
 * @param   data           Pointer to the element data to copy into the array.
 *
 * @return  Returns true on success, or false if a memory reallocation fails.
 */
bool push_dynamic_array(DynamicArray_t* dynamic_array, const void* data);

// ---------------------------------------------------------------------------------------------------------------

/**
 * @brief   Retrieves a pointer to an element within a dynamic array
 *
 * @details Returns a direct pointer to the element at the specified index.
 *          The returned pointer refers to the memory owned by the array and
 *          remains valid until the array is reallocated or freed.
 *          If the index is out of range, the function returns NULL.
 *
 * @param   dynamic_array  Pointer to the target DynamicArray_t.
 * @param   index          Zero-based index of the element to retrieve.
 *
 * @return  Pointer to the requested element, or NULL if the index is invalid.
 */
void* get_dynamic_array_element(const DynamicArray_t* dynamic_array, size_t index);

// ---------------------------------------------------------------------------------------------------------------

/**
 * @brief   Clears all elements from a dynamic array.
 *
 * @details Resets the element count of the given DynamicArray_t to zero,
 *          effectively marking the array as empty. This function does not
 *          deallocate or shrink the underlying memory. The capacity remains
 *          unchanged so that the array can be reused without reallocation.
 *
 * @param   dynamic_array Pointer to the target DynamicArray_t.
 *
 * @return  Always returns true to indicate the operation completed successfully.
 */
bool clear_dynamic_array(DynamicArray_t* dynamic_array);

// ---------------------------------------------------------------------------------------------------------------

/**
 * @brief   Frees and completely resets a dynamic array.
 *
 * @details Releases all memory owned by the given DynamicArray_t and
 *          resets its fields to a safe, zeroed state. After this call,
 *          the array is considered uninitialized and must not be used
 *          again unless re-created with create_dynamic_array().
 *          Calling this function on an already-empty array is safe and
 *          has no effect.
 *
 * @param   dynamic_array Pointer to the target DynamicArray_t.
 *
 * @return  Returns true if memory was freed, or false if the array was
 *          already empty.
 */
bool free_dynamic_array(DynamicArray_t* dynamic_array);

// ---------------------------------------------------------------------------------------------------------------

/**
 * @brief   Ensures the dynamic array has at least the specified capacity.
 *
 * @details Expands the internal memory buffer of the given DynamicArray_t
 *          if its current capacity is smaller than @p new_capacity. This
 *          function does not modify the current element count or initialize
 *          new memory, it simply guarantees that enough space is available
 *          for future insertions without additional reallocations.
 *
 * @param   dynamic_array  Pointer to the target DynamicArray_t.
 * @param   new_capacity   Desired minimum capacity (number of elements).
 *
 * @return  Returns true if the buffer was successfully reallocated or
 *          if the existing capacity already met the requirement.
 *          Returns false only if memory reallocations fails.
 */
bool reserve_dynamic_array(DynamicArray_t* dynamic_array, size_t new_capacity);

// ---------------------------------------------------------------------------------------------------------------

/**
 * @brief   Shrinks the allocated memory of a dynamic array to fit its contents.
 *
 * @details Reduces the internal capacity of the given DynamicArray_t so that it
 *          matches the current element count. This function releases any unused
 *          memory beyond the number of active elements. If the array is empty,
 *          all allocated memory is freed and the array is reset to a zeroed state.
 *
 * @param   dynamic_array  Pointer to the target DynamicArray_t.
 *
 * @return  Returns true if the buffer was successfully shrunk or freed,
 *          or false if a memory reallocation failed.
 *
 * @note    Shrinking an array can help with memory usage, but may result in
 *          additional reallocations if new elements are added later.
 */
bool shrink_dynamic_array(DynamicArray_t* dynamic_array);

// ---------------------------------------------------------------------------------------------------------------

/**
 * @brief   Removes the last element from a dynamic array.
 *
 * @details Decrements the element count of the given DynamicArray_t and
 *          optionally copies the removed element into the user-provided
 *          @p destination buffer. The internal memory buffer remains
 *          allocated and unchanged in size. If the array is empty, no
 *          action is taken.
 *
 * @param   dynamic_array  Pointer to the target DynamicArray_t.
 * @param   destination    Optional pointer to a buffer that will receive
 *                         a copy of the removed element. Pass NULL if the
 *                         value is not needed.
 *
 * @return  Returns true if an element was successfully removed,
 *          or false if the array was already empty.
 */
bool pop_dynamic_array(DynamicArray_t* dynamic_array, void* destination);

// ---------------------------------------------------------------------------------------------------------------

/**
 * @brief   Copies all elements from one dynamic array to another.
 *
 * @details Performs a deep copy of the given DynamicArray_t into the
 *          destination. this function allocates a new memory buffer for the
 *          destination array sized to match the source's capacity, and copies
 *          all active elements (up to @p count) into it. The destination array
 *          should be uninitialized or freed before calling this function to
 *          avoid memory leaks.
 *
 * @param   dynamic_array_source       Pointer to the source DynamicArray_t to copy from.
 * @param   dynamic_array_destination  Pointer to the destination DynamicArray_t to copy into.
 *
 * @return  Returns true if an element was successfully copied, or false if a
 *          memory allocation failed or the source array contained no data.
 */
bool copy_dynamic_array(const DynamicArray_t* dynamic_array_source, DynamicArray_t* dynamic_array_destination);

// ---------------------------------------------------------------------------------------------------------------

#endif
