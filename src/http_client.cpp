#include "http_client.hpp"
#include <curl/curl.h>
#include <string>

namespace wf::http_client {
    HttpClient::HttpClient() {
        headers = nullptr;
    };

    int HttpClient::init() {
        int isReady = 0;
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            isReady = 1;
        }

        return isReady;
    }

    void HttpClient::appendHeader(std::string headerName, std::string value) {
        std::string header = headerName + ": " + value;
        headers = curl_slist_append(headers, header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    void HttpClient::sendRequest(std::string url) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));

        }
    }

    HttpClient::~HttpClient() {
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}
