#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>
#include <iostream>
#include "http_request.hpp"

using json = nlohmann::json;

namespace forecast::http_request {
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
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpRequest::handleResponse);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        } else {
            status = clientStatus::ERROR;
        }

        return isReady;
    }

    size_t HttpRequest::handleResponse(void* contents, size_t size, size_t nmemb, void* instance) {
        size_t real_size = size * nmemb;
        static_cast<HttpRequest*>(instance)->writeResponse(contents, real_size);

        return real_size;
    }

    void HttpRequest::clearBuffer() {
        readBuffer = "";
    }

    void HttpRequest::writeResponse(void* contents, size_t content_size) {
        readBuffer.append(static_cast<char*>(contents), content_size);
    }

    void HttpRequest::appendHeader(std::string headerName, std::string value) {
        std::string header = headerName + ": " + value;
        headers = curl_slist_append(headers, header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    void HttpRequest::send(std::string url) {
        // Clear the buffer of any leftover data from previous responses
        clearBuffer();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
        }
    }

    std::string HttpRequest::getResponse() {
        return readBuffer;
    }

    json HttpRequest::getJsonResponse() {
        json jsonResponse = json::parse(readBuffer);
        return jsonResponse;
    }

    HttpRequest::~HttpRequest() {
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
}
