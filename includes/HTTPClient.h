#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <string>
#include <vector>
#include <curl/curl.h> 

class HTTPClient {
public:

    static std::string sendGETRequest(const std::string& url, const std::vector<std::string>& headers = {}) {
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // Set custom headers if provided
            struct curl_slist* header_list = nullptr;
            for (const auto& header : headers) {
                header_list = curl_slist_append(header_list, header.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

            std::string response;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                response = "Error: " + std::string(curl_easy_strerror(res));
            }

            // Clean up header list
            curl_slist_free_all(header_list);

            curl_easy_cleanup(curl);
            return response;
        } else {
            return "Error: Failed to initialize curl.";
        }
    }

    static std::string sendPOSTRequest(const std::string& url, const std::string& data, const std::vector<std::string>& headers = {}) {
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

            // Set custom headers if provided
            struct curl_slist* header_list = nullptr;
            for (const auto& header : headers) {
                header_list = curl_slist_append(header_list, header.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);

            std::string response;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                response = "Error: " + std::string(curl_easy_strerror(res));
            }

            // Clean up header list
            curl_slist_free_all(header_list);

            curl_easy_cleanup(curl);
            return response;
        } else {
            return "Error: Failed to initialize curl.";
        }
    }


private:
    // Callback function to write received data into a string
    static size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
        data->append(ptr, size * nmemb);
        return size * nmemb;
    }
};

#endif // HTTP_CLIENT_H


// #ifndef HTTP_CLIENT_H
// #define HTTP_CLIENT_H

// #include <string>
// #include <vector>
// #include <memory>
// #include <curl/curl.h>

// class HTTPClient {
// public:
//     HTTPClient() {
//         curl_global_init(CURL_GLOBAL_ALL);
//     }

//     ~HTTPClient() {
//         curl_global_cleanup();
//     }

//     HTTPClient(const HTTPClient&) = delete;
//     HTTPClient& operator=(const HTTPClient&) = delete;
//     HTTPClient(HTTPClient&&) = delete;
//     HTTPClient& operator=(HTTPClient&&) = delete;

//     static std::string sendGETRequest(const std::string& url, const std::vector<std::string>& headers = {}) {
//         CURL* curl = curl_easy_init();
//         if (!curl) {
//             throw std::runtime_error("Failed to initialize curl.");
//         }

//         try {
//             setupRequest(curl, url, headers);
//             std::string response = performRequest(curl);
//             curl_easy_cleanup(curl);
//             return response;
//         } catch (...) {
//             curl_easy_cleanup(curl);
//             throw;
//         }
//     }

//     static std::string sendPOSTRequest(const std::string& url, const std::string& data, const std::vector<std::string>& headers = {}) {
//         CURL* curl = curl_easy_init();
//         if (!curl) {
//             throw std::runtime_error("Failed to initialize curl.");
//         }

//         try {
//             setupRequest(curl, url, headers);
//             curl_easy_setopt(curl, CURLOPT_POST, 1L);
//             curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

//             std::string response = performRequest(curl);
//             curl_easy_cleanup(curl);
//             return response;
//         } catch (...) {
//             curl_easy_cleanup(curl);
//             throw;
//         }
//     }

// private:
//     static size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
//         data->append(ptr, size * nmemb);
//         return size * nmemb;
//     }

//     static void setupRequest(CURL* curl, const std::string& url, const std::vector<std::string>& headers) {
//         curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//         setupHeaders(curl, headers);
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
//     }

//     static std::string performRequest(CURL* curl) {
//         std::string response;
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
//         CURLcode res = curl_easy_perform(curl);
//         if (res != CURLE_OK) {
//             throw std::runtime_error("Failed to perform HTTP request: " + std::string(curl_easy_strerror(res)));
//         }
//         return response;
//     }

//     static void setupHeaders(CURL* curl, const std::vector<std::string>& headers) {
//         if (!headers.empty()) {
//             std::shared_ptr<curl_slist> header_list(nullptr, &curl_slist_free_all);
//             for (const auto& header : headers) {
//                 header_list.reset(curl_slist_append(header_list.get(), header.c_str()));
//             }
//             curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list.get());
//         }
//     }
// };

// #endif // HTTP_CLIENT_H
