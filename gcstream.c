#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>

#define DEBUG 0

static int upload(void)
{
	int return_val = EXIT_SUCCESS;
	CURLcode code = CURLE_OK;

	CURL *curl = curl_easy_init();
	struct curl_slist *headers = NULL;

	if (curl) {
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
	}

	return return_val;
}

int main()
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

	return_val = upload();
	
	curl_global_cleanup();
	return return_val;
}
