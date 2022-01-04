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
#include <omp.h>

struct instruction_t{
    std::string op;
    std::string x;
    std::string y;
};

std::vector<instruction_t> load_input(const std::string& file){
    std::vector<instruction_t> ret;
    std::ifstream fs(file);
    std::string line;
    std::string dummy;
    while(std::getline(fs, line)){
        std::stringstream ss(line);
        instruction_t ins;
        ss >> ins.op >> ins.x >> ins.y;
        ret.push_back(ins);
    }    
    return ret;
}

struct program_t
{
    program_t(const std::vector<instruction_t>& in) : instructions(in) {
        for(char c='a'; c<='d'; ++c){
            regs[std::string(1,c)] = 0;
        }
    }

    void run()
    {
        auto get = [&](const std::string& s){
            return (s[0]>='a' && s[0]<='z') ? regs[s] : std::stoll(s);
        };

        while(ipos < instructions.size())
        {
            auto& ins = instructions[ipos];

            if(ins.op == "cpy"){
                regs[ins.y] = get(ins.x);
            }else if(ins.op == "out"){
                if(get(ins.x)){
                    binary |= (1ULL << bit); 
                }
                bit++;
            }else if(ins.op == "inc"){
                regs[ins.x] ++;
            }else if(ins.op == "dec"){
                regs[ins.x] --;
            }else if(ins.op == "jnz"){
                if(get(ins.x) != 0){
                    ipos += get(ins.y) - 1;
                }
            }

            ipos++;

            if(bit >= 32){
                break;
            }
        }
    }

    int64_t a_value() const{
        return regs.at("a");
    }

    int bit = 0;
    unsigned binary = 0;
    std::vector<instruction_t> instructions;
    std::map<std::string,int64_t> regs;
    size_t ipos = 0;
};

auto part1(const std::vector<instruction_t>& in)
{
    size_t i = 0;
    while(true){
        program_t prog(in);
        prog.regs["a"] = i;
        prog.run();
        if(prog.binary == 0xAAAAAAAA){
            break;
        }
        i++;
    }
    return i; 
}

void main()
{
    auto actual_values = load_input("../src/day25/input.txt");

    std::cout << "part1: " << part1(actual_values) << std::endl;
}
