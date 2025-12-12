# Graph-Based Movie Recommender Engine (C++)

A high-performance, custom-built recommendation engine that suggests movies based on genre connectivity and actor relationships. Built entirely in C++ **without using the Standard Template Library (STL)** for core data structures to demonstrate manual memory management and algorithmic understanding.

## ? Key Features

* **Data Structures Built from Scratch:**
    * **AVL Tree:** For O(log n) balanced searching of movie titles.
    * **Hash Table:** Custom chaining implementation for O(1) Actor & Genre lookups.
    * **Graph (Adjacency List):** Models relationships between movies for recommendation logic.
    * **Custom Queue:** Implemented for Breadth-First Search (BFS) traversal.
* **Memory Management:** Full manual control over heap memory with custom destructors to ensure zero memory leaks.
* **Graph Algorithms:** Uses BFS to find the "shortest path" between two movies and to generate recommendations based on shared attributes.
* **Fuzzy Search Handling:** Includes robust string parsing to handle special characters and CSV edge cases.

## ??? Tech Stack

* **Language:** C++ (C++11/14 Standard)
* **Paradigm:** Object-Oriented Programming (OOP)
* **Optimization:** Pointer-based manipulation to minimize data copying.
* **Interface:** ANSI Color-coded CLI for better user experience.

## ?? How to Run

1.  **Clone the Repository:**
    ```bash
    git clone [https://github.com/YOUR-USERNAME/Graph-Based-Movie-Recommender-CPP.git](https://github.com/YOUR-USERNAME/Graph-Based-Movie-Recommender-CPP.git)
    ```
2.  **Open in Visual Studio:**
    * Open the `.sln` file OR import the source files.
3.  **Compile & Run:**
    * Ensure `movie_metadata.csv` is in the same directory as the executable.
    * Build and Run (Ctrl+F5).

## ?? Performance Analysis

| Operation | Data Structure | Time Complexity |
| :--- | :--- | :--- |
| **Search Title** | AVL Tree | **O(log N)** |
| **Search Actor** | Hash Table | **O(1) Avg** |
| **Recommendations** | Graph (BFS) | **O(V + E)** |
| **Insert Movie** | AVL + Hash | **O(log N)** |

## ????? Author

**Arbaz**
* *BS Artificial Intelligence @ FAST NUCES*
