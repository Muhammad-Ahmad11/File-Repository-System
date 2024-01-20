# Generic Template-Based Hash Table

A generic template-based Hash Table implemented in C++ with STL linked lists, prioritizing file access using waiting queues. This system efficiently manages file requests, handles access priorities, and ensures seamless user interaction.

## Tech Stack

<a href="https://www.w3schools.com/cpp/" target="_blank" rel="noreferrer"> <img src="https://raw.githubusercontent.com/devicons/devicon/master/icons/cplusplus/cplusplus-original.svg" alt="cplusplus" width="40" height="40"/> </a>

## Functionality

- **Insert a File**: Utilize a hash function based on file ID to add files to the table.
- **Request File Access**: Input user ID, file ID, and priority, allowing users to request read or write access.
- **Release File**: Grant access to the next user with the highest priority, handling exclusive write access appropriately.
- **Print Hash Table**: Display file information, including current access, next user, and waiting users for each file.
- **Load Data**: Import file and user data from an input file with specified format.
- **Menu Options**: Interact through a user-friendly menu, including file insertion, deletion, access requests, and releasing files.

## Input File Format

- File ID: 5000-99999
- User ID: 0-10000
- Priority: 0-100
- Access Type: R (read) or W (write)

## Usage

1. Print Hash Table
2. Load Data
3. Insert a File
4. Delete a File (if not accessed)
5. Request File Access
6. Release File

Explore the power of generic template-based hash tables in C++ for efficient file management!
