#ifndef CURL_CLIENT_H
#define CURL_CLIENT_H

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

namespace forecast::http_request {
    enum class clientStatus {NOT_INITALIZED, READY, ERROR};

    class HttpRequest {
        public:
            HttpRequest();
            ~HttpRequest();

            int init();
            void appendHeader(std::string header_name, std::string value);
            void send(std::string url);
            clientStatus getStatus();
            std::string getResponse();
            json getJsonResponse();

        private:
            CURL* curl;
            CURLcode res;
            curl_slist* headers;
            clientStatus status;
            std::string readBuffer;
            static size_t handleResponse(void* contents, size_t size, size_t nmemb, void* buffer);
            void clearBuffer();
            void writeResponse(void* contents, size_t content_size);
    };
}

#endif
