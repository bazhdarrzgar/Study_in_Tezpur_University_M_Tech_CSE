# Learning C Through the Rainfall Project

Absolutely. We can learn **C through this exact rainfall project**, instead of learning random C syntax first.

Your assignment is designed around three main Data Structures/Algorithms ideas: **Divide & Conquer, Sorting, and Binary Search**. The assignment specifically asks you to explain the code to your instructor, and explanation is worth **6/10 marks**, so understanding what every part does is important.

---

# 1. First understand what the whole program does

Imagine your CSV file contains something like:

```text
YEAR,JAN,FEB,MAR,...,DEC,ANNUAL
1901,20.5,30.2,45.1,...,100.2,900.5
1902,15.2,25.4,40.0,...,120.3,850.7
...
```

Your C program loads this data and lets you do four things:

```text
                Rainfall Program
                       |
        +--------------+--------------+
        |              |              |
     Display       Find min/max      Sort
        |              |              |
     records       Linear/D&C     Merge/Quick
                                      |
                                      v
                                  Binary Search
```

The menu:

```text
1. Display all records
2. Find max/min rainfall for a month (linear & D&C)
3. Sort by annual rainfall (Merge & Quick)
4. Search annual rainfall range (Binary Search)
5. Exit
```

So this is not really one giant problem. It's several smaller C concepts combined together.

---

# 2. The first thing: `#include`

At the beginning:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
```

Think of `#include` as:

> "C, please give my program access to these already-made tools."

### `stdio.h`

```c
#include <stdio.h>
```

Means **standard input/output**.

It gives you things like:

```c
printf()
scanf()
fopen()
fclose()
fgets()
fprintf()
```

For example:

```c
printf("Hello");
```

prints something on the screen.

And:

```c
scanf("%d", &choice);
```

gets input from the keyboard.

---

### `stdlib.h`

```c
#include <stdlib.h>
```

Provides things such as:

```c
malloc()
free()
atoi()
atof()
rand()
srand()
```

You'll see all of these later.

For example:

```c
int year = atoi(token);
```

converts text into an integer.

---

### `string.h`

```c
#include <string.h>
```

Used for string-related operations.

This program uses:

```c
strtok()
memcpy()
```

We'll explain both.

---

### `time.h`

```c
#include <time.h>
```

Used here to help generate different random numbers:

```c
srand((unsigned)time(NULL));
```

This is important for **Randomized Quick Sort**.

---

# 3. `#define`

Next:

```c
#define MAX_RECORDS 200
#define MONTHS 12
```

This is a constant.

Instead of writing:

```c
12
```

everywhere, the programmer writes:

```c
MONTHS
```

So:

```c
for (int i = 0; i < MONTHS; i++)
```

really means:

```c
for (int i = 0; i < 12; i++)
```

And:

```c
RainfallRecord records[MAX_RECORDS];
```

means:

```c
RainfallRecord records[200];
```

You can think:

```text
#define MONTHS 12

MONTHS
  ↓
12
```

---

# 4. The most important part: `struct`

Now we reach a very important C concept.

```c
typedef struct {
    int year;
    double monthly[MONTHS];
    double annual;
} RainfallRecord;
```

If you don't understand this, the rest of the program will be difficult.

So let's slow down.

## What is a structure?

Suppose I want to represent one rainfall record.

One record needs:

```text
Year
January rainfall
February rainfall
March rainfall
...
December rainfall
Annual rainfall
```

Instead of having unrelated variables:

```c
int year;
double jan;
double feb;
double mar;
...
double annual;
```

we can put them together into one object.

That's what a `struct` does.

---

## This:

```c
struct {
    int year;
    double monthly[MONTHS];
    double annual;
}
```

creates a structure containing:

```text
RainfallRecord
│
├── year
├── monthly[0]
├── monthly[1]
├── monthly[2]
├── ...
├── monthly[11]
└── annual
```

And:

```c
typedef ... RainfallRecord;
```

gives this structure the name:

```c
RainfallRecord
```

So now we can write:

```c
RainfallRecord r;
```

and `r` contains an entire rainfall record.

---

# 5. `int` vs `double`

You will see these everywhere.

### `int`

Used for whole numbers:

```c
int year = 1901;
```

Examples:

```text
1
10
1901
2015
```

### `double`

