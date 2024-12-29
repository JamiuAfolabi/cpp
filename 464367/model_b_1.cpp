#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>


#define API_KEY "YOUR_COINGECKO_API_KEY"  // Replace this with your actual API key
#define BASE_URL "https://api.coingecko.com/api/v3/"

std::string performAPIrequest(const std::string& url) {
    CURL *curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "failed to initialize curl\n";
        return response;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    struct curl_slist *headers = NULL;
    std::string header_string = "Accept: application/json";

    if (API_KEY != "YOUR_COINGECKO_API_KEY") {
        header_string += "\r\n";
        header_string += "Authorization: Bearer " + API_KEY;
    }

    headers = curl_slist_append(headers, header_string.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::vector<char> buffer;
    size_t buffersize = 0;

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* ptr, size_t size, size_t nmemb, void* userp) {
        auto buf = reinterpret_cast<std::vector<char>*>(userp);
        size_t len = size * nmemb;
        buf->insert(buf->end(), reinterpret_cast<const char*>(ptr), reinterpret_cast<const char*>(ptr) + len);
        return len;
    });

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        curl_easy_cleanup(curl);
        return response;
    }
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);

    response = std::string(buffer.begin(), buffer.end());
    buffer.clear();

    return response;
}


int main() {
    std::vector<std::string> coins = {"bitcoin", "ethereum", "cardano"};
    std::vector<std::string> currencies = {"usd", "eur", "jpy"};

    for (const std::string& coin : coins) {
        std::cout << "--- Data for " << coin << " ---\n";
        for (const std::string& currency : currencies) {
            std::string url = BASE_URL + "simple/price?ids=" + coin + "&vs_currencies=" + currency;
            std::string response = performAPIrequest(url);

            if (!response.empty()) {
                try {
                    nlohmann::json j = nlohmann::json::parse(response);
                    double price = j.at(coin).at(currency);
                    std::cout << coin << " in " << currency << ": $" << price << "\n";
                } catch (const nlohmann::json::exception& e) {
                    std::cerr << "Error parsing JSON response: " << e.what() << "\n";
                }
