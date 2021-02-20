#ifndef FAVORITE_HPP
#define FAVORITE_HPP
#include <string>
#include <unordered_map>
#include <vector>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;

class favorite{
public:
    favorite(){referenced = 0};
    favorite(string name){
        referenced = 0;
        identifier = name;
    }

    void set_name(string name){ identifier = name; }
    void set_ref(){ referenced = 1; }
    void reset_ref(){ referenced = 0; }

    string get_name(){ return identifier; }
    int read_hierarchy(string name){ return hierarchy[name]; }
    void modify_hierarchy(string name, int place){ hierarchy[name] = place; }


private:
    int referenced;
    string identifier;
    std::unordered_map<string,int> hierarchy; //mapping to 0 implies instance is greater than key, 1 -> less than
};

void enum_data(std::vector<favorite> input){
    for(int i = 0; i < input.size(); i++){
        std::cout << input[i].get_name() << std::endl;
    }
}

void RelSort(vector<favorite> &data){
    //Initial sort
    for(int i = 0; i < data.size(); i+=2){
        int result = _compare(data[i],data[i+1]);
        if(result){ //User has indicated data[i] is greater than data[i+1]
            data[i].modify_hierarchy(data[i+1].get_name(),0);
            data[i+1].modify_hierarchy(data[i].get_name(),1);
        }
        else{       //User has indicated data[i] is less than data[i+1]
            data[i].modify_hierarchy(data[i+1].get_name(),1);
            data[i+1].modify_hierarchy(data[i].get_name(),0);
        }
    }
    //TODO: Continue implementing from here

}

void _arrange(vector<favorite> &data){
    //TODO: Implement once initial sort is handled above
}

int _compare(favorite a, favorite b){
    int choice;
    cout << "Left or right, 1 or 0:" << endl;
    cout << a.get_name() << " or " << b.get_name() << "?" << endl;
    cin >> choice;
    return choice;
}

#endif