Used for decimal numbers:

```c
double rainfall = 125.6;
```

Examples:

```text
125.6
89.2
1000.45
```

Rainfall can have decimals, so the program uses `double`.

---

# 6. Arrays

Look at:

```c
double monthly[MONTHS];
```

Since:

```c
MONTHS = 12
```

this becomes:

```c
double monthly[12];
```

An array is simply a collection of values.

Think of:

```text
monthly
```

as 12 boxes:

```text
Index:     0      1      2      3      ...     11
         +------+------+------+------+---------+
monthly: | JAN  | FEB  | MAR  | APR  | ... | DEC |
         +------+------+------+------+---------+
```

Important:

**C starts counting arrays from 0.**

So:

```c
monthly[0]
```

= January

```c
monthly[1]
```

= February

...

```c
monthly[11]
```

= December.

---

# 7. Global variables

Next:

```c
RainfallRecord records[MAX_RECORDS];
int recordCount = 0;
```

These are global variables.

The program creates an array that can hold 200 rainfall records:

```c
records[200]
```

Imagine:

```text
records
│
├── records[0] → 1901
├── records[1] → 1902
├── records[2] → 1903
├── ...
└── records[199]
```

`recordCount` tells us how many records are actually loaded.

Initially:

```c
int recordCount = 0;
```

because we haven't read the CSV yet.

After reading the file, perhaps:

```text
recordCount = 115
```

Then only:

```c
records[0] ... records[114]
```

contain useful data.

---

# 8. Global counters

Then:

```c
long long mergeComparisons = 0;
long long mergeSwaps = 0;
long long quickComparisons = 0;
long long quickSwaps = 0;
```

These count how much work the sorting algorithms perform.

For example:

```c
mergeComparisons++;
```

means:

> "We just performed one comparison in Merge Sort."

The `++` means:

```c
mergeComparisons = mergeComparisons + 1;
```

---

## Why `long long`?

Because the number of comparisons can become large.

```c
int
```

can store reasonably large whole numbers, but:

```c
long long
```

can store much larger whole numbers.

---

# 9. Another structure: `MinMax`

```c
typedef struct {
    double min;
    double max;
    int minYear;
    int maxYear;
} MinMax;
```

This structure stores four things:

```text
MinMax
│
├── min       → smallest rainfall
├── max       → largest rainfall
├── minYear   → year with smallest rainfall
└── maxYear   → year with largest rainfall
```

For example:

```text
min = 25.4
minYear = 1905

max = 320.7
maxYear = 1982
```

Now instead of returning four separate values, a function can return one `MinMax`.

---

# 10. Functions

C programs are divided into **functions**.

For example:

```c
void displayAll()
```

means:

> "Here is a function called `displayAll`."

You can think of a function as a small machine.

```text
Input
  ↓
[ FUNCTION ]
  ↓
Output
```

For example:

```c
displayAll();
```

means:

> "Run the displayAll machine."

---

# 11. Understanding a function declaration

Look at:

```c
int readCSV(const char *filename)
```

This looks complicated, but break it apart.

```text
int
 ↓
return type

readCSV
   ↓
function name

(const char *filename)
        ↓
      input
```

So:

```c
int readCSV(...)
```

means:

> This function is called `readCSV`, takes some input, and returns an integer.

---

# 12. What does `void` mean?

For example:

```c
void displayAll()
```

`void` means:

> This function doesn't return a value.

Compare:

```c
int readCSV(...)
```

returns an integer.

But:

```c
void displayAll(...)
```

doesn't return anything.

---

# 13. Understanding pointers — VERY IMPORTANT

You'll see things like:

```c
int *minYear
```

and:

```c
double *minVal
```

Don't panic. 😄

A pointer basically stores the **address of another variable**.

Suppose:

```c
int x = 10;
```

You can imagine memory like:

```text
Variable       Memory
------         ------
x              10
```

If we do:

```c
int *p = &x;
```

then:

```text
p
 ↓
[address of x]
       ↓
      x
      10
```

`&x` means:

> "Give me the memory address of x."

And:

```c
*p
```

means:

> "Go to that address and access the value there."

---

# 14. Why does `scanf` use `&`?

You have:

```c
scanf("%d", &choice);
```

Why not:

```c
scanf("%d", choice);
```

