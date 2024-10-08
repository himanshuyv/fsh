#include "../../header/headers.h"

int matchedFileCount = 0;
char matchedFilePath[DIRECTORY_BUFFER_SIZE];
int search(char* directory, char* target, bool dirOnlyFlag, bool fileOnlyFlag, bool oneFlag, int initialPathLength) {
    int exitCode = 0;
    struct dirent** dir;
    int fileCount = scandir(directory, &dir, NULL, NULL);
    if (directory[strlen(directory) - 1] != '/')
        strcat(directory, "/");
    for (int i = 0; i < fileCount; i++) {
        struct dirent* file = dir[i];
        int fileNameLength = strlen(file->d_name);
        if (fileNameLength == 0 || file->d_name[0] == '.')
            continue;

        char absPath[DIRECTORY_BUFFER_SIZE];
        getAbsolutePath(directory, file->d_name, absPath);
        // exitCode = getStat(directory, file->d_name, &fileStat);
        
        int directoryLength = strlen(directory);
        if (directory[directoryLength - 1] != '/') {
            strcat(directory, "/");
            directoryLength++;
        }
        strcat(directory, file->d_name);
        if (!fileOnlyFlag && (file->d_type == DT_DIR)) {
            if (strstr(file->d_name, target)) {
                colorPrintf(DIRECTORY_COLOR, "%s\n", directory + initialPathLength);
                matchedFileCount++;
                strcpy(matchedFilePath, absPath);
            }
            search(directory, target, dirOnlyFlag, fileOnlyFlag, oneFlag, initialPathLength);
        } else if (!dirOnlyFlag) {
            if (strstr(file->d_name, target)) {
                colorPrintf(EXECUTABLE_COLOR, "%s\n", directory + initialPathLength);
                matchedFileCount++;
                strcpy(matchedFilePath, absPath);
            }
        }
        directory[directoryLength] = '\0';
    }

    if (fileCount >= 0) freeDirent(dir, fileCount);
    return exitCode;
}

int setSeekFlags(Subcommand command, char* target, char* directory, bool* dirOnlyFlag, bool* fileOnlyFlag, bool* oneFlag) {
    if (command->argc == 1) return EXEC_FAILURE;
    strcpy(directory, ".");
    int nonFlagArguments = 0;
    for (int i = 1; i < command->argc; i++) {
        if (command->argv[i][0] == '-') {
            for (int j = 1; j < strlen(command->argv[i]); j++) {
                switch (command->argv[i][j]) {
                    case 'd': *dirOnlyFlag = true; break;
                    case 'e': *oneFlag = true; break;
                    case 'f': *fileOnlyFlag = true; break;
                    default:
                        errorPrintf("Unknown option for seek \'%c\'\n", command->argv[i][j]);
                        return EXEC_FAILURE;
                }
            }
        } else {
            nonFlagArguments++;
            if (nonFlagArguments > 2) {
                errorPrintf("Too many arguments for seek\n");
                return EXIT_FAILURE;
            }

            if (nonFlagArguments == 1) {
                strcpy(target, command->argv[i]);
            } else if (nonFlagArguments == 2) {
                strcpy(directory, command->argv[i]);
            }
        }
    }

    if (nonFlagArguments == 0) {
        errorPrintf("No target found to search\n");
        return EXEC_FAILURE;
    }

    if (*dirOnlyFlag && *fileOnlyFlag) {
        errorPrintf("Command seek recieved both -d and -f flags\n");
        return EXEC_FAILURE;
    }

    return EXEC_SUCCESS;
}

int handleOneFlag() {
    int exitCode = 0;
    struct stat fileStat;
    if (stat(matchedFilePath, &fileStat) == -1) {
        errorPrintf("Error calling stat on \'%s\'\n", matchedFilePath);
        return EXEC_FAILURE;
    } else {
        if ((fileStat.st_mode & S_IFMT) == S_IFDIR) {
            if (fileStat.st_mode & S_IXUSR) {
                if (chdir(matchedFilePath)) {
                    errorPrintf("Cannot chdir into \'%s\'\n", matchedFilePath);
                    return EXEC_FAILURE;
                }
            } else {
                errorPrintf("No execute permission for \'%s\'\n", matchedFilePath);
                return EXEC_FAILURE;
            }
        } else {
            if (fileStat.st_mode & S_IRUSR) {
                FILE* matchedFile = fopen(matchedFilePath, "r");
                if (matchedFile == NULL) {
                    errorPrintf("Cannot chdir into \'%s\'\n", matchedFilePath);
                    return EXEC_FAILURE;
                }

                char c;
                while ((c = getc(matchedFile)) != EOF) {
                    printf("%c", c);
                }
                fclose(matchedFile);
            } else {
                errorPrintf("No read permission for \'%s\'\n", matchedFilePath);
                return EXEC_FAILURE;
            }
        }
    }

    return exitCode;
}

int seek(Subcommand command) {
    int exitCode = 0;
    char dirPath[DIRECTORY_BUFFER_SIZE];
    char target[DIRECTORY_BUFFER_SIZE];
    bool dirOnlyFlag = false, fileOnlyFlag = false, oneFlag = false;

    if (command->argc == 1) {
        errorPrintf("Too few arguments for seek\n");
        return EXEC_FAILURE;
    }

    if (setSeekFlags(command, target, dirPath, &dirOnlyFlag, &fileOnlyFlag, &oneFlag))
        return EXEC_FAILURE;

    replaceTildaWithHome(dirPath);
    if (dirPath[strlen(dirPath) - 1] != '/')
        strcat(dirPath, "/");
    matchedFileCount = 0;
    exitCode = search(dirPath, target, dirOnlyFlag, fileOnlyFlag, oneFlag, strlen(dirPath));
    if (matchedFileCount == 0)
        printf("No match found!\n");
    else {
        if (oneFlag && matchedFileCount == 1)
            handleOneFlag();
    }
    return exitCode;
}