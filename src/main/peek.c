#include "../header/headers.h"

/*

green for executables, white for files and blue for directories

*/

void printFileName(struct dirent* file, struct stat fileStat) {
    switch (file->d_type) {
        case DT_REG:
            char* color = COLOR_WHITE;
            if (fileStat.st_mode & S_IXUSR) color = COLOR_GREEN;
            colorPrintf(color, "%s ", file->d_name);
            break;
        case DT_DIR:
            colorPrintf(COLOR_BLUE, "%s ", file->d_name);
            break;
        default:
            colorPrintf(COLOR_RED, "%s* ", file->d_name);
            break;
    }
}

int peekHelperShort(char* prefix, struct dirent** dir, int fileCount, bool allFlag) {
    for (int i = 0; i < fileCount; i++) {
        // printf("here\n");
        struct dirent* file = dir[i];
        struct stat fileStat;
        if (!allFlag && file->d_name[0] == '.') continue;
        char absFile[DIRECTORY_BUFFER_SIZE];
        strcpy(absFile, prefix);
        strcat(absFile, file->d_name);
        int statErrorCode = stat(absFile, &fileStat);
        if (statErrorCode == -1)
            fprintf(stderr, "[ERROR]: Error calling stat on %s\n",
                    file->d_name);
        printFileName(file, fileStat);
    }
    printf("\n");

    for (int i = 0; i < fileCount; i++) {
        free(dir[i]);
    }
    free(dir);

    return EXEC_SUCCESS;
}

int makeFileLine(char* prefix, struct dirent* file) {
    struct stat fileStat;
    char absPath[DIRECTORY_BUFFER_SIZE];
    strcpy(absPath, prefix);
    strcat(absPath, file->d_name);
    int statErrorCode = stat(absPath, &fileStat);
    if (statErrorCode == -1) {
        fprintf(stderr, "[ERROR]: Error calling stat on %s\n", file->d_name);
        return EXEC_FAILURE;
    }
    // File type
    char fileTypeChar = '-';
    switch (fileStat.st_mode & S_IFMT) {
        case S_IFDIR: fileTypeChar = 'd'; break;
        case S_IFCHR: fileTypeChar = 'c'; break;
        case S_IFLNK: fileTypeChar = 'l'; break;
    }

    // Permissions
    char permissionString[10];
    permissionString[9] = '\0';
    for (int i = 0; i < 9; i++) permissionString[i] = '-';
    int permissions[9] = {
        S_IRUSR, // User read
        S_IWUSR, // User write
        S_IXUSR, // User execute
        S_IRGRP, // Group read
        S_IWGRP, // Group write 
        S_IXGRP, // Group execute
        S_IROTH, // Everyone Else read
        S_IWOTH, // Everyone Else write
        S_IXOTH  // Everyone Else execute
    };

    for (int categ = 0; categ < 3; categ++) {
        int readIdx = 3 * categ;
        int writeIdx = 3 * categ + 1;
        int executeIdx = 3 * categ + 2;

        if (fileStat.st_mode & permissions[readIdx]) permissionString[readIdx] = 'r';
        if (fileStat.st_mode & permissions[writeIdx]) permissionString[writeIdx] = 'w';
        if (fileStat.st_mode & permissions[executeIdx]) permissionString[executeIdx] = 'x';
    }

    // Number of linked hard links
    nlink_t numberOfLinks = fileStat.st_nlink;

    // Owner
    struct passwd* pw = getpwuid(fileStat.st_uid);
    if (pw == NULL) {
        fprintf(stderr, "[ERROR]: Error in calling getpwuid() for user id \'%u\', errno = %d", fileStat.st_uid, errno);
        return EXEC_FAILURE;
    }
    char* ownerName = pw->pw_name;

    // Group
    struct group* gr = getgrgid(fileStat.st_gid);
    if (gr == NULL) {
        fprintf(stderr, "[ERROR]: Error in calling getgrgid() for group id \'%u\', errno = %d", fileStat.st_gid, errno);
        return EXEC_FAILURE;
    }
    char* groupName = gr->gr_name;

    // size
    off_t fileSize = fileStat.st_size;

    // Date
    char* fileModifiedTime = ctime(&fileStat.st_mtime);
    int fileModifiedTimeLen = strlen(fileModifiedTime);
    fileModifiedTime[fileModifiedTimeLen - 1] = '\0'; 

    printf("%c%s %lu %s %s %ld %s ", fileTypeChar, permissionString, numberOfLinks, ownerName, groupName, fileSize, fileModifiedTime);
    printFileName(file, fileStat);
    printf("\n");
    return EXEC_SUCCESS;
}

int peekHelperLong(char* prefix, struct dirent** dir, int fileCount, bool allFlag) {
    int exitCode = 0;
    for (int i = 0; i < fileCount; i++) {
        struct dirent* file = dir[i];
        if (!allFlag && file->d_name[0] == '.') continue;
        exitCode |= makeFileLine(prefix, file);
    }

    for (int i = 0; i < fileCount; i++) {
        free(dir[i]);
    }
    free(dir);

    return exitCode;
}

int setFlags(Command* command, bool* allFlagPtr, bool* longFlagPtr, char* dirPath) {
    strcpy(dirPath, ".");
    if (command->argc == 1) {
        *allFlagPtr = false;
        *longFlagPtr = false;
        return EXEC_SUCCESS;
    }

    int aCt = 0, lCt = 0;
    for (int i = 1; i < command->argc; i++) {
        if (command->argv[i][0] != '-') strcpy(dirPath, command->argv[i]);
        else {
            for (int j = 1; j < strlen(command->argv[i]); j++) {
                if (command->argv[i][j] != 'a' && command->argv[i][j] != 'l') {
                    fprintf(stderr, "[ERROR]: Unknown option for peek \'%c\'\n", command->argv[i][j]);
                    return EXEC_FAILURE;
                }

                if (command->argv[i][j] == 'a') aCt++;
                else lCt++;
            }
        }
    }

    *allFlagPtr = aCt > 0;
    *longFlagPtr = lCt > 0;
    return EXEC_SUCCESS;
}

int peek(Command* command) {
    int exitCode = 0;
    char dirPath[DIRECTORY_BUFFER_SIZE];
    bool allFlag = 0, longFlag = 0;

    if (setFlags(command, &allFlag, &longFlag, dirPath))
        return EXEC_FAILURE;

    replaceTildaWithHome(dirPath);
    strcat(dirPath, "/");
    struct dirent** dir;
    int fileCount = scandir(dirPath, &dir, NULL, alphasort);
    if (fileCount < 0) {
        fprintf(stderr, "[ERROR]: Cannot scandir \'%s\'\n", dirPath);
        return EXEC_FAILURE;
    }

    if (longFlag) exitCode = peekHelperLong(dirPath, dir, fileCount, allFlag);
    else exitCode = peekHelperShort(dirPath, dir, fileCount, allFlag);
    return exitCode;
}