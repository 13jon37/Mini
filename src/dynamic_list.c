#include "include/dynamic_list.h"

#include <stdlib.h>

internal dynamic_list_t *
init_dynamic_list(size_t item_size)
{
    dynamic_list_t *list = calloc(1, sizeof(struct DYNAMIC_LIST_STRUCT));
    
    list->item_size = item_size;
    list->length = 0; 
    list->items = (void*) 0;
    
    return list;
}

internal void
dynamic_list_append(dynamic_list_t *list, void *item)
{
    list->size += 1; // Increase size because we're appending an item
    
    if (list->items == (void*) 0)
    {
        list->items = calloc(list->length, list->item_size);
    }
    else
    {
        list->items = realloc(list->items, list->length * list->item_size);
    }
    
    list->items[list->size - 1] = item;
}