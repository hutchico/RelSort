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
    
    //DEBUG: get contents of instance
    void enum_relations(){
        cout << endl << "Name: " << identifier << endl;
        cout << "Relations: " << endl;
        cout << "Less Than: ";
        for(int i = 0; i < higher.size(); i++)
            cout << higher[i] << " , ";
        cout << endl << "Greater Than: ";
        for(int i = 0; i < lower.size(); i++)
            cout << lower[i] << " , ";
        cout << endl;
    }

private:
    string identifier;
    vector<string> lower;  //record instances which are LESS THAN this
    vector<string> higher; //"" GREATER
};

//DEBUG: confirm relation contents across all inputs
void spit_data(vector<favorite> data){
    cout << "Details: " << endl;
    for(int i = 0; i < data.size(); i++){
        data[i].enum_relations();
    }
};

void enum_data(vector<favorite> input){
    cout << "Contents:" << endl;
    for(int i = 0; i < input.size(); i++){
        cout << input[i].get_name() << endl;
    }
    cout << endl;
};

//DEBUG: print pairs
void enum_pairs(vector<pair<string,string>> pairs){
    for(int i = 0; i < pairs.size(); i++){
        cout << pairs[i].first << " " << pairs[i].second << endl;
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

//'member' variable refers to the array position of the member being emplaced
void emplace_member(vector<favorite> &data,map<string,int> &directory, int member){

};

int check_sort_compliant(vector<favorite> data, map<string,int> directory, int to_check){
    //Verify that an array member at position to_check is positioned between all values greater than and less than itself
    // If this isn't the case, return the array position of the offending member.
    data[to_check].enum_relations();
    int refPos = directory[data[to_check].get_name()];
    for(int i = 0; i < data[to_check].get_highSize(); i++){
        int testRef = directory[data[to_check].read_higher(i)];
        if(refPos < testRef)
            continue;
        else
            return testRef;
    }
    for(int i = 0; i < data[to_check].get_lowSize(); i++){
        int testRef = directory[data[to_check].read_lower(i)];
        if(refPos > testRef)
            continue;
        else
            return testRef;
    }
    return -1;
};

void record_compare(favorite &left, favorite &right){
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

//remove a given name from the tracking vector and return its position
void remove_name(vector<string> &subset, string toRem){
    //DEBUG
    for(int i = 0; i < subset.size(); i++){
        if(subset[i] == toRem){
            subset.erase(subset.begin()+i);
            break;
        }
    }
};

//follow name removal by removing references to that name
void remove_pairs(vector<pair<int,int>> &tried, int value){
    //NOTE: value refers to the array position of a value, not test case name!
    
    for(int i = 0; i <= tried.size(); i++){
        if(tried[i].first == value || tried[i].second == value){
            tried.erase(tried.begin()+i);
            i = 0;
        }
    }
}

void survey(vector<favorite> &data,map<string,int> directory){
    srand((unsigned)time(0));
    vector<string> subset;  //records members which have less than N relations
    vector<pair<string,string>> tried;
    //map<string,int> times_compared;
    pair<string,string> compair;
    for(int i = 0; i < data.size(); i++){
        subset.push_back(data[i].get_name());
        //times_compared[data[i].get_name()] = 0; //exception prevention
    }
    while(!confirm_relation_min(data)){
        int maxRoll = subset.size();
        int left = rand() % maxRoll;    //roll two array positions
        int right = rand() % maxRoll;
        if(right == left)
            continue; //Don't compare a member to itself

        string leftName = subset[left];
        string rightName = subset[right];
        compair.first = leftName;
        compair.second = rightName;
        if(test_pair(compair,tried)) //this value is a repeat, start over and hopefully get a different set.
            continue; 
        tried.push_back(compair);

        left = directory[leftName];
        right = directory[rightName];
        record_compare(data[left],data[right]); //perform comparison, modify relations

        /*
        times_compared[leftName] = times_compared[leftName] + 1;
        times_compared[rightName] = times_compared[rightName] + 1;
        */
        enum_pairs(tried);
        /*
        if(data[left].get_hierarchy_size() >= NUM_SURVEYS){
            cout << "removing left:" << leftName << endl; //DEBUG: verify branch
            remove_name(subset,data[left].get_name());   
        }
        if(data[right].get_hierarchy_size() >= NUM_SURVEYS){
            cout << "removing right:" << rightName << endl; //DEBUG: verify branch
            remove_name(subset,data[right].get_name());
        }
        */
    }


};

void arrange(vector<favorite> &data,map<string,int> &directory){
    //Start from the bottom of the list, move entry to just above its highest entry
    
};

void RelSort(vector<favorite> &data){
    //Reference dictionary recording initial array positions
    map<string,int> directory;
    for(int i = 0; i < data.size(); i++){
        directory[data[i].get_name()] = i;
    }
    
    survey(data,directory);

    //arrange(data,directory);

    enum_data(data);

    spit_data(data);

    //enum_data(data); //DEBUG

};

#endif