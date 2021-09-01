# Advanced version of Dining Philosophers Problem
Simulation of advanced version of Dining Philosophers problem that covers C, Unix/Linux Multithreading and  Process Synchronization
# Task
The famous Dining Philosophers problem consists of the eating-thinking cycle. Knowing this, the entrepreneur opened a place called DinoPhilo. There are 10 round tables. Each table is for 8 people. Customer acceptance is in the form of a philosopher group of 8, and each group is taken to a table. (All members has to sit same table) When the place is full, the arriving groups are put on hold outside. The group that eats their food gets up and table is cleared, and the table is immediately set for the next group. At each table opening, there is 2 kilos of rice in the large rice pot in the middle. Each philosopher eats 100 grams of rice in each eating and thinking cycle. When the rice in the pot is finished, if there is a philosopher who cannot eat even a bite in the group of 8 people, the group orders the waiter to freshen the pot (2 kilos) without leaving the table, and the eating-thinking cycle starts again. Even the rice is finished,if there is a memeber who can not eat, group order 2 kilos of rice and cycle continues . If everyone at the table has eaten, the group leaves without paying the bill (checks on entrepreneur).
## DP_f.c
It is a C program that solves the above problem, with a separate thread for each philosopher. When the program starts, it takes the number of philosopher group as a parameter from the command line. For example 100 groups. Philosophers may have different priority level therefore setting 5 different priority levels and randomly choosing one of them for each thread. philosophers eating time and thinking time may be different therefore , the eating and thinking times of philosopher are also chosen randomly that are between 1 and 5 milliseconds at each step. Output is the bill of each table. The bill contains table opening + table refresher + rice cost. Opening a table from scratch is 99.90 TL, table refresh is 19.90 TL and 2 kg rice is 25.8 TL. When the program ends, tables detailed payment statement is given and total bill is calculated. 
# Compile and Run
## How to Compile
1.	Open a shell on the machine you wish to compile on.
2.	Change directory (cd) to the directory that contains DP_f.c
3.	Enter the following command. 

```bash
gcc DP_f.c -o <output_file> -lpthread
```

## How to run
1.	Open a shell on the machine you wish to run on.
2.	Change directory (cd) to the directory that contains DP_f.c" 
3.	Enter the following command.

```bash
./<output_file>
```

## Input
Enter the group no.

