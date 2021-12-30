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

std::vector<std::string> load_input(const std::string& file){
    std::vector<std::string> ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line)){
        ret.push_back(line);
    }   
    return ret;
}

auto part1(const std::vector<std::string>& signal)
{
    std::vector<std::vector<int>> hists(signal[0].size(), std::vector<int>(26,0));

    for(auto& line : signal){
        for(int i=0; i<line.size(); ++i){
            hists[i][line[i]-'a']++;
        }
    }

    std::string result;
    for(auto& hist : hists){
        auto it = std::max_element(hist.begin(), hist.end());
        result += (char)(std::distance(hist.begin(),it))+'a';
    }

    return result;
}

auto part2(const std::vector<std::string>& signal)
{
    std::vector<std::vector<int>> hists(signal[0].size(), std::vector<int>(26,0));

    for(auto& line : signal){
        for(int i=0; i<line.size(); ++i){
            hists[i][line[i]-'a']++;
        }
    }

    std::string result;
    for(auto& hist : hists){
        auto it = std::min_element(hist.begin(), hist.end(), [](auto& a,auto& b){
            int aa = a==0 ? INT_MAX : a;  
            int bb = b==0 ? INT_MAX : b;  
            return aa < bb;
        });
        result += (char)(std::distance(hist.begin(),it))+'a';
    }

    return result;
}

void main()
{
    auto test_values = load_input("../src/day06/example_input.txt");
    auto actual_values = load_input("../src/day06/input.txt");

    std::cout << "part1: " << part1(test_values) << std::endl;
    std::cout << "part1: " << part1(actual_values) << std::endl;

    std::cout << "part2: " << part2(test_values) << std::endl;
    std::cout << "part2: " << part2(actual_values) << std::endl;
}
