#include <unistd.h>
#include <string.h>
#include <iostream>

#include <filesystem>
#include <dirent.h>

#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

using namespace std;
#define fs std::filesystem

// Local forward decls
string findParm(string, int, char* []);

void listFilesInDir(const string&, const string&,
    const string&);

void filterAndPrint(const string&, const string&,
    const string&);

void printFileAttributes(__mode_t fileStatus, bool isLink);
void printFileOwner(struct stat* fileStatus);
void printFileSize(long int fileSize);
void printCreateModifiedDate(struct stat* fileStatus);
void printFileName(const string& fileName);
void printFileLink(const string& fileName);

string addLeadZeroToNN(const string& nn);
bool startsWith(const string&, const string&);
bool endsWith(const string&, const string&);


/*****************************************************************
 ** lookForLong -s string -t target -e endsWith
 **/
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
    fs::path targetPath;
    if (targetPathString.empty()) {
        targetPath = fs::canonical(fs::current_path());
    } else {
        // Ensure target exists, and isn't a directory.
        if (!fs::exists(targetPathString)) {
            cout << "Target doesn\'t exist, halting." << endl;
            return 0;
        }
        if (!fs::is_directory(fs::status(targetPathString))) {
            cout << "Target isn\'t a directory, halting." << endl;
            return 0;
        }
        targetPath = fs::canonical(targetPathString);
    }

    // Gopher it!
    listFilesInDir(targetPath, fileNameString, endsWithString);
}

/*****************************************************************
 ** Main loop, recurse through dirs, print matches.
 **/
void listFilesInDir(const string& targetPath,
    const string& fileNameString, const string& endsWithString) {

    DIR* dirFileHandle = opendir(targetPath.c_str());
    if (!dirFileHandle) {
        return;
    }

    struct dirent* dirFileEntry;
    while ((dirFileEntry = readdir(dirFileHandle)) != NULL) {
        string newTargetPath = dirFileEntry->d_name;

        // Ignore "movement" entries.
        if (endsWith(newTargetPath, "..") ||
            endsWith(newTargetPath, ".")) {
            continue;
        }

        // Get full file name, adjust for root.
        newTargetPath = (targetPath == "/") ?
            targetPath + newTargetPath :
            targetPath + "/" + newTargetPath;

        // When user scans root, avoid user home land, & media.
        // ... Requires user to search seperately.
        if (targetPath == "/" &&
            (startsWith(newTargetPath, "/home") ||
             startsWith(newTargetPath, "/media"))) {
            continue;
        }

        // Process entry.
        filterAndPrint(newTargetPath, fileNameString,
            endsWithString);

        // Recurse down if we're a directory.
        if (dirFileEntry->d_type == DT_DIR) {
            listFilesInDir(newTargetPath, fileNameString,
                endsWithString);
        }
    }

    closedir(dirFileHandle);
}

/*****************************************************************
 ** Helper method for each dir entry.
 **/
void filterAndPrint(const string& targetPath,
    const string& fileNameString, const string& endsWithString) {

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

    // Retrieve any link file info.
    const bool TARGET_IS_LINKFILE =
        fs::is_symlink(targetPath);

    struct stat fileStatus = {0};
    struct stat fileLinkStatus = {0};

    // Retrieve file info.
    try {
        stat(targetPath.c_str(), &fileStatus);
    } catch (fs::filesystem_error &e) { }

    if (TARGET_IS_LINKFILE) {
        try {
            lstat(targetPath.c_str(), &fileLinkStatus);
        } catch (fs::filesystem_error &e) { }
    }

    // Print file attribute fields.
    printFileAttributes(TARGET_IS_LINKFILE ?
        fileLinkStatus.st_mode : fileStatus.st_mode,
        TARGET_IS_LINKFILE);
    printFileOwner(&fileStatus);

    // Print file size field.
    printFileSize(TARGET_IS_LINKFILE ?
        fileLinkStatus.st_size : fileStatus.st_size);

    // Print rest.
    printCreateModifiedDate(TARGET_IS_LINKFILE ?
        &fileLinkStatus : &fileStatus);

    printFileName(targetPath);
    printFileLink(targetPath);

    // And done.
    printf("\n");
}

/*****************************************************************
 ** Helper method for file attributes.
 **/
void printFileAttributes(__mode_t fileAttributes, bool isLink) {
    printf(isLink ? "l" :
        S_ISDIR(fileAttributes) ? "d" :
            "-");

    printf((fileAttributes & S_IRUSR) ? "r" : "-");
    printf((fileAttributes & S_IWUSR) ? "w" : "-");
    printf((fileAttributes & S_IXUSR) ? "x" : "-");

    printf((fileAttributes & S_IRGRP) ? "r" : "-");
    printf((fileAttributes & S_IWGRP) ? "w" : "-");
    printf((fileAttributes & S_IXGRP) ? "x" : "-");

    printf((fileAttributes & S_IROTH) ? "r" : "-");
    printf((fileAttributes & S_IWOTH) ? "w" : "-");
    printf((fileAttributes & S_IXOTH) ? "x" : "-");

    printf("  ");
}

/*****************************************************************
 ** Helper method for file owner.
 **/
void printFileOwner(struct stat* fileStatus) {
    const struct passwd *pw = getpwuid(fileStatus->st_uid);

    const string fileOwner = (pw != 0) ? pw->pw_name : "";
    printf("%-8.8s  ", fileOwner.c_str());
}

/*****************************************************************
 ** Helper method for file size.
 **/
void printFileSize(long int fileSize) {
    printf("%10li  ", fileSize);
}

/*****************************************************************
 ** Helper method for file access date.
 **/
void printCreateModifiedDate(struct stat* fileStatus) {
    const static string MONTH_NAMES[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

    const std::time_t accessTime = fileStatus->st_mtime;
    const std::tm* localAccessTime =
        std::localtime(&accessTime);

    printf("%s %s %s %s:%s  ",
        MONTH_NAMES[localAccessTime->tm_mon].c_str(),
        addLeadZeroToNN(std::to_string(
            localAccessTime->tm_mday)).c_str(),
        std::to_string(localAccessTime->tm_year + 1900).c_str(),
        addLeadZeroToNN(std::to_string(
            localAccessTime->tm_hour)).c_str(),
        addLeadZeroToNN(std::to_string(
            localAccessTime->tm_min)).c_str());
}

/*****************************************************************
 ** Helper method for file name.
 **/
void printFileName(const string& fileName) {
    printf("%s", fileName.c_str());
}

/*****************************************************************
 ** Helper method for file link name.
 **/
void printFileLink(const string& fileName) {
    try {
        const fs::path filePath(fileName);
        if (!fs::is_symlink(fileName)) {
            return;
        }
        cout << " -> " << fs::read_symlink(fileName);
    } catch (fs::filesystem_error &e) { }
}

/*****************************************************************
 ** General method helper.
 **/
string addLeadZeroToNN(const string& nn) {
    return (nn.length() < 2) ? "0" + nn : nn;
}

/*****************************************************************
 ** General method helper.
 **/
bool startsWith(const string& str, const string& prefix) {
    if (prefix.size() > str.size()) {
        return false;
    }

    return equal(str.begin(), str.begin() + prefix.size(),
        prefix.begin());
}

/*****************************************************************
 ** General method helper.
 **/
bool endsWith(const string& str, const string& postfix) {
    if (postfix.size() > str.size()) {
        return false;
    }

    return equal(str.begin() + str.size() - postfix.size(),
        str.end(), postfix.begin());
}
