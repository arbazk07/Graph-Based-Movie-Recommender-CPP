#pragma once
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

// ANSI COLORS
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

namespace Utils {
    // Clean string (keep alphanumeric + spaces)
    inline string cleanString(string text) {
        string output = "";
        for (char c : text) {
            // FIX: Cast to (unsigned char) to prevent crash on special symbols
            if (isalnum(static_cast<unsigned char>(c)) || c == ' ' || c == ':' || c == '-') {
                output.push_back(c);
            }
        }
        return output;
    }

    // Manual String to Int
    inline int stringToInt(string text) {
        int result = 0;
        for (char c : text) {
            if (c >= '0' && c <= '9') {
                result = result * 10 + (c - '0');
            }
        }
        return result;
    }

    // Manual String to Double
    inline double stringToDouble(string text) {
        double result = 0.0;
        int i = 0;
        while (i < text.length() && text[i] != '.') {
            if (text[i] >= '0' && text[i] <= '9') {
                result = result * 10 + (text[i] - '0');
            }
            i++;
        }
        if (i < text.length() && text[i] == '.') {
            i++;
            double factor = 0.1;
            while (i < text.length()) {
                if (text[i] >= '0' && text[i] <= '9') {
                    result = result + (text[i] - '0') * factor;
                    factor *= 0.1;
                }
                i++;
            }
        }
        return result;
    }

    // Timer Class for Benchmarking
    class Timer {
        chrono::high_resolution_clock::time_point start;
    public:
        Timer() { start = chrono::high_resolution_clock::now(); }
        void printDuration() {
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            cout << CYAN << " [Time: " << duration.count() / 1000.0 << " ms]" << RESET << endl;
        }
    };
}