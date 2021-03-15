using System;
using System.IO;
using System.Collections.Generic;

namespace RelSort
{
    class Program
    {
        public static void Main(){

            StreamReader file = new StreamReader("input.txt");
            List<Favorite> data = new List<Favorite>();
            Favorite obj;
            string entry;
            int min_surveys;

            while(!file.EndOfStream){
                entry = file.ReadLine();
                obj = new Favorite(entry);
                data.Add(obj);
            }
            file.Close();

            min_surveys = data.Count * (data.Count / 4); //sort function will need to perform AT LEAST this many comparisons to build a list

            Functions.print_data(data);

            Functions.RelSort(ref data);

            Functions.print_data(data);

            //Functions.spit_rels(data); //DEBUG

        }
    }
}
