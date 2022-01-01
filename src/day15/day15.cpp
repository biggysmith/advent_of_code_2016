#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <functional>

struct disc_t{
    int pos;
    int positions;
};

std::vector<disc_t> load_input(const std::string& file){
    std::vector<disc_t> ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line)){
        int num;
        disc_t disc;
        sscanf(line.c_str(),"Disc #%d has %d positions; at time=0, it is at position %d", &num, &disc.positions, &disc.pos);
        ret.push_back(disc);
    }   

    return ret;
}

auto process(const std::vector<disc_t>& in, bool part2)
{
    auto discs = in;
    if(part2){
        discs.push_back({0,11});
    }

    int time = 0;

    while(true)
    {
        bool success = true;
        for(int i=0; i<discs.size(); ++i){
            discs[i].pos = (discs[i].pos + 1) % discs[i].positions;
            success &= ((discs[i].pos+i) % discs[i].positions) == 0;
        }
        
        if(success){
            return time;
        }

        time++;
    }

    return 0;
}

void main()
{
    auto test_values = load_input("../src/day15/example_input.txt");
    auto actual_values = load_input("../src/day15/input.txt");

    std::cout << "part1: " << process(test_values, false) << std::endl;
    std::cout << "part1: " << process(actual_values, false) << std::endl;

    std::cout << "part2: " << process(actual_values, true) << std::endl;
}