Because `scanf` needs the **address** where it should store the user's answer.

For example:

```c
int choice;
scanf("%d", &choice);
```

means:

```text
User enters 3
      ↓
scanf
      ↓
go to address of choice
      ↓
store 3
```

---

# 15. Reading the CSV

Now let's study:

```c
int readCSV(const char *filename) {
```

This function opens the rainfall CSV.

---

## Opening the file

```c
FILE *fp = fopen(filename, "r");
```

There are several concepts here.

### `FILE *fp`

`FILE` represents a file.

`fp` is a pointer to that file.

### `fopen`

```c
fopen(filename, "r")
```

means:

> Open the file for reading.

`"r"` = read.

So:

```c
FILE *fp = fopen(filename, "r");
```

means:

> Open this file and give me a handle/pointer so I can work with it.

---

# 16. Checking whether opening failed

```c
if (!fp) {
```

The `!` means **NOT**.

So:

```c
!fp
```

means:

> "fp is not valid."

If the file couldn't be opened:

```c
printf("Error: cannot open file %s\n", filename);
return -1;
```

`return -1` tells the caller:

> Something went wrong.

---

# 17. Character arrays / strings

```c
char line[1024];
```

A `char` stores one character.

For example:

```c
char c = 'A';
```

A string in C is an array of characters.

For example:

```c
char name[] = "Hello";
```

Internally:

```text
H e l l o \0
```

The `\0` marks the end of the string.

Here:

```c
char line[1024];
```

means:

> Create space for a line containing up to about 1023 characters plus the terminating `\0`.

---

# 18. Reading a line

```c
fgets(line, sizeof(line), fp)
```

means:

> Read one line from the file.

`sizeof(line)` tells it how much space is available.

---

# 19. Skipping the header

The CSV probably begins:

```text
YEAR,JAN,FEB,MAR,...
```

The program doesn't want to treat that as rainfall data.

So:

```c
if (!fgets(line, sizeof(line), fp)) {
    fclose(fp);
    return -1;
}
```

reads the first line and effectively skips it.

---

# 20. The `while` loop

```c
while (fgets(line, sizeof(line), fp)) {
```

means:

> Keep reading lines while there are lines available.

So conceptually:

```text
Read line
   ↓
Process line
   ↓
Read next line
   ↓
Process line
   ↓
...
End of file
   ↓
Stop
```

---

# 21. `strtok`

This is one of the more confusing C functions.

Suppose the line is:

```text
1901,20.5,30.2,40.8
```

We need to separate it:

```text
1901
20.5
30.2
40.8
```

The program uses:

```c
char *token = strtok(line, ",");
```

`","` is the separator.

So:

```text
1901,20.5,30.2,40.8
 ↑
 first token
```

gives:

```text
"1901"
```

Then:

```c
token = strtok(NULL, ",");
```

gets the next token:

```text
"20.5"
```

Then:

```text
"30.2"
```

and so on.

---

# 22. `atoi`

```c
int year = atoi(token);
```

The token is text:

```text
"1901"
```

But we need a number:

```text
1901
```

`atoi` means:

> ASCII to integer.

So:

```c
atoi("1901")
```

produces:

```text
1901
```

---

# 23. `atof`

Similarly:

```c
months[i] = atof(token);
```

`atof` means:

> ASCII to floating-point number.

For example:

```c
atof("123.45")
```

becomes:

```text
123.45
```

---

# 24. The `for` loop

You have:

```c
for (int i = 0; i < MONTHS; i++) {
```

This has three parts:

```c
for (start; condition; update)
```

Here:

```c
int i = 0
```

Start at zero.

```c
i < MONTHS
```

Continue while `i < 12`.

```c
i++
```

Increase `i` by 1.

So:

```text
i = 0
i = 1
i = 2
...
i = 11
```

Exactly 12 times.

---

# 25. Storing the record

Eventually:

```c
records[count].year = year;
```

This is very important.

Suppose:

```text
count = 0
year = 1901
```

Then:

```c
records[0].year = 1901;
```

The `.` means:

> Access something inside the structure.

So:

```c
records[0].year
```

means:

> The year belonging to record 0.

Similarly:

```c
records[0].annual
```

means:

> The annual rainfall of record 0.

And:

```c
records[0].monthly[5]
```

means:

> June rainfall of record 0.

---

# 26. Increasing `count`

```c
count++;
```

means:

```c
count = count + 1;
```

So:

```text
count = 0
read first record
count = 1

read second record
count = 2

read third record
count = 3
```

At the end:

```c
recordCount = count;
```

Now the rest of the program knows how many records were loaded.

---

# 27. Closing the file

```c
fclose(fp);
```

Always close a file after you're finished with it.

Think:

```text
fopen()
   ↓
use file
   ↓
fclose()
```

---

# 28. `displayAll()`

Now:

```c
void displayAll()
```

prints every record.

The first `printf` is mainly formatting the table.

You don't need to memorize all the `%` formatting immediately.

Important ones here:

```text
%d      integer
%f      floating point
%.1f    floating point with 1 decimal place
%s      string
%lld    long long
```

For example:

```c
printf("%d", 1901);
```

prints:

```text
1901
```

And:

```c
printf("%.1f", 123.456);
```

prints:

```text
123.5
```

---

# 29. Accessing every record

```c
for (int i = 0; i < recordCount; i++)
```

means:

```text
record 0
record 1
record 2
...
last record
```

Then:

```c
printf("%-6d", records[i].year);
```

prints the year.

And:

```c
records[i].monthly[m]
```

gets a particular month's rainfall.

---

# 30. Part A — finding minimum and maximum

The assignment asks you to find the years with maximum and minimum rainfall for a particular month, both normally and using divide & conquer.

There are two algorithms.

---

# 31. Linear search

The function:

```c
findMinMaxLinear(...)
```

simply looks at every record.

Imagine June rainfall:

```text
Year     June
1901     100
1902     50
1903     200
1904     75
1905     150
```

Start:

```text
minimum = 100
maximum = 100
```

Then check 50:

```text
50 < 100
```

so:

```text
minimum = 50
```

Then check 200:

```text
200 > 100
```

so:

```text
maximum = 200
```

And continue.

---

# 32. Why pointers appear here

Look:

```c
void findMinMaxLinear(int month, int *minYear, int *maxYear,
                      double *minVal, double *maxVal)
```

The function needs to produce four results:

```text
minimum year
maximum year
minimum rainfall
maximum rainfall
```

But a normal C function can return only one value directly.

So pointers allow the function to modify variables belonging to the caller.

When calling:

```c
findMinMaxLinear(monthIdx, &minYear, &maxYear,
                 &minVal, &maxVal);
```

we pass their addresses.

Then inside:

```c
*minVal = ...
```

means:

> Change the actual `minVal` variable in the caller.

This is one of the most important pointer concepts in C.

---

# 33. Divide & Conquer

Now the interesting algorithm:

```c
MinMax findMinMaxDC(int left, int right, int month)
```

Divide & Conquer means:

> Break a large problem into smaller problems, solve the smaller problems, then combine the answers.

For example, suppose we have 8 records:

```text
[0 1 2 3 4 5 6 7]
```

Divide:

```text
[0 1 2 3] [4 5 6 7]
```

Then:

```text
[0 1] [2 3] [4 5] [6 7]
```

Eventually:

```text
[0] [1] [2] [3] [4] [5] [6] [7]
```

Then combine the answers.

---

# 34. Recursion

This function calls itself:

```c
MinMax leftRes = findMinMaxDC(left, mid, month);
MinMax rightRes = findMinMaxDC(mid + 1, right, month);
```

That's called **recursion**.

A recursive function is a function that calls itself.

The critical part is:

```c
if (left == right)
```

This is the **base case**.

If there's only one record:

```text
min = that record
max = that record
```

No need to divide anymore.

Then the function returns.

---

# 35. Visualizing the recursion

Suppose:

```text
findMinMaxDC(0, 7)
```

becomes:

```text
             [0..7]
             /    \
         [0..3]   [4..7]
         /  \      /  \
      [0..1][2..3][4..5][6..7]
```

Then eventually each part reaches one element.

After that, results travel back upward.

That's the heart of **Divide & Conquer**.

---

# 36. The ternary operator `?:`

You see:

```c
res.min = (leftRes.min < rightRes.min)
          ? leftRes.min
          : rightRes.min;
```

This is a shorter version of `if/else`.

This:

```c
condition ? value1 : value2
```

means:

```text
if condition is true
    use value1
else
    use value2
```

So:

```c
(leftRes.min < rightRes.min)
    ? leftRes.min
    : rightRes.min
```

means:

> If the left minimum is smaller, use it; otherwise use the right minimum.

---

# 37. Part B — Sorting

The assignment asks for two sorting algorithms:

```text
1. Merge Sort
2. Randomized Quick Sort
```

and wants comparisons/swaps counted and sorted data written into separate files.

---

# 38. Merge Sort idea

Suppose annual rainfall is:

```text
[80, 30, 100, 20]
```

Merge Sort divides it:

```text
[80, 30] [100, 20]
```

Then:

```text
[80] [30] [100] [20]
```

Then merges:

```text
[30, 80] [20, 100]
```

Finally:

```text
[20, 30, 80, 100]
```

So Merge Sort is:

```text
DIVIDE
   ↓
DIVIDE
   ↓
DIVIDE
   ↓
SORT SMALL PARTS
   ↓
MERGE
   ↓
MERGE
   ↓
SORTED
```

---

# 39. The `merge()` function

```c
void merge(RainfallRecord arr[], int left, int mid, int right,
           RainfallRecord temp[])
```

This combines two already-sorted sections.

For example:

```text
left part:   [20, 50, 90]
right part:  [30, 40, 100]
```

It compares:

```text
20 vs 30
```

takes 20.

Then:

```text
50 vs 30
```

takes 30.

Then:

```text
50 vs 40
```

takes 40.

And so on.

---

# 40. Three indexes: `i`, `j`, `k`

```c
int i = left, j = mid + 1, k = left;
```

Think:

```text
i → current item in left half
j → current item in right half
k → where to put the next item
```

So:

```text
LEFT HALF             RIGHT HALF
   ↑                      ↑
   i                      j

                 TEMP
                   ↑
                   k
```

---

# 41. Comparison counter

```c
mergeComparisons++;
```

Every time the program compares:

```c
arr[i].annual <= arr[j].annual
```

it increases the counter.

This is needed because the assignment specifically asks for a table showing comparisons and swaps.

---

# 42. Temporary array

Merge Sort uses:

```c
RainfallRecord temp[]
```

because it needs temporary space while merging.

For example:

```text
arr:
[80, 30, 100, 20]

temp:
[30, 80, 20, 100]
```

Then the temporary values are copied back.

---

# 43. Merge Sort recursion

```c
void mergeSortRec(...)
```

contains:

```c
if (left < right)
```

This means:

> If there are at least two elements, continue dividing.

Then:

```c
int mid = (left + right) / 2;
```

finds the middle.

Then:

```c
mergeSortRec(arr, left, mid, temp);
```

sorts the left half.

And:

```c
mergeSortRec(arr, mid + 1, right, temp);
```

sorts the right half.

Finally:

```c
merge(arr, left, mid, right, temp);
```

combines them.

---

# 44. Quick Sort

Quick Sort works differently.

It chooses a:

> **pivot**

For example:

```text
[80, 30, 100, 20, 50]
```

Suppose pivot = 50.

Then it tries to arrange:

```text
values <= 50     pivot     values > 50
     ↓             ↓             ↓
[30,20]          [50]          [80,100]
```

Then Quick Sort recursively sorts the left and right sections.

---

# 45. Randomized pivot

The code:

```c
int randIdx = low + rand() % (high - low + 1);
```

chooses a random position.

Then:

```c
swapRecords(&arr[randIdx], &arr[high]);
```

moves that random pivot to the end.

This is why it's called:

> Randomized Quick Sort.

---

# 46. `swapRecords`

```c
void swapRecords(RainfallRecord *a, RainfallRecord *b)
```

This swaps two entire rainfall records.

Suppose:

```text
A = 1901
B = 1950
```

After:

```c
swapRecords(&A, &B);
```

we have:

```text
A = 1950
B = 1901
```

---

# 47. Why `*a` and `*b`?

Because:

```c
a
```

and:

```c
b
```

are addresses.

So:

```c
*a
```

means:

> The actual record stored at address `a`.

The swap:

```c
RainfallRecord temp = *a;
*a = *b;
*b = temp;
```

means:

```text
temporary = A
A = B
B = temporary
```

This is a classic C pointer example.

---

# 48. Partition

This function:

```c
int partition(...)
```

is the heart of Quick Sort.

It:

1. chooses a pivot
2. moves smaller/equal values to the left
3. leaves larger values on the right
4. puts the pivot into its final position

Then:

```c
return i + 1;
```

returns the pivot's final position.

---

# 49. `srand()` and `rand()`

Before Quick Sort:

```c
srand((unsigned)time(NULL));
```

This initializes the random-number generator.

Then:

```c
rand()
```

produces random numbers.

The reason for:

```c
time(NULL)
```

is that the current time changes, so the random sequence isn't identical every time you run the program.

---

# 50. Dynamic memory: `malloc`

Now one of the biggest C concepts:

```c
RainfallRecord *mergeArr =
    (RainfallRecord*)malloc(recordCount * sizeof(RainfallRecord));
```

This creates memory **during program execution**.

Why?

The program knows `recordCount` only after reading the CSV.

If there are 115 records:

```c
malloc(115 * sizeof(RainfallRecord))
```

creates enough memory for 115 records.

---

# 51. `sizeof`

```c
sizeof(RainfallRecord)
```

asks C:

> How many bytes are required for one `RainfallRecord`?

Then:

```c
recordCount * sizeof(RainfallRecord)
```

means:

> Give me enough bytes for `recordCount` records.

---

# 52. `free`

Whenever you use:

```c
malloc()
```

you should eventually use:

```c
free()
```

The program does:

```c
free(mergeArr);
free(quickArr);
free(temp);
```

This releases the memory.

Think:

```text
malloc()
   ↓
borrow memory
   ↓
use memory
   ↓
free()
   ↓
return memory
```

---

# 53. `memcpy`

The program does:

```c
memcpy(mergeArr, records,
       recordCount * sizeof(RainfallRecord));
```

This copies the records.

Why?

Because we don't want Merge Sort to destroy the original data.

So:

```text
records
   |
   +----> mergeArr
   |
   +----> quickArr
```

Then Merge Sort works on:

```text
mergeArr
```

and Quick Sort works on:

```text
quickArr
```

---

# 54. Writing CSV output

```c
writeSortedToFile(...)
```

opens:

```c
fopen(filename, "w");
```

Here `"w"` means:

> Write.

Then:

```c
fprintf(fp, ...)
```

writes formatted text into the file instead of the screen.

Compare:

```c
printf()
```

→ screen

```c
fprintf(fp, ...)
```

→ file

The assignment requires separate sorted output files.

---

# 55. Part C — Binary Search

Now we get to Binary Search.

The assignment asks the program to search for records within a given **range of annual rainfall**.

---

# 56. `lower_bound`

```c
int lower_bound(...)
```

finds the first position where:

```text
annual >= low
```

For:

```text
10 20 30 40 50 60
```

and:

```text
low = 35
```

it returns the position of:

```text
40
```

---

# 57. `upper_bound`

```c
int upper_bound(...)
```

finds the first position where:

```text
annual > high
```

Suppose:

```text
10 20 30 40 50 60 70
```

and:

```text
high = 60
```

It points just after `60`.

So together:

```text
lower_bound → first acceptable record
upper_bound → position after last acceptable record
```

Then:

```c
for (int i = l; i < r; i++)
```

prints everything between them.

---

# 58. The global sorted array

```c
RainfallRecord sortedRecords[MAX_RECORDS];
int sortedCount = 0;
```

This exists specifically so Binary Search has sorted data available.

Binary Search **requires sorted data**.

That's a fundamental rule:

> **Binary Search works on sorted data.**

---

# 59. `main()` — the boss of the program

Finally:

```c
int main()
```

This is where the program starts.

Think of it as:

```text
START PROGRAM
      ↓
     main()
      ↓
everything begins
```

---

# 60. Loading the CSV

First:

```c
if (readCSV("A2.1_rainfall.csv") <= 0)
```

The program calls:

```c
readCSV()
```

If something goes wrong or no records are loaded:

```c
printf("No data loaded. Check file.\n");
return 1;
```

`return 1` means the program ended because of an error.

---

# 61. `do while`

Then:

```c
do {
    ...
} while (choice != 0);
```

This means:

> Show the menu at least once, then keep showing it until the user chooses 0.

For example:

