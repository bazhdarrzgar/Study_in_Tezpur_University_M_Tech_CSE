and see if .c language code have all te requirement save the result in a folder Assignment 2.2
please look at this assement 

"


CSMT503						Advanced Programming Lab 1
CSMT533					Data Structures and Algorithms Lab
Assignment 2.2
Objectives: 
To understand and implement greedy algorithms in C/C++.

Exercises:
Huffman coding is a lossless data compression technique. It assigns variable-length binary prefix codes to characters, with shorter codes for more frequent characters and longer codes for less frequent ones, thereby reducing the overall size of the data. It is a greedy algorithm that constructs a prefix tree in bottom up fashion, by merging the two least frequent characters at each step [1][2]. 

Write a program to produce Huffman codes for any user given text file. Display the codes generated for each character. Compare the number of bits required to store the file using the Huffman code with fixed-length code and ASCII code.
Store the text compressed using Huffman codes in a binary file. You will need to store the codes along with a header that includes the details required for decoding/decompressing the file. You may use the extension .huf to distinguish your compressed file.
Read the header of the .huf file and use the header information to decompress, read, and display the contents of the compressed text file.

[Hints: The Huffman code tree can be stored as a preorder or postorder traversal. Special EOF character codes can be used to indicate the end of file. This can then be used to indicate end of the header section. ]

Submission Instructions:
Rename the files using your own roll number and the assignment number. For example, if your roll number is CSE22001 and the file corresponds to Assignment 1, then the filename will be CSE22001_Lab1.c. 
Students are expected to explain their code and show a demo of the output to the course instructor or teaching assistant. Only after the demo, the marks for the corresponding assignment will be recorded. Simply uploading the file in Google Classroom will not be considered as a submission.
All assignments will be initially scored out of 10 (unless stated otherwise), and then added to the final evaluation plan with a weight. 
Out of 10 marks: 1 is for timely submission, 3 for output, and 6 for explanation. 


References:
Huffman, D. (1952). "A Method for the Construction of Minimum-Redundancy Codes". Proceedings of the IRE. 40 (9): 1098–1101. doi:10.1109/JRPROC.1952.273898
Cormen, T.H., Leiserson, C.E., Rivest, R.L., and Stein, C. (2009). Introduction to algorithms. MIT Press. [Chapter 16, Section 16.3]
https://www.youtube.com/watch?v=co4_ahEDCho&t=10s



" 
