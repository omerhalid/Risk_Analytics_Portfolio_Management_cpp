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
    return portfolioReturns[index];
}