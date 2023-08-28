#include <filesystem>
#include <iostream>
#include <dirent.h>
#include <string>
#include <unistd.h>


using namespace std;
using STRING = std::string;

namespace FS = std::filesystem;


// Local forward decls
STRING findParm(STRING, int, char* []);

void listFilesInDir(const STRING&, const STRING&, const STRING&);

void filterAndPrint(const STRING&, const STRING&, const STRING&);

static bool startsWith(const STRING&, const STRING&);
static bool endsWith(const STRING&, const STRING&);


// *****************************************************************
// *** lookForLong -s string -t target -e endsWith               ***
// *****************************************************************

int main(int argc, char* argv[]) {

    // String, and guard the inputs.
    STRING fileNameString = "";
    STRING targetPathString = ".";
    STRING endsWithString = "";

    if (argc > 1) {
        fileNameString = std::string(argv[1]);
        if (argc > 2) {
            targetPathString = std::string(argv[2]);
            if (argc > 3) {
                endsWithString = std::string(argv[3]);
            }
        }
    }

    // Default to working directory.
    FS::path targetPath;
    if (targetPathString.empty()) {
        targetPath = FS::canonical(FS::current_path());
    } else {
        // Ensure target exists, and isn't a directory.
        if (!FS::exists(targetPathString)) {
            cout << "Target doesn\'t exist, halting." << endl;
            return 0;
        }
        if (!FS::is_directory(FS::status(targetPathString))) {
            cout << "Target isn\'t a directory, halting." << endl;
            return 0;
        }
        targetPath = FS::canonical(targetPathString);
    }

    // Gopher it!
    listFilesInDir(targetPath, fileNameString, endsWithString);
}

// *****************************************************************
// *** Main loop, recurse through dirs, print matches            ***
// *****************************************************************

void listFilesInDir(const STRING& targetPath, const STRING& fileNameString,
                    const STRING& endsWithString) {

    DIR* dirFileHandle = opendir(targetPath.c_str());
    if (!dirFileHandle) {
        // cout << "listFilesInDir() targetPath: " << targetPath <<
        //           " not read due to error: " << errno << endl;
        return;
    }

    struct dirent* dirFileEntry;
    while ((dirFileEntry = readdir(dirFileHandle)) != NULL) {
        STRING newTargetPath = dirFileEntry->d_name;
        // cout << "listFilesInDir() reads : " << newTargetPath << " size: " <<
        //     newTargetPath.length() << endl;

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

        // Process first entry.
        filterAndPrint(newTargetPath, fileNameString, endsWithString);

        // If dir/folder, drop in and process all, else we're just done.
        if (dirFileEntry->d_type == DT_DIR) {
            listFilesInDir(newTargetPath, fileNameString, endsWithString);
        }
    }

    closedir(dirFileHandle);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
// *****************************************************************
// *** Helper method for each dir entry                          ***
// *****************************************************************

void filterAndPrint(const STRING& targetPath, const STRING& fileNameString,
                    const STRING& endsWithString) {

    // Ensure fileNameString match.
    if (!fileNameString.empty()) {
        if (targetPath.find(fileNameString) == STRING::npos) {
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
    // cout << targetPath << endl;
    STRING systemString = "ls -alFd " + targetPath;
    system(systemString.c_str());
}
#pragma GCC diagnostic pop

// *****************************************************************
// *** Helper method                                             ***
// *****************************************************************

static bool
startsWith(const STRING& str, const STRING& prefix) {
    if (prefix.size() > str.size()) {
        return false;
    }

    return std::equal(str.begin(), str.begin() + prefix.size(), prefix.begin());
}

// *****************************************************************
// *** Helper method                                             ***
// *****************************************************************

static bool
endsWith(const STRING& str, const STRING& postfix) {
    if (postfix.size() > str.size()) {
        return false;
    }

    return std::equal(str.begin() + str.size() - postfix.size(), str.end(), postfix.begin());
}
