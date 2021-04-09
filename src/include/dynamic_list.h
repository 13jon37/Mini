/* date = April 5th 2021 5:03 pm */

#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

// Thanks to Ianertson on YouTube!

typedef struct DYNAMIC_LIST_STRUCT {
    size_t item_size;
    size_t length;
    void **items;
} dynamic_list_t;

internal dynamic_list_t *init_dynamic_list(size_t item_size);
internal void dynamic_list_append(dynamic_list_t *list, void *item);

#endif //DYNAMIC_LIST_H
