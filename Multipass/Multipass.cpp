#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "istreamwrapper.h"
#include <curl.h>
#include <iostream>
#include <vector>

using namespace rapidjson;

class UbuntuImageFetcher {
public:
    virtual std::vector<std::string> getSupportedReleases() = 0;
    virtual std::string getCurrentLTSVersion() = 0;
    virtual std::string getSha256ForRelease(const std::string& release) = 0;
};

class SimplestreamsImageFetcher : public UbuntuImageFetcher {
public:
    std::vector<std::string> getSupportedReleases() override {
        std::vector<std::string> releases;

        // Fetch the Simplestreams JSON data
        std::string jsonUrl = "https://cloud-images.ubuntu.com/releases/streams/v1/com.ubuntu.cloud:released:download.json";
        std::string jsonData = fetchJsonData(jsonUrl);

        // Parse the JSON data
        Document document;
        document.Parse(jsonData.c_str());

        // Extract the releases
        if (document.HasMember("products")) {
            const Value& products = document["products"];
            for (SizeType i = 0; i < products.Size(); i++) {
                const Value& product = products[i];
                if (product.HasMember("name") && product["name"].IsString()) {
                    std::string release = product["name"].GetString();
                    releases.push_back(release);
                }
            }
        }

        return releases;
    }

    std::string getCurrentLTSVersion() override {
        // Fetch the Simplestreams JSON data
        std::string jsonUrl = "https://cloud-images.ubuntu.com/releases/streams/v1/com.ubuntu.cloud:released:download.json";
        std::string jsonData = fetchJsonData(jsonUrl);

        // Parse the JSON data
        Document document;
        document.Parse(jsonData.c_str());

        // Find the LTS version
        std::string ltsVersion;
        if (document.HasMember("series") && document["series"].HasMember("lts") && document["series"]["lts"].IsString()) {
            ltsVersion = document["series"]["lts"].GetString();
        }

        return ltsVersion;
    }

    std::string getSha256ForRelease(const std::string& release) override {
        // Fetch the Simplestreams JSON data
        std::string jsonUrl = "https://cloud-images.ubuntu.com/releases/streams/v1/com.ubuntu.cloud:released:download.json";
        std::string jsonData = fetchJsonData(jsonUrl);

        // Parse the JSON data
        Document document;
        document.Parse(jsonData.c_str());

        // Find the sha256 for the given release
        std::string sha256;
        if (document.HasMember("products")) {
            const Value& products = document["products"];
            for (SizeType i = 0; i < products.Size(); i++) {
                const Value& product = products[i];
                if (product.HasMember("name") && product["name"].IsString() && product.HasMember("disk1.img") && product["disk1.img"].HasMember("sha256") && product["disk1.img"]["sha256"].IsString()) {
                    std::string productName = product["name"].GetString();
                    if (productName == release) {
                        sha256 = product["disk1.img"]["sha256"].GetString();
                        break;
                    }
                }
            }
        }

        return sha256;
    }

private:
    std::string fetchJsonData(const std::string& url) {
        std::string data;
        CURL* curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            }
            curl_easy_cleanup(curl);
        }
        return data;
    }

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
};

int main() {
    SimplestreamsImageFetcher fetcher;

    // Get and print the supported releases
    std::cout << "Supported Releases:" << std::endl;
    std::vector<std::string> releases = fetcher.getSupportedReleases();
    for (const std::string& release : releases) {
        std::cout << release << std::endl;
    }

    // Get and print the current LTS version
    std::cout << "Current LTS Version:" << std::endl;
    std::string ltsVersion = fetcher.getCurrentLTSVersion();
    std::cout << ltsVersion << std::endl;

    // Get and print the sha256 for a specific release (e.g., "bionic")
    std::string release = "bionic";
    std::cout << "SHA256 for " << release << ":" << std::endl;
    std::string sha256 = fetcher.getSha256ForRelease(release);
    std::cout << sha256 << std::endl;

    return 0;
}