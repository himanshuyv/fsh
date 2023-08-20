#ifndef __WARP_H
#define __WARP_H


/*
    Changes directory to that specified by path.
    Empty string path and "~" change directory to home
    "-" changes to previous directory
*/
int changeDirectory(char* path);
#define NO_SUCH_DIRECTORY 1
#define PERMISSION_DENIED 2


#endif