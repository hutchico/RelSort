using System;
using System.IO;
using System.Collections.Generic;

namespace RelSort
{
    class Program
    {
        public static void Main(){

            StreamReader file = new StreamReader("input.txt");
            List<Favorite> input = new List<Favorite>();
            List<Favorite> data = new List<Favorite>();
            var rand = new Random();
            Favorite obj;
            string entry;

            while(!file.EndOfStream){
                entry = file.ReadLine();
                obj = new Favorite(entry);
                input.Add(obj);
            }
            file.Close();

            while(data.Count != input.Count) { //Randomize the list contents
                int pos = rand.Next(input.Count);
                data.Add(input[pos]);
                input.RemoveAt(pos);
            }

            Functions.print_data(data);

            Functions.RelSort(ref data);

            Functions.print_data(data);

            //Functions.spit_rels(data); //DEBUG

        }
    }
}
