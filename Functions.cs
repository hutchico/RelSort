using System;
using System.Collections.Generic;

namespace RelSort {
    public static class Functions {
        
        public static void RelSort(ref List<Favorite> data){
            Dictionary<string,int> table = new Dictionary<string,int>();
            for(int i = 0; i < data.Count; i++) {
                table[data[i].get_name()] = i;
            }
            survey(ref data, ref table);
            System.Console.WriteLine("Sorting...");
            arrange(ref data,ref table); //initial list sort
            interpolate_relations(ref data, table); //update all list members based on first sort
            arrange(ref data,ref table); //sort again, this time with a complete relation map (theoretically)
        }
        
        internal static void survey(ref List<Favorite> data, ref Dictionary<string,int> table){
            var rand = new Random();
            KeyValuePair<string,string> compair;
            List<KeyValuePair<string,string>> tried = new List<KeyValuePair<string,string>>();
            int maxComparisons = (int)Math.Ceiling((double)data.Count / 2); //minimum relations per item for a complete list
            int currentMax = 1;
            int numComparisons = 0;
            int left, right;
            while(currentMax <= maxComparisons) {
                //outer
                int debug = -1;
                for(int i = 0; i < data.Count; i+=2) {
                    Favorite pleft = data[i]; //C# doesn't let you pass this as an argument apparently
                    Favorite pright;
                    left = i;
                    right = i + 1;
                    if(data.Count % 2 == 1 && left == data.Count - 1) {//catch odd number lists
                        pright = data[rand.Next(data.Count)];
                        while(pright == pleft)
                            pright = data[rand.Next(data.Count)];

                    }
                    else
                        pright = data[right];
                    while(true) {
                        compair = new KeyValuePair<string,string>(pleft.get_name(),pright.get_name());
                        if(test_pair(compair,tried)) {
                            /*
                            two edge cases to handle:
                            pair has been compared before, try and shift right comparator down one array value
                            pair has been compared, left has been compared to every other value already, 
                                    so shifting right value will never result in a meaningful comparison
                            */
                            while(true) {
                                if(pleft.get_hierarchy_size() == data.Count - 1) {
                                    if(left == data.Count - 1)
                                        left = -1;
                                    pleft = data[++left];
                                }
                                else {
                                    if(right >= data.Count - 1)
                                        right = -1;
                                    ++right;
                                    pright = data[right];
                                }
                                if(pright == pleft) //continue this loop until we have two unique comparisons to make
                                    continue; 
                                break; //should only run once unless the two wind up equal somehow
                            }
                        }
                        else {
                            tried.Add(compair);
                            break;
                        }
                    }
                    compare(ref pleft,ref pright);
                    data[table[pleft.get_name()]] = pleft; 
                    data[table[pright.get_name()]] = pright;
                    ++numComparisons;
                }
                interpolate_relations(ref data,table);
                arrange(ref data,ref table);
                ++currentMax;
            }
            //NOTE: this probably isn't right since the survey function was rewritten to be systematic rather than random
            System.Console.WriteLine("Performed {0} tests out of possible {1}.",numComparisons,(data.Count * (data.Count - 1)) / 2);
        }

        internal static bool check_relation_min(List<Favorite> data, int minNum){ 
            foreach(Favorite mem in data) {
                if(mem.get_hierarchy_size() >= minNum)
                    continue;
                else
                    return false;
            }
            return true;
        }

        internal static bool test_pair(KeyValuePair<string,string> input,List<KeyValuePair<string,string>> db){
            KeyValuePair<string,string> inverse = new KeyValuePair<string,string>(input.Value,input.Key);
            if(input.Value == input.Key) { //DEBUG: attempt to catch a fatal error
                System.Console.WriteLine("Warning: Program attempted to compare {0} to itself.",input.Key);
                
                return true;
            }
            for(int i = 0; i < db.Count; i++) {
                //if(db[i] == input || db[i] == inverse) //why doesn't C# Pairs have a built in == comparison like C++ does?
                if((db[i].Value == input.Value && db[i].Key == input.Key) || (db[i].Value == inverse.Value && db[i].Key == inverse.Key))
                    return true;
            }
            return false;
        }

