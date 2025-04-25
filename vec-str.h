#ifndef __VEC_STR_H__
#define __VEC_STR_H__

#include <stddef.h>

#define MIN_CAP 16

struct VecStr
{
    struct String
    {
        char *data;
        size_t len;
    } *items;

    size_t size;
    size_t capacity;
};

void vec_str_init(struct VecStr *self, size_t capacity);
void vec_str_free(struct VecStr *self);
void vec_str_push(struct VecStr *self, const char *elem, size_t len);
/* 
    Get contiguous memory strings from member chuncks
    of self->data
*/
char *vec_str_line(const struct VecStr *self);
#endif
