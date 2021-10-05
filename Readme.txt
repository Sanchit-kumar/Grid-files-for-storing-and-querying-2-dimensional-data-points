
Compile:-  g++ problem.cpp
Running:-  ./a.out

Cells are numbered first from origin and towards the y-axis firstly, and then move towards x-axis. The example is given for cell numbering:-
|    |    |
|_C2_|_C5_|	
|    |	  |
|_C1_|_C4_|
|    |    |
| C0 | C3 |

--Logical split will take place when it is not sharing a bucket with the other cell, otherwise only physical split will take place.
--Logical split will take place when some physical split is performed, whose boundary passes through other grid regions. 
--If a cell is sharing a bucket with another cell, then it will point to the same bucket which its parent cell (before splitting) was pointing to.
--Logical split will convert to physical split without any new boundary split, when a particular bucket is already shared by multiple cells.
--If the number of duplicates are more than bucket size, bucket overflow happens thus another bucket will be created to maintain the same.

---OUTPUT---

--All logs are also printed to console explaining how I got to the solution by making different logical and physical splitting, but for overall final result, please consider the bottom section of the result i.e. " #### FINAL OUTPUT ####" section.There, all bucket data along with cell range and cell mapping is printed.


