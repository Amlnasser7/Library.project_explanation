# Library Management System

A comprehensive C-based library management system that allows both administrators and users to manage and interact with a book collection. The system provides functionality for adding, removing, searching, borrowing, and returning books.

## Table of Contents
- Overview
- Features
- File Structure
- Detailed Function Explanations
- How to Compile and Run
- Usage Guide
- Data Storage Format

## Overview

This library management system is designed with a modular architecture, separating concerns between different components:
- **Book Management**: Core data structures and utilities
- **Admin Functions**: Administrative operations (add, remove, view statistics)
- **User Functions**: User operations (search, borrow, return)
- **Data Handling**: File I/O operations for persistence
- **Main Interface**: Menu-driven interface for both admin and user modes

## Features

### Admin Mode
- Add new books to the library
- Remove existing books
- View all books in the library
- View only borrowed books
- Count total books with statistics (available/borrowed)

### User Mode
- Search books by title (case-insensitive, partial matching)
- Filter search results by author or publication year
- View all available books
- Borrow available books
- Return borrowed books

### Technical Features
- Unique ID generation for each book
- Memory-safe string operations
- Input buffer management
- Modular code architecture
- File-based data persistence (ready for implementation)

## File Structure

```
Library Management System/
├── book.h          # Book structure definition and utility functions
├── book.c          # Implementation of book utilities
├── admin.h         # Admin function declarations
├── admin.c         # Admin functionality implementation
├── user.h          # User function declarations
├── user.c          # User functionality implementation
├── data_handler.h  # File I/O function declarations
├── data_handler.c  # File I/O implementation
└── main.c          # Main program and menu interface
```

## Detailed Function Explanations

### Book Management (`book.h` / `book.c`)

#### Core Data Structure
```c
typedef struct {
    int id;                      // Unique identifier
    char title[MAX_TITLE_LEN];   // Book title (200 chars max)
    char author[MAX_AUTHOR_LEN]; // Author name (200 chars max)
    int publication_year;        // Year of publication
    int is_borrowed;            // Borrowing status (0=available, 1=borrowed)
} Book;
```

#### Key Functions

**`void display_book(const Book *book)`**
- **Purpose**: Display formatted details of a single book
- **Parameters**: Pointer to a Book structure
- **Output**: Formatted book information including ID, title, author, year, and status

**`int get_next_id(const Book *books, int count)`**
- **Purpose**: Generate the next unique ID for a new book
- **Logic**: Finds the maximum existing ID and returns max_id + 1
- **Parameters**: Array of books and current count
- **Returns**: Next available unique ID

**`void clear_input_buffer()`**
- **Purpose**: Clear stdin buffer to prevent input issues
- **Usage**: Called after `scanf()` operations to remove leftover newline characters
- **Implementation**: Reads and discards characters until newline or EOF

### Admin Functions (`admin.h` / `admin.c`)

#### Global Data Management
```c
Book library[MAX_BOOKS];  // Main book storage (max 800 books)
int book_count = 0;       // Current number of books
int next_id = 1;          // Next ID to assign
```

#### Data Access Functions
**`Book* get_library_address()`**
- **Purpose**: Provide external access to the library array
- **Returns**: Pointer to the first element of the library array
- **Usage**: Allows user.c to access the same data

**`int* get_book_count_address()`**
- **Purpose**: Provide external access to book count
- **Returns**: Address of the book_count variable
- **Usage**: Enables shared state between admin and user modules

#### Core Admin Functions

**`void addBook()`**
- **Process Flow**:
  1. Check if library is full (MAX_BOOKS limit)
  2. Clear input buffer for clean string input
  3. Read book title using `fgets()` (allows spaces)
  4. Remove newline character from title
  5. Read author name using `fgets()`
  6. Remove newline character from author
  7. Read publication year using `scanf()`
  8. Assign unique ID and set as available
  9. Increment book count
  10. Display confirmation with assigned ID

**`void removeBook()`**
- **Process Flow**:
  1. Check if library has books to remove
  2. Prompt for book ID to remove
  3. Search for book using `find_book_by_id()`
  4. Display found book details for confirmation
  5. Ask for user confirmation (y/n)
  6. If confirmed, shift all subsequent books one position left
  7. Decrement book count
  8. Display success message

