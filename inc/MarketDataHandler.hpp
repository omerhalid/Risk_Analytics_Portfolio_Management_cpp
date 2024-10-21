#ifndef MARKET_DATA_HANDLER_H
#define MARKET_DATA_HANDLER_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "C:/Users/katka/source/market_data_handler/curl/include/curl/curl.h"
#include "C:/Users/katka/source/market_data_handler/json/json.hpp"

class MarketDataHandler {
public:
    MarketDataHandler(const std::string& apiKey) : apiKey(apiKey) {}

    void fetchMarketData(const std::string& symbol, 
                         std::unordered_map<std::string, std::vector<double>>& hashmap, 
                         std::unordered_map<std::string, std::vector<double>>& dailyReturn);

private:
    std::string apiKey;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
    void parseMarketData(const std::string& jsonData, const std::string& symbol, 
                         std::unordered_map<std::string, std::vector<double>>& hashmap, 
                         std::unordered_map<std::string, std::vector<double>>& dailyReturn);
};

#endif // MARKET_DATA_HANDLER_H