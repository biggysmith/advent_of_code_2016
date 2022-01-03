#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <functional>
#include <set>
#include <map>
#include <queue>

struct range_t{
    size_t low = 0;
    size_t high = 0;
};

bool operator<(const range_t& a, const range_t& b) {
    return std::make_tuple(a.low,a.high) < std::make_tuple(b.low,b.high);
}

std::vector<range_t> load_input(const std::string& file){
    std::vector<range_t> ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line)){
        range_t range;
        sscanf(line.c_str(),"%zu-%zu", &range.low, &range.high);
        ret.push_back(range);
    }   
    return ret;
}

struct interval_set
{
    void insert(const range_t& range)
    {
        auto after = ranges.upper_bound(range.low);
        auto insert_range = after;

        if (after == ranges.begin() || std::prev(after)->second < range.low) {
            insert_range = ranges.insert(after, {range.low,range.high+1}); // +1 since inclusive
        }   
        else {
            insert_range = std::prev(after);
            if (insert_range->second >= range.high+1) {
                return;
            }   
            else {
                insert_range->second = range.high+1;
            }   
        }   

        while (after != ranges.end() && range.high+1 >= after->first) {
            insert_range->second = std::max(after->second, insert_range->second);
            after = ranges.erase(after);
        }   
    }

    std::map<size_t,size_t> ranges; // stupid std::set iterators are not mutable, so lets abuse map
};

auto process(const std::vector<range_t>& in, size_t mx)
{
    interval_set intervals;
    for(auto& range : in){
        intervals.insert({range.low,range.high});
    }

    size_t free = mx + 1ull;
    for(auto& [low,high] : intervals.ranges){
        free -= high - low;
    }

    return std::make_pair(intervals.ranges.begin()->second, free);
}

void main()
{
    auto test_values = load_input("../src/day20/example_input.txt");
    auto actual_values = load_input("../src/day20/input.txt");

    auto part1 = process(test_values, 9);
    auto part2 = process(actual_values, UINT_MAX);

    std::cout << "part1: " << part1.first << std::endl;
    std::cout << "part1: " << part2.first << std::endl;

    std::cout << "part2: " << part1.second << std::endl;
    std::cout << "part2: " << part2.second << std::endl;
}
