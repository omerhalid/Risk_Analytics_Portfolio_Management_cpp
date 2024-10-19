#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <unordered_map>
#include <vector>
#include <string>

class Portfolio {
public:
    Portfolio(const std::unordered_map<std::string, double>& weights) : weights(weights) {}

    void addDailyReturns(const std::string& symbol, const std::vector<double>& returns);
    std::vector<double> calculatePortfolioReturns() const;
    double calculateVaR(double confidenceLevel) const;

private:
    std::unordered_map<std::string, double> weights;
    std::unordered_map<std::string, std::vector<double>> dailyReturns;
};

#endif // PORTFOLIO_H