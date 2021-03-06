using System;
using System.Collections.Generic;

namespace RelSort
{
    public class Favorite
    {
        public Favorite(){
            identifier = null;
            lower = new List<string>();
            higher = new List<string>();
        }
        public Favorite(string name){
            identifier = name;
            lower = new List<string>();
            higher = new List<string>();
        }

        public string get_name(){ return identifier; }
        public int get_hierarchy_size(){ return lower.Count + higher.Count; }
        public int get_lowSize(){ return lower.Count; }
        public int get_highSize(){ return higher.Count; }

        public string read_lower(int pos){ return lower[pos]; }
        public string read_higher(int pos){ return higher[pos]; }

        public void inc_lower(string name){ lower.Add(name); }
        public void inc_higher(string name){ higher.Add(name); }

        //DEBUG: get contents of object
        public void print_relations(){
            System.Console.WriteLine("Name: " + identifier);
            System.Console.WriteLine("Relations: ");
            System.Console.WriteLine("Less than: ");
            foreach(string rel in higher){
                System.Console.WriteLine(rel);
            }
            System.Console.WriteLine("Greater than: ");
            foreach (string rel in lower){
                System.Console.WriteLine(rel);
            }
        }

        private List<string> lower;
        private List<string> higher;
        private string identifier;
    }
}