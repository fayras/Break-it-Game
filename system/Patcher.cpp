//
// Created by Dimi on 30.03.2018.
//

#include <iostream>
#include <unzipper.h>
#include "Patcher.hpp"
#include "json.hpp"
#include "../Config.hpp"

using json = nlohmann::json;

namespace {
    size_t writeString(void *ptr, size_t size, size_t nmemb, std::string *data) {
        data->append((char *) ptr, size * nmemb);
        return size * nmemb;
    }

    size_t writeFile(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        size_t written = fwrite(ptr, size, nmemb, stream);
        return written;
    }
}

Patcher::Patcher() {
    status = Status::FETCHING_INFO;
    latest_version_url_future = std::async(std::launch::async, [this] {
        CURL *curl = curl_easy_init();
        if (curl) {
            setOptions(curl, GITHUB_RELEASE_PATH);

            std::string response_string;
            std::string header_string;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeString);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

            curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            auto responseJson = json::parse(response_string);
            std::string tag = responseJson["tag_name"];
            if(tag.compare(GAME_VERSION_TAG) > 0) {
                auto assets = responseJson["assets"];
                for(auto& asset : assets) {
                    std::string browser_download_url = asset["browser_download_url"];
                    if (browser_download_url.find(std::string(PLATFORM_NAME) + ".zip") != std::string::npos) {
                        status = Status::READY_TO_DOWNLOAD;
                        return browser_download_url;
                    }
                }

            }

            status = Status::NO_UPDATE;
        } else {
            status = Status::FAILED;
        }

        return std::string();
    });
}

void Patcher::setOptions(CURL *handle, const char *url) {
    curl_easy_setopt(handle, CURLOPT_URL, url);
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "curl/7.59.0");
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(handle, CURLOPT_CAINFO, crt.c_str());
}

Patcher::Status Patcher::getStatus() const {
    return status;
}

void Patcher::download() {
    if (status != Status::READY_TO_DOWNLOAD) {
        return;
    }

    download_future = std::async(std::launch::async, [this] {
        std::string browser_download_url = latest_version_url_future.get();
        if(!browser_download_url.empty()) {
            status = Status::DOWNLOADING;
            std::string tempFilename("Break-It-Patch.zip");
            FILE *patch_file = fopen(tempFilename.c_str(), "wb");
            // FILE *logfile = fopen("dump.txt", "wb");
            CURL *fileCurl = curl_easy_init();

            if(!fileCurl) {
                status = Status::FAILED;
                return;
            }

            setOptions(fileCurl, browser_download_url.c_str());
            curl_easy_setopt(fileCurl, CURLOPT_WRITEFUNCTION, writeFile);
            curl_easy_setopt(fileCurl, CURLOPT_WRITEDATA, patch_file);
            // curl_easy_setopt(fileCurl, CURLOPT_VERBOSE, 1L);
            // curl_easy_setopt(fileCurl, CURLOPT_STDERR, logfile);

            curl_easy_perform(fileCurl);
            curl_easy_cleanup(fileCurl);
            fclose(patch_file);
            // fclose(logfile);

            status = Status::UPDATING;

            zipper::Unzipper unzipper(tempFilename);
            unzipper.extract("update");
            unzipper.close();
            remove(tempFilename.c_str());

            status = Status::DONE;
        }
    });
}
