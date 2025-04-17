# Simple File System in C

This project demonstrates a **basic file system simulation** in C, where users can create, navigate, and manipulate directories and files. It copies the behavior of a simple file system using a **tree structure** (directory nodes, child, sibling relationships).

## Features

- **Create directories** (`mkdir`)
- **Remove directories or files** (`rm`)
- **Change directories** (`cd`)
- **List contents of the current directory** (`ls`)
- **Print current working directory** (`pwd`)
- **Save the file system state to a file** (`save`)
- **Reload the file system state from a file** (`reload`)

## How It Works

This simple file system simulates directories and files using a tree structure, where:
- Each **node** represents a directory or file.
- **Children** represent nested files or directories.
- **Siblings** represent files or directories at the same level.

The program uses basic commands to interact with the file system, similar to a command-line interface.

### Example Commands:
- `mkdir <dirname>`: Create a new directory.
- `rmdir <dirname>`: Remove an existing directory.
- `ls`: List contents of the current directory.
- `cd <dirname>`: Change to a different directory.
- `pwd`: Print the current working directory path.
- `rm <filename>`: Remove a file.
- `save`: Save the current file system to a file.
- `reload`: Reload the file system from a file.

## Usage

1. Compile the program:
   ```bash
   gcc -o filesystem main.c functions.c
