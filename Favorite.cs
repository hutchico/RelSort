using System;
using System.Collections.Generic;

namespace RelSort
{
    public class Favorite
    {
        public Favorite(){
            name = null;
        }
        public Favorite(string name){
            this.name = name;
            hierarchy = new List<KeyValuePair<string,int>>();
            num_higher = 0;
            num_lower = 0;
        }

        public string get_name(){ return name; }
        public int get_hierarchy_size(){ return hierarchy.Count; }

        public string get_relName(int pos) { return hierarchy[pos].Key; }
        public int get_relType(int pos) { return hierarchy[pos].Value; }
        public int get_lower() { return num_lower; }
        public int get_higher() { return num_higher; }

       

        public void add_relation(string rel,int dir){
            hierarchy.Add(new KeyValuePair<string,int>(rel,dir));
            if(dir == 1)
                ++num_higher;
            else
                ++num_lower;
        }

        //DEBUG: get contents of object
        public void print_relations(){
            System.Console.WriteLine("Name: " + name);
            if(num_higher > 0)
                System.Console.WriteLine("Less than: ");
            foreach(KeyValuePair<string,int> entry in hierarchy){
                if(entry.Value == 1)
                    System.Console.WriteLine(entry.Key);
            }
            if(num_lower > 0)
                System.Console.WriteLine("Greater than: ");
            foreach(KeyValuePair<string,int> entry in hierarchy) {
                if(entry.Value == 0)
                    System.Console.WriteLine(entry.Key);
            }
        }

        //private List<string> lower;
        //private List<string> higher;
        private readonly string name;

        private List<KeyValuePair<string,int>> hierarchy; //return to a previous design choice
        private int num_lower;
        private int num_higher;
    }
}