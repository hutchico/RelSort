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
using std::map;
using std::unordered_map;

#define NUM_SURVEYS 5

class favorite{
public:

    void set_name(string name){ identifier = name; }
    void add_partner(string input){ partners.push_back(input); }
    void reset_partners(){ partners.clear(); }

    string get_name(){ return identifier; }
    int get_hierarchy_size(){ return hierarchy.size(); }
    unordered_map<string,int> get_hierarchy(){ return hierarchy; }

    int read_hierarchy(string name){ return hierarchy[name]; }
    void modify_hierarchy(string name, int place){ hierarchy[name] = place; }
    

private:
    map<string,int> partners; //Array recording values this member was swapped with in the same cycle
    string identifier;
    unordered_map<string,int> hierarchy; //mapping to 0 implies instance is greater than key, 1 -> less than
};

void enum_data(vector<favorite> input){
    cout << "Contents:" << endl;
    for(int i = 0; i < input.size(); i++){
        cout << input[i].get_name() << endl;
    }
    cout << endl;
};
//DEBUG: Get contents of a single instance
void enum_relation(favorite a){
    cout << endl << a.get_name() << endl;
    cout << "size: " << a.get_hierarchy_size() << endl;
    cout << "has relations: " << endl;
    for (const auto & x: a.get_hierarchy()){
        cout << x.first << " : " << x.second << endl;
    }
    cout << endl;
}

void reset_references(vector<favorite> &data){
    for(int i = 0; i < data.size(); i++){
        data[i].reset_partners();
    }
};

void swap_members(vector<favorite> &data,map<string,int> &directory, int left, int right){
    favorite temp;
    if(left != right){
        cout << " swapping " << left << " and " << right << endl;
        string name_left = data[left].get_name();
        string name_right = data[right].get_name();
        
        directory[data[left].get_name()] = right;
        directory[data[right].get_name()] = left;
        temp = data[left];
        data[left] = data[right];
        data[right] = temp;
    }
    else
        cout << "something went wrong, attempting to swap same position" << endl;
};

int check_sort_compliant(vector<favorite> data, map<string,int> directory, int to_check){
    //Verify that an array member is positioned between values greater than and less than itself
    // If this isn't the case, return the array position of the offending member.
    enum_relation(data[to_check]);
    string refName = data[to_check].get_name();
    int refPos = directory[refName];
    for( const auto& x: data[to_check].get_hierarchy()){
        int direction = data[to_check].read_hierarchy(x.first);
        int testRef = directory[x.first];
        if(direction){  //given member is less than
            if(refPos > testRef){
                continue;
            }
            else
                return testRef;
        }
        else{           //given member is greater than
            if(refPos < testRef){
                continue;
            }
            else
                return testRef;
        }
    }
    return -1;
};

void record_compare(favorite &left, favorite &right, int result){
    if(result){ //User has indicated data[i] is greater than data[i+1]
            left.modify_hierarchy(right.get_name(),0);
            right.modify_hierarchy(left.get_name(),1);
        }
    else{       //User has indicated data[i] is less than data[i+1]
        left.modify_hierarchy(right.get_name(),1);
        right.modify_hierarchy(left.get_name(),0);
    }
};

int _compare(favorite a, favorite b){
    int choice;
    cout << "Left or right, 1 or 0:" << endl;
    cout << a.get_name() << " or " << b.get_name() << "?" << endl;
    cin >> choice;
    return choice;
};

void survey(vector<favorite> &data){
    srand((unsigned)time(0));
    int left, right;
    int max_pos = data.size();
    for(int i = 0; i < NUM_SURVEYS; i++){
        left = rand() % max_pos;
        while(true){
            right = rand() % max_pos;
            if(left != right)
                break;
        }
        int result = _compare(data[left],data[right]);
        record_compare(data[left],data[right],result);
    }

};

void _arrange(vector<favorite> &data,map<string,int> directory){
    //Idea: Iterate through array swapping entries, when a swap happens, restart the loop.
    //      As surveys increase this will become more computationally expensive.
    int current_ref = 0;
    while(true){
        enum_data(data);
        if(current_ref >= data.size()){
            break;
        }
        int rel = check_sort_compliant(data,directory,current_ref);
        if(rel == -1){
            current_ref++;
            continue;
        }
        swap_members(data,directory,current_ref,rel);
        current_ref = 0;
    }
};

void RelSort(vector<favorite> &data){
    //Reference dictionary for quickly finding array positions
    map<string,int> directory;
    for(int i = 0; i < data.size(); i++){
        directory[data[i].get_name()] = i;
    }

    //Initial survey, different in that comparisons are not random.
    for(int i = 0; i < data.size(); i+=2){
        int result = _compare(data[i],data[i+1]);
        record_compare(data[i],data[i+1],result);
    }
    _arrange(data,directory);
    //reset_references(data);
    enum_data(data);
    survey(data);
    _arrange(data,directory);
    enum_data(data);

};

#endif