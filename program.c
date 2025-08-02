// ==================== book.h ====================
#ifndef BOOK_H                    // Include guard: prevents multiple inclusions of this header
#define BOOK_H                    // Define the include guard macro


#define MAX_TITLE_LEN 200         // Maximum length for book title string (199 chars + null terminator)
#define MAX_AUTHOR_LEN 200        // Maximum length for author name string (199 chars + null terminator)
#define MAX_BOOKS 800             // Maximum number of books that can be stored in the library

// Define the Book structure - represents a single book with all its properties
typedef struct {
    int id;                       // Unique identifier for each book
    char title[MAX_TITLE_LEN];    // Array to store the book's title
    char author[MAX_AUTHOR_LEN];  // Array to store the author's name
    int publication_year;         // Year the book was published
    int is_borrowed;              // Flag: 1 if borrowed, 0 if available
} Book;

// Function prototypes (declarations only - actual code is in book.c)
void display_book(const Book *book);           // Display details of a single book
int get_next_id(const Book *books, int count); // Get next available unique ID
void clear_input_buffer();                     // Clear stdin buffer after scanf

#endif // BOOK_H               // End of include guard

// ==================== book.c ====================
#include <stdio.h>             // Include for printf, scanf functions
#include <string.h>            // Include for string manipulation functions
#include <limits.h>            // Include for INT_MIN constant

#include "book.h"              // Include our book header file

// Function to display details of a single book
void display_book(const Book *book) {
    printf("ID: %d\n", book->id);                    // Print the book's unique ID
    printf("Title: %s\n", book->title);              // Print the book's title
    printf("Author: %s\n", book->author);            // Print the author's name
    printf("Year: %d\n", book->publication_year);    // Print publication year
    printf("Status: %s\n", book->is_borrowed ? "Borrowed" : "Available"); // Print status using ternary operator
    printf("--------------------\n");                // Print separator line
}

// Function to get the next available unique ID for a new book
// It finds the maximum existing ID and increments it by 1.
int get_next_id(const Book *books, int count) {
    if (count == 0) {                                 // If no books exist in the library
        return 1;                                     // Start with ID 1 for the first book
    }
    int max_id = INT_MIN;                            // Initialize with smallest possible integer
    for (int i = 0; i < count; i++) {               // Loop through all existing books
        if (books[i].id > max_id) {                  // If current book's ID is larger than max found so far
            max_id = books[i].id;                    // Update max_id to current book's ID
        }
    }
    return max_id + 1;                               // Return the next available ID (max + 1)
}

// Function to clear the standard input buffer
// Used to prevent issues with leftover newline characters after scanf
void clear_input_buffer() {
    int c;                                           // Variable to store each character
    while ((c = getchar()) != '\n' && c != EOF);    // Read and discard characters until newline or EOF
}

// ==================== admin.h ====================
#ifndef ADMIN_H                // Include guard to prevent multiple inclusions
#define ADMIN_H                // Define the include guard macro

#include <stdio.h>             // Include standard I/O functions
#include "book.h"              // Include Book structure definition

// Function declarations only - actual implementations are in admin.c
int find_book_by_id(int id);                           // Find book index by ID
void addBook();                                         // Add a new book to library
void removeBook();                                      // Remove a book from library
void countTotalBooks();                                 // Display book statistics
void admin_mode();                                      // Admin menu interface
void view_all_books(const Book books[], int count);    // Display all books
void view_borrowed_books(const Book books[], int count); // Display only borrowed books
void invalid_choice();                                  // Handle invalid menu choices

// Function declarations to get addresses of global variables from admin.c
Book* get_library_address();                           // Returns pointer to library array
int* get_book_count_address();                         // Returns pointer to book_count variable
int* get_next_id_address();                            // Returns pointer to next_id variable

#endif                         // End of include guard

// ==================== admin.c ====================
#include <stdio.h>             // Include for printf, scanf, fgets functions
#include <string.h>            // Include for strcspn function (string operations)
#include <ctype.h>             // Include for character type functions (not used here but included)
#include "book.h"              // Include Book structure and related functions
#include "admin.h"             // Include admin function declarations

Book library[MAX_BOOKS];       // Global array to store all books in the library
int book_count = 0;           // Global variable tracking number of books currently in library
int next_id = 1;              // Global variable for generating unique book IDs

