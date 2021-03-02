#ifndef FAVORITE_HPP
#define FAVORITE_HPP
#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <assert.h> //DEBUG

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::map;
using std::pair;

#define NUM_SURVEYS 3

class favorite{
public:

    void set_name(string name){ identifier = name; }

    string get_name(){ return identifier; }
    int get_hierarchy_size(){ return lower.size() + higher.size(); }
    int get_lowSize(){ return lower.size(); }
    int get_highSize(){ return higher.size(); }

    vector<string> get_lower(){ return lower; }
    vector<string> get_higher(){ return higher; }
    string read_lower(int pos){ return lower[pos]; }
    string read_higher(int pos){ return higher[pos]; }

    void inc_lower(string name){ lower.push_back(name); }
    void inc_higher(string name){ higher.push_back(name); }
    
    

private:
    string identifier;
    vector<string> lower;  //record instances which are LESS THAN this
    vector<string> higher; //"" GREATER
};

//DEBUG: get contents of instance
void enum_relations(favorite item){
        cout << "Name: " << item.get_name() << endl;
        cout << "Relations: " << endl;
        cout << "Less Than: ";
        for(int i = 0; i < item.get_highSize(); i++)
            cout << item.read_higher(i) << " , ";
        cout << endl << "Greater Than: ";
        for(int i = 0; i < item.get_lowSize(); i++)
            cout << item.read_lower(i) << " , ";
        cout << endl;
}

//DEBUG: confirm relation contents across all inputs
void spit_data(vector<favorite> data){
    cout << "Details: " << endl;
    for(int i = 0; i < data.size(); i++){
        enum_relations(data[i]);
    }
};

//DEBUG: print pairs
void enum_pairs(vector<pair<string,string>> pairs){
    for(int i = 0; i < pairs.size(); i++){
        cout << pairs[i].first << " " << pairs[i].second << endl;
    }
    cout << endl;
}

void enum_data(vector<favorite> input){
    cout << "Contents:" << endl;
    for(int i = 0; i < input.size(); i++){
        cout << input[i].get_name() << endl;
    }
    cout << endl;
};

void swap(vector<favorite> &data,map<string,int> &directory, int left, int right){
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

//insert member at position, shuffle dictionary references down one value
void insert_item(vector<favorite> &data,map<string,int> &directory, string target, int loc, int mode){
    enum_relations(data[directory[target]]);
    assert(loc >= 0 && loc < data.size());

    int targetLoc = directory[target];
    favorite out = data[targetLoc];
    int i;
    
    if(mode == 1){
        i = loc; //placing above a value
        data.erase(data.begin()+targetLoc);
        data.insert(data.begin()+loc,out);
    }
    else{
        i = targetLoc; //placing below a value
        data.insert(data.begin()+loc,out);
        data.erase(data.begin()+targetLoc);
    }

    if(loc == data.size()-1){ //perform a swap operation if we've inserted a value at the end of the vector
        out = data[data.size()-1];
        data[data.size()-1] = data[loc];
        data[loc] = out;
    }
    for(; i < data.size(); i++){
        directory[data[i].get_name()] = i;
    }
};

//Verify that an array member at position to_check is positioned between all values greater than and less than itself
bool check_sort_compliant(vector<favorite> data, map<string,int> directory, int to_check){
    //enum_relations(data[to_check]);
    int refPos = directory[data[to_check].get_name()];
    for(int i = 0; i < data[to_check].get_highSize(); i++){
        int testRef = directory[data[to_check].read_higher(i)];
        if(refPos > testRef)
            continue;
        else
            return false;
    }
    for(int i = 0; i < data[to_check].get_lowSize(); i++){
        int testRef = directory[data[to_check].read_lower(i)];
        if(refPos < testRef)
            continue;
        else
            return false;
    }
    return true;
};

//ask user which member is greater, record result implictly within each member
void compare(favorite &left, favorite &right){
    int choice;
    cout << "Left or right, 1 or 0:" << endl;
    cout << left.get_name() << " or " << right.get_name() << "?" << endl;
    cin >> choice;
    if(choice){ //User has indicated data[i] is greater than data[i+1]
            left.inc_lower(right.get_name());
            right.inc_higher(left.get_name());
        }
    else{       //User has indicated data[i] is less than data[i+1]
        left.inc_higher(right.get_name());
        right.inc_lower(left.get_name());
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
bool test_pair(pair<string,string> input, vector<pair<string,string>> db){
    pair<string,string> input_inv; //invert the pair to check for backwards comparison
    input_inv.first = input.second;
    input_inv.second = input.first;
    for(int i = 0; i < db.size(); i++){
        if(db[i] == input || db[i] == input_inv)
            return true;
    }
    return false;
};

void survey(vector<favorite> &data,map<string,int> directory){
    srand((unsigned)time(0));
    vector<pair<string,string>> tried;
    pair<string,string> compair;
    int maxRoll = data.size();
    while(!confirm_relation_min(data)){
        int left = rand() % maxRoll;    
        int right = rand() % maxRoll;
        if(right == left)
            continue; //Don't compare a member to itself

        compair.first = data[left].get_name();
        compair.second = data[right].get_name();
        if(test_pair(compair,tried)) //this value is a repeat, start over and hopefully get a different set.
            continue; 
        tried.push_back(compair);

        compare(data[left],data[right]); //perform comparison, modify relations
    }

};

//determine the array position directly below this instance
//as in, lowest array position this instance can be above
int find_greatest_array_val(favorite input, map<string,int> directory){
    int greatest = INT32_MAX; //arbitrary large number
    for(int i = 0; i < input.get_lowSize(); i++){
        int expanded = directory[input.read_lower(i)];
        if(expanded < greatest)
            greatest = expanded;
    }
    assert(greatest < INT32_MAX);
    return greatest;
}

//determine the array position directly above this instance
//as in, highest array position this instance can be below
int find_lowest_array_val(favorite input, map<string,int> directory){
    int lowest = 0;
    for(int i = 0; i < input.get_highSize(); i++){
        int expanded = directory[input.read_higher(i)];
        if(expanded > lowest)
            lowest = expanded;
    }
    assert(lowest != 0);
    return lowest;
}

void arrange(vector<favorite> &data,map<string,int> &directory){
    //Start from the bottom of the list, move entry to just above its highest entry
    //after moving an entry, start at the bottom again
    int current_ref = data.size() - 1;
    while(current_ref >= 0){
        //cout << "currently sorting: " << current_ref << endl;
        enum_data(data);
        if(!check_sort_compliant(data,directory,current_ref)){
            if(data[current_ref].get_lowSize() > 0) //Attempt to place instance above its highest low value
                insert_item(data,directory,data[current_ref].get_name(),find_greatest_array_val(data[current_ref],directory), 1);
            else{                                   //if this isn't possible, just move it to the bottom of the array
                int pos = find_lowest_array_val(data[current_ref],directory);
                insert_item(data,directory,data[current_ref].get_name(),pos,2);
            }
            current_ref = data.size() - 1;
        }
        else
            current_ref--;
    }
};

void RelSort(vector<favorite> &data){
    //Reference dictionary recording initial array positions
    map<string,int> directory;
    for(int i = 0; i < data.size(); i++){
        directory[data[i].get_name()] = i;
    }
    
    survey(data,directory);
    cout << "Sorting, please wait." << endl;
    arrange(data,directory);
    cout << "Final order: " << endl;
    enum_data(data);

    spit_data(data);

    //enum_data(data); //DEBUG

};

#endif