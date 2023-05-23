#include <curl/curl.h>
#include <string>
#include "http_request.hpp"

namespace wf::http_request {
    HttpRequest::HttpRequest() {
        headers = nullptr;
        status = clientStatus::NOT_INITALIZED;
    };

    clientStatus HttpRequest::getStatus() {
        return status;
    }

    int HttpRequest::init() {
        int isReady = 0;
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            status = clientStatus::READY;
        } else {
            status = clientStatus::ERROR;
        }

        return isReady;
    }

    void HttpRequest::appendHeader(std::string headerName, std::string value) {
        std::string header = headerName + ": " + value;
        headers = curl_slist_append(headers, header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    void HttpRequest::send(std::string url) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
        }
    }

    HttpRequest::~HttpRequest() {
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}
