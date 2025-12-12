#include "DataStructures.h"
#include "Utils.h"
#include <algorithm> // For max

// --- MOVIE HELPERS ---
void Movie::addActor(string name) {
    StringNode* newNode = new StringNode(name);
    newNode->next = actors;
    actors = newNode;
}

void Movie::addGenre(string genre) {
    StringNode* newNode = new StringNode(genre);
    newNode->next = genres;
    genres = newNode;
}

// --- QUEUE IMPLEMENTATION ---
CustomQueue::CustomQueue() : front(nullptr), rear(nullptr) {}

CustomQueue::~CustomQueue() {
    while (!isEmpty()) pop();
}

void CustomQueue::push(int val) {
    IntNode* temp = new IntNode(val);
    if (rear == nullptr) {
        front = rear = temp;
        return;
    }
    rear->next = temp;
    rear = temp;
}

int CustomQueue::pop() {
    if (front == nullptr) return -1;
    IntNode* temp = front;
    int val = temp->value;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    delete temp;
    return val;
}

bool CustomQueue::isEmpty() { return front == nullptr; }

// --- AVL IMPLEMENTATION ---
MovieAVL::MovieAVL() : root(nullptr) {}
MovieAVL::~MovieAVL() { destroyTree(root); }

void MovieAVL::destroyTree(AVLNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

int MovieAVL::getHeight(AVLNode* node) { return node ? node->height : 0; }
int MovieAVL::getBalance(AVLNode* node) { return node ? getHeight(node->left) - getHeight(node->right) : 0; }

AVLNode* MovieAVL::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

AVLNode* MovieAVL::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

AVLNode* MovieAVL::insertHelper(AVLNode* node, Movie* movie) {
    if (!node) return new AVLNode(movie);
    if (movie->title < node->movie->title) node->left = insertHelper(node->left, movie);
    else if (movie->title > node->movie->title) node->right = insertHelper(node->right, movie);
    else return node;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && movie->title < node->left->movie->title) return rightRotate(node);
    if (balance < -1 && movie->title > node->right->movie->title) return leftRotate(node);
    if (balance > 1 && movie->title > node->left->movie->title) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && movie->title < node->right->movie->title) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

void MovieAVL::insert(Movie* movie) { root = insertHelper(root, movie); }

Movie* MovieAVL::searchHelper(AVLNode* node, string title) {
    if (!node) return nullptr;
    if (node->movie->title == title) return node->movie;
    if (title < node->movie->title) return searchHelper(node->left, title);
    return searchHelper(node->right, title);
}

Movie* MovieAVL::search(string title) { return searchHelper(root, title); }

AVLNode* MovieAVL::findMin(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != nullptr) current = current->left;
    return current;
}

AVLNode* MovieAVL::deleteHelper(AVLNode* root, string title) {
    if (!root) return root;
    if (title < root->movie->title) root->left = deleteHelper(root->left, title);
    else if (title > root->movie->title) root->right = deleteHelper(root->right, title);
    else {
        if ((!root->left) || (!root->right)) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) { temp = root; root = nullptr; }
            else *root = *temp;
            delete temp;
        }
        else {
            AVLNode* temp = findMin(root->right);
            root->movie = temp->movie;
            root->right = deleteHelper(root->right, temp->movie->title);
        }
    }
    if (!root) return root;
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) { root->left = leftRotate(root->left); return rightRotate(root); }
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) { root->right = rightRotate(root->right); return leftRotate(root); }
    return root;
}

void MovieAVL::remove(string title) { root = deleteHelper(root, title); }


// --- HASH TABLE IMPLEMENTATION ---
MovieHash::MovieHash() {
    for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
}

MovieHash::~MovieHash() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode* entry = table[i];
        while (entry) {
            HashNode::MovieLink* link = entry->movieList;
            while (link) {
                HashNode::MovieLink* tempLink = link;
                link = link->next;
                delete tempLink;
            }
            HashNode* temp = entry;
            entry = entry->next;
            delete temp;
        }
    }
}

