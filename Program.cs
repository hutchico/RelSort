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
            while(!file.EndOfStream){
                entry = file.ReadLine();
               
                obj = new Favorite(entry);
                
                data.Add(obj);
            }
            file.Close();

            Functions.print_data(data);

            Functions.RelSort(ref data);

            Functions.print_data(data);

            Functions.spit_rels(data); //DEBUG

        }
    }
}
