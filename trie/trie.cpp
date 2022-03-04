#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>

using namespace std;

const int DEPTH = 3;
const char delimiters[] = " \n\t";

// Word trie node
struct WordTrieNode {
    std::string word;
    std::unordered_map<std::string, WordTrieNode *> *children;

    // TODO: Make sure count doesn't overflow
    int count;
};

struct WordTrieNode *createNode(void) {
    struct WordTrieNode *curr = new WordTrieNode;
    curr->children = new std::unordered_map<std::string, WordTrieNode *>;
    curr->count = 0;

    // TODO: Do I need to declare this?
    // curr->word = NULL;
}

void incrementNode(struct WordTrieNode *root, std::string *phrase) {
    struct WordTrieNode *curr = root;

    // Traverse to a leaf in the tree
    std::string *currWord;
    std::unordered_map<std::string, WordTrieNode *> *map;
    std::unordered_map<std::string, WordTrieNode *>::iterator it;
    for (int i = 0; i < DEPTH; i++) {       
        // Get next word to traverse down the tree   
        currWord = &phrase[i];

        // Get list of current node's children
        map = curr->children;
        it = map->find(*currWord);

        // Traverse down the tree
        if (it != map->end() && it->second != NULL) {
            // If node has children and the word exists, traverse to that node
            curr = it->second;
        } else {
            // If the word has not been seen, create a new node
            WordTrieNode *newNode = createNode();
            newNode->word = *currWord;

            // Add new node to current node's children
            map->insert(std::pair<std::string, WordTrieNode *>(newNode->word, newNode));

            // Traverse to new node
            curr = newNode;
        }       
    }

    // Increment the count on the leaf node
    curr->count++;
}

void printTree(WordTrieNode *root) {
    // TODO: Support tries with a depth of non-3
    std::unordered_map<std::string, WordTrieNode *> *map = root->children;
    std::unordered_map<std::string, WordTrieNode *>::iterator it;
    for (it = map->begin(); it != map->end(); it++) {
        std::unordered_map<std::string, WordTrieNode *> *map1 = it->second->children;
        std::unordered_map<std::string, WordTrieNode *>::iterator it1;
        for (it1 = map1->begin(); it1 != map1->end(); it1++) {
            std::unordered_map<std::string, WordTrieNode *> *map2 = it1->second->children;
            std::unordered_map<std::string, WordTrieNode *>::iterator it2;
            for (it2 = map2->begin(); it2 != map2->end(); it2++) {
                if (it2->second->count > 3)
                    printf("%s %s %s: %d\n", it->second->word.data(), it1->second->word.data(), it2->second->word.data(), it2->second->count);
            }
        }
        
    }
}

void basicTest(WordTrieNode *root) {
    std::string pair1[3] = {"hello", "world", "i"};
    std::string pair2[3] = {"hello", "world", "you"};
    std::string pair3[3] = {"hello", "world", "i"};
    std::string pair4[3] = {"why", "world", "i"};
    std::string pair5[3] = {"you", "wouldnt", "even"};
    std::string pair6[3] = {"you", "wouldnt", "even"};
    std::string pair7[3] = {"you", "wouldnt", "even"};
    std::string pair8[3] = {"you", "wouldnt", "dare"};
    
    incrementNode(root, pair1);
    incrementNode(root, pair2);
    incrementNode(root, pair3);
    incrementNode(root, pair4);
    incrementNode(root, pair5);
    incrementNode(root, pair6);
    incrementNode(root, pair7);
    incrementNode(root, pair8);

    WordTrieNode *one = root->children->find("why")->second;
    WordTrieNode *two = one->children->find("world")->second;
    int out = two->children->find("i")->second->count;
    std::cout << (int) out << "\n";
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
        while (getline(handler, line)) {
            // Add each word in the line to the trie
            i = 0, j = 0;
            while (j != SIZE_MAX) {
                // Find the ending index of the next word
                j = line.find_first_of(delimiters, i);
                
                // Assemble the phrase by pushing the word to the end and popping the beginning word
                phrase[0] = phrase[1];
                phrase[1] = phrase[2];
                phrase[2] = line.substr(i, j-i);

                // Increment the corresponding phrase in the trie
                incrementNode(root, phrase);

                // Update pointer for next word                
                i = j + 1;
            }
        }
        handler.close();
    }
    return root;
}

int main(int argc, char *argv[]) {
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

    printTree(root);
}