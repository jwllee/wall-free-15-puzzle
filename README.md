# Wall-free 15 puzzle problem
This problem is a modified version of the 15 puzzle problem where there are no side walls. This means that a number at the first position of a row can go to the last position of the same row in a single step if the last position is an empty space.

### How to run the program?
Using one of the puzzles in the directory ```Program/tests```, go the directory ```Program``` and run 
```  
  make
  ./solver [test.txt] [output.txt]
```
where ```test.txt``` is the file path to the selected puzzle and ```output.txt``` is the desired file path for the output text. 

The output will contain the number of minimum required steps and afterwards the *x*, *y* positions of the pieces to be moved at each step.

### Solution
The crux of the solution is to use the A* algorithm with a priority queue with a hashtable so that verifying the existence of states takes constant time on average.

### Notes
- There is a report in the directory ```Informe``` which contains some analysis of the solution.
