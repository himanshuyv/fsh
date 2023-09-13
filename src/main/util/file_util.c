#include "../../header/headers.h"

void replaceHomeDirPrefix(char* path) {
    if (!isHomeDirPrefix(path)) return;
    int pathLength = strlen(path);
    int homeDirectoryLength = strlen(homeDirectory);
    for (int i = 1; i - 1 + homeDirectoryLength <= pathLength; i++) {
        path[i] = path[i - 1 + homeDirectoryLength];
    }

    path[0] = '~';
    path[pathLength - homeDirectoryLength + 1] = '\0';
}

bool isHomeDirPrefix(char* path) {
    int homeDirectoryLength = strlen(homeDirectory);
    int pathLength = strlen(path);
    if (pathLength < homeDirectoryLength) return false;
    for (int i = 0; i < homeDirectoryLength; i++) {
        if (homeDirectory[i] != path[i]) return false;
    }

    return true;
}

void replaceTildaWithHome(char* path) {
    if (path[0] != '~') return;
    char temp[DIRECTORY_BUFFER_SIZE] = {'\0'};
    strcpy(temp, homeDirectory);
    strcat(temp, path + 1);
    strcpy(path, temp);
}

void getAbsolutePath(char* prefix, char* fileName, char* buffer) {
    // printf("prefix: %s fileName: %s\n", prefix, fileName);
    strcpy(buffer, prefix);
    strcat(buffer, fileName);
}

int getStat(char* prefix, char* fileName, struct stat* fileStat) {
    char absPath[DIRECTORY_BUFFER_SIZE];
    getAbsolutePath(prefix, fileName, absPath);
    if (stat(absPath, fileStat) == -1) {
        errorPrintf("Error calling stat on %s\n", absPath);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void printFileName(struct dirent* file, struct stat fileStat) {
    switch (file->d_type) {
        case DT_REG:
            char* color = FILE_COLOR;
            if (fileStat.st_mode & S_IXUSR) color = EXECUTABLE_COLOR;
            colorPrintf(color, "%s ", file->d_name);
            break;
        case DT_DIR:
            colorPrintf(DIRECTORY_COLOR, "%s ", file->d_name);
            break;
        default:
            colorPrintf(COLOR_RED, "%s* ", file->d_name);
            break;
    }
}

void freeDirent(struct dirent** dir, int fileCount) {
    for (int i = 0; i < fileCount; i++) free(dir[i]);
    free(dir);
}