int MovieHash::hashFunction(string key) {
    unsigned long hash = 5381;
    for (char c : key) hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

void MovieHash::insert(string key, Movie* movie) {
    int index = hashFunction(key);
    HashNode* entry = table[index];
    while (entry) {
        if (entry->key == key) {
            HashNode::MovieLink* newLink = new HashNode::MovieLink(movie);
            newLink->next = entry->movieList;
            entry->movieList = newLink;
            return;
        }
        entry = entry->next;
    }
    HashNode* newNode = new HashNode(key);
    newNode->movieList = new HashNode::MovieLink(movie);
    newNode->next = table[index];
    table[index] = newNode;
}

void MovieHash::search(string key) {
    int index = hashFunction(key);
    HashNode* entry = table[index];
    while (entry) {
        if (entry->key == key) {
            HashNode::MovieLink* current = entry->movieList;
            cout << GREEN << "Found Movies for '" << key << "':" << RESET << endl;
            while (current) {
                cout << "- " << current->movie->title << endl;
                current = current->next;
            }
            return;
        }
        entry = entry->next;
    }
    cout << RED << "Not Found: " << key << RESET << endl;
}

// --- GRAPH IMPLEMENTATION ---
MovieGraph::MovieGraph(int n) : numMovies(n) {
    adjList = new AdjListNode * [n];
    for (int i = 0; i < n; i++) adjList[i] = nullptr;
}

MovieGraph::~MovieGraph() {
    for (int i = 0; i < numMovies; i++) {
        AdjListNode* temp = adjList[i];
        while (temp) {
            AdjListNode* del = temp;
            temp = temp->next;
            delete del;
        }
    }
    delete[] adjList;
}

void MovieGraph::addEdge(int src, int dest) {
    AdjListNode* newNode = new AdjListNode(dest);
    newNode->next = adjList[src];
    adjList[src] = newNode;

    newNode = new AdjListNode(src);
    newNode->next = adjList[dest];
    adjList[dest] = newNode;
}

void MovieGraph::getRecommendations(int startId, Movie** movieDB) {
    bool* visited = new bool[numMovies];
    for (int i = 0; i < numMovies; i++) visited[i] = false;

    CustomQueue q;
    visited[startId] = true;
    q.push(startId);

    int count = 0;
    cout << CYAN << "Similar Recommendations:" << RESET << endl;

    while (!q.isEmpty() && count < 10) {
        int curr = q.pop();
        if (curr != startId) {
            cout << count + 1 << ". " << movieDB[curr]->title << endl;
            count++;
        }
        AdjListNode* temp = adjList[curr];
        while (temp) {
            if (!visited[temp->dest]) {
                visited[temp->dest] = true;
                q.push(temp->dest);
            }
            temp = temp->next;
        }
    }
    delete[] visited;
}

void MovieGraph::getShortestPath(int startId, int endId, Movie** movieDB) {
    int* parent = new int[numMovies];
    bool* visited = new bool[numMovies];
    for (int i = 0; i < numMovies; i++) { visited[i] = false; parent[i] = -1; }

    CustomQueue q;
    visited[startId] = true;
    q.push(startId);
    bool found = false;

    while (!q.isEmpty()) {
        int curr = q.pop();
        if (curr == endId) { found = true; break; }
        AdjListNode* temp = adjList[curr];
        while (temp) {
            if (!visited[temp->dest]) {
                visited[temp->dest] = true;
                parent[temp->dest] = curr;
                q.push(temp->dest);
            }
            temp = temp->next;
        }
    }

    if (found) {
        cout << GREEN << "Shortest Path:" << RESET << endl;
        // Backtrack (using a simple array to store path)
        int path[100]; // Assuming path < 100
        int idx = 0;
        int curr = endId;
        while (curr != -1) {
            path[idx++] = curr;
            curr = parent[curr];
        }
        for (int i = idx - 1; i >= 0; i--) {
            cout << movieDB[path[i]]->title;
            if (i > 0) cout << " -> ";
        }
        cout << endl;
    }
    else {
        cout << RED << "No Connection Found." << RESET << endl;
    }
    delete[] parent;
    delete[] visited;
}