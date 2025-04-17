#include "functions.h"


char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd",
     "creat", "rm", "reload", "save", "quit", NULL}; 
NODE *root = NULL;  
NODE *cwd = NULL;   

// Create a child node under the parent
void makechild(NODE *parent, char filetype, char *name)
{
    parent->child = (NODE *)malloc(sizeof(NODE));
    strcpy(parent->child->name, name);
    parent->child->child = 0;
    parent->child->sibling = 0;
    parent->child->parent = parent;
    parent->child->type = filetype;
}

// Create a sibling node of the older sibling
void makesibling(NODE *parent, NODE *older_sibling, char filetype, char *name)
{
    older_sibling->sibling = (NODE *)malloc(sizeof(NODE));
    strcpy(older_sibling->sibling->name, name);
    older_sibling->sibling->sibling = 0;
    older_sibling->sibling->child = 0;
    older_sibling->sibling->parent = parent;
    older_sibling->sibling->type = filetype;
}

// List all child nodes (files/directories)
int ls(NODE *cwd)
{
    NODE *temp = cwd;
    if (temp->child)
    {
        temp = temp->child;
        printf("Type: %c\tName: %s\n", temp->type, temp->name);
        while (temp->sibling)
        {
            temp = temp->sibling;
            printf("Type: %c\tName: %s\n", temp->type, temp->name);
        }
    }
    return 0;
}

// Prints the path for the txt file creation
void path(NODE *cwd, char *cwd_path)
{
    if (cwd == NULL)
    {
        printf("Error: cwd is not set.\n");
        return;
    }

    if (cwd->parent == NULL)  
    {
        strcat(cwd_path, cwd->name);
        strcat(cwd_path, "/");
        return;
    }

    path(cwd->parent, cwd_path);  // Recurse to parent node
    strcat(cwd_path, cwd->name);
    strcat(cwd_path, "/");
}

// Check if the pathname already exists
int namechecker(char *pathname, NODE *root)
{
    NODE *temproot = root;
    NODE *sibling;
    while (temproot->child)
    {
        if (temproot->sibling)
        {
            sibling = temproot->sibling;
            while (sibling->sibling)
            {
                if (strcmp(sibling->name, pathname) == 0)
                {
                    return -1;  // Name already exists
                }
                sibling = sibling->sibling;
            }
        }
        if (strcmp(temproot->child->name, pathname) == 0)
        {
            return -1;  // Name already exists
        }
        temproot = temproot->child;
    }
    return 1;  // Name is unique
}

// Change the current working directory
NODE *cd(NODE *target, char *pathway)
{
    // Special case for cd to go up to the parent directory
    if (strcmp("cd", pathway) == 0 && target->parent != NULL)
    {
        return target->parent;  // Go up to the parent
    }

    // Traverse through siblings to find the target directory
    NODE *temp = target;
    while (temp)
    {
        if (strcmp(temp->name, pathway) == 0)  // Found the target
        {
            return temp;
        }
        temp = temp->sibling;
    }

    // If not found in siblings recurse for child nodes
    if (target->child)
    {
        return cd(target->child, pathway);  // Recurse for child nodes
    }

    // If not found return the current cwd
    printf("Path not found\n");
    return cwd;
}


// Remove a node (file or directory)
int rm(char *pathname, NODE *root, char type)
{
    if (root == NULL)
    {
        printf("Path not found\n");
        return -1;
    }

    // If the node matches the pathname and type delete it
    if (strcmp(pathname, root->name) == 0 && root->type == type)
    {
        if (root->child || root->type != type)
        {
            printf("Deletion cancelled, there are files within this path or the file is not of type %c.\n", type);
            return -1;
        }
        else
        {
            if (root->sibling)
            {
                root->parent->child = root->sibling;
            }
            else
            {
                root->parent->child = NULL;
            }
            free(root);
            return 1;
        }
    }
    // Recurse for child nodes and siblings
    if (rm(pathname, root->child, type) == 1)  // Try to delete from child
        return 1;

    return rm(pathname, root->sibling, type);  // Try to delete from sibling
}

// Create a new node (file or directory)
int mk(NODE *cwd, char *pathname, char type)
{
    if (*pathname == '\0')  // Base case: empty pathname
    {
        return 1;
    }

    // Check if cwd has a child node
    if (!cwd->child)
    {
        makechild(cwd, type, pathname);
    }
    else
    {
        NODE *temp = cwd->child;
        while (temp->sibling)  // Traverse through siblings
        {
            temp = temp->sibling;
        }
        makesibling(cwd, temp, type, pathname);  // Add new sibling at the end
    }
    strcpy(pathname, "\0");
    return 1;
}

// Print the current working directory path
void pwd(NODE *cwd)
{
    if (cwd == 0)
    {
        printf("Error: cwd is not set.\n");
        return;
    }

    if (cwd->parent == 0)  
    {
        printf("TYPE:%c \tNAME: %s/\n", cwd->type, cwd->name);
        return;
    }

    pwd(cwd->parent);  // Recursive call to print parent path first
    printf("TYPE:%c \tNAME: %s/\n", cwd->type, cwd->name);  // Print current dir name
}

// Save the filesystem state to a file
void save(NODE *root, FILE *file)
{
    NODE *temp = root;
    if (root == NULL)
    {
        return;
    }
    else
    {
        while (temp)
        {
            char pathname[70] = " ";  
            path(temp, pathname);  // Get full path of the node
            fprintf(file, "%c%s\n", temp->type, pathname);  // Write to file
            temp = temp->sibling;
        }
        save(root->child, file);  // Recurse for child nodes
    }
}

// Initialize the filesystem
int initialize()
{
    root = (NODE *)malloc(sizeof(NODE));
    strcpy(root->name, "/");
    root->parent = 0;
    root->sibling = 0;
    root->child = 0;
    root->type = 'D';
    cwd = root;
    printf("Filesystem initialized!\n");
    return 1;
}

// Reload the filesystem state from a file
void reload(FILE *file)
{
    char text[100] = "", *token;
    fgets(text, sizeof(text), file);  // Root is already initialized
    while (fgets(text, sizeof(text), file) != NULL)
    {
        char type = text[0], name[20] = "";
        token = strtok(text, "/");
        while (*token != '\n')
        {
            strcpy(name, token);
            token = strtok(NULL, "/");
        }
        mk(root, name, type);  // Create directory/file
    }
}

// Use to find the command from the list
int find_command(char *user_command)
{
    int i = 0;
    while (cmd[i])  // Loop through the array of commands
    {
        if (strcmp(user_command, cmd[i]) == 0)  // Check if the user command matches a predefined command
        {
            return i;  // Return the index of the matched command
        }
        i++;
    }
    return -1;  // Return -1 if the command is not found
}