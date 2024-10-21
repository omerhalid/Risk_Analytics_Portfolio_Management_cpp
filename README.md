# Risk Analytics Portfolio Management

Welcome to the Risk Analytics Portfolio Management project! This project is designed to fetch market data, calculate portfolio returns, and compute Value at Risk (VaR) for a given portfolio of stocks.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Setup](#setup)
- [Building the Project](#building-the-project)
- [Running the Executable](#running-the-executable)
- [Running Tests](#running-tests)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This project fetches market data from the Alpha Vantage API, calculates daily returns for a portfolio of stocks, and computes the Value at Risk (VaR) at a 95% confidence level. The project is written in C++ and uses `libcurl` for making HTTP requests and `nlohmann/json` for parsing JSON data.

## Features

- Fetch market data for a portfolio of stocks
- Calculate daily returns for each stock
- Compute portfolio returns
- Calculate Value at Risk (VaR) at a 95% confidence level
- Print detailed results

## Prerequisites

- C++17 compatible compiler (e.g., `g++`)
- CMake (version 3.10 or higher)
- `libcurl` library
- `nlohmann/json` library
- Google Test (for running tests)

## Setup

1. Clone the repository:

   ```sh
   git clone https://github.com/omerhalid/risk-analytics-portfolio-management.git
   cd risk-analytics-portfolio-management
   ```
2. Install the required libraries:

  **libcurl**: Follow the instructions on the libcurl website to install libcurl.
  **nlohmann/json**: Follow the instructions on the nlohmann/json GitHub page to install the JSON library.
  **Google Tes**t: Follow the instructions on the Google Test GitHub page to install Google Test.

3. Compile and run the project (Cmake option will come soon.)
   **COMPILE**
  ```sh
  g++ -std=c++17 -Iinc -c src/main.cpp -o build/main.o
  g++ -std=c++17 -Iinc -c src/MarketDataHandler.cpp -o build/MarketDataHandler.o
  g++ -std=c++17 -Iinc -c src/Portfolio.cpp -o build/Portfolio.o
  g++ build/main.o build/MarketDataHandler.o build/Portfolio.o -o build/RiskAnalyticsPortfolioManagement -L"C:/Users/katka/source/market_data_handler/curl/lib" -lcurl
  ```
  **RUN**
  ```sh
  cd build
  RiskAnalyticsPortfolioManagement.exe
  ```

**Contributing**
Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

**License**
This project is licensed under the MIT License.
   
