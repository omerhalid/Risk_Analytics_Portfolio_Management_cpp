#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "C:/Users/katka/source/market_data_handler/curl/include/curl/curl.h"
#include "C:/Users/katka/source/market_data_handler/json/json.hpp"

// Callback function to handle data received from cURL
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to read .env file and set environment variables
void loadEnv() {
    std::ifstream envFile(".env");
    if (!envFile.is_open()) {
        std::cerr << "Failed to open .env file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(envFile, line)) {
        std::istringstream lineStream(line);
        std::string key;
        if (std::getline(lineStream, key, '=')) {
            std::string value;
            if (std::getline(lineStream, value)) {
                std::string envVar = key + "=" + value;
                putenv(envVar.c_str());
            }
        }
    }
    envFile.close();
}

std::string fetchMarketData(const std::string& apiKey, const std::string& symbol) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&outputsize=compact&apikey=" + apiKey;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);  // Timeout
        // Disable SSL verification (not recommended for production)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize cURL" << std::endl;
    }

    // Print the response for debugging
    std::cout << "Response: " << readBuffer << std::endl;

    return readBuffer;
}

std::vector<double> parseMarketData(const std::string& jsonData) {
    std::unordered_map<std::string, int> hashmap;
    std::vector<double> closePrices;
    try {
        auto json = nlohmann::json::parse(jsonData);
        std::cout << "Processing market data...\n";
        
        for (auto& [timestamp, data] : json["Time Series (Daily)"].items()) {
            double closePrice = std::stod(data["4. close"].get<std::string>());
            closePrices.push_back(closePrice);
        }

    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    } catch (const nlohmann::json::type_error& e) {
        std::cerr << "JSON type error: " << e.what() << std::endl;
    }
    return closePrices;
}

int main() {
    try {
        // Load environment variables
        loadEnv();

        const char* apiKey = std::getenv("API_KEY");
        if (apiKey == nullptr) {
            std::cerr << "API_KEY not found in environment variables" << std::endl;
            return 1;
        }

        std::cout << "Which stock? :";
        std::cout << "\n";
        std::string stockName;
        std::cin >> stockName;

        std::cout << "Fetching market data for " << stockName << "...\n";
        std::string response = fetchMarketData(apiKey, stockName);
        std::cout << "Market data fetched. Parsing data...\n";
        std::vector<double> closingPrices = parseMarketData(response);

        std::cout << "Last 100 days closing prices for " << stockName << ":\n";
        for (size_t i = 0; i < closingPrices.size(); ++i) {
            std::cout << "Day " << i + 1 << ": " << closingPrices[i] << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}