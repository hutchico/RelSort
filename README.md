# Relational Sorting
This is the fruit of a small project I envisioned aimed toward those who are clinically indecisive like myself.  
The idea here being that instead of asking someone to build a numerical list from a large (N > 10) range of options, you simply compare those options two at a time, asking "which of these is better?" rather than "which of all these is best?"  
Since the idea here is to process large lists of options, it would be cumbersome to do binary comparisons between every single option, so the algorithm instead opts to make comparisons until it has done so a few times for each option (configurable).  
At this point, it then rearranges the list until every entry is between other entries which are greater or lesser than itself.  
Current implementation is CLI only but that will change once I figure out a good GUI option.