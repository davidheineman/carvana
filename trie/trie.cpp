#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

const int DEPTH = 3;
const char delimiters[] = " \n\t";

// Word trie node
struct WordTrieNode {
    string word;
    unordered_map<string, WordTrieNode *> *children;

    // TODO: Make sure count doesn't overflow
    int count;
};

struct WordTrieNode *createNode(void) {
    struct WordTrieNode *curr = new WordTrieNode;
    curr->children = new unordered_map<string, WordTrieNode *>;
    curr->count = 0;

    // TODO: Do I need to declare this?
    // curr->word = NULL;
}

void incrementNode(struct WordTrieNode *root, string *phrase) {
    struct WordTrieNode *curr = root;

    // Traverse to a leaf in the tree
    string *currWord;
    unordered_map<string, WordTrieNode *> *map;
    unordered_map<string, WordTrieNode *>::iterator it;
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
            map->insert(pair<string, WordTrieNode *>(newNode->word, newNode));

            // Traverse to new node
            curr = newNode;
        }       
    }

    // Increment the count on the leaf node
    curr->count++;
}

void printTree(WordTrieNode *root) {
    // TODO: Support tries with a depth of non-3
    unordered_map<string, WordTrieNode *> *map = root->children;
    unordered_map<string, WordTrieNode *>::iterator it;
    for (it = map->begin(); it != map->end(); it++) {
        unordered_map<string, WordTrieNode *> *map1 = it->second->children;
        unordered_map<string, WordTrieNode *>::iterator it1;
        for (it1 = map1->begin(); it1 != map1->end(); it1++) {
            unordered_map<string, WordTrieNode *> *map2 = it1->second->children;
            unordered_map<string, WordTrieNode *>::iterator it2;
            for (it2 = map2->begin(); it2 != map2->end(); it2++) {
                if (it2->second->count > 3)
                    printf("%s %s %s: %d\n", it->second->word.data(), it1->second->word.data(), it2->second->word.data(), it2->second->count);
            }
        }
    }
}

void basicTest(WordTrieNode *root) {
    string pair1[3] = {"hello", "world", "i"};
    string pair2[3] = {"hello", "world", "you"};
    string pair3[3] = {"hello", "world", "i"};
    string pair4[3] = {"why", "world", "i"};
    string pair5[3] = {"you", "wouldnt", "even"};
    string pair6[3] = {"you", "wouldnt", "even"};
    string pair7[3] = {"you", "wouldnt", "even"};
    string pair8[3] = {"you", "wouldnt", "dare"};
    
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
    cout << (int) out << "\n";
}

WordTrieNode *assembleTreeFromFile(char *filename) {
    // Create root node
    WordTrieNode *root = createNode();

    // Load the text file as a stream
    ifstream handler;
    handler.open(filename);
    if (handler.is_open()) {
        string line;
        string currWord;
        size_t i;
        size_t j;

        // TODO: Need to handle edge cases better
        string phrase[3] = {*(new string("Hello")), *(new string("Hello")), *(new string("Hello"))};
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

void printTopPairs(char* filename) {
    WordTrieNode *root = assembleTreeFromFile(filename);

    // Create a priority queue to store values
    priority_queue<pair<int, string>> pq;

    // Iterates through entire tree
    unordered_map<string, WordTrieNode *> *map = root->children;
    unordered_map<string, WordTrieNode *>::iterator it;
    for (it = map->begin(); it != map->end(); it++) {
        unordered_map<string, WordTrieNode *> *map1 = it->second->children;
        unordered_map<string, WordTrieNode *>::iterator it1;
        for (it1 = map1->begin(); it1 != map1->end(); it1++) {
            unordered_map<string, WordTrieNode *> *map2 = it1->second->children;
            unordered_map<string, WordTrieNode *>::iterator it2;
            for (it2 = map2->begin(); it2 != map2->end(); it2++) {
                // Add each phrase to the priority queue
                stringstream ss;
                ss << it->second->word.data() << " " << it1->second->word.data() << " " << it2->second->word.data();
                string s = ss.str();

                pq.push(make_pair(it2->second->count, s));

                // If the priority queue is larger than 100, delete values
                // TODO: Keep the priority queue below 100 values
                // if (pq.size() > 100)
                //     pq.pop();
            }
        }
    }

    // Print the top 100 values from the priority queue
    // while (!pq.empty()) {
    for (int i = 0; i < 100; i++) {
        pair<int, string> next = pq.top();
        cout << next.first << " : " << next.second << "\n";
        pq.pop();
    }
}

int main(int argc, char *argv[]) {
    // TODO: Process standard input if it exists
    char* filename;
    if (argc > 1) {
        // Iterate through all filenames
        for (int i = 1; i < argc; i++) {
            cout << "Processing file: " << argv[i] << "\n\n";
            filename = argv[i];
            printTopPairs(filename);
            cout << "\n";
        }
    } else {
        cout << "Need to give filename as input!";
    }   
    return 0;
}