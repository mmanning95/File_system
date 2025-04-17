/****************************************************************************
*Melvin Manning
*JAN 2024
*
*NOTES
*     leaving old commented code in to use for studying later on where I made mistakes
/******************************************************************************/

#include "functions.h"

#include <stdio.h>
#include "functions.h"

int main()
{
    initialize();  // Initialize the filesystem
    int choice = 1, flag = 0, stopper = 1;
    cwd = root;
    char cwd_pathname[50] = "", userpath[50] = "";
    FILE *mem;

    while (stopper)
    {
        char com[30];
        flag = 0;

        printf("Enter command \nSupported commands(mkdir, ls, cd, pwd, rm, reload, save, quit):  ");
        scanf("%s", com);
        choice = find_command(com);  // Find the command from the list

        switch (choice)
        {
            case 0: // mkdir
                // Create a new directory
                printf("Please enter an unused directory pathname: ");
                scanf("%s", userpath);
                flag = namechecker(userpath, root);
                if (flag == -1)
                {
                    printf("Directory not created due to error\n");
                }
                else
                {
                    mk(cwd, userpath, 'D');
                    printf("Directory was created\n");
                }
                break;

            case 2: // ls
                // List the contents of the current working directory
                ls(cwd);
                break;

            case 3: // cd
                // Change the current working directory
                printf("What file/directory would you like to go to? Enter 'cd' to go up one level: ");
                scanf("%s", userpath);
                cwd = cd(root, userpath);
                break;

            case 4: // pwd
                // Print the current working directory path
                pwd(cwd);
                break;

            case 6: // rm
                // Remove a file or directory
                printf("What would you like to remove? ");
                scanf("%s", userpath);
                flag = rm(userpath, root, 'F');  
                if (flag == 1)
                {
                    printf("The path %s has been removed\n", userpath);
                }
                break;

            case 7: // reload
                // Reload the filesystem state from a file
                mem = fopen("fssim_manning.txt", "r");
                if (mem == NULL)
                {
                    printf("File was not opened properly\n");
                }
                else
                {
                    reload(mem);
                    fclose(mem);
                }
                break;

            case 8: // save
                // Save the filesystem state to a file
                mem = fopen("fssim_manning.txt", "w");
                if (mem == NULL)
                {
                    printf("File was not opened properly\n");
                }
                else
                {
                    save(root, mem);
                    fclose(mem);
                }
                break;

            case 9: // quit
                // Exit the program and save the filesystem state
                stopper = 0;
                mem = fopen("fssim_manning.txt", "w");
                if (mem != NULL)
                {
                    save(root, mem);
                    fclose(mem);
                }
                break;

            default: // Command not found
                printf("Command not found\n");
                break;
        }
    }

    return 0;
}
