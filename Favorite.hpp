#ifndef FAVORITE_HPP
#define FAVORITE_HPP
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::map;
using std::unordered_map;
using std::pair;

#define NUM_SURVEYS 3

class favorite{
public:

    void set_name(string name){ identifier = name; }

    string get_name(){ return identifier; }
    int get_hierarchy_size(){ return hierarchy.size(); }
    unordered_map<string,int> get_hierarchy(){ return hierarchy; }

    int read_hierarchy(string name){ return hierarchy[name]; }
    void modify_hierarchy(string name, int place){ hierarchy[name] = place; }
    

private:
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

void emplace_member(vector<favorite> &data,map<string,int> &directory, int member){

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

void record_compare(favorite &left, favorite &right){
    int choice;
    cout << "Left or right, 1 or 0:" << endl;
    cout << left.get_name() << " or " << right.get_name() << "?" << endl;
    cin >> choice;
    if(choice){ //User has indicated data[i] is greater than data[i+1]
            left.modify_hierarchy(right.get_name(),0);
            right.modify_hierarchy(left.get_name(),1);
        }
    else{       //User has indicated data[i] is less than data[i+1]
        left.modify_hierarchy(right.get_name(),1);
        right.modify_hierarchy(left.get_name(),0);
    }
};

//Check if every favorite member has at least N relations
bool confirm_relation_min(vector<favorite> data){
    for(int i = 0; i < data.size(); i++){
        if(data[i].get_hierarchy_size() >= NUM_SURVEYS)
            continue;
        else
            return false;
    }
    return true;
};

//Check if a pair has been used before
bool test_pair(pair<int,int> input, vector<pair<int,int>> db){
    pair<int,int> input_inv; //invert the pair to check for backwards comparison
    input_inv.first = input.second;
    input_inv.second = input.first;
    for(int i = 0; i < db.size(); i++){
        if(db[i] == input || db[i] == input_inv)
            return true;
    }
    return false;
};

//remove a given name from the vector
void _remove_name(vector<string> &subset, string toRem){
    for(int i = 0; i < subset.size(); i++){
        if(subset[i] == toRem){
            subset.erase(subset.begin()+i);
            break;
        }
    }
};

void survey(vector<favorite> &data,map<string,int> directory){
    srand((unsigned)time(0));
    vector<string> subset;  //records members which have less than N relations
    vector<pair<int,int>> tried;
    pair<int,int> compair;
    for(int i = 0; i < data.size(); i++){
        subset.push_back(data[i].get_name());
    }
    while(!confirm_relation_min(data)){
        int maxRoll = subset.size();
        int left = rand() % maxRoll;
        int right = rand() % maxRoll;
        if(right == left)
            continue; //Don't compare a member to itself

        string leftName = subset[left];
        string rightName = subset[right];
        left = directory[leftName];
        right = directory[rightName];
        compair.first = left;
        compair.second = right;
        if(test_pair(compair,tried)) //this value is a repeat, start over and hopefully get a different set.
            continue; 
        
        record_compare(data[left],data[right]); //perform comparison, modify relations

        if(data[left].get_hierarchy_size() >= NUM_SURVEYS){
            _remove_name(subset,data[left].get_name());
        }
        if(data[right].get_hierarchy_size() >= NUM_SURVEYS){
            _remove_name(subset,data[right].get_name());
        }
        tried.push_back(compair);
    }


};

void _arrange(vector<favorite> &data,map<string,int> directory){
    //Idea: Iterate through array swapping entries, when a swap happens, restart the loop.
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
    //Reference dictionary recording initial array positions
    map<string,int> directory;
    for(int i = 0; i < data.size(); i++){
        directory[data[i].get_name()] = i;
    }

    //Initial survey, different in that comparisons are not random.
    
    survey(data,directory);
    _arrange(data,directory);
    enum_data(data);

};

#endif