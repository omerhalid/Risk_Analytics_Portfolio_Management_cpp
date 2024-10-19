// #include <gtest/gtest.h>
// #include "../MarketDataHandler.hpp"
// #include "../Portfolio.hpp"

// // Test case for MarketDataHandler
// TEST(MarketDataHandlerTest, FetchMarketData) {
//     std::string apiKey = "your_api_key";
//     MarketDataHandler handler(apiKey);

//     std::unordered_map<std::string, std::vector<double>> hashmap;
//     std::unordered_map<std::string, std::vector<double>> dailyReturn;

//     handler.fetchMarketData("AAPL", hashmap, dailyReturn);

//     ASSERT_FALSE(hashmap["AAPL"].empty());
//     ASSERT_FALSE(dailyReturn["AAPL"].empty());
// }

// // Test case for Portfolio
// TEST(PortfolioTest, CalculateVaR) {
//     std::unordered_map<std::string, double> weights = {
//         {"AAPL", 0.4},
//         {"MSFT", 0.3},
//         {"GOOGL", 0.3}
//     };

//     Portfolio portfolio(weights);

//     std::vector<double> returns = {0.01, 0.02, -0.01};
//     portfolio.addDailyReturns("AAPL", returns);
//     portfolio.addDailyReturns("MSFT", returns);
//     portfolio.addDailyReturns("GOOGL", returns);

//     double var = portfolio.calculateVaR(0.95);
//     ASSERT_LE(var, 0); // VaR should be less than or equal to 0
// }

// int main(int argc, char **argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }