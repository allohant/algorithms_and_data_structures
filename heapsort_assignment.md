Please sort a set of integers (in the range [10<sup>9</sup>, 10<sup>9</sup>]) in ascending order using the heap sort algorithm.

### Input:
In the first line of the input file there is an integer specifying the number of test cases N. This is followed by a description of the successive test cases, with the description of each case contained in one line. The first number in the line (k) corresponds to the number of elements in the set, then there are k numbers in the order corresponding to the current order of the elements in the set.

### Output:
The output contains successively N blocks describing the sorting process, separated by empty lines. Each block must contain exactly k lines, containing in turn: the contents of the complete heap created during sorting after each successive heap, without the heap containing only the root and without the further part of the array, and finally the sorted set.

### Example:

|Input: | Output: |
| --- | --- |
|2 | 2 1|
|2 2 1 | 1 2|
|5 5 1 8 3 7 | |
||8 7 5 1 3
||7 3 5 1
||5 1 3
||3 1
||1 3 5 7 8
