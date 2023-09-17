#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace std;  
#define FILE_SYS std::filesystem

// Local forward decls
string findParm(string, int, char* []);

void listFilesInDir(const string&, const string&, const string&);

void filterAndPrint(const string&, const string&, const string&);

static bool startsWith(const string&, const string&);
static bool endsWith(const string&, const string&);


// *****************************************************************
// *** lookFor -s string -t target -e endsWith                   ***
// *****************************************************************

int main(int argc, char* argv[]) {

    // String, and guard the inputs.
    string fileNameString = "";
    string targetPathString = ".";
    string endsWithString = "";

    if (argc > 1) {
        fileNameString = string(argv[1]);
        if (argc > 2) {
            targetPathString = string(argv[2]);
            if (argc > 3) {
                endsWithString = string(argv[3]);
            }
        }
    }

    // Default to working directory.
    FILE_SYS::path targetPath;
    if (targetPathString.empty()) {
        targetPath = FILE_SYS::canonical(FILE_SYS::current_path());
    } else {
        // Ensure target exists, and isn't a directory.
        if (!FILE_SYS::exists(targetPathString)) {
            cout << "Target doesn\'t exist, halting." << endl;
            return 0;
        }
        if (!FILE_SYS::is_directory(FILE_SYS::status(targetPathString))) {
            cout << "Target isn\'t a directory, halting." << endl;
            return 0;
        }
        targetPath = FILE_SYS::canonical(targetPathString);
    }

    // Gopher it!
    listFilesInDir(targetPath, fileNameString, endsWithString);
}

// *****************************************************************
// *** Main loop, recurse through dirs, print matches            ***
// *****************************************************************

void listFilesInDir(const string& targetPath, const string& fileNameString,
                    const string& endsWithString) {

    DIR* dirFileHandle = opendir(targetPath.c_str());
    if (!dirFileHandle) {
        return;
    }

    struct dirent* dirFileEntry;
    while ((dirFileEntry = readdir(dirFileHandle)) != NULL) {
        string newTargetPath = dirFileEntry->d_name;

        // Ignore "movement" entries.
        if (endsWith(newTargetPath, "..") || endsWith(newTargetPath, ".")) {
            continue;
        }

        // Get full file name, adjust for root.
        newTargetPath = (targetPath == "/") ?
            targetPath + newTargetPath : targetPath + "/" + newTargetPath;

        // Feature: When user scans root, avoid user home land,
        //          Requires user to search seperately.
        if (targetPath == "/" && startsWith(newTargetPath, "/home")) {
            continue;
        }

        // Process entry.
        filterAndPrint(newTargetPath, fileNameString, endsWithString);

        // If dir/folder, drop in and process all, else we're just done.
        if (dirFileEntry->d_type == DT_DIR) {
            listFilesInDir(newTargetPath, fileNameString, endsWithString);
        }
    }

    closedir(dirFileHandle);
}

// *****************************************************************
// *** Helper method for each dir entry                          ***
// *****************************************************************

void filterAndPrint(const string& targetPath, const string& fileNameString,
                    const string& endsWithString) {

    // Ensure fileNameString match.
    if (!fileNameString.empty()) {
        if (targetPath.find(fileNameString) == string::npos) {
            return;
        }
    }

    // Ensure extension match.
    if (!endsWithString.empty()) {
        if (!endsWith(targetPath, endsWithString)) {
            return;
        }
    }

    // Default, all matchs, so print!
    cout << targetPath << endl;
}

// *****************************************************************
// *** Helper method                                             ***
// *****************************************************************

static bool
startsWith(const string& str, const string& prefix) {
    if (prefix.size() > str.size()) {
        return false;
    }

    return equal(str.begin(), str.begin() + prefix.size(), prefix.begin());
}

// *****************************************************************
// *** Helper method                                             ***
// *****************************************************************

static bool
endsWith(const string& str, const string& postfix) {
    if (postfix.size() > str.size()) {
        return false;
    }

    return equal(str.begin() + str.size() - postfix.size(), str.end(), postfix.begin());
}
