#include <stdlib.h>
#include <stdio.h>

#if ALLOC_DEBUG == 1
#define alloc_debug(a ...) printf("ALC> " a);
#else
#define alloc_debug(...)
#endif

// CHUNKS
#define CHUNK_QTY 0x4000
typedef struct
{
	void *start;
	size_t size;
} _chunk_t;
typedef _chunk_t _chunk_list_t[CHUNK_QTY];

// Used chunks
static _chunk_list_t allocated = {0};
static _chunk_list_t freed = {0};

int insert_chunk(_chunk_t *pChunk, _chunk_list_t list)
{
	for (int i = 0; i < CHUNK_QTY; i++)
	{
		// Find an empty chunk
		if (list[i].start == 0 && list[i].size == 0)
		{
			list[i].start = pChunk->start;
			list[i].size = pChunk->size;
			return 1;
		}
	}
	return 0;
}

int merge_remove_chunk(_chunk_t *pChunk, _chunk_list_t list)
{
	for (int i = 0; i < CHUNK_QTY; i++)
	{
		// Check if list is before the chunk
		if (list[i].start + list[i].size == pChunk->start)
		{
			list[i].size += pChunk->size;
			pChunk->size = 0;
			pChunk->start = 0;
			merge_remove_chunk(&list[i], list);
			return 1;
		}
		// Check if list is after the chunk
		if (pChunk->start + pChunk->size == list[i].start)
		{
			list[i].start = pChunk->start;
			list[i].size = pChunk->size + list[i].size;
			pChunk->size = 0;
			pChunk->start = 0;
			merge_remove_chunk(&list[i], list);
			return 1;
		}
	}
	return 0;
}

int merge_chunk(_chunk_t *pChunk, _chunk_list_t list)
{
	for (int i = 0; i < CHUNK_QTY; i++)
	{
		// Check if list is before the chunk
		if (list[i].start + list[i].size == pChunk->start)
		{
			list[i].size += pChunk->size;
			merge_remove_chunk(&list[i], list);
			return 1;
		}
		// Check if list is after the chunk
		if (pChunk->start + pChunk->size == list[i].start)
		{
			list[i].start = pChunk->start;
			list[i].size = pChunk->size + list[i].size;
			merge_remove_chunk(&list[i], list);
			return 1;
		}
	}

	return 0;
}

int remove_chunk_by_size(_chunk_t *pChunk, _chunk_list_t list, const size_t size)
{
	for (int i = 0; i < CHUNK_QTY; ++i)
	{
		if (list[i].size >= size)
		{
			pChunk->start = list[i].start;
			pChunk->size = size;
			list[i].size -= size;
			if (list[i].size == 0)
				list[i].start = NULL;
			else
				list[i].start += size;
			return 1;
		}
	}
	return 0;
}

int remove_chunk_by_address(_chunk_t *pChunk, _chunk_list_t list, const void *ptr)
{
	for (int i = 0; i < CHUNK_QTY; ++i)
	{
		_chunk_t *chunk = &list[i];
		if (list[i].start == ptr)
		{
			pChunk->size = chunk->size;
			pChunk->start = chunk->start;
			chunk->size = 0;
			chunk->start = 0;
			return 1;
		}
	}
	return 0;
}

void alloc_init(void *start, size_t cap)
{
	// Reset the chunk lists
	for (int i = 0; i < CHUNK_QTY; ++i)
	{
		freed[i].size = 0;
		freed[i].start = 0;
		allocated[i].size = 0;
		allocated[i].start = 0;
	}

	// Set the first chunk. This is this chunk that will be 
	// cut and merge to create the entire alloc-free cycle!
	freed[0].start = start;
	freed[0].size = cap;

#if ALLOC_DEBUG == 2
	for (int i = 0; i < CHUNK_QTY; ++i)
	{
		if (freed[i].size == 0) break;
		printf("[%p,%X]", freed[i].start, freed[i].size);
	}
	printf("\n");
#endif
}

void free(void *ptr)
{
	// Ignore if try to free NULL
	if (ptr == NULL) {
		alloc_debug("W:ptr is NULL\n", ptr);
		return;
	}

	_chunk_t chunk;
	if (remove_chunk_by_address(&chunk, allocated, ptr) == 0)
	{
		alloc_debug("W:ptr %p is not allocated\n", ptr);
		return;
	}

	if (merge_chunk(&chunk, freed) == 0 && insert_chunk(&chunk, freed) == 0)
	{
		alloc_debug("W:Fail to insert the chunk!\n");
		return;
	}

	alloc_debug("I:Free %p\n", ptr);

#if ALLOC_DEBUG == 2
	for (int i = 0; i < CHUNK_QTY; ++i)
	{
		if (freed[i].size == 0) break;
		printf("[%p,%X]", freed[i].start, freed[i].size);
	}
	printf("\n");
#endif
}

void *malloc(size_t size)
{
	if (size == 0) {
		alloc_debug("E:Try to alloc 0 Bytes\n");
		return NULL;
	}

	_chunk_t chunk;
	if (remove_chunk_by_size(&chunk, freed, size) == 0)
	{
		alloc_debug("E:Heap is full\n");
		return NULL;
	}

	if (insert_chunk(&chunk, allocated) == 0)
	{
		alloc_debug("E:Fail to insert chunk\n");
		return NULL;
	}
	alloc_debug("I:Alloc %p\n", chunk.start);

#if ALLOC_DEBUG == 2
	for (int i = 0; i < CHUNK_QTY; ++i)
	{
		if (freed[i].size == 0) break;
		printf("[%p,%X]", freed[i].start, freed[i].size);
	}
	printf("\n");
#endif
	return chunk.start;
}