#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "C:/Users/katka/source/market_data_handler/curl/include/curl/curl.h"
#include "C:/Users/katka/source/market_data_handler/json/json.hpp"

// Callback function to handle data received from cURL
size_t WriteCallback(
    void* contents, 
    size_t size, 
    size_t nmemb, 
    void* userp) 
    {

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

void parseMarketData(
    const std::string& jsonData, 
    const std::string& symbol, 
    std::unordered_map<std::string, 
    std::vector<double>>& hashmap, 
    std::unordered_map<std::string, 
    std::vector<double>>& dailyReturn) 
{

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
        for(int i = 1; i < hashmap[symbol].size(); ++i)
        {
            double dailyReturnValue = (closePrices[i] - closePrices[i - 1]) / closePrices[i - 1];
            dailyReturn[symbol].push_back(dailyReturnValue);
        }

    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    } catch (const nlohmann::json::type_error& e) {
        std::cerr << "JSON type error: " << e.what() << std::endl;
    }
}

void fetchMarketData(
    const std::string& apiKey, 
    const std::string& symbol, 
    std::unordered_map<std::string, std::vector<double>>& hashmap, 
    std::unordered_map<std::string, std::vector<double>>& dailyReturn) 
    {

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

    parseMarketData(readBuffer, symbol, hashmap, dailyReturn);
}

std::vector<double> calculatePortfolioReturns(
    const std::unordered_map<std::string, std::vector<double>>& dailyReturns,
    const std::unordered_map<std::string, double>& portfolioWeights) 
{
    std::vector<double> portfolioReturns;
    size_t numDays = dailyReturns.begin()->second.size();

    for (size_t day = 0; day < numDays; ++day) 
    {
        double portfolioReturn = 0.0;

        for (const auto& [symbol, returns] : dailyReturns) {
            portfolioReturn += returns[day] * portfolioWeights.at(symbol);
        }
        portfolioReturns.push_back(portfolioReturn);
    }

    return portfolioReturns;    

}

void logger(std::unordered_map<std::string, std::vector<double>>& hashmap, std::string& stockName)
{
    if(hashmap.find(stockName) != hashmap.end())
        {
            std::cout << "Last 100 days for " << stockName << ":\n";
            const auto& prices = hashmap[stockName];
            for (int i = 0; i < prices.size(); ++i) 
            {
                std::cout << "Day " << i + 1 << ": " << hashmap[stockName][i] << "\n";
            }
        }
        else
            throw std::invalid_argument("Wrong stock name");
}

double calculateVar(const std::vector<double>& portfolioReturns)
{
    constexpr double confidenceLevel = 0.95;

    size_t index = static_cast<size_t>((1 - confidenceLevel) * portfolioReturns.size());

    double var = portfolioReturns[index];

    return var;
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

        std::unordered_map<std::string, std::vector<double>> hashmap;
        std::unordered_map<std::string, std::vector<double>> dailyReturn;

        std::unordered_map<std::string, double> portfolioWeights = {
            {"AAPL", 0.4},  // 40% of the portfolio is invested in Apple
            {"MSFT", 0.3},  // 30% of the portfolio is invested in Microsoft
            {"GOOGL", 0.3}  // 30% of the portfolio is invested in Google
        };

        // Fetch data for all stocks in the portfolio
        for (const auto& [stockName, weight] : portfolioWeights) {
            std::cout << "Fetching market data for " << stockName << "...\n";
            fetchMarketData(apiKey, stockName, hashmap, dailyReturn);
            std::cout << "Market data fetched for " << stockName << ".\n";
        }

        // Calculate portfolio returns
        std::vector<double> portfolioReturns = calculatePortfolioReturns(dailyReturn, portfolioWeights);

        // Sort the portfolio returns
        std::cout << "Sorting the returns..\n";
        std::sort(portfolioReturns.begin(), portfolioReturns.end());
        std::cout << "Sorted!\n";

        // Calculate VaR
        double var = calculateVar(portfolioReturns);
        std::cout << "VaR = " << var << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}
