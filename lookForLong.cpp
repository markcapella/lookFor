#include <dirent.h>
#include <filesystem>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
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
// *** lookForLong -s string -t target -e endsWith               ***
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

        // Feature: When user scans root, avoid user home land, & media
        //          Requires user to search seperately.
        if (targetPath == "/" &&
            (startsWith(newTargetPath, "/home") ||
             startsWith(newTargetPath, "/media"))) {
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
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
    // Shell and query ls ... , collect stdout result string.
    string resultString;

    const string SYSTEM_STRING = "ls -alFd \"" + targetPath + "\"";
    if (FILE* stdoutPipe = popen(SYSTEM_STRING.c_str(), "r")) {
        const int STDOUT_STRING_MAX = 8192;

        array<char, STDOUT_STRING_MAX> buffStrings;
        while (fgets(buffStrings.data(), STDOUT_STRING_MAX, stdoutPipe) != NULL) {
            resultString += buffStrings.data();
        }
        pclose(stdoutPipe);
    }

    // Find all column seperators (' ') in ls result String to
    // enhance the output. File size field needs right align or output
    // is annoyingly ragged.
    const int RESULT_COLS_MAX = 5;
    int resultColIndex[RESULT_COLS_MAX];

    resultColIndex[0] = resultString.find(" ");
    for (int i = 1; i < RESULT_COLS_MAX; i++) {
        resultColIndex[i] = resultString.find(" ", resultColIndex[i - 1] + 1);
    }

    // Right alight the file size column.
    const int GROUP_COLUMN_INDEX = 3;
    const int SIZE_COLUMN_INDEX = 4;

    const int SIZE_COLUMN_PAD_TO = 10;
    resultString.insert(resultColIndex[GROUP_COLUMN_INDEX], SIZE_COLUMN_PAD_TO -
        (resultColIndex[SIZE_COLUMN_INDEX] - resultColIndex[GROUP_COLUMN_INDEX]) + 1, ' ');

    // Endl provided in stdout results.
    cout << resultString;
}
#pragma GCC diagnostic pop

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
