#pragma once
#include <string>
#include <iostream>

using namespace std;

// --- LINKED LIST NODE FOR STRINGS ---
// (Used for Actors and Genres lists)
struct StringNode {
    string value;
    StringNode* next;
    StringNode(string v) : value(v), next(nullptr) {}
};

// --- MOVIE OBJECT ---
struct Movie {
    int id;
    string title;
    int year;
    double rating;
    StringNode* actors; // Head of Linked List
    StringNode* genres; // Head of Linked List

    Movie(int i, string t, int y, double r)
        : id(i), title(t), year(y), rating(r), actors(nullptr), genres(nullptr) {
    }

    // Helper functions implemented in DataStructures.cpp
    void addActor(string name);
    void addGenre(string genre);
};