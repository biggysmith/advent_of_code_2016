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
#include <charconv>

std::string load_input(const std::string& file){
    std::string ret;
    std::ifstream fs(file);
    std::string line;
    std::getline(fs, line);
    return line;
}

size_t decompress(const std::string_view& input, int version)
{
    size_t size = 0;

    for(size_t i=0; i<input.size(); )
    {
        if(input[i] == '('){
            size_t x = input.find_first_of('x', i);
            size_t right_bracket = input.find_first_of(')', i);
            int num_chars, num_repeat;
            std::from_chars(input.data()+i+1, input.data()+x, num_chars);
            std::from_chars(input.data()+x+1, input.data()+right_bracket, num_repeat);
            i = right_bracket+1;
            if(version == 1){
                size += num_chars * num_repeat;
            }else{
                std::string_view repeat_view = std::string_view(input.data()+i,num_chars);
                size += decompress(repeat_view, version) * num_repeat;
            }
            i += num_chars;
        }else{
            size++;
            i++;
        }
    }

    return size;
}

void main()
{
    auto actual_values = load_input("../src/day09/input.txt");

    std::cout << "part1: " << decompress(actual_values, 1) << std::endl;
    std::cout << "part2: " << decompress(actual_values, 2) << std::endl;
}
