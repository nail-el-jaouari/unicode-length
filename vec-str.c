#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vec-str.h"

void vec_str_init(struct VecStr *self, size_t capacity)
{
    self->items = malloc(capacity * sizeof *self->items);
    
    self->size = 0;

    if (self->items == NULL)
    {
        self->capacity = 0;
        return;
    }

    self->capacity = capacity;
}

void vec_str_free(struct VecStr *self)
{
    for (size_t i = 0; i < self->size; i++)
    {
        free(self->items[i].data);
    }

    free(self->items);

    self->items = NULL;
    self->size = 0;
    self->capacity = 0;
}

void vec_str_push(struct VecStr *self, const char *elem, size_t len)
{
    if (self->size == self->capacity)
    {
        self->capacity *= 2;
        self->items = reallocarray(self->items,
                self->capacity, sizeof *self->items);
    }

    self->items[self->size].data = (char *)elem;
    self->items[self->size].len  = len;
    self->size++;
}

char *vec_str_line(const struct VecStr *self)
{
    size_t len = 0;

    for (size_t i = 0; i < self->size; i++)
    {
        len += self->items[i].len;
    }

    char *line = malloc(sizeof *line * (len + 1));
    size_t position = 0;

    if (line == NULL)
    {
        return NULL;
    }

    for (size_t i = 0; i < self->size; i++)
    {
        memcpy(line + position,
            self->items[i].data,
            self->items[i].len);
           
        position += self->items[i].len;
    }

    line[len] = '\0';

    return line;
}