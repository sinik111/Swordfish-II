#pragma once

/**
 * Macro-based "template" dynamic array for C.
 * Use instead of union-based list: one array type per element type.
 *
 * Usage:
 *   #include "Bullet.h"
 *   DEFINE_DYNAMIC_ARRAY(Bullet, 32)
 *   DEFINE_DYNAMIC_ARRAY_REMOVE_BY_ID(Bullet)
 *
 *   DynamicArray_Bullet* arr = DynamicArray_Bullet_Create(32);
 *   DynamicArray_Bullet_Push(arr, &bullet);
 *   for (size_t i = 0; i < arr->count; i++) { ... &arr->items[i] ... }
 *   DynamicArray_Bullet_RemoveById(arr, id);
 *   DynamicArray_Bullet_Destroy(arr);
 */

#include <stdlib.h>
#include "TypeDefines.h"

#define DEFINE_DYNAMIC_ARRAY(T, INIT_CAP) \
typedef struct DynamicArray_##T { \
	T* items; \
	size_t count; \
	size_t capacity; \
} DynamicArray_##T; \
\
static inline DynamicArray_##T* DynamicArray_##T##_Create(size_t initial_capacity) { \
	size_t cap = (initial_capacity) > 0 ? (initial_capacity) : (INIT_CAP); \
	DynamicArray_##T* arr = (DynamicArray_##T*)malloc(sizeof(DynamicArray_##T)); \
	if (!arr) return NULL; \
	arr->items = (T*)malloc(cap * sizeof(T)); \
	if (!arr->items) { free(arr); return NULL; } \
	arr->count = 0; \
	arr->capacity = cap; \
	return arr; \
} \
\
static inline void DynamicArray_##T##_Destroy(DynamicArray_##T* arr) { \
	if (!arr) return; \
	free(arr->items); \
	arr->items = NULL; \
	arr->count = arr->capacity = 0; \
	free(arr); \
} \
\
static inline void DynamicArray_##T##_Push(DynamicArray_##T* arr, const T* item) { \
	if (!arr || !item) return; \
	if (arr->count >= arr->capacity) { \
		size_t new_cap = arr->capacity * 2; \
		T* p = (T*)realloc(arr->items, new_cap * sizeof(T)); \
		if (!p) return; \
		arr->items = p; \
		arr->capacity = new_cap; \
	} \
	arr->items[arr->count++] = *item; \
} \
\
static inline void DynamicArray_##T##_RemoveAt(DynamicArray_##T* arr, size_t index) { \
	if (!arr || index >= arr->count) return; \
	if (index != arr->count - 1) \
		arr->items[index] = arr->items[arr->count - 1]; \
	arr->count--; \
} \
\
static inline void DynamicArray_##T##_Clear(DynamicArray_##T* arr) { \
	if (arr) arr->count = 0; \
} \
\
static inline T* DynamicArray_##T##_At(DynamicArray_##T* arr, size_t index) { \
	if (!arr || index >= arr->count) return NULL; \
	return &arr->items[index]; \
}

/**
 * Optional: adds RemoveById for element types that have a long long 'id' field.
 * Use after DEFINE_DYNAMIC_ARRAY(T, cap).
 */
#define DEFINE_DYNAMIC_ARRAY_REMOVE_BY_ID(T) \
static inline int DynamicArray_##T##_RemoveById(DynamicArray_##T* arr, long long id) { \
	if (!arr) return 0; \
	for (size_t i = 0; i < arr->count; i++) { \
		if (arr->items[i].id == id) { \
			DynamicArray_##T##_RemoveAt(arr, i); \
			return 1; \
		} \
	} \
	return 0; \
}
