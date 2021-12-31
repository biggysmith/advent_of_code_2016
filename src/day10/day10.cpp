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

struct instructions_t{
    int give_low_to;
    int give_high_to;
    bool give_low_to_bot = 1;
    bool give_high_to_bot = 1;
};

struct state_t{
    std::map<int,std::vector<int>> bots;
    std::map<int,instructions_t> instructions;
};

state_t load_input(const std::string& file){
    state_t ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line)){
        if(line[0] == 'v'){
            int val, bot;
            sscanf(line.c_str(),"value %d goes to bot %d", &val, &bot);
            ret.bots[bot].push_back(val);
        }else{
            instructions_t ins;
            int bot;
            char low_recip[20]; 
            char high_recip[20];
            sscanf(line.c_str(),"bot %d gives low to %s %d and high to %s %d", &bot, low_recip, &ins.give_low_to, high_recip, &ins.give_high_to);
            
            if(strcmp(low_recip, "output") == 0){
                ins.give_low_to_bot = false;
            }
            if(strcmp(high_recip, "output") == 0){
                ins.give_high_to_bot = false;
            }
            ret.instructions[bot] = ins; 
        }
        
    }   
    return ret;
}

int take_low(std::vector<int>& chips){
    auto it = std::min_element(chips.begin(),chips.end());
    int val = *it;
    chips.erase(it);
    return val;
}

int take_high(std::vector<int>& chips){
    auto it = std::max_element(chips.begin(),chips.end());
    int val = *it;
    chips.erase(it);
    return val;
}

void give(std::vector<int>& chips, int val){
    chips.push_back(val);
}

auto process(const state_t& in, bool part1, int low_cmp, int high_cmp)
{
    std::map<int,std::vector<int>> bots = in.bots;
    std::map<int,std::vector<int>> outputs;

    while(true)
    {
        auto it = std::find_if(bots.begin(), bots.end(), [](auto& a){
            auto& [_,chips] = a;
            return chips.size() > 1;
        });

        if(it == bots.end()){
            break;
        }

        auto& [bot_id,chips] = *it;
        auto& bot_ins = in.instructions.at(bot_id);

        int low_chip = take_low(chips);
        if(bot_ins.give_low_to_bot){
            give(bots[bot_ins.give_low_to], low_chip);
        }else{
            give(outputs[bot_ins.give_low_to], low_chip);
        }

        int high_chip = take_high(chips);
        if(bot_ins.give_high_to_bot){
            give(bots[bot_ins.give_high_to], high_chip);
        }else{
            give(outputs[bot_ins.give_high_to], high_chip);
        }

        if(part1 && low_chip == low_cmp && high_chip == high_cmp){
            return bot_id;
        }
    }

    return outputs[0].front() * outputs[1].front() * outputs[2].front();
}

void main()
{
    auto test_values = load_input("../src/day10/example_input.txt");
    auto actual_values = load_input("../src/day10/input.txt");

    std::cout << "part1: " << process(test_values, true, 2, 5) << std::endl;
    std::cout << "part1: " << process(actual_values, true, 17, 61) << std::endl;
    std::cout << "part2: " << process(actual_values, false, 17, 61) << std::endl;
}
