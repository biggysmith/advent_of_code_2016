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
    std::string x,y;
};

std::vector<instruction_t> load_input(const std::string& file){
    std::vector<instruction_t> ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line)){
        instruction_t ins;
        char x[20];
        char y[20];
        if(line.find("rotate left") != std::string::npos){
            sscanf(line.c_str(),"rotate left %s steps", x);
            ins.op = "rotate left";
            ins.x = std::string(x);
        }else if(line.find("rotate right") != std::string::npos){
            sscanf(line.c_str(),"rotate right %s steps", x);
            ins.op = "rotate right";
            ins.x = std::string(x);
        }else if(line.find("rotate based") != std::string::npos){
            sscanf(line.c_str(),"rotate based on position of letter %s", x);
            ins.op = "rotate based";
            ins.x = std::string(x);
        }else if(line.find("reverse") != std::string::npos){
            ins.op = "reverse";
            sscanf(line.c_str(),"reverse positions %s through %s", x, y);
            ins.x = std::string(x);
            ins.y = std::string(y);
        }else if(line.find("move") != std::string::npos){
            ins.op = "move";
            sscanf(line.c_str(),"move position %s to position %s", x, y);
            ins.x = std::string(x);
            ins.y = std::string(y);
        }else if(line.find("swap position") != std::string::npos){
            ins.op = "swap position";
            sscanf(line.c_str(),"swap position %s with position %s", x, y);
            ins.x = std::string(x);
            ins.y = std::string(y);
        }else if(line.find("swap letter") != std::string::npos){
            ins.op = "swap letter";
            sscanf(line.c_str(),"swap letter %s with letter %s", x, y);
            ins.x = std::string(x);
            ins.y = std::string(y);
        }
        ret.push_back(ins);
    }   
    return ret;
}

int mod(int a,int b){
    return (b + (a%b)) % b;
}

void rotate_left(std::string& code, const std::string& x, const std::string& y){
    std::rotate(code.begin(), code.begin()+mod(std::stoi(x), (int)code.size()), code.end());
}

void rotate_right(std::string& code, const std::string& x, const std::string& y){
    std::rotate(code.begin(), code.begin()+mod((int)code.size()-std::stoi(x), (int)code.size()), code.end());
}

void reverse(std::string& code, const std::string& x, const std::string& y){
    std::reverse(code.begin()+std::stoi(x), code.begin()+std::stoi(y)+1);
}

void rotate_based(std::string& code, const std::string& x, const std::string& y, bool right=true){
    auto ix = (int)code.find(x);
    if(right){
        ix = (ix >= 4) ? ix+2 : ix+1;
        rotate_right(code, std::to_string(ix), y);
    }else{
        ix = ix/2 + (ix%2 == 1 || ix==0 ? 1 : 5);
        rotate_left(code, std::to_string(ix), y);
    }
}

void move(std::string& code, const std::string& x, const std::string& y){
    char c = code[std::stoi(x)];
    code.erase(code.begin()+std::stoi(x));
    code.insert(code.begin()+std::stoi(y), c);
}

void swap_pos(std::string& code, const std::string& x, const std::string& y){
    std::swap(code[std::stoi(x)], code[std::stoi(y)]);
}

void swap_letter(std::string& code, const std::string& x, const std::string& y){
    auto ix = code.find(x);
    auto iy = code.find(y);
    std::swap(code[ix], code[iy]);
}

auto process1(const std::string& str, const std::vector<instruction_t>& in)
{
    auto code = str;

    for(auto it=in.begin(); it!=in.end(); ++it)
    {
        auto& ins = *it;
        if(ins.op == "rotate left"){
            rotate_left(code, ins.x, ins.y);
        }else if(ins.op == "rotate right"){
            rotate_right(code, ins.x, ins.y);
        }else if(ins.op == "rotate based"){
            rotate_based(code, ins.x, ins.y);
        }else if(ins.op == "reverse"){
            reverse(code, ins.x, ins.y);
        }else if(ins.op == "move"){
            move(code, ins.x, ins.y);
        }else if(ins.op == "swap position"){
            swap_pos(code, ins.x, ins.y);
        }else if(ins.op == "swap letter"){
            swap_letter(code, ins.x, ins.y);
        }
    }

    return code;
}

auto process2(const std::string& str, const std::vector<instruction_t>& in)
{
    auto code = str;

    for(auto it=in.rbegin(); it!=in.rend(); ++it)
    {
        auto& ins = *it;
        if(ins.op == "rotate left"){
            rotate_right(code, ins.x, ins.y);
        }else if(ins.op == "rotate right"){
            rotate_left(code, ins.x, ins.y);
        }else if(ins.op == "rotate based"){
            rotate_based(code, ins.x, ins.y, false);
        }else if(ins.op == "reverse"){
            reverse(code, ins.x, ins.y);
        }else if(ins.op == "move"){
            move(code, ins.y, ins.x);
        }else if(ins.op == "swap position"){
            swap_pos(code, ins.x, ins.y);
        }else if(ins.op == "swap letter"){
            swap_letter(code, ins.x, ins.y);
        }

    }

    return code; 
}

void main()
{
    auto test_values = load_input("../src/day21/example_input.txt");
    auto actual_values = load_input("../src/day21/input.txt");

    std::cout << "part1: " << process1("abcde", test_values) << std::endl;
    std::cout << "part1: " << process1("abcdefgh", actual_values) << std::endl;

    std::cout << "part2: " << process2("fbgdceah", actual_values) << std::endl;
}
