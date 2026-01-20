#include <iostream>
#include <filesystem> // Requires C++17 standard
#include <fstream>    // For file I/O
#include <string>

// 'fs' is a common alias for std::filesystem
namespace fs = std::filesystem;
using namespace std;

int main() {
    cout << "=== C++17 Filesystem Demo ===" << endl;

    // --- 1. Path Manipulation ---
    // The '/' operator is overloaded to append paths correctly on any OS (Windows/Linux)
    fs::path rootDir = "DemoFolder";
    fs::path filePath = rootDir / "info.txt"; 

    cout << "\n1. Path Operations:" << endl;
    cout << "   Target Path: " << filePath << endl;
    cout << "   Filename: " << filePath.filename() << endl;
    cout << "   Extension: " << filePath.extension() << endl;

    // --- 2. Create Directory ---
    cout << "\n2. Environment Setup:" << endl;
    
    // Clean up previous run if it exists
    if (fs::exists(rootDir)) {
        cout << "   Removing existing '" << rootDir << "' directory..." << endl;
        fs::remove_all(rootDir); // Recursive delete
    }

    // Create new directory
    if (fs::create_directory(rootDir)) {
        cout << "   Created directory: " << rootDir << endl;
    }

    // --- 3. Create and Write to File ---
    cout << "   Creating file: " << filePath << endl;
    
    // Use standard fstream to create content
    ofstream file(filePath);
    if (file.is_open()) {
        file << "Hello, C++ Filesystem!" << endl;
        file << "This file was created programmatically." << endl;
        file.close();
    }

    // --- 4. File Information & Copying ---
    cout << "\n3. File Operations:" << endl;
    
    if (fs::exists(filePath)) {
        // Get file size
        uintmax_t size = fs::file_size(filePath);
        cout << "   File Size: " << size << " bytes" << endl;

        // Copy file
        fs::path copyPath = rootDir / "backup.txt";
        fs::copy_file(filePath, copyPath);
        cout << "   Copied 'info.txt' to 'backup.txt'" << endl;
    }

    // --- 5. Iterate Directory Contents ---
    cout << "\n4. Listing Directory Contents:" << endl;
    
    // fs::directory_iterator allows looping through files in a folder
    if (fs::exists(rootDir) && fs::is_directory(rootDir)) {
        for (const auto& entry : fs::directory_iterator(rootDir)) {
            cout << "   Found: " << entry.path().filename();
            if (entry.is_regular_file()) cout << " [FILE]";
            if (entry.is_directory()) cout << " [DIR]";
            cout << endl;
        }
    }

    // --- 6. Renaming ---
    cout << "\n5. Renaming:" << endl;
    fs::path newName = rootDir / "renamed_data.txt";
    fs::rename(filePath, newName); // Moves/Renames info.txt to renamed_data.txt
    cout << "   Renamed 'info.txt' to 'renamed_data.txt'" << endl;

    cout << "\n(Demo complete. Check the '" << rootDir << "' folder in your directory)" << endl;

    return 0;
}
