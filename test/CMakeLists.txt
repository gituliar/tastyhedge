add_executable(
    tastyhedge_test

    "tastyhedge_test.cpp"

    "Analytics/Model_BlackScholes_test.cpp"
)

find_package(GTest REQUIRED)
enable_testing()

target_link_libraries(
    tastyhedge_test
    PRIVATE
        tastyhedge_lib

        GTest::gtest
        GTest::gtest_main
)

gtest_discover_tests(
    tastyhedge_test
)
