using System;
using System.Collections.Generic;

namespace RelSort
{
    public class Favorite
    {
        public Favorite(){
            name = null;
            lower = new List<string>();
            higher = new List<string>();
        }
        public Favorite(string name){
            this.name = name;
            lower = new List<string>();
            higher = new List<string>();
        }

        public string get_name(){ return name; }
        public int get_hierarchy_size(){ return lower.Count + higher.Count; }
        public int get_lowSize(){ return lower.Count; }
        public int get_highSize(){ return higher.Count; }

        public string read_lower(int pos){ return lower[pos]; }
        public string read_higher(int pos){ return higher[pos]; }

        public void inc_lower(string rel){ lower.Add(rel); }
        public void inc_higher(string rel){ higher.Add(rel); }

        //DEBUG: get contents of object
        public void print_relations(){
            System.Console.WriteLine("Name: " + name);
            if(higher.Count > 0)
                System.Console.WriteLine("Less than: ");
            foreach(string rel in higher){
                System.Console.WriteLine(rel);
            }
            if(lower.Count > 0)
                System.Console.WriteLine("Greater than: ");
            foreach (string rel in lower){
                System.Console.WriteLine(rel);
            }
        }

        private List<string> lower;
        private List<string> higher;
        private readonly string name;

        //private List<KeyValuePair<string,int>> hierarchy; //beginnings of a return to a previous design choice
    }
}