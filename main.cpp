#include <iostream>
#include <fstream>
#include <unordered_map>
#include <sstream>  // Add this header
#include "MarketDataHandler.hpp"
#include "Portfolio.hpp"

void loadEnv() {
    std::ifstream envFile(".env");
    if (!envFile.is_open()) {
        std::cerr << "Failed to open .env file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(envFile, line)) {
        std::istringstream lineStream(line);  // Correct usage
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

int main() {
    try {
        // Load environment variables
        loadEnv();

        const char* apiKey = std::getenv("API_KEY");
        if (apiKey == nullptr) {
            std::cerr << "API_KEY not found in environment variables" << std::endl;
            return 1;
        }

        MarketDataHandler marketDataHandler(apiKey);

        std::unordered_map<std::string, double> portfolioWeights = {
            {"AAPL", 0.4},  // 40% of the portfolio is invested in Apple
            {"MSFT", 0.3},  // 30% of the portfolio is invested in Microsoft
            {"GOOGL", 0.3}  // 30% of the portfolio is invested in Google
        };

        Portfolio portfolio(portfolioWeights);

        std::unordered_map<std::string, std::vector<double>> hashmap;
        std::unordered_map<std::string, std::vector<double>> dailyReturn;

        // Fetch data for all stocks in the portfolio
        for (const auto& [stockName, weight] : portfolioWeights) {
            std::cout << "Fetching market data for " << stockName << "...\n";
            marketDataHandler.fetchMarketData(stockName, hashmap, dailyReturn);
            portfolio.addDailyReturns(stockName, dailyReturn[stockName]);
            std::cout << "Market data fetched for " << stockName << ".\n";
        }

        // Calculate VaR
        double var = portfolio.calculateVaR(0.95);
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