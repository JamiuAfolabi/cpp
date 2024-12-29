#include <iostream>
#include <curl/curl.h>
#include <string>
#include <map>
#include <vector>
#include <future>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

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
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); 

        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed for " << url << ": " << curl_easy_strerror(res) << std::endl;
            return "";
        }
    }
    return readBuffer;
}

int main() {
    std::string currencies = "bitcoin,ethereum,cardano";

    std::map<std::string, std::string> fiatCurrencies = {
        {"usd", "USD"},
        {"eur", "EUR"},
        {"gbp", "GBP"},
        {"jpy", "JPY"}
    };

    std::vector<std::future<std::string>> results;

    for (const auto& fiat : fiatCurrencies) {
        results.push_back(std::async(std::launch::async, fetchCryptoData, currencies, fiat.first));
    }

    // Wait for all results and print them
    for (size_t i = 0; i < results.size(); ++i) {
        const std::string& data = results[i].get();
        std::cout << "Prices in " << fiatCurrencies.at(std::to_string(i)) << ": " << data << std::endl;
    }

    return 0;
}  
