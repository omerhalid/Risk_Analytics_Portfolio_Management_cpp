#include "MarketDataHandler.hpp"

size_t MarketDataHandler::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void MarketDataHandler::parseMarketData(const std::string& jsonData, const std::string& symbol, 
                                        std::unordered_map<std::string, std::vector<double>>& hashmap, 
                                        std::unordered_map<std::string, std::vector<double>>& dailyReturn) {
    std::vector<double> closePrices;
    try {
        auto json = nlohmann::json::parse(jsonData);
        std::cout << "Processing market data...\n";
        
        for (auto& [timestamp, data] : json["Time Series (Daily)"].items()) {
            double closePrice = std::stod(data["4. close"].get<std::string>());
            closePrices.push_back(closePrice);
        }

        hashmap[symbol] = closePrices;

        // daily returns
        for (size_t i = 1; i < closePrices.size(); ++i) {
            double dailyReturnValue = (closePrices[i] - closePrices[i - 1]) / closePrices[i - 1];
            dailyReturn[symbol].push_back(dailyReturnValue);
        }

    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    } catch (const nlohmann::json::type_error& e) {
        std::cerr << "JSON type error: " << e.what() << std::endl;
    }
}

void MarketDataHandler::fetchMarketData(const std::string& symbol, 
                                        std::unordered_map<std::string, std::vector<double>>& hashmap, 
                                        std::unordered_map<std::string, std::vector<double>>& dailyReturn) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        std::string url = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=" + symbol + "&outputsize=compact&apikey=" + apiKey;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);  // Timeout
        // Disable SSL verification (not recommended for production)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize cURL" << std::endl;
    }

    parseMarketData(readBuffer, symbol, hashmap, dailyReturn);
}