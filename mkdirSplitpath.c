#include "types.h"
#include <stdlib.h>
#include <string.h>
extern struct NODE* root;
extern struct NODE* cwd;
const char deliminator[] = "/";

//make directory
void mkdir(char pathName[]){
    if (strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char baseName[64];
    char dirName[64];

    struct NODE* parent = splitPath(pathName, baseName, dirName);
    if (parent == NULL) {
        return;
    }

    struct NODE* curr = parent->childPtr;
    while (curr != NULL) {
        if (strcmp(curr->name, baseName) == 0) {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        curr = curr->siblingPtr;
    }

    touch(pathName);

    curr = parent->childPtr;
    while (curr != NULL) {
        if (strcmp(curr->name, baseName) == 0) {
            curr->fileType = 'D'; 
            break;
        }
        curr = curr->siblingPtr;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathNameB, char* baseName, char* dirName){
    if (pathNameB == NULL || strlen(pathNameB) == 0) {
        return NULL;
    }

    char pathName[128];
    strcpy(pathName, pathNameB);

    int tokenListSize = 2;
    int tokenCount = 0;

    char** tokenList = malloc(tokenListSize * sizeof(char*));

    char *token = strtok(pathName, deliminator);
    while (token != NULL) {

        if (tokenCount >= tokenListSize) {
            tokenListSize *= 2;
            tokenList = realloc(tokenList, tokenListSize * sizeof(char*));
        }

        tokenList[tokenCount] = token;
        tokenCount++;

        token = strtok(NULL, deliminator);
    }

    struct NODE* cod = (pathName[0] == '/') ? root : cwd; 

    for (int i = 0; i < tokenCount -1; i++) {
        char* str = tokenList[i];
        if (cod->childPtr == NULL){
            printf("ERROR: directory %s does not exist\n", str);
            free(tokenList);
            return NULL;
        }
        cod=cod->childPtr;
        while (strcmp(cod->name, str) != 0) {
            if (cod->siblingPtr != NULL) {
                cod = cod->siblingPtr;
            } else {
                printf("ERROR: directory %s does not exist\n", str);
                free(tokenList);
                return NULL;
            }
        }
    }

    strcpy(dirName, "");
    if (pathName[0] == '/') {
        strcpy(dirName, "/");
    }
    for (int j = 0; j < tokenCount - 1; j++) {
        strcat(dirName, tokenList[j]);
        if (j < tokenCount - 2) {
            strcat(dirName, "/");
        }
    }

    if (tokenCount > 0) {
        strcpy(baseName, tokenList[tokenCount - 1]);
    } else {
        strcpy(baseName, "");
    }

    free(tokenList);
    return cod;
}
