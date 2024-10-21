#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class Portfolio {
public:
    Portfolio(const std::unordered_map<std::string, double>& weights) : weights(weights) {}

    void addDailyReturns(const std::string& symbol, const std::vector<double>& returns);
    std::vector<double> calculatePortfolioReturns() const;
    double calculateVaR(double confidenceLevel) const;
    double calculateCVar(const std::vector<double>& portfolioReturns, double confidenceLevel);

    static std::unordered_map<std::string, double> portfolioSelector();
    static void printResult(const Portfolio& portfolio);

private:
    std::unordered_map<std::string, double> weights;
    std::unordered_map<std::string, std::vector<double>> dailyReturns;
};

#endif // PORTFOLIO_H