// Functions to get addresses of global variables - allows other files to access these variables
Book* get_library_address() {
    return library;            // Return pointer to the first element of library array
}

int* get_book_count_address() {
    return &book_count;        // Return address of book_count variable
}

int* get_next_id_address() {
    return &next_id;           // Return address of next_id variable
}

// Function to find book by ID (without pointers)
int find_book_by_id(int id) {
    for (int i = 0; i < book_count; i++) {           // Loop through all books in library
        if (library[i].id == id) {                   // If current book's ID matches searched ID
            return i;                                 // Return the index of the found book
        }
    }
    return -1;                                       // Return -1 if book not found
}

// Function to add a book
void addBook() {
    if (book_count >= MAX_BOOKS) {                   // Check if library is full
        printf("Library is full and you Cannot add more books.\n"); // Display error message
        return;                                       // Exit function early
    }
    
    printf("\n Add New Book \n");                   // Display section header
    
    clear_input_buffer();                           // Clear any leftover input from previous scanf
    
    printf("Enter the book title: ");               // Prompt user for title
    fgets(library[book_count].title, MAX_TITLE_LEN, stdin); // Read title including spaces
    library[book_count].title[strcspn(library[book_count].title, "\n")] = 0; // Remove newline character
    
    printf("Enter author name: ");                  // Prompt user for author
    fgets(library[book_count].author, MAX_AUTHOR_LEN, stdin); // Read author name including spaces
    library[book_count].author[strcspn(library[book_count].author, "\n")] = 0; // Remove newline character
    
    printf("Enter publication year: ");             // Prompt user for year
    scanf("%d", &library[book_count].publication_year); // Read publication year as integer
    
    library[book_count].id = next_id++;             // Assign current next_id and increment it
    library[book_count].is_borrowed = 0;            // Set book as available (not borrowed)
    
    book_count++;                                   // Increment total book count

    printf("Book added successfully! (ID: %d)\n", library[book_count-1].id); // Confirm addition with ID
}

// Function to remove books
void removeBook() {
    if (book_count == 0) {                          // Check if library is empty
        printf("No books in this library to remove.\n"); // Display message
        return;                                      // Exit function early
    }
    
    int id;                                         // Variable to store book ID to remove
    printf("\n Remove Book\n");                    // Display section header
    printf("Enter book ID to remove: ");           // Prompt for book ID
    scanf("%d", &id);                              // Read book ID

    int index = find_book_by_id(id);               // Find the book's index in array
    if (index == -1) {                             // If book not found
        printf("Book with ID %d not found.\n", id); // Display error message
        return;                                     // Exit function
    }
    printf("Book was found: \"%s\" by %s\n", library[index].title, library[index].author); // Show found book

    printf("Are you sure you wanna remove this book? (y/n): "); // Confirmation prompt
    char confirm;                                   // Variable to store user's confirmation
    scanf(" %c", &confirm);                        // Read confirmation (space before %c to skip whitespace)

    if (confirm == 'y' || confirm == 'Y') {        // If user confirms removal
        for (int i = index; i < book_count - 1; i++) { // Shift all books after removed book
            library[i] = library[i + 1];           // Copy next book to current position
        }
        book_count--;                               // Decrease total book count
        printf("Book with ID %d removed successfully.\n", id); // Confirm removal
    } else {                                        // If user cancels
        printf("Book removal canceled.\n");        // Display cancellation message
    }
}

// Function to count total books
void countTotalBooks() {
    printf("\n Book Count \n");                    // Display section header
    printf("Total books in library: %d\n", book_count); // Display total count

    int available = 0, borrowed = 0;               // Variables to count available and borrowed books
    for (int i = 0; i < book_count; i++) {         // Loop through all books
        if (library[i].is_borrowed) {              // If current book is borrowed
            borrowed++;                             // Increment borrowed counter
        } else {                                    // If current book is available
            available++;                            // Increment available counter
        }
    }

    printf("Available books: %d\n", available);    // Display available count
    printf("Borrowed books: %d\n", borrowed);      // Display borrowed count
}

