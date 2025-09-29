#include "include/catch.hpp"

#include "../src/include/binary_search.hpp"

TEST_CASE("BinarySearch::search") {
    SECTION("Finds existing element") {
        std::vector<int> arr = {1, 2, 3, 4, 5};
        REQUIRE_NOTHROW(BinarySearch::search(arr, 3));
    }

    SECTION("Throws error for non-existing element") {
        std::vector<int> arr = {1, 2, 3, 4, 5};
        REQUIRE_THROWS_AS(BinarySearch::search(arr, 6), std::runtime_error);
    }

    SECTION("Handle negative numbers") {
        std::vector<int> arr = {-8, -5, -3, -1, -2};
        REQUIRE_NOTHROW(BinarySearch::search(arr, -3));
        REQUIRE_THROWS_AS(BinarySearch::search(arr, 1), std::runtime_error);
    }
}

TEST_CASE("BinarySearch::quicksort") {
    SECTION("Sorts an unsorted array") {
        std::vector<int> arr = {5, 3, 8, 1, 2};
        BinarySearch::quicksort(arr, 0, arr.size() - 1);
        REQUIRE(arr == std::vector<int>({1, 2, 3, 5, 8}));
    }

    SECTION("Sorts an unsorted array with negatives") {
        std::vector<int> arr = {5, -3, 8, 1, -2};
        BinarySearch::quicksort(arr, 0, arr.size() - 1);
        REQUIRE(arr == std::vector<int>({-3, -2, 1, 5, 8}));
    }
}