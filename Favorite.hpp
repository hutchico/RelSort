#ifndef FAVORITE_HPP
#define FAVORITE_HPP
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;

class favorite{
public:
    favorite(){ referenced = 0; }

    void set_name(string name){ identifier = name; }
    void set_ref(int input){ referenced = input; }

    string get_name(){ return identifier; }
    int get_ref(){ return referenced; }
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
    //Reference dictionary for quickly finding array positions
    std::map<string,int> directory;
    for(int i = 0; i < data.size(); i++){
        directory[data[i].get_name()] = i;
    }

    //Initial survey, this is different in that comparisons are not random.
    for(int i = 0; i < data.size(); i+=2){
        int result = _compare(data[i],data[i+1]);
        data[i].set_ref(1);
        data[i+1].set_ref(1);
        if(result){ //User has indicated data[i] is greater than data[i+1]
            data[i].modify_hierarchy(data[i+1].get_name(),0);
            data[i+1].modify_hierarchy(data[i].get_name(),1);
        }
        else{       //User has indicated data[i] is less than data[i+1]
            data[i].modify_hierarchy(data[i+1].get_name(),1);
            data[i+1].modify_hierarchy(data[i].get_name(),0);
        }
    }
    reset_references(data);

}

void survey(vector<favorite> &data){
    srand((unsigned)time(0));
    int left, right;
    int max_pos = data.size();
    while(check_complete(data)){
        while(true){
            left = rand() % max_pos;
            if(!data[left].get_ref()){
                break;
            }
        }
        while(true){
            right = rand() % max_pos;
            if(right != left && !data[right].get_ref()){
                break;
            }
        }
    }

}

void _arrange(vector<favorite> &data,std::map<string,int> directory){
    //TODO: Implement once initial sort is handled above
}

int check_complete(vector<favorite> data){ //Verify if the entire array has member Referenced set to 1
    for(int i = 0; i < data.size(); i++){
        if(data[i].get_ref()){
            continue;
        }
        else
            return 0;
    }
    return 1;
}

void reset_references(vector<favorite> &data){
    for(int i = 0; i < data.size(); i++){
        data[i].set_ref(0);
    }
}

int _compare(favorite a, favorite b){
    int choice;
    cout << "Left or right, 1 or 0:" << endl;
    cout << a.get_name() << " or " << b.get_name() << "?" << endl;
    cin >> choice;
    return choice;
}

#endif