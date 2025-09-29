#include "include/binary_search.hpp"

#include <stdexcept>

namespace BinarySearch {
    void search(const std::vector<int>& arr, int target) {
        int left = 0;
        int right = arr.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (arr[mid] == target) {
                return;
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        throw std::runtime_error("Target not found in array");
    }

    void quicksort(std::vector<int>& arr, int left, int right) {
        if (left < right) {
            int pivot = arr[right];
            int i = left - 1;

            for (int j = left; j < right; ++j) {
                if (arr[j] <= pivot) {
                    ++i;
                    std::swap(arr[i], arr[j]);
                }
            }
            std::swap(arr[i + 1], arr[right]);
            int partitionIndex = i + 1;

            quicksort(arr, left, partitionIndex - 1);
            quicksort(arr, partitionIndex + 1, right);
        }
    }
};