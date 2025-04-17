#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char name[64];          
    char type;             // type of the node (example: 'D' for directory, 'F' for file)
    struct node *child, *sibling, *parent;  
} NODE;

//global variables
extern NODE *root;        // Root of the filesystem (root directory)
extern NODE *cwd;         // Current working directory
extern char *cmd[];     // List of commands supported by the filesystem

//prototypes

int find_command(char *user_command);
void path(NODE *cwd, char *cwd_path);
void makechild(NODE *parent, char filetype, char *name);
void makesibling(NODE *parent, NODE *older_sibling, char filetype, char *name);
int ls(NODE *cwd);
int namechecker(char *pathname, NODE *root);
NODE *cd(NODE *target, char *pathway);
int rm(char *pathname, NODE *root, char type);
int mk(NODE *cwd, char *pathname, char type);
void pwd(NODE *cwd);
void path(NODE *cwd, char *cwd_path);
void save(NODE *root, FILE *file);
int initialize();
void reload(FILE *file);


#endif // FUNCTIONS_H
