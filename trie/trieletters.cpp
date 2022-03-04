#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

const int DEPTH = 3;
const char DELIMITERS[] = " \n\t";
const int ALPHABET_SIZE = 26;

// Word trie node
struct WordTrieNode {
    char* word;
    struct WordTrieNode *children[ALPHABET_SIZE];

    // TODO: Make sure count doesn't overflow
    int count;
};

struct WordTrieNode *createNode(void) {
    struct WordTrieNode *curr = new WordTrieNode;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        curr->children[i] = NULL;
    curr->count = 0;
    return curr;

    // TODO: Do I need to declare this?
    // curr->word = NULL;
}

void incrementNode(struct WordTrieNode *root, char **phrase) {
    struct WordTrieNode *curr = root;

    // Traverse to a leaf in the tree
    char **currPhrase = phrase;
    char *currLetter;
    int i = 0;
    int phraseIdx = 0;
    while (i > -1) {
        // Get letter
        currLetter = &phrase[phraseIdx][i];

        // Get list of current node's children
        // TODO: support non lowercase characters
        if (*currLetter - 'a' > 'z')
            break;
        
        WordTrieNode *next = curr->children[*currLetter - 'a'];

        // Traverse down the tree
        if (next != NULL) {
            // If the letter node exists, traverse to it
            curr = next;
        } else {
            // If the word has not been seen, create a new node
            WordTrieNode *newNode = createNode();
            // this gives a segfault error, currLetter is accessing
            // memory addresses that don't exist
            *newNode->word = currLetter[0];
            
            // Add new node to current node's children
            curr->children[*currLetter - 'a'] = newNode;

            // Traverse to new node
            curr = newNode;
        }

        cout << phrase[phraseIdx][i] << "\n";

        if (phrase[phraseIdx + 1] == 0 && phrase[phraseIdx][i + 1] == 0) {
            // We've reached the end of the phrase
            cout << "end phrase\n";
            i = -1;
        } else if (phrase[phraseIdx][i + 1] == 0) {
            // We've reached the end of a word
            cout << "next word\n";
            phraseIdx++;
            i = 0;
        } else {
            i++;
        }
    }

    // Increment the count on the leaf node
    curr->count++;
}

void printTree(WordTrieNode *curr, std::string *currWord) {
    cout << curr->word << "\n";
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (curr->children[i] != NULL) {
            printTree(curr->children[i], currWord);
        }
    }
    
    if (curr->count > 0)
        cout << curr->word << " : " << curr->count << "\n";
}

void basicTest(WordTrieNode *root) {
    char* pair1[4] = {(char*) "hello", (char*) "world", (char*) "i", NULL};
    char* pair2[4] = {(char*) "hello", (char*) "world", (char*) "you", NULL};
    char* pair3[4] = {(char*) "hello", (char*) "world", (char*) "i", NULL};
    char* pair4[4] = {(char*) "why", (char*) "world", (char*) "i", NULL};
    char* pair5[4] = {(char*) "you", (char*) "wouldnt", (char*) "even", NULL};
    char* pair6[4] = {(char*) "you", (char*) "wouldnt", (char*) "even", NULL};
    char* pair7[4] = {(char*) "you", (char*) "wouldnt", (char*) "even", NULL};
    char* pair8[4] = {(char*) "you", (char*) "wouldnt", (char*) "try", NULL};
    
    incrementNode(root, pair1);
    incrementNode(root, pair2);
    incrementNode(root, pair3);
    incrementNode(root, pair4);
    incrementNode(root, pair5);
    incrementNode(root, pair6);
    incrementNode(root, pair7);
    incrementNode(root, pair8);

    // WordTrieNode *one = root->children->find("why")->second;
    // WordTrieNode *two = one->children->find("world")->second;
    // int out = two->children->find("i")->second->count;
    // std::cout << (int) out << "\n";
}

WordTrieNode *assembleTreeFromFile(char *filename) {
    // Create root node
    WordTrieNode *root = createNode();

    // Load the text file as a stream
    ifstream handler;
    handler.open(filename);
    if (handler.is_open()) {
        string line;
        std::string currWord;
        size_t i;
        size_t j;

        // TODO: Need to handle edge cases better
        std::string phrase[3] = {*(new std::string("Hello")), *(new std::string("Hello")), *(new std::string("Hello"))};
        while (true) { //getline(handler, line)
            // Add each word in the line to the trie
            i = 0, j = 0;
            while (j != SIZE_MAX) {
                // Find the ending index of the next word
                j = line.find_first_of(DELIMITERS, i);
                
                // Assemble the phrase by pushing the word to the end and popping the beginning word
                phrase[0] = phrase[1];
                phrase[1] = phrase[2];
                phrase[2] = line.substr(i, j-i);

                // Increment the corresponding phrase in the trie
                // incrementNode(root, phrase);

                // Update pointer for next word                
                i = j + 1;
            }
        }
        handler.close();
    }
    return root;
}

int mainother(int argc, char *argv[]) {
    char* filename;
    if (argc == 2) {
        std::cout << "Processing file: " << argv[1] << "\n\n";
        filename = argv[1];
    } else {
        filename = (char *) "test_small.txt";
    }

    WordTrieNode *root = assembleTreeFromFile(filename);

    // https://stackoverflow.com/questions/19227698/write-a-program-to-find-100-largest-numbers-out-of-an-array-of-1-billion-numbers
    // to find the top 100 pairs:
    // create a priority queue
    // iterate through the tree
    // add each phrase to the priority queue

    printTree(root, new std::string(""));
}

int main() {
    WordTrieNode *root = createNode();
    basicTest(root);
    printTree(root, new std::string(""));
    return 0;
}