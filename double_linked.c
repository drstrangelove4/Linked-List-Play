// Headers
#include <stdio.h>
#include <stdlib.h>

// Todo
/*
- Delete a node
- Edit a node's value
*/


// --------------------------------------------------------------------------------------------------------------------

// Custom Types
typedef struct dllist
{
    int number;
    struct dllist *prev;
    struct dllist *next;
} dllist;

typedef struct
{
    int return_int;
    int return_code;
} return_object;

// --------------------------------------------------------------------------------------------------------------------

// Signatures
dllist *new_node(int value, dllist *next_pointer, dllist *previous_pointer);
int free_memory_left(dllist *prev_pointer);
int free_memory_right(dllist *next_pointer);
return_object search(dllist *root, int search);
return_object search_left(dllist *root, int search);
return_object search_right(dllist *root, int search);
void append_left(dllist *new_node, dllist *left_node);
void append_right(dllist *new_node, dllist *right_node);
void free_memory(dllist *root);
void insert_node(dllist *before_node, dllist *after_node);
void print_next(dllist *next);
void print_previous(dllist *previous);

// --------------------------------------------------------------------------------------------------------------------

int main()
{
    // Using a function to return a pointer to a list element.
    dllist *root = new_node(0, NULL, NULL);
    if (root == NULL)
    {
        return 1;
    }
    dllist *right = new_node(10, NULL, root);
    if (right == NULL)
    {
        return 1;
    }
    dllist *left = new_node(-10, root, NULL);
    if (left == NULL)
    {
        return 1;
    }

    // Set the root to point at each list element
    root->next = right;
    root->prev = left;

    // Print all previous elements
    print_previous(root);
    print_next(root);

    // Add a new element onto the right
    dllist *new_right = new_node(20, NULL, NULL);
    if (new_right == NULL)
    {
        return 1;
    }
    dllist *new_left = new_node(-20, NULL, NULL);
    if (new_left == NULL)
    {
        return 1;
    }

    // Add new nodes to the end of the chain.
    append_left(new_left, root);
    append_right(new_right, root);
    print_previous(root);
    print_next(root);

    // Search
    return_object find = search(root, 10);
    if (find.return_code == 1)
    {
        printf("Search not found\n\n");
    }
    else

    {
        printf("Search term found: %i\n\n", find.return_int);
    }

    // Insert a new node between root and the next node it is pointing at
    // Create a new node
    dllist *insert_next = new_node(5, NULL, NULL);
    insert_node(root, insert_next);

    // Insert a node without saving it as a variable
    append_right(new_node(50, NULL, NULL), root);
    append_left(new_node(-30, NULL, NULL), root);

    // Print the new chain
    print_next(root);
    print_previous(root);

    // Free memory
    free_memory(root);
}

// --------------------------------------------------------------------------------------------------------------------

dllist *new_node(int value, dllist *next_pointer, dllist *previous_pointer)
// Creates a DLList node and sets the initial pointers
{
    dllist *new = malloc(sizeof(dllist));
    if (new == NULL)
    {
        return NULL;
    }
    new->number = value;
    new->next = next_pointer;
    new->prev = previous_pointer;

    return new;
}

// --------------------------------------------------------------------------------------------------------------------
// Appending nodes

void append_right(dllist *new_node, dllist *right_node)
// Appends a node onto the furthest right of the chain.
{
    // Base case
    if (right_node->next == NULL)
    {
        right_node->next = new_node;
        new_node->prev = right_node;
        return;
    }
    append_right(new_node, right_node->next);
}

void append_left(dllist *new_node, dllist *left_node)
// Appends a node onto the furthest left of the chain.
{
    // Base case
    if (left_node->prev == NULL)
    {
        left_node->prev = new_node;
        new_node->next = left_node;
        return;
    }
    append_left(new_node, left_node->prev);
}

// --------------------------------------------------------------------------------------------------------------------

// Insert Nodes

void insert_node(dllist *before_node, dllist *after_node)
// Inserts a node into the list
{    
    // Set pointers of the new node 
    after_node->next = before_node->next;
    after_node->prev = before_node;

    // Set pointers of before node to point at inserted node
    before_node->next = after_node;

    // Set pointer of next node's prev to new node
    after_node->next->prev = after_node;
}
// --------------------------------------------------------------------------------------------------------------------

