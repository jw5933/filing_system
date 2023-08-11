#include <sys/stat.h>
#include <iostream>
#include <dirent.h>
#include <cstdio>
#include <CoreServices/CoreServices.h>
#include "Magic.h"

using namespace std;

int main() {
    magic_t magic = magic_open(MAGIC_MIME_TYPE);
    if (magic == nullptr) {
        cout << "Magic could not be initialized" << endl;
        return 1;
    }

    if (magic_load(magic, nullptr)){
        cout << "Magic database could not be loaded" << endl;
        return 1;
    }

    struct dirent *directoryEntry;
    struct stat status; //helps determine if its a file or folder

    DIR *directory;
    std::string originPath = "/users/jade/desktop/"; //where the program is executing
    directory = opendir(originPath.c_str());

    if (directory != nullptr) {
        for (directoryEntry = readdir(directory); directoryEntry != nullptr; directoryEntry = readdir(directory)) { //check all items in directory
            std::string entryName = directoryEntry->d_name;
            if (entryName[0] == '.') continue;
            string entryPath = originPath + entryName;
            string type;

            if (stat(entryPath.c_str(), &status) == 0) { //check if file or directory; stat returns 0 on success and -1 on failure
                if (status.st_mode & S_IFDIR) { //checks if its a directory
                    type = "is a folder.";
                }
                else if (S_ISREG(status.st_mode)) {
                    type = "is a file.";

                    const char* mimeType = magic_file(magic, entryPath.c_str());
                    if (mimeType == nullptr){
                        cout << "magic could not find mime type of file " << entryName << endl;
                    }
                    else {
                        cout << entryName << " has mime type: " << mimeType << endl;
                    }
//                    continue;
                    std::string extension = "image/png";
                    if (mimeType != extension) continue;

                    CFURLRef fileRef = CFURLCreateWithFileSystemPath(NULL,
                                                              CFStringCreateWithCString(nullptr, entryPath.c_str(), NULL),
                                                              CFURLPathStyle::kCFURLPOSIXPathStyle,
                                                              false);
                    CFURLRef * outRef;
                    OSStatus osStatus = LSOpenCFURLRef(fileRef, nullptr);
                    cout << "OS status " << osStatus << endl;

                    //error handling TODO
//                    if (osStatus != noErr) {}
//                    if (fileRef != nullptr) {}


                    //The CFURLCreateWithString function returns a CFURLRef that
                    // needs to be manage and release using CFRelease, not outRef
                    CFRelease(fileRef);
                }
                else type = "is not a folder or file.";
            }
            std::cout << directoryEntry->d_name << " " << type << std::endl;
        }
        closedir(directory);
        magic_close(magic);
    }
    else {
        std::cout << "Cannot open originPath " << originPath << std::endl;
    }
}

////getting browser url
//CFURLRef ref = CFURLCreateWithString(nullptr,
//                                     CFStringCreateWithCString(nullptr, "https://github.com/ThibsG/file_typing", NULL),
//                                     nullptr);
//CFStringRef urlString = CFURLGetString(ref);
//cout << CFStringGetCStringPtr(urlString, kCFStringEncodingUTF8) << endl;
