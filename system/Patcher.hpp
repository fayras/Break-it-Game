#ifndef BREAK_IT_PATCHER_HPP
#define BREAK_IT_PATCHER_HPP

#include <string>
#include <curl/curl.h>
#include <future>

class Patcher {
    public:
        enum class Status {
            NO_UPDATE,
            FETCHING_INFO,
            READY_TO_DOWNLOAD,
            DOWNLOADING,
            DOWNLOAD_DONE,
            UPDATING,
            DONE,
            FAILED
        };

        Patcher();
        Status getStatus() const;
        void download();
        void patch() const;

    private:
        std::string crt{"curl-ca-bundle.crt"};
        std::atomic<Status> status;
        std::atomic<float> progress{0.f};
        std::future<std::string> latest_version_url_future;
        std::future<void> download_future;

        void setOptions(CURL* handle, const char* url);
};

#endif //BREAK_IT_PATCHER_HPP
