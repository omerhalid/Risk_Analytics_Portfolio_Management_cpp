#include "Portfolio.hpp"
#include <algorithm>
#include <stdexcept>

void Portfolio::addDailyReturns(const std::string& symbol, const std::vector<double>& returns) {
    dailyReturns[symbol] = returns;
}

std::vector<double> Portfolio::calculatePortfolioReturns() const {
    std::vector<double> portfolioReturns;
    size_t numDays = dailyReturns.begin()->second.size();

    for (size_t day = 0; day < numDays; ++day) {
        double portfolioReturn = 0.0;

        for (const auto& [symbol, returns] : dailyReturns) {
            portfolioReturn += returns[day] * weights.at(symbol);
        }
        portfolioReturns.push_back(portfolioReturn);
    }

    return portfolioReturns;
}

double Portfolio::calculateVaR(double confidenceLevel) const {
    std::vector<double> portfolioReturns = calculatePortfolioReturns();
    std::sort(portfolioReturns.begin(), portfolioReturns.end());

    size_t index = static_cast<size_t>((1 - confidenceLevel) * portfolioReturns.size());
    return portfolioReturns[index]; // it might be benefical to return -var since VaR is typically a negative number (representing a loss)
}

std::unordered_map<std::string, double> Portfolio::portfolioSelector() {
    std::unordered_map<std::string, double> portfolioWeights;
    std::cout << "Welcome to the Portfolio Selector!" << std::endl;
    std::cout << "Please enter the number of stocks in your portfolio: ";
    int numStocks;
    std::cin >> numStocks;

    for (int i = 0; i < numStocks; ++i) {
        std::string stockSymbol;
        double weight;

        std::cout << "Enter stock symbol #" << (i + 1) << ": ";
        std::cin >> stockSymbol;

        std::cout << "Enter weight for " << stockSymbol << " (as a decimal, e.g., 0.4 for 40%): ";
        std::cin >> weight;

        portfolioWeights[stockSymbol] = weight;
    }

    std::cout << "\nYour portfolio weights are:" << std::endl;
    for (const auto& pair : portfolioWeights) {
        std::cout << pair.first << ": " << pair.second * 100 << "%" << std::endl;
    }

    return portfolioWeights;
}

void Portfolio::printResult(const Portfolio& portfolio)
{
    double var = portfolio.calculateVaR(0.95);
    std::cout << "VaR = " << var << "\n";
    std::cout << "The Value at Risk (VaR) at a 95% confidence level means that there is a 95% chance that the portfolio will not lose more than " 
              << -var << " in a given day. Conversely, there is a 5% chance that the portfolio could lose more than this amount.\n";
}

// integrate it in main.cpp
double calculateCVar(const std::vector<double>& portfolioReturns, double confidenceLevel)
{
    size_t index = static_cast<size_t>((1 - confidenceLevel) * portfolioReturns.size());
    double sum = 0.0;
    for (size_t i = 0; i < index; ++i) {
        sum += portfolioReturns[i];
    }
    return sum / index;  // Average loss in worst cases
}