// View all books
void view_all_books(const Book books[], int count) {
    if (count == 0) {                              // Check if no books exist
        printf(" No books in the library.\n");     // Display message
        return;                                     // Exit function
    }

    printf(" All Books in the Library:\n");        // Display section header
    printf("--------------------------------------------------\n"); // Display separator

    for (int i = 0; i < count; i++) {             // Loop through all books
        printf("ID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nBorrowed: %s\n\n", // Display book details
               books[i].id,                         // Book ID
               books[i].title,                      // Book title
               books[i].author,                     // Book author
               books[i].publication_year,           // Publication year
               books[i].is_borrowed ? "Yes" : "No"); // Borrowed status using ternary operator
    }
}

// View borrowed books
void view_borrowed_books(const Book books[], int count) {
    int found = 0;                                 // Flag to track if any borrowed books found

    printf(" Borrowed Books:\n");                  // Display section header
    printf("--------------------------------------------------\n"); // Display separator

    for (int i = 0; i < count; i++) {             // Loop through all books
        if (books[i].is_borrowed) {                // If current book is borrowed
            found = 1;                              // Set found flag
            printf("ID: %d\nTitle: %s\nAuthor: %s\nYear: %d\n\n", // Display borrowed book details
                   books[i].id,                     // Book ID
                   books[i].title,                  // Book title
                   books[i].author,                 // Book author
                   books[i].publication_year);      // Publication year
        }
    }

    if (!found) {                                  // If no borrowed books found
        printf(" No books are currently borrowed.\n"); // Display message
    }
}

// ==================== user.h ====================
#ifndef USER_H                 // Include guard to prevent multiple inclusions
#define USER_H                 // Define the include guard macro

#include "book.h"              // Include Book structure definition
#include <stdio.h>             // Include standard I/O functions
#include <stdlib.h>            // Include standard library functions
#include <ctype.h>             // Include character type functions

// Function declarations only (no definitions in header)
void viewAvailableBooks();     // Display all available (non-borrowed) books
void borrowBook();             // Allow user to borrow a book
void returnBook();             // Allow user to return a borrowed book
void searchByTitle();          // Search books by title
void filterByAuthor();         // Filter search results by author
void filterByYear();           // Filter search results by publication year
void toLowerStr(char *str);    // Convert string to lowercase
void init_user_pointers();     // Initialize pointers to admin data

#endif                         // End of include guard

// ==================== user.c ====================
#include "user.h"              // Include user function declarations
#include <string.h>            // For: strcspn, strncpy, strstr functions
#include <ctype.h>             // For: tolower function
#include <stdio.h>             // Include standard I/O functions
#include <stdlib.h>            // Include standard library functions
#include "book.h"              // Include Book structure and related functions
#include "admin.h"             // Include admin functions to access global data

#define MAX_RESULTS 100        // Maximum number of search results to store

// Get pointers to admin data - these will point to global variables in admin.c
static Book *library_ptr = NULL;      // Pointer to the library array
static int *book_count_ptr = NULL;    // Pointer to book count variable
static int *next_id_ptr = NULL;       // Pointer to next ID variable

Book searchResults[MAX_RESULTS];      // Array to store search results
int searchResultCount = 0;            // Counter for number of search results

// Initialize pointers to admin data
void init_user_pointers() {
    library_ptr = get_library_address();      // Get address of library array from admin.c
    book_count_ptr = get_book_count_address(); // Get address of book_count from admin.c
    next_id_ptr = get_next_id_address();       // Get address of next_id from admin.c
}

// Function to convert string to lowercase
void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++) {            // Loop through each character until null terminator
        str[i] = tolower((unsigned char) str[i]); // Convert character to lowercase
    }
}

//**************************Search By Title******************************//