**`int find_book_by_id(int id)`**
- **Purpose**: Locate a book in the library by its ID
- **Algorithm**: Linear search through the library array
- **Returns**: Index of found book or -1 if not found
- **Time Complexity**: O(n) where n is the number of books

**`void countTotalBooks()`**
- **Functionality**:
  - Display total number of books
  - Count and display available books
  - Count and display borrowed books
  - Provides statistical overview of library status

### User Functions (`user.h` / `user.c`)

#### Pointer-Based Data Access
```c
static Book *library_ptr = NULL;      // Points to admin's library array
static int *book_count_ptr = NULL;    // Points to admin's book count
static int *next_id_ptr = NULL;       // Points to admin's next ID
```

**`void init_user_pointers()`**
- **Purpose**: Initialize pointers to admin's global data
- **Process**: Calls admin's getter functions to obtain addresses
- **Ensures**: User functions work with the same data as admin functions

#### Search and Filter Functions

**`void searchByTitle()`**
- **Advanced Search Process**:
  1. Initialize data pointers if needed
  2. Get search term from user (allows partial matches)
  3. Convert search term to lowercase for case-insensitive search
  4. Loop through all books in library
  5. Convert each book title to lowercase
  6. Use `strstr()` to find partial matches
  7. Store matching books in `searchResults[]` array
  8. Display all matching books
  9. Offer filtering options (by author or year)

**`void toLowerStr(char *str)`**
- **Purpose**: Convert entire string to lowercase
- **Implementation**: Uses `tolower()` function on each character
- **Usage**: Enables case-insensitive string comparisons

**`void filterByAuthor()`**
- **Process**:
  1. Get author name from user
  2. Search only within previous search results
  3. Use `strstr()` to find author name in book's author field
  4. Display matching books from filtered results

**`void filterByYear()`**
- **Process**:
  1. Get publication year from user
  2. Search only within previous search results
  3. Compare exact year match
  4. Display matching books from filtered results

#### Borrowing System Functions

**`void viewAvailableBooks()`**
- **Logic**:
  1. Check if library is empty
  2. Loop through all books
  3. Display only books where `is_borrowed == 0`
  4. Count and display total available books
  5. Show message if no books available

**`void borrowBook()`**
- **Detailed Process**:
  1. Check if library is empty
  2. Get book ID from user
  3. Search for book with matching ID
  4. Check if book exists and is available
  5. If available: set `is_borrowed = 1` and confirm
  6. If already borrowed: display error message
  7. If not found: display "book not found" message

**`void returnBook()`**
- **Detailed Process**:
  1. Check if library is empty
  2. Get book ID from user
  3. Search for book with matching ID
  4. Check if book exists and is borrowed
  5. If borrowed: set `is_borrowed = 0` and confirm
  6. If not borrowed: display error message
  7. If not found: display "book not found" message

### Data Handling (`data_handler.h` / `data_handler.c`)

#### File Format
The system uses semicolon-separated values (CSV with semicolon delimiter):
```
ID;Title;Author;Year;IsBorrowed
1;The Great Gatsby;F. Scott Fitzgerald;1925;0
2;To Kill a Mockingbird;Harper Lee;1960;1
```

**`int load_books(const char *filename, Book *books, int *count)`**
- **File Reading Process**:
  1. Attempt to open file in read mode
  2. If file doesn't exist, set count to 0 and return 0
  3. Reset book count before loading
  4. Use `fscanf()` with format string to parse semicolon-separated data
  5. Format: `"%d;%99[^;];%99[^;];%d;%d\n"`
    - `%d` - Read integer (ID)
    - `%99[^;]` - Read up to 99 characters until semicolon (Title)
    - `%99[^;]` - Read up to 99 characters until semicolon (Author)
    - `%d` - Read integer (Year)
    - `%d` - Read integer (Borrowed Status)
  6. Continue until MAX_BOOKS reached or end of file
  7. Close file and return success status

**`int save_books(const char *filename, const Book *books, int count)`**
- **File Writing Process**:
  1. Open file in write mode (overwrites existing content)
  2. Check if file opened successfully
  3. Loop through all books
  4. Write each book's data in semicolon-separated format
  5. Use `fprintf()` to write formatted data
  6. Close file and return success status

### Main Program (`main.c`)

#### Menu System Architecture

