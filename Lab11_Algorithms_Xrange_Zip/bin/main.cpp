#include "../lib/algorithm_lib.h"
#include "../lib/xrange.h"
#include "../lib/zip.h"

#include <iostream>
#include <list>
#include <vector>

void RangeTest();
void ZipTest();

int main() {
    //RangeTest();
    //ZipTest();


    std::list l = {1, 2, 3, 4, 5};
    std::vector v = {'a', 'b', 'c', 'd'};
    auto zipped = zip(l,v);

    std::vector<std::pair<int, char>> test = {zipped.begin(), zipped.end()};
    for (auto& x : test) {
        std::cout << x.first << ' ' << x.second << '\n';
    }
    return 0;
}

void RangeTest() {
    auto range = xrange(1, 5);
    std::vector<int> v1 = {range.begin(), range.end()};
    std::vector<int> v2 = {range.begin(), range.end()};

    for (auto x: v1) {
        std::cout << x << ' ';
    }
    std::cout << '\n';

    for (auto x: v2) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

void ZipTest() {
    std::list<int> l = {1};
    std::vector<char> v = {'a'};
    auto zipped1 = zip(l, v);

    std::list<int> ll = {1, 2};
    std::vector<char> vv = {'a'};
    auto zipped2 = zip(ll, vv);

    for (auto value: zipped2) {
        std::cout << value.first << " " << value.second << std::endl;
    }
}