void searchByTitle() {
    // Initialize pointers if not done already
    if (library_ptr == NULL) {                 // Check if pointers are not initialized
        init_user_pointers();                  // Initialize pointers to admin data
    }
    
    char search_title[100];                    // Buffer to store search term
    printf("Enter the book title to search: "); // Prompt user for search term
    clear_input_buffer();                      // Clear input buffer
    fgets(search_title, sizeof(search_title), stdin); // Read search term with spaces
    search_title[strcspn(search_title, "\n")] = 0; // Remove newline character

    int found = 0;                             // Flag to track if any books found
    searchResultCount = 0;                     // Reset search results counter

    char lower_search_title[100];              // Buffer for lowercase search term
    strncpy(lower_search_title, search_title, sizeof(lower_search_title)); // Copy search term
    lower_search_title[sizeof(lower_search_title) - 1] = '\0'; // Ensure null termination
    toLowerStr(lower_search_title);            // Convert search term to lowercase

    printf("\nSearching through %d books...\n", *book_count_ptr); // Display search info

    for (int i = 0; i < *book_count_ptr; i++) { // Loop through all books in library
        char lower_title[100];                  // Buffer for lowercase book title
        strncpy(lower_title, library_ptr[i].title, sizeof(lower_title)); // Copy book title
        lower_title[sizeof(lower_title) - 1] = '\0'; // Ensure null termination
        toLowerStr(lower_title);               // Convert book title to lowercase

        if (strstr(lower_title, lower_search_title)) { // If search term found in title (case-insensitive)
            searchResults[searchResultCount++] = library_ptr[i]; // Add to search results
            printf("\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nStatus: %s\n", // Display book details
                   library_ptr[i].id, library_ptr[i].title, library_ptr[i].author,
                   library_ptr[i].publication_year,
                   library_ptr[i].is_borrowed ? "Borrowed" : "Available"); // Status using ternary operator
            found = 1;                          // Set found flag
        }
    }

    if (!found) {                              // If no books found
        printf("No books found with title containing \"%s\".\n", search_title); // Display message
        return;                                 // Exit function
    }

    // Offer filter options
    int choice;                                // Variable to store user choice
    printf("\nFilter search results:\n1. By Author\n2. By Year\n3. Exit\nChoice: "); // Display filter menu
    scanf("%d", &choice);                      // Read user choice

    switch (choice) {                          // Execute based on user choice
        case 1: filterByAuthor(); break;       // Filter by author
        case 2: filterByYear(); break;         // Filter by year
        case 3: default: break;                // Exit or default case
    }
}

//**************************filter By Author******************************//

void filterByAuthor() {
    char author[100];                          // Buffer to store author name
    printf("Enter author's name to filter: "); // Prompt for author name
    clear_input_buffer();                      // Clear input buffer
    fgets(author, sizeof(author), stdin);      // Read author name with spaces
    author[strcspn(author, "\n")] = 0;         // Remove newline character

    int found = 0;                             // Flag to track if any books found
    for (int i = 0; i < searchResultCount; i++) { // Loop through search results only
        if (strstr(searchResults[i].author, author)) { // If author name found in book's author
            printf("\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nStatus: %s\n", // Display book details
                   searchResults[i].id,         // Book ID
                   searchResults[i].title,      // Book title
                   searchResults[i].author,     // Book author
                   searchResults[i].publication_year, // Publication year
                   searchResults[i].is_borrowed ? "Borrowed" : "Available"); // Status
            found = 1;                          // Set found flag
        }
    }

    if (!found) {                              // If no books found by that author
        printf("No books found by that author in the search results.\n"); // Display message
    }
}

//**************************filter By Year******************************//

void filterByYear() {
    int year;                                  // Variable to store publication year
    printf("Enter publication year to filter: "); // Prompt for year
    scanf("%d", &year);                        // Read year as integer

    int found = 0;                             // Flag to track if any books found
    for (int i = 0; i < searchResultCount; i++) { // Loop through search results only
        if (searchResults[i].publication_year == year) { // If publication year matches
            printf("\nID: %d\nTitle: %s\nAuthor: %s\nYear: %d\nStatus: %s\n", // Display book details
                   searchResults[i].id,         // Book ID
                   searchResults[i].title,      // Book title
                   searchResults[i].author,     // Book author
                   searchResults[i].publication_year, // Publication year
                   searchResults[i].is_borrowed ? "Borrowed" : "Available"); // Status
            found = 1;                          // Set found flag
        }
    }

    if (!found) {                              // If no books found from that year
        printf("No books found from that year in the search results.\n"); // Display message
    }
}

//**************************User Borrowing Functions ******************************//