        internal static void compare(ref Favorite left,ref Favorite right){
            int pchoice;
            string lname = left.get_name();
            string rname = right.get_name();

            System.Console.WriteLine("Left or right, 1/0:");
            System.Console.WriteLine("{0} or {1}?",lname,rname);

            //DEBUG: remove user input from testing
            if(lname.Length > 7)
                lname = lname.Substring(6,2);
            else
                lname = lname.Substring(6,1);
            if(rname.Length > 7)
                rname = rname.Substring(6,2);
            else
                rname = rname.Substring(6,1);

            if(System.Int32.Parse(lname) < System.Int32.Parse(rname))
                pchoice = 1;
            else 
                pchoice = 0;

            System.Console.WriteLine(pchoice);


            lname = left.get_name(); //DEBUG: reset name values from earlier
            rname = right.get_name();

            /*
            pchoice = System.Int32.Parse(System.Console.ReadLine());
            */
            if(pchoice == 1) {
                left.add_relation(rname,0);
                right.add_relation(lname,1);
            }
            else {
                left.add_relation(rname,1);
                right.add_relation(lname,0);
            }
        }
        //Update relations to fill in missing relations after Size/2 comparisons have been made
        internal static void interpolate_relations(ref List<Favorite> data, Dictionary<string,int> table) {
            /*
            observation: if A>B and B>C, logically A>C
            start at the top, for each entry: go down list,
                if that entry is greater/less than another entry, 
                   add that entry to the main entry's list with the same polarity
            */
            for(int i = 0; i < data.Count; i++) {
                for(int j = 0; j < data[i].get_hierarchy_size(); j++) {
                    if(data[i].get_hierarchy_size() == data.Count - 1)
                        break; //if a list member already has relations to everything but itself, move on
                    KeyValuePair<string,int> target = data[i].get_relation(j);
                    int listPos = table[target.Key];
                    copy_relations(ref data,i,listPos,target.Value);
                }
            }
        }

        #region Arrange
        internal static void arrange(ref List<Favorite> data,ref Dictionary<string,int> table) {
            int current_ref = data.Count - 1;
            //insert the bottom of the list somewhere else in the list, restart at bottom. repeat until every list member is positioned properly
            while(current_ref >= 0) {
                if(!check_sort_compliant(data,table,current_ref)) {
                    if(data[current_ref].get_lower() > 0)
                        insert_item(ref data,ref table,data[current_ref].get_name(),find_greatest_array_val(data[current_ref],table),1);
                    else //if you can't insert it above something, try and insert it below something instead. implies current_ref != last array position
                        insert_item(ref data,ref table,data[current_ref].get_name(),find_lowest_array_val(data[current_ref],table),2);
                    current_ref = data.Count - 1; //restart from the bottom either way
                }
                else
                    --current_ref;
            }
        }
        //verify a list member is positioned between its respectively greater and lesser values per user survey
        internal static bool check_sort_compliant(List<Favorite> data,Dictionary<string,int> table,int to_check) {
            int refPos = table[data[to_check].get_name()];
            for(int i = 0; i < data[to_check].get_hierarchy_size(); i++) {
                int dir = data[to_check].get_relType(i);
                int testRef = table[data[to_check].get_relName(i)];
                if(dir == 1) {
                    if(refPos > testRef)
                        continue;
                    else
                        return false;
                }
                else {
                    if(refPos < testRef)
                        continue;
                    else
                        return false;
                }
            }
            return true;
        }

        internal static void insert_item(ref List<Favorite> data,ref Dictionary<string,int> table,string target,int loc,int mode) {
            int targetLoc = table[target];
            int i;
            Favorite temp_copy = data[targetLoc];

            if(mode == 1) {
                i = loc;
                data.RemoveAt(targetLoc);
                data.Insert(loc,temp_copy);
            }
            else {
                i = targetLoc;
                data.Insert(loc,temp_copy);
                data.RemoveAt(targetLoc);
            }

            for(; i < data.Count; i++)
                table[data[i].get_name()] = i;
        }

        //find the earliest array value this Favorite can be placed above in the array
        internal static int find_greatest_array_val(Favorite input,Dictionary<string,int> table) {
            int greatest = table.Count + 1; //
            for(int i = 0; i < input.get_hierarchy_size(); i++) {
                if(input.get_relType(i) == 1)
                    continue;   //only care about relations that are below this Favorite
                int rel_pos = table[input.get_relName(i)];
                if(rel_pos < greatest)
                    greatest = rel_pos;
            }
            return greatest;
        }
        //find the last array value this Favorite can be placed below in the array
        internal static int find_lowest_array_val(Favorite input,Dictionary<string,int> table) {
            int lowest = -1;
            for(int i = 0; i < input.get_hierarchy_size(); i++) {
                if(input.get_relType(i) == 0)
                    continue;   //see above
                int rel_pos = table[input.get_relName(i)];
                if(rel_pos > lowest)
                    lowest = rel_pos;
            }
            return lowest;
        }
        #endregion

        //copy a subset of the relations in donor to the relations in recipient
        internal static void copy_relations(ref List<Favorite> data, int recipient, int donor, int mode) {
            for(int i = 0; i < data[donor].get_hierarchy_size(); i++) {
                if(data[recipient].test_contents(data[donor].get_relation(i)))
                    continue; //if recipient already has this relation, don't copy it again
                else {
                    if(data[donor].get_relType(i) == mode) //if it's the same polarity of relation, copy it
                        data[recipient].add_relation(data[donor].get_relName(i),data[donor].get_relType(i));
                }
            }
        }

        public static void print_data(List<Favorite> data) {
            System.Console.WriteLine("List value:");
            for(int i = 0; i < data.Count; i++) {
                System.Console.WriteLine(data[i].get_name());
            }
        }
        //DEBUG: get all relations at end of run
        public static void spit_rels(List<Favorite> data) {
            foreach(Favorite mem in data)
                mem.print_relations();
        }
    }
}
