# Relational Sorting
---
A conversion of the original program in C++ to C#, in preparation for future GUI integration.  
This is the fruit of a small project I envisioned aimed toward those who are clinically indecisive like myself.  
The idea here being that instead of asking someone to build a numerical list from a large (N > 10) range of options, you simply compare those options two at a time, asking "which of these is better?" rather than "which of all these is best?"
Since the idea here is to process large lists of options, it would be cumbersome to do binary comparisons between every single option, so the algorithm instead opts to make comparisons until it has done so a few times for each option (configurable) before extrapolating further comparisons from the data it already has.
Idea being that if A>B and B>C, then logically A>C.  
At this point, the algorithm rearranges the list until every entry is between other entries which are greater or lesser than itself.
Current implementation is CLI only but that is subject to future change.
