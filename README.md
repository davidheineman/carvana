## Carvana Coding Challeng
This is my solution to the carvana coding challenge! It efficiently finds the most frequent tri-grams in a document.

### Requirements
- `gcc` compiler
- If you want to test the naive solution, you need to have `jupyter` with `pandas` installed (`pip install jupyter pandas`)

### How to use
- Clone this repository
- Enter the `trie` folder
- Compile the trie with `g++ trie.cpp -o trie`
- Run the trie with `./trie [filename(s)]`

*The repository contains data in `data/test_small.txt` and `data/test_tiny.txt`*

### How it works
`naive.ipynb` contains python code on various naive solutions

> It clearly runs inefficiently. Every solution loads the entire text file into memory and creates a large hashmap containing every pair of words, meaning the code is time and space inefficient

`trie.cpp` contains C++ code which creates a [trie](https://arxiv.org/pdf/1806.09447.pdf) to keep track of words

> This tree contains words at each node, making the tree more space efficient than a hash map. It also scans the corpus one line at a time to allow for files larger than memory.

![tree](tree.jpg)

### What's next
I spent most of my time on efficiency, rather than meeting the extra requirements. I would start by testing my code and building out the `trie.cpp` to have it support other applications than just counting pairs of words.

Some test cases I haven't considered:
- Non-unicode characters
- Handling line-endings and small pairs of words
- Handling words with a large number of characters
- Handling large files with entirely unique characters

Also, I started writing `trieletters.cpp` as a more efficient implementation of `trie.cpp` which uses individual letters as nodes in the tree rather than entire words. This uses more primitive datatypes,  less expensive C objects and would run faster. I ran into road blocks with `segmentation fault` errors, which is why this solution is half-way done.