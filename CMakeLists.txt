cmake_minimum_required(VERSION 3.10)
project(RiskAnalyticsPortfolioManagement)

set(CMAKE_CXX_STANDARD 17)

# Add the library target
add_library(RiskAnalyticsLib
    MarketDataHandler.cpp
    Portfolio.cpp
)

# Include directories
target_include_directories(RiskAnalyticsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Specify the path to libcurl
set(CURL_INCLUDE_DIR "C:/Users/katka/source/market_data_handler/curl/include")
set(CURL_LIBRARY "C:/Users/katka/source/market_data_handler/curl/lib/libcurl.lib")

# Include and link libcurl
target_include_directories(RiskAnalyticsLib PUBLIC ${CURL_INCLUDE_DIR})
target_link_libraries(RiskAnalyticsLib PUBLIC ${CURL_LIBRARY})

# Add the main executable
add_executable(RiskAnalyticsPortfolioManagement main.cpp)

# Link the main executable with the library
target_link_libraries(RiskAnalyticsPortfolioManagement PRIVATE RiskAnalyticsLib)

# Add the tests
add_subdirectory(tests)