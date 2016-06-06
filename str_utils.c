#include "str_utils.h"

#include "stdlib.h"
#include "string.h"

char *json_get_value_string(size_t *out_len,
                            const char *json, const char *key)
{
	char *ptr = strstr(json, key);
	*out_len = 0;
	if (ptr) {
		ptr = strchr(ptr, '"');
		if (ptr) {
			++ptr;
			ptr = strchr(ptr, '"');
			if (ptr) {
				++ptr;
				if (*ptr != '\0') {
					char *end_ptr = strchr(ptr, '"');
					if (end_ptr) {
						*out_len = end_ptr - ptr;
					} else {
						ptr = NULL;
					}
				} else {
					ptr = NULL;
				}
			}
		}
	}

	return ptr;
}
