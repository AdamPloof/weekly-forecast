#ifndef CURL_CLIENT_H
#define CURL_CLIENT_H

#include <curl/curl.h>
#include <string>

namespace wf::http_request {
    enum class clientStatus {NOT_INITALIZED, READY, ERROR};

    class HttpRequest {
        public:
            HttpRequest();
            ~HttpRequest();

            int init();
            void appendHeader(std::string header_name, std::string value);
            void send(std::string url);
            clientStatus getStatus();

        private:
            CURL* curl;
            CURLcode res;
            curl_slist* headers;
            clientStatus status;
    };
}

#endif
