# A-File-Compress-SDK
In this project, an SDK is encapsulated in C++, which can realize the function of compressing files. The project is based on the Huffman tree, and the bottom layer is implemented through priority queues.

# Compress
The compression function is implemented based on the Huffman tree. The so-called file compression is actually to convert the corresponding character encoding into another encoding that occupies a smaller number of bytes for storage. The space required for 1 character is 1 byte, that is, 8 bits. The original character string can be re-encoded, and the original character can be replaced with binary bits. In this way, the character that appears more frequently is replaced with a shorter encoding, and the character string is compressed.

# Create a Huffman Tree
1. From the given n weights {w1,w2,w3,...,wn}, construct n extended binary tree forests with only root nodes F= {T1,T2,T3,...,Tn}, each of which is extended binary tree Ti has only one root node with weight wi, and both left and right children are empty.

2. Repeat the following steps until there is only one tree left in F:

a. Select two expanded binary trees with the smallest root node weight in F to construct a new binary tree as the left and right subtrees. Set the weight of the root node of the new binary tree to the sum of the weight of the root node on its left and right subtrees.

b. Delete the two binary trees in F;

c. Add the new binary tree to F;

The final result is the Huffman tree.

# Bottom Layer
priority queues. The implementation of a priority queue written by the author is in the files "priorityqueue.h" and "priorityqueue.cpp".

# How to use
You can add the four files in the src folder to your project file, then run "main.cpp" and enter the path of the file to be compressed to complete the compression.

# Reference
https://blog.csdn.net/qq_33724710/article/details/52201777
