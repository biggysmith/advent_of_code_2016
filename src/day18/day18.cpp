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

std::string load_input(const std::string& file){
    std::ifstream fs(file);
    std::string line;
    std::getline(fs, line);
    return line;
}

std::string trapify(const std::string& row){
    std::string next_row(row.size(), '.');
    for(int i=0; i<row.size(); ++i){
        bool l = i>0 ? row[i-1] == '^' : false;
        bool r = i<(row.size()-1) ? row[i+1] == '^' : false;
       
        if(l ^ r) {
            next_row[i] = '^';
        }else{
            next_row[i] = '.';
        }
    }
    return next_row;
}

auto process(const std::string& in,int rows)
{
    std::string row = in;
    size_t sum = std::count(row.begin(),row.end(),'.');

    for(int i=1; i<rows; ++i){
        row = trapify(row);
        sum += std::count(row.begin(),row.end(),'.');
    }

    return sum;
}

void main()
{
    auto test_values = load_input("../src/day18/example_input.txt");
    auto actual_values = load_input("../src/day18/input.txt");

    std::cout << "part1: " << process(test_values, 10) << std::endl;
    std::cout << "part1: " << process(actual_values, 40) << std::endl;

    std::cout << "part2: " << process(actual_values, 400000) << std::endl;
}
