#include <iostream>
#include <fstream>
#include <vector>
#include "Favorite.hpp"

int main(){

    std::ifstream file;
    std::vector<favorite> data;
    favorite entry;

    file.open("input");
    while(true){
        std::string inp_name;
        if(file.eof()){
            break;
        }
        getline(file,inp_name);
        entry.set_name(inp_name);
        data.push_back(entry);
    }
    file.close();

    RelSort(data);
    enum_data(data);

    return 0;
}