// Search for X value

return_object search(dllist *root, int search)
// Base function. Calls for searches in both directions of the list starting from the root.
{
    // Search linked list in either direction.
    return_object left = search_left(root, search);
    return_object right = search_right(root, search);

    return_object new_return;

    // If we find the object return its value and success code.
    if (left.return_int == search && left.return_code == 0)
    {
        new_return.return_code = 0;
        new_return.return_int = search;
    }
    else if (right.return_int == search && right.return_code == 0)
    {
        new_return.return_code = 0;
        new_return.return_int = search;
    }
    // If we don't find the object return an error code.
    else
    {
        new_return.return_code = 1;
        new_return.return_int = 0;
    }
    return new_return;
}

return_object search_left(dllist *root, int search)
// Searchs the left or previous side of the list for the search term.
{
    // Base cases. Finding the number or encountering null.
    return_object new_return;
    if (root->number == search)
    {
        new_return.return_code = 0;
        new_return.return_int = search;
    }
    else if (root->prev == NULL)
    {
        new_return.return_code = 1;
        new_return.return_int = 0;
    }
    // Continue searching if we haven't found null or the number.
    else
    {
        new_return = search_left(root->prev, search);
    }
    return new_return;
}

return_object search_right(dllist *root, int search)
// Searchs the left or previous side of the list for the search term.
{
    // Base cases. Finding the number or encountering null.
    return_object new_return;
    if (root->number == search)
    {
        new_return.return_code = 0;
        new_return.return_int = search;
    }
    else if (root->next == NULL)
    {
        new_return.return_code = 1;
        new_return.return_int = 0;
    }
    // Continue searching if we haven't found null or the number.
    else
    {
        new_return = search_right(root->next, search);
    }
    return new_return;
}

// --------------------------------------------------------------------------------------------------------------------

// Printing nodes

void print_previous(dllist *previous)
// Prints the previous node in the chain.
{
    // Print the current number
    printf("%i\n", previous->number);

    // Base case. Stop seaching for nodes down the chain when we find a NULL pointer.
    if (previous->prev == NULL)
    {
        printf("End of chain!\n\n");
        return;
    }
    // Call the function to continue down the chain.
    print_previous(previous->prev);
}

void print_next(dllist *next_node)
// Prints the next node in the chain.
{
    // Print the current number
    printf("%i\n", next_node->number);

    // Base case. Stop seaching for nodes down the chain when we find a NULL pointer.
    if (next_node->next == NULL)
    {
        printf("End of chain!\n\n");
        return;
    }
    // Call the function to continue down the chain.
    print_next(next_node->next);
}

// --------------------------------------------------------------------------------------------------------------------
// Free memory

void free_memory(dllist *root)
// Searches for nodes down each side of the list and frees memory used by malloc.
{
    /* We need the recursive function to return something to signal root to call free on itself.
    We need to free the list from null pointer, backwards to root to avoid breaking the chain
    and leaving nodes floating in memory.*/

    int left = free_memory_left(root->prev);
    int right = free_memory_right(root->next);
    if (left == 0 && right == 0)
    {
        free(root);
        printf("Memory has been freed\n.");
        return;
    }
    else
    {
        printf("There was an error freeing memory");
    }
}

int free_memory_left(dllist *prev_pointer)
{
    // Base case - stop searching for nodes when we find null.
    if (prev_pointer->prev == NULL)
    {
        free(prev_pointer);
        return 0;
    }
    // Calls the the function on the next node in the chain if null isn't found.
    free_memory_left(prev_pointer->prev);
    free(prev_pointer);
    return 0;
}

int free_memory_right(dllist *next_pointer)
{
    // Base case - stop searching for nodes when we find null.
    if (next_pointer->next == NULL)
    {
        free(next_pointer);
        return 0;
    }
    // Calls the the function on the next node in the chain if null isn't found.
    free_memory_right(next_pointer->next);
    free(next_pointer);
    return 0;
}
