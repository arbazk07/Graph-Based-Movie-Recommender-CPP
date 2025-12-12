#pragma once
#include "Movie.h" // <--- CRITICAL LINK
#include <string>
#include <iostream>

using namespace std;

const int TABLE_SIZE = 10007;

// --- CUSTOM NODES ---

// For Graph Edges
struct AdjListNode {
    int dest;
    AdjListNode* next;
    AdjListNode(int d) : dest(d), next(nullptr) {}
};

// For BFS Queue
struct IntNode {
    int value;
    IntNode* next;
    IntNode(int v) : value(v), next(nullptr) {}
};

// --- DATA STRUCTURE CLASSES ---

// Custom Queue class for BFS
class CustomQueue {
private:
    IntNode* front;
    IntNode* rear;
public:
    CustomQueue();
    ~CustomQueue();
    void push(int val);
    int pop();
    bool isEmpty();
};

// AVL Tree Class
struct AVLNode {
    Movie* movie;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(Movie* m) : movie(m), left(nullptr), right(nullptr), height(1) {}
};

class MovieAVL {
private:
    AVLNode* root;
    int getHeight(AVLNode* node);
    int getBalance(AVLNode* node);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insertHelper(AVLNode* node, Movie* movie);
    Movie* searchHelper(AVLNode* node, string title);
    AVLNode* deleteHelper(AVLNode* node, string title);
    AVLNode* findMin(AVLNode* node);
    void destroyTree(AVLNode* node);

public:
    MovieAVL();
    ~MovieAVL();
    void insert(Movie* movie);
    void remove(string title);
    Movie* search(string title);
};

// Hash Table Class
struct HashNode {
    string key;
    struct MovieLink {
        Movie* movie;
        MovieLink* next;
        MovieLink(Movie* m) : movie(m), next(nullptr) {}
    } *movieList;
    HashNode* next;
    HashNode(string k) : key(k), movieList(nullptr), next(nullptr) {}
};

class MovieHash {
private:
    HashNode* table[TABLE_SIZE];
    int hashFunction(string key);

public:
    MovieHash();
    ~MovieHash();
    void insert(string key, Movie* movie);
    void search(string key);
};

// Graph Class
class MovieGraph {
private:
    AdjListNode** adjList;
    int numMovies;

public:
    MovieGraph(int n);
    ~MovieGraph();
    void addEdge(int src, int dest);
    void getRecommendations(int startId, Movie** movieDB);
    void getShortestPath(int startId, int endId, Movie** movieDB);
};