// عرض جميع الكتب المتاحة (التي لم يتم استعارتها)
// Display all available books (that haven't been borrowed)
void viewAvailableBooks() {
    // Initialize pointers if not done already
    if (library_ptr == NULL) {                 // Check if pointers not initialized
        init_user_pointers();                  // Initialize pointers to admin data
    }

    if (*book_count_ptr == 0) {                // Check if library is empty
        printf("No books found, currently.\n"); // Display message
        return;                                 // Exit function
    }

    printf("\nAvailable book list:\n");        // Display section header
    printf("--------------------------------------------------\n"); // Display separator
    
    int available_count = 0;                   // Counter for available books
    for (int i = 0; i < *book_count_ptr; i++) { // Loop through all books
        if (!library_ptr[i].is_borrowed) {     // If book is not borrowed (available)
            printf("ID: %d | %s by %s (%d)\n", // Display book info in compact format
                   library_ptr[i].id,           // Book ID
                   library_ptr[i].title,        // Book title
                   library_ptr[i].author,       // Book author
                   library_ptr[i].publication_year); // Publication year
            available_count++;                  // Increment available counter
        }
    }
    
    if (available_count == 0) {                // If no available books found
        printf("No books are currently available for borrowing.\n"); // Display message
    }
}

void borrowBook() {
    // Initialize pointers if not done already
    if (library_ptr == NULL) {                 // Check if pointers not initialized
        init_user_pointers();                  // Initialize pointers to admin data
    }

    int id, found = 0;                         // Variables for book ID and found flag

    if (*book_count_ptr == 0) {                // Check if library is empty
        printf("No books in the library.\n");  // Display message
        return;                                 // Exit function
    }

    printf("Enter the book ID you desire to borrow: "); // Prompt for book ID
    scanf("%d", &id);                          // Read book ID

    for (int i = 0; i < *book_count_ptr; i++) { // Loop through all books
        if (library_ptr[i].id == id && !library_ptr[i].is_borrowed) { // If book found and available
            library_ptr[i].is_borrowed = 1;    // Mark book as borrowed
            found = 1;                          // Set found flag
            printf("The book \"%s\" by %s is borrowed successfully.\n", // Confirm borrowing
                   library_ptr[i].title, library_ptr[i].author);
            break;                              // Exit loop
        } else if (library_ptr[i].id == id && library_ptr[i].is_borrowed) { // If book found but already borrowed
            printf("The book \"%s\" is already borrowed.\n", library_ptr[i].title); // Display message
            return;                             // Exit function
        }
    }

    if (!found) {                              // If book not found
        printf("The book with ID %d is not found.\n", id); // Display error message
    }
}

void returnBook() {
    // Initialize pointers if not done already
    if (library_ptr == NULL) {                 // Check if pointers not initialized
        init_user_pointers();                  // Initialize pointers to admin data
    }

    int id, found = 0;                         // Variables for book ID and found flag

    if (*book_count_ptr == 0) {                // Check if library is empty
        printf("No books in the library.\n");  // Display message
        return;                                 // Exit function
    }

    printf("Enter the book ID you want to return: "); // Prompt for book ID
    scanf("%d", &id);                          // Read book ID

    for (int i = 0; i < *book_count_ptr; i++) { // Loop through all books
        if (library_ptr[i].id == id && library_ptr[i].is_borrowed) { // If book found and borrowed
            library_ptr[i].is_borrowed = 0;    // Mark book as available
            found = 1;                          // Set found flag
            printf("The book \"%s\" by %s is returned successfully.\n", // Confirm return
                   library_ptr[i].title, library_ptr[i].author);
            break;                              // Exit loop
        } else if (library_ptr[i].id == id && !library_ptr[i].is_borrowed) { // If book found but not borrowed
            printf("The book \"%s\" is not currently borrowed.\n", library_ptr[i].title); // Display message
            return;                             // Exit function
        }
    }

    if (!found) {                              // If book not found
        printf("The book with ID %d is not found.\n", id); // Display error message
    }
}

// ==================== data_handler.h ====================
#ifndef DATA_HANDLER_H         // Include guard to prevent multiple inclusions
#define DATA_HANDLER_H         // Define the include guard macro

#include "book.h"              // Include header for Book structure

// Function declarations for data handling
// Loads book data from a file into the 'books' array
int load_books(const char *filename, Book *books, int *count);
// Saves book data from the 'books' array to a file
int save_books(const char *filename, const Book *books, int count);

