#ifndef STR_UTILS_H
#define STR_UTILS_H
#include <stdlib.h>
/*
 * This funciton takes a NULL terminated JSON object and returns a pointer to a
 * location of the begining of a substring of {@code json} which has length
 * {@code out_len} and which denotes the value associated with the {@code key}.
 */
char *json_get_value_string(size_t *out_len,
                            const char *json, const char *key);
#endif
