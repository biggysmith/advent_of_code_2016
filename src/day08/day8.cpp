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

struct operation_t{
    std::string type;
    int a,b;
};

std::vector<operation_t> load_input(const std::string& file){
    std::vector<operation_t> ret;
    std::ifstream fs(file);
    std::string line;
    std::string type;
    std::string rotate_type;
    char d;
    while(std::getline(fs, line)){
        operation_t op;
        std::stringstream ss(line);
        ss >> type;
        if(type == "rect"){
            rotate_type = "";
            ss >> op.a >> d >> op.b;
        }else if(type == "rotate"){
            ss >> rotate_type;
            ss >> d >> d >> op.a >> d >> d >> op.b;
        }
        ret.push_back({ type+rotate_type, op.a, op.b });
    }   

    return ret;
}

struct grid_t{
    grid_t() {}
    grid_t(int w,int h) : points(w*h,'.'), width(w), height(h) {}

    char& get(int x,int y) { return points[y*width+x]; }
    const char& get(int x,int y) const { return points[y*width+x]; }

    std::vector<char> points;
    int width, height;
};

int mod(int a,int b){
    return (b + (a%b)) % b;
}

void process(grid_t& dst,const grid_t& src,const operation_t& op){
    if(op.type == "rotaterow"){
        for(int x=0; x<dst.width; ++x){
            dst.get(x,op.a) = src.get(mod(x-op.b,src.width),op.a);
        }
    }
    else if(op.type == "rotatecolumn"){
        for(int y=0; y<dst.height; ++y){
            dst.get(op.a,y) = src.get(op.a,mod(y-op.b,src.height));
        }
    }
    else if(op.type == "rect"){
        for(int y=0; y<op.b; ++y){
            for(int x=0; x<op.a; ++x){
                dst.get(x,y) = '#';
            }
        }
    }
}

void print(const grid_t& grid)
{
    for(int y=0; y<grid.height; ++y){
        for(int x=0; x<grid.width; ++x){
            std::cout << (grid.get(x,y) == '#' ? '#' : ' ');
        }
        std::cout << std::endl;
    }
}

auto render(const std::vector<operation_t>& operations)
{
    grid_t src(50, 6);
    grid_t dst = src;

    for(auto& operation : operations){
        process(dst, src, operation);
        src = dst;
    }

    std::cout << "part1: " << std::count(dst.points.begin(), dst.points.end(), '#') << std::endl;

    std::cout << "part2:" << std::endl;
    print(dst);
}

void main()
{
    auto actual_values = load_input("../src/day08/input.txt");

    render(actual_values);
}
