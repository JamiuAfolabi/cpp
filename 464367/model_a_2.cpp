#include <iostream>
#include <curl/curl.h>
#include <string>
#include <map>
#include <future>
#include <vector>

// Function to handle the response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

// Function to fetch data from CoinGecko API
std::string fetchCryptoData(const std::string& ids, const std::string& vs_currencies) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    std::string url = "https://api.coingecko.com/api/v3/simple/price?ids=" + ids + "&vs_currencies=" + vs_currencies;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 seconds timeout

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return "";
        }
    }
    return readBuffer;
}

// Function to fetch data asynchronously
std::future<std::string> fetchCryptoDataAsync(const std::string& ids, const std::string& vs_currencies) {
    return std::async(std::launch::async, fetchCryptoData, ids, vs_currencies);
}

int main() {
    // List of cryptocurrencies
    std::string currencies = "bitcoin,ethereum,cardano";
    
    // Adding various fiat currencies to iterate
    std::vector<std::string> fiatCurrencies = {"usd", "eur", "gbp", "jpy"};
    std::vector<std::future<std::string>> futures;

    // Launch asynchronous operations
    for (const auto& fiat : fiatCurrencies) {
        futures.push_back(fetchCryptoDataAsync(currencies, fiat));
    }

    // Retrieve results
    for (size_t i = 0; i < futures.size(); ++i) {
        const auto& fiat = fiatCurrencies[i];
        std::string data = futures[i].get();
        if(!data.empty()) {
            std::cout << "Prices in " << fiat << ": " << data << std::endl;
        } else {
            std::cout << "Failed to retrieve data for " << fiat << std::endl;
        }
    }

    return 0;
}