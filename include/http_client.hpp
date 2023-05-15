#ifndef CURL_CLIENT_H
#define CURL_CLIENT_H

#include <curl/curl.h>
#include <string>

namespace wf::http_client {
    class HttpClient {
    public:
        HttpClient();
        ~HttpClient();

        int init();
        void appendHeader(std::string header_name, std::string value);
        void sendRequest(std::string url);

    private:
        CURL* curl;
        CURLcode res;
        curl_slist* headers;
    };
}

#endif