**`void admin_mode()`**
- **Menu Options**:
  1. Add Book → calls `addBook()`
  2. Remove Book → calls `removeBook()`
  3. View All Books → calls `view_all_books(library, book_count)`
  4. Count Total Books → calls `countTotalBooks()`
  5. View Borrowed Books → calls `view_borrowed_books(library, book_count)`
  6. Exit Admin Mode → returns to main menu

**`void user_mode()`**
- **Menu Options**:
  1. Search Book → calls `searchByTitle()`
  2. View Available Books → calls `viewAvailableBooks()`
  3. Borrow Book → calls `borrowBook()`
  4. Return Book → calls `returnBook()`
  5. Exit User Mode → returns to main menu

**`int main()`**
- **Program Flow**:
  1. Display welcome message
  2. Enter infinite loop for main menu
  3. Display main menu options (Admin/User/Exit)
  4. Read user choice
  5. Execute corresponding mode function
  6. Return to main menu after mode completion
  7. Exit only when user chooses option 3

## How to Compile and Run

### Compilation
```bash
# Compile all source files together
gcc -o library_system main.c admin.c user.c book.c data_handler.c

# Alternative with warnings enabled
gcc -Wall -Wextra -o library_system main.c admin.c user.c book.c data_handler.c
```

### Running the Program
```bash
./library_system
```

## Usage Guide

### Starting the System
1. Run the compiled program
2. You'll see the main menu with three options:
   - Admin Mode
   - User Mode
   - Exit

### Admin Mode Operations

#### Adding Books
1. Select "Admin Mode" → "Add Book"
2. Enter book title (can include spaces)
3. Enter author name (can include spaces)
4. Enter publication year (integer)
5. System automatically assigns unique ID

#### Removing Books
1. Select "Admin Mode" → "Remove Book"
2. Enter the ID of the book to remove
3. System shows book details for confirmation
4. Type 'y' or 'Y' to confirm removal

#### Viewing Statistics
1. Select "Admin Mode" → "Count Total Books"
2. View total books, available books, and borrowed books

### User Mode Operations

#### Searching for Books
1. Select "User Mode" → "Search Book"
2. Enter partial or complete book title
3. View search results
4. Optionally filter by author or publication year

#### Borrowing Books
1. Select "User Mode" → "View Available Books" (to see available books)
2. Select "User Mode" → "Borrow Book"
3. Enter the ID of the book you want to borrow
4. System confirms successful borrowing

#### Returning Books
1. Select "User Mode" → "Return Book"
2. Enter the ID of the book you want to return
3. System confirms successful return

## Data Storage Format

The system is designed to work with text files using semicolon-separated values:

### File Format Specification
- **Delimiter**: Semicolon (;)
- **Line Ending**: Newline (\n)
- **Encoding**: ASCII/UTF-8
- **Field Order**: ID;Title;Author;Year;IsBorrowed

### Example Data File
```
1;Pride and Prejudice;Jane Austen;1813;0
2;1984;George Orwell;1949;1
3;The Catcher in the Rye;J.D. Salinger;1951;0
4;To Kill a Mockingbird;Harper Lee;1960;1
5;Lord of the Flies;William Golding;1954;0
```

### Field Descriptions
- **ID**: Unique integer identifier (auto-generated)
- **Title**: Book title (up to 199 characters)
- **Author**: Author name (up to 199 characters)
- **Year**: Publication year (4-digit integer)
- **IsBorrowed**: Borrowing status (0 = available, 1 = borrowed)

## Technical Implementation Details

### Memory Management
- **Static Arrays**: Uses fixed-size arrays for predictable memory usage
- **No Dynamic Allocation**: Avoids memory leaks and allocation errors
- **Buffer Safety**: String operations use safe functions with length limits

### String Handling
- **Safe Input**: Uses `fgets()` instead of `gets()` for buffer safety
- **Newline Removal**: Uses `strcspn()` to remove newline characters
- **Case-Insensitive Search**: Converts strings to lowercase for comparison

### Error Handling
- **Input Validation**: Checks for invalid menu choices
- **Boundary Checks**: Prevents array overflow
- **File Error Handling**: Graceful handling of file I/O errors
- **User Feedback**: Clear error messages for all failure cases

### Modular Design Benefits
- **Separation of Concerns**: Each module has specific responsibilities
- **Code Reusability**: Functions can be easily reused
- **Maintainability**: Easy to modify individual components
- **Testability**: Each module can be tested independently

This documentation provides a comprehensive understanding of the library management system's architecture, functionality, and usage patterns.
