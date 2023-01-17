#include <utility>
#include <functional>
#include <iostream>
#include <vector>

uint32_t leftSearch(std::vector<int32_t>& mass, int32_t search) {
    auto left = 0;
    auto right = mass.size() - 1;
    auto mid = (left + right) / 2;
    while (right - left > 1) {
        if (mass[mid] < search) {
            left = mid;
        } else {
            right = mid;
        }
        mid = (left + right) / 2;
    }
    if (mass[left] == search) {
        return left + 1;
    } else if (mass[right] == search) {
        return right + 1;
    }
    return 0;
}

uint32_t rightSearch(std::vector<int32_t>& mass, int32_t search) {
    auto left = 0;
    auto right = mass.size() - 1;
    auto mid = (left + right) / 2;
    while (right - left > 1) {
        if (mass[mid] <= search) {
            left = mid;
        } else {
            right = mid;
        }
        mid = (left + right) / 2;
    }
    if (mass[right] == search) {
        return right + 1;
    } else if (mass[left] == search) {
        return left + 1;
    }
    return 0;
}

int main() {
    uint32_t n, m;
    std::cin >> n >> m;
    std::vector<int32_t> first;
    std::vector<int32_t> second;
    int32_t elem;
    for (auto i = 0; i < n;  i++) {
        std::cin >> elem;
        first.push_back(elem);
    }
    for (auto i = 0; i < m;  i++) {
        std::cin >> elem;
        auto l = leftSearch(first, elem);
        auto r = rightSearch(first, elem);
        if (l == 0 && r == 0) {
            std::cout << 0 << '\n';
        } else {
            std::cout << l << ' ' << r << '\n';
        }
    }
}
