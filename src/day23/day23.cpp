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
            }else if(ins.op == "mul"){
                regs[ins.y] = get(ins.x) * get(ins.y);
            }else if(ins.op == "add"){
                regs[ins.y] += get(ins.x);
            }else if(ins.op == "inc"){
                regs[ins.x] ++;
            }else if(ins.op == "dec"){
                regs[ins.x] --;
            }else if(ins.op == "tgl"){
                auto jmp = get(ins.x);
                if(ipos+jmp < instructions.size()){
                    auto& ins = instructions[ipos+jmp];
                    if(ins.op == "inc"){
                        ins.op = "dec";
                    }else if(ins.op == "dec"){
                        ins.op = "inc";
                    }else if(ins.op == "tgl"){
                        ins.op = "inc";
                    }else if(ins.op == "jnz"){
                        ins.op = "cpy";
                    }else if(ins.op == "cpy"){
                        ins.op = "jnz";
                    }
                }
            }else if(ins.op == "jnz"){
                if(get(ins.x) != 0){
                    ipos += get(ins.y) - 1;
                }
            }

            ipos++;
        }
    }

    int64_t a_value() const{
        return regs.at("a");
    }

    std::vector<instruction_t> instructions;
    std::map<std::string,int64_t> regs;
    size_t ipos = 0;
};

auto part1(const std::vector<instruction_t>& in)
{
    program_t prog(in);
    prog.regs["a"] = 7;
    prog.run();
    return prog.a_value();
}

auto part2(const std::vector<instruction_t>& in)
{
    program_t prog(in);
    prog.regs["a"] = 12;
    prog.run();
    return prog.a_value();
}

void main()
{
    auto test_values = load_input("../src/day23/example_input.txt");
    auto actual_values = load_input("../src/day23/input.txt");
    auto actual_values1 = load_input("../src/day23/input2.txt");

    std::cout << "part1: " << part1(test_values) << std::endl;
    std::cout << "part1: " << part1(actual_values) << std::endl;

    std::cout << "part2: " << part2(actual_values1) << std::endl;
}
