#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "str_utils.h"

#define DEBUG 0

/*
 * Precondition: curl_global_init() must have been called.
 * 
 * @return EXIT_SUCCESS on success, otherwise exit failure.
 */
static int config(void)
{
	int return_val = EXIT_SUCCESS;

	CURL *curl = curl_easy_init();
	if (curl) {
		const char *data = "client_id=279340423993-vm2nq86ntcv8j2a0iih6ejuh7hcarsdf.apps.googleusercontent.com&scope=https://www.googleapis.com/auth/devstorage.read_write";
		const char *url = "https://accounts.google.com/o/oauth2/device/code";
		CURLcode code = CURLE_OK;
		char *response = NULL;
		size_t response_size = 0;

		/*****************NOT PORTABLE - POSIX.1-2008*********************/
		FILE *mem_file = open_memstream(&response, &response_size);/******/
		/*****************************************************************/

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, mem_file);
		
		code = curl_easy_perform(curl);
		fclose(mem_file);
		if (code == CURLE_OK) {
			size_t len;
			char *user_code = json_get_value_string(&len, response,
			                                        "user_code");
			puts("Enter user code: ");
			fwrite(user_code, 1, len, stdout);
			puts("\nat https://www.google.com/device");
		} else {
			fprintf(stderr, "Error sending initial config request.\n");
			return_val = EXIT_FAILURE;
		}


		free(response);

		curl_easy_cleanup(curl);
	}

	return return_val;
}

static int upload(void)
{
	int return_val = EXIT_SUCCESS;

	CURL *curl = curl_easy_init();
	if (curl) {
		struct curl_slist *headers = NULL;
		CURLcode code = CURLE_OK;

		curl_easy_setopt(curl, CURLOPT_URL,
		"https://www.googleapis.com/upload/storage/v1/b/henry-elliott-nearline/o?uploadType=media&name=new");
		curl_easy_setopt(curl, CURLOPT_POST, 1L);

#if DEBUG
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif

		headers = curl_slist_append(headers,
		    "Content-Type: application/octet-stream");
		headers = curl_slist_append(headers,
		    "Transfer-Encoding: chunked");
		headers = curl_slist_append(headers,
		    "Expect:");
		code = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		code = curl_easy_perform(curl);
		if (code == CURLE_OK) {
			long http_status;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status);
			if (http_status != 200) {
				fprintf(stderr,
			        "Upload failed. HTTP status %ld returned by server.\n",
					http_status);
				return_val = EXIT_FAILURE;
			}

		} else {
			fprintf(stderr, "Upload failed: %s\n",
			        curl_easy_strerror(code));
			return_val = EXIT_FAILURE;
		}

		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	} else {
		fprintf(stderr, "Could not initialize curl handle.\n");
		return_val = EXIT_FAILURE;
	}

	return return_val;
}

int main(int argc, char *argv[])
{
	CURLcode code = CURLE_OK;
	int return_val = EXIT_SUCCESS;

	code = curl_global_init(CURL_GLOBAL_DEFAULT);
	if (code != CURLE_OK) {
		fprintf(stderr, "Could not initialize libcurl: %s\n",
		        curl_easy_strerror(code));
		return_val = EXIT_FAILURE;
		return return_val;
	}

	if (argc == 2 && strcmp(argv[1], "config") == 0) {
		return_val = config();
	} else {
		return_val = upload();
	}

#if DEBUG
	fprintf(stderr, "Reaching end of main.\n");
#endif
	curl_global_cleanup();
	return return_val;
}
