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

auto part1(const std::vector<std::string>& lines)
{
    std::string code = "";

    int pos = 4;
    for(auto& line : lines){
        for(auto c : line){
            if(c=='U'){
                pos = std::max(pos%3, pos-3);
            }else if(c=='R'){
                pos = std::min((3*(pos/3))+2, pos+1);
            }else if(c=='D'){
                pos = std::min(6+pos%3, pos+3);
            }else if(c=='L'){
                pos = std::max((3*(pos/3)), pos-1);
            }
        }

        //code += std::to_string(pos + 1);
        code += (pos+1)+'0';
    }

    return code;
}

auto part2(const std::vector<std::string>& lines)
{
    std::vector<char> lookup { 
        '0', '0', '1', '0', '0',
        '0', '2', '3', '4', '0',
        '5', '6', '7', '8', '9',
        '0', 'A', 'B', 'C', '0',
        '0', '0', 'D', '0', '0'
    };

    std::vector<int> mn { 2,1,0,1,2 };
    std::vector<int> mx { 2,3,4,3,2 };

    std::string code = "";

    int pos_x = 0;
    int pos_y = 2;
    for(auto& line : lines){
        for(auto c : line){
            if(c=='U'){
                pos_y -= 1;
                pos_y = std::max(mn[pos_x],pos_y);
            }else if(c=='R'){
                pos_x += 1;
                pos_x = std::min(mx[pos_y],pos_x);
            }else if(c=='D'){
                pos_y += 1;
                pos_y = std::min(mx[pos_x],pos_y);
            }else if(c=='L'){
                pos_x -= 1;
                pos_x = std::max(mn[pos_y],pos_x);
            }

        }

        code += lookup[pos_y*5+pos_x];
    }

    return code;
}

void main()
{
    auto test_values = load_input("../src/day02/example_input.txt");
    auto actual_values = load_input("../src/day02/input.txt");

    std::cout << "part1: " << part1(test_values) << std::endl;
    std::cout << "part1: " << part1(actual_values) << std::endl;

    std::cout << "part2: " << part2(test_values) << std::endl;
    std::cout << "part2: " << part2(actual_values) << std::endl;
}
