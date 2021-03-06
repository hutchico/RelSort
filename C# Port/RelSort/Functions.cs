using System;
using System.Collections.Generic;

namespace RelSort {
    public static class Functions {
        public static void print_data(List<Favorite> data) {
            System.Console.WriteLine("List value:");
            for(int i = 0; i < data.Count; i++) {
                System.Console.WriteLine(data[i].get_name());
            }
        }
        public static void RelSort(ref List<Favorite> data) {
            Dictionary<string,int> table = new Dictionary<string,int>();
            for(int i = 0; i < data.Count; i++) {
                table[data[i].get_name()] = i;
            }

            survey(ref data);
            System.Console.WriteLine("Sorting...");
            arrange(ref data,ref table);

        }

        internal static void survey(ref List<Favorite> data) {
            var rand = new Random();
            KeyValuePair<string,string> compair;
            List<KeyValuePair<string,string>> tried = new List<KeyValuePair<string,string>>();
            int maxRoll = data.Count;
            while(!confirm_relation_min(data)){
                int left = rand.Next(maxRoll);
                int right = rand.Next(maxRoll);
                if(right == left)
                    continue; //don't compare an object to itself
                string DEBUG_1 = data[left].get_name();
                string DEBUG_2 = data[right].get_name();
                compair = new KeyValuePair<string,string>(data[left].get_name(),data[right].get_name());
                if(test_pair(compair,tried))
                    continue;
                tried.Add(compair);
                var pleft = data[left];
                var pright = data[right];
                compare(ref pleft,ref pright);
                data[left] = pleft;
                data[right] = pright;
            }

        }

        internal static int find_lowest_array_val(Favorite input, Dictionary<string,int> table){
            int lowest = -1;
            for(int i = 0; i < input.get_highSize(); i++) {
                int expanded = table[input.read_higher(i)];
                if(expanded > lowest)
                    lowest = expanded;
            }
            return lowest;
        }

        internal static int find_greatest_array_val(Favorite input, Dictionary<string,int> table){
            int greatest = table.Count + 1; //
            for(int i = 0; i < input.get_lowSize(); i++) {
                int expanded = table[input.read_lower(i)];
                if(expanded < greatest)
                    greatest = expanded;
            }
            return greatest;
        }

        internal static void arrange(ref List<Favorite> data, ref Dictionary<string,int> table){
            int current_ref = data.Count - 1;

            while(current_ref >= 0){
                if(!check_sort_compliant(data,table,current_ref)) {
                    if(data[current_ref].get_lowSize() > 0)
                        insert_item(ref data,ref table,data[current_ref].get_name(),find_greatest_array_val(data[current_ref],table),1);
                    else
                        insert_item(ref data,ref table,data[current_ref].get_name(),find_lowest_array_val(data[current_ref],table),2);
                    current_ref = data.Count - 1;
                }
                else
                    --current_ref;
            }
        }

        internal static void insert_item(ref List<Favorite> data, ref Dictionary<string,int> table, string target, int loc, int mode){
            int targetLoc = table[target];
            int i;
            Favorite temp_copy = data[targetLoc];

            if(mode == 1){
                i = loc;
                data.RemoveAt(targetLoc);
                data.Insert(loc,temp_copy);
            }
            else{
                i = targetLoc;
                data.Insert(loc,temp_copy);
                data.RemoveAt(targetLoc);
            }

            for(; i < data.Count; i++)
                table[data[i].get_name()] = i;
        }

        internal static void compare(ref Favorite left, ref Favorite right){
            string choice;
            int pchoice;
            string lname = left.get_name();
            string rname = right.get_name();

            System.Console.WriteLine("Left or right, 1/0:");
            System.Console.WriteLine("{0} or {1}?",lname,rname);
            choice = System.Console.ReadLine();
            pchoice = System.Int32.Parse(choice);
            if(pchoice == 1) {
                left.inc_lower(rname);
                right.inc_higher(lname);
            }
            else{
                left.inc_higher(rname);
                right.inc_lower(lname);
            }
        }

        internal static bool test_pair(KeyValuePair<string,string> input, List<KeyValuePair<string,string>> db){
            KeyValuePair<string,string> inverse = new KeyValuePair<string,string>(input.Value,input.Key);
            for(int i = 0; i < db.Count; i++){
                //if(db[i] == input || db[i] == inverse) //why can't C# do this when C++ does it no problem?
                if((db[i].Value == input.Value && db[i].Key == input.Key) || (db[i].Value == inverse.Value && db[i].Key == inverse.Key))
                    return true;
            }
            /*
            if((db.Exists(x => x.Key == input.Key) && db.Exists(x => x.Value == input.Value)) || (db.Exists(x => x.Key == input.Value) && db.Exists(x => x.Value == input.Key))){
                return true; //lambda expressions scare me
            }
            */
            return false;
        }

        internal static bool confirm_relation_min(List<Favorite> data) {
            foreach(Favorite mem in data){
                if(mem.get_hierarchy_size() >= Constants.num_surveys)
                    continue;
                else
                    return false;
            }
            return true;
        }

        internal static bool check_sort_compliant(List<Favorite> data, Dictionary<string,int> table, int to_check){
            int refPos = table[data[to_check].get_name()];
            for(int i = 0; i < data[to_check].get_highSize(); i++) {
                int testRef = table[data[to_check].read_higher(i)];
                if(refPos > testRef)
                    continue;
                else
                    return false;
            }
            for(int i = 0; i < data[to_check].get_lowSize(); i++) {
                int testRef = table[data[to_check].read_lower(i)];
                if(refPos < testRef)
                    continue;
                else
                    return false;
            }
            return true;
        }
    }
}