#endif // DATA_HANDLER_H       // End of include guard

// ==================== data_handler.c ====================
#include <stdio.h>             // For fopen, fclose, fscanf, fprintf functions
#include <stdlib.h>            // For general utilities (though not directly used here)
#include <string.h>            // For strcspn function

#include "data_handler.h"      // Include our data handler declarations

// Function to load books from a text file
int load_books(const char *filename, Book *books, int *count) {
    FILE *file = fopen(filename, "r");         // Open file for reading
    if (file == NULL) {                        // If file cannot be opened
        // If the file doesn't exist, it's normal for the first run
        // or if it's empty. Initialize count to 0.
        *count = 0;                            // Set book count to 0
        return 0;                              // Return 0 to indicate failure to open/load
    }

    *count = 0;                                // Reset book count before loading
    // Read 5 fields (ID, Title, Author, Year, is_borrowed) using semicolon as delimiter
    while (*count < MAX_BOOKS &&              // While we haven't reached max books and
           fscanf(file, "%d;%99[^;];%99[^;];%d;%d\n", // Read formatted data from file
                  &books[*count].id,           // Read book ID
                  books[*count].title,         // Read book title (up to 99 chars, stop at semicolon)
                  books[*count].author,        // Read book author (up to 99 chars, stop at semicolon)
                  &books[*count].publication_year, // Read publication year
                  &books[*count].is_borrowed) == 5) { // Read borrowed status, check if all 5 fields read
        (*count)++;                            // Increment book count
    }

    fclose(file);                              // Close the file
    return 1;                                  // Return 1 to indicate successful loading
}

// Function to save books to a text file
int save_books(const char *filename, const Book *books, int current_book_count) {
    FILE *file = fopen(filename, "w");         // Open file for writing (clears old content)
    if (file == NULL) {                        // If file cannot be opened for writing
        perror("Error opening file for saving"); // Print error message with system error info
        return 0;                              // Return 0 to indicate save failure
    }

    for (int i = 0; i < current_book_count; i++) { // Loop through all books to save
        fprintf(file, "%d;%s;%s;%d;%d\n",      // Write book data to file with semicolon delimiters
                books[i].id,                    // Write book ID
                books[i].title,                 // Write book title
                books[i].author,                // Write book author
                books[i].publication_year,      // Write publication year
                books[i].is_borrowed);          // Write borrowed status
    }

    fclose(file);                              // Close the file
    return 1;                                  // Return 1 to indicate successful saving
}

// ==================== main.c ====================
#include <stdio.h>             // Include standard I/O functions
#include <stdlib.h>            // Include standard library functions (for exit)
#include "admin.h"             // Include admin function declarations
#include "user.h"              // Include user function declarations
#include "book.h"              // Include Book structure and related functions

// Declare external variables - these are defined in admin.c
extern Book library[MAX_BOOKS]; // External reference to library array
extern int book_count;           // External reference to book count

// Admin mode: simple menu for demonstration
void admin_mode() {
    int choice;                                // Variable to store user's menu choice
    printf("Entering Admin Mode...\n");       // Display mode entry message
    printf("1. Add Book\n");                  // Display menu option 1
    printf("2. Remove Book\n");               // Display menu option 2
    printf("3. View All Books\n");            // Display menu option 3
    printf("4. Count Total Books\n");         // Display menu option 4
    printf("5. View Borrowed Books\n");       // Display menu option 5
    printf("6. Exit Admin Mode\n");           // Display menu option 6
    printf("Enter your choice: ");            // Prompt for choice
    scanf("%d", &choice);                     // Read user's choice

    switch (choice) {                         // Execute based on user choice
        case 1:                               // If user chose option 1
            addBook();                        // Call function to add a book
            break;                            // Exit switch statement
        case 2:                               // If user chose option 2
            removeBook();                     // Call function to remove a book
            break;                            // Exit switch statement
        case 3:                               // If user chose option 3
            view_all_books(library, book_count); // Call function to view all books
            break;                            // Exit switch statement
        case 4:                               // If user chose option 4
            countTotalBooks();                // Call function to count books
            break;                            // Exit switch statement
        case 5:                               // If user chose option 5
            view_borrowed_books(library, book_count); // Call function to view borrowed books