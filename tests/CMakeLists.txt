# Enable testing
enable_testing()

# Add the test executable
add_executable(test_main test_main.cpp)

# Link the test executable with the library
target_link_libraries(test_main PRIVATE RiskAnalyticsLib)

# Add test cases
add_test(NAME TestMain COMMAND test_main)