```text
MENU
 ↓
user chooses 1
 ↓
display
 ↓
MENU
 ↓
user chooses 3
 ↓
sort
 ↓
MENU
 ↓
user chooses 0
 ↓
EXIT
```

---

# 62. `switch`

This:

```c
switch (choice)
```

checks what the user entered.

For example:

```c
case 1:
    displayAll();
    break;
```

means:

> If the user entered 1, call `displayAll()`.

Then:

```c
case 2:
    doPartA();
    break;
```

means:

> If the user entered 2, perform Part A.

And so on.

---

# 63. Why `break`?

Without:

```c
break;
```

C could continue executing the next cases.

So:

```c
case 1:
    displayAll();
    break;
```

means:

```text
case 1
  ↓
display
  ↓
STOP this switch
```

---

# 64. The complete program in your head

You should now think about the program like this:

```text
                    main()
                      |
                      ↓
              Read rainfall CSV
                      |
                      ↓
                 Show MENU
                      |
       +--------------+--------------+
       |              |              |
       ↓              ↓              ↓
      1              2              3
   Display       Min / Max         Sort
   records       rainfall       /       \
                                Merge    Quick
                                Sort     Sort
                                  |
                                  ↓
                           output CSV files
       |
       ↓
       4
       |
       ↓
 Binary Search
       |
       ↓
 Find rainfall range
```

---

# 65. The C concepts you are actually learning

Don't try to memorize the entire program.

This project is teaching you these C concepts:

| C concept | Where you see it |
|---|---|
| Variables | `int choice`, `double low` |
| Constants | `#define MONTHS 12` |
| Arrays | `monthly[MONTHS]` |
| Structures | `RainfallRecord` |
| Functions | `displayAll()`, `doPartA()` |
| Parameters | `findMinMaxLinear(...)` |
| Return values | `return count` |
| Pointers | `int *minYear` |
| Address operator | `&choice` |
| Dereference | `*minVal` |
| Strings | `char line[1024]` |
| File handling | `fopen`, `fclose` |
| Loops | `for`, `while`, `do while` |
| Conditions | `if`, `else` |
| Switch | menu |
| Dynamic memory | `malloc`, `free` |
| Random numbers | `rand`, `srand` |
| Recursion | Merge Sort/D&C/Quick Sort |
| Structures + arrays | `records[i].monthly[m]` |

And the algorithms are:

```text
Linear Search
      ↓
Divide & Conquer
      ↓
Merge Sort
      ↓
Randomized Quick Sort
      ↓
Binary Search
```

That lines up directly with what your assignment asks you to implement.

---

# 66. One important issue in your code

There is also something you should notice for your viva/demo.

Inside `doPartB()`, after sorting:

```c
free(mergeArr);
free(quickArr);
free(temp);
```

the comments say that the sorted array will be kept for Binary Search, but the code **doesn't actually copy `mergeArr` or `quickArr` into `sortedRecords` there**.

Instead, `doPartC()` sorts the records again when:

```c
sortedCount == 0
```

So the comments and implementation don't completely match.

That's not necessarily fatal because `doPartC()` handles the sorting itself, but it's something worth understanding rather than blindly memorizing.

---

# 67. Recommended learning order

Since you said **you don't know C**, don't try to understand the whole program at once.

We should learn it in this order:

### Lesson 1 — C basics

```c
int
double
char
variables
printf
scanf
if
for
while
```

### Lesson 2 — Arrays + structures

```c
int numbers[10];

RainfallRecord records[200];
```

### Lesson 3 — Functions + pointers

```c
void function(...)
int *x
&x
*x
```

### Lesson 4 — Files + CSV

```c
FILE *
fopen()
fgets()
strtok()
fclose()
```

### Lesson 5 — Divide & Conquer + recursion

Understand:

```c
findMinMaxDC()
```

### Lesson 6 — Merge Sort

Understand:

```c
mergeSortRec()
merge()
```

### Lesson 7 — Quick Sort

Understand:

```c
partition()
quickSortRec()
swapRecords()
```

### Lesson 8 — Binary Search

Understand:

```c
lower_bound()
upper_bound()
```

### Lesson 9 — Full program

Finally, put everything together and learn how to explain every function to your instructor in simple words, which is especially important because your assignment gives **6 marks for explanation**.
