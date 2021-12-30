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

struct triangle_t{
    int a,b,c;
};

std::vector<triangle_t> load_input(const std::string& file){
    std::vector<triangle_t> ret;
    std::ifstream fs(file);
    std::string line;
    while(std::getline(fs, line)){
        triangle_t tri;
        std::stringstream ss(line);
        ss >> tri.a >> tri.b >> tri.c;
        ret.push_back(tri);
    }   

    return ret;
}

bool valid(const triangle_t& t){
    return t.a + t.b > t.c && t.a + t.c > t.b && t.b + t.c > t.a;
}

auto part1(const std::vector<triangle_t>& triangles)
{
    return std::count_if(triangles.begin(), triangles.end(), valid);
}

auto part2(const std::vector<triangle_t>& triangles)
{
    int valid_sum = 0;
    for(int t=0; t<triangles.size(); t+=3){
        valid_sum += valid({triangles[t].a,triangles[t+1].a,triangles[t+2].a});
        valid_sum += valid({triangles[t].b,triangles[t+1].b,triangles[t+2].b});
        valid_sum += valid({triangles[t].c,triangles[t+1].c,triangles[t+2].c});
    }
    return valid_sum;
}

void main()
{
    auto actual_values = load_input("../src/day03/input.txt");

    std::cout << "part1: " << part1(actual_values) << std::endl;
    std::cout << "part2: " << part2(actual_values) << std::endl;
}
