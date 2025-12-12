#include <iostream>
#include <fstream>
#include "DataStructures.h"
#include "Utils.h"

using namespace std;
using namespace Utils;

const int MAX_MOVIES = 5000;

class SystemManager {
public:
    MovieAVL avl;
    MovieHash actorIndex;
    MovieHash genreIndex;
    MovieGraph* graph;
    Movie* movieDB[MAX_MOVIES];
    int movieCount;

    SystemManager() : graph(nullptr), movieCount(0) {
        for (int i = 0; i < MAX_MOVIES; i++) movieDB[i] = nullptr;
    }

    // Helper to parse CSV fields manually
    string parseCSVField(string& line, int& pos) {
        string result = "";
        bool inQuotes = false;
        if (pos < line.length() && line[pos] == '"') { inQuotes = true; pos++; }

        while (pos < line.length()) {
            char c = line[pos];
            if (inQuotes && c == '"') {
                if (pos + 1 < line.length() && line[pos + 1] == '"') { result += '"'; pos++; }
                else inQuotes = false;
            }
            else if (!inQuotes && c == ',') { pos++; return result; }
            else result.push_back(c);
            pos++;
        }
        return result;
    }

    void loadData(string filename) {
        cout << YELLOW << "[*] Loading Database from " << filename << "..." << RESET << endl;
        ifstream file(filename);
        if (!file.is_open()) { cout << RED << "Error: File not found!" << RESET << endl; return; }

        string line;
        getline(file, line); // Skip Header

        while (getline(file, line) && movieCount < MAX_MOVIES) {
            int pos = 0;
            string temp, title, yearStr, ratingStr, genreStr, actor1, actor2, actor3;

            // NOTE: This assumes standard IMDB format. Adjust indices if your CSV differs.
            // Loop through columns to find specific indices
            for (int i = 0; i <= 25; i++) {
                temp = parseCSVField(line, pos);
                if (i == 6) actor2 = temp;
                if (i == 9) genreStr = temp;
                if (i == 10) actor1 = temp;
                if (i == 11) title = temp;
                if (i == 14) actor3 = temp;
                if (i == 23) yearStr = temp;
                if (i == 25) ratingStr = temp;
            }

            title = cleanString(title);
            if (title == "") continue;

            Movie* m = new Movie(movieCount, title, stringToInt(yearStr), stringToDouble(ratingStr));
            if (actor1 != "") m->addActor(cleanString(actor1));
            if (actor2 != "") m->addActor(cleanString(actor2));
            if (actor3 != "") m->addActor(cleanString(actor3));

            // Split Genre manually
            string g = "";
            for (char c : genreStr) {
                if (c == '|') { if (g != "") m->addGenre(cleanString(g)); g = ""; }
                else g += c;
            }
            if (g != "") m->addGenre(cleanString(g));

            // Insert into Data Structures
            avl.insert(m);
            movieDB[movieCount] = m;

            // Update Hashes
            StringNode* curr = m->actors;
            while (curr) { actorIndex.insert(curr->value, m); curr = curr->next; }

            curr = m->genres;
            while (curr) { genreIndex.insert(curr->value, m); curr = curr->next; }

            movieCount++;
        }

        cout << GREEN << "[+] Loaded " << movieCount << " movies." << RESET << endl;

        // Build Graph Connections (Simple approach: Same Genre)
        cout << YELLOW << "[*] Building Graph..." << RESET << endl;
        graph = new MovieGraph(movieCount);

        // Connect each movie to next 50 movies if they share a genre
        for (int i = 0; i < movieCount; i++) {
            for (int j = i + 1; j < i + 50 && j < movieCount; j++) {
                StringNode* g1 = movieDB[i]->genres;
                bool connected = false;
                while (g1) {
                    StringNode* g2 = movieDB[j]->genres;
                    while (g2) {
                        if (g1->value == g2->value) {
                            graph->addEdge(i, j);
                            connected = true;
                            break;
                        }
                        g2 = g2->next;
                    }
                    if (connected) break;
                    g1 = g1->next;
                }
            }
        }
        cout << GREEN << "[+] System Ready!" << RESET << endl;
    }

    void printMovie(Movie* m) {
        cout << "\n" << BOLD << "==============================" << RESET << endl;
        cout << CYAN << " TITLE : " << RESET << m->title << endl;
        cout << CYAN << " YEAR  : " << RESET << m->year << endl;
        cout << CYAN << " RATING: " << RESET << m->rating << "/10" << endl;
        cout << " CAST  : ";
        StringNode* curr = m->actors;
        while (curr) { cout << curr->value << (curr->next ? ", " : ""); curr = curr->next; }
        cout << "\n GENRE : ";
        curr = m->genres;
        while (curr) { cout << curr->value << (curr->next ? ", " : ""); curr = curr->next; }
        cout << "\n" << BOLD << "==============================" << RESET << "\n";
    }

    void run() {
        int choice;
        string input, input2;
        while (true) {
            cout << "\n" << BOLD << MAGENTA << "--- MOVIE NEXUS ---" << RESET << endl;
            cout << "1. Search Title\n2. Search Actor\n3. Recommend\n4. Shortest Path\n5. Exit\n>> ";
            if (!(cin >> choice)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
            cin.ignore();

            if (choice == 1) {
                cout << "Enter Title: "; getline(cin, input);
                Timer t;
                Movie* m = avl.search(cleanString(input));
                if (m) printMovie(m); else cout << RED << "Not Found." << RESET << endl;
                t.printDuration();
            }
            else if (choice == 2) {
                cout << "Enter Actor: "; getline(cin, input);
                actorIndex.search(cleanString(input));
            }
            else if (choice == 3) {
                cout << "Enter Title: "; getline(cin, input);
                Movie* m = avl.search(cleanString(input));
                if (m) graph->getRecommendations(m->id, movieDB);
                else cout << RED << "Movie not found." << RESET << endl;
            }
            else if (choice == 4) {
                cout << "Start Movie: "; getline(cin, input);
                cout << "End Movie: "; getline(cin, input2);
                Movie* m1 = avl.search(cleanString(input));
                Movie* m2 = avl.search(cleanString(input2));
                if (m1 && m2) graph->getShortestPath(m1->id, m2->id, movieDB);
                else cout << RED << "Invalid Movies." << RESET << endl;
            }
            else if (choice == 5) break;
        }
    }
};

int main() {
    SystemManager sys;
    sys.loadData("movie_metadata.csv"); // Ensure this file exists in the folder!
    sys.run();
    return 0;
}