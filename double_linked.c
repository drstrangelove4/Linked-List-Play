// Headers
#include <stdio.h>
#include <stdlib.h>

// --------------------------------------------------------------------------------------------------------------------

// Custom Types
typedef struct dllist
{
    int number;
    struct dllist *prev;
    struct dllist *next;
} dllist;

// --------------------------------------------------------------------------------------------------------------------

// Signatures
dllist *new_node(int value, dllist *next_pointer, dllist *previous_pointer);
int free_memory_left(dllist *prev_pointer);
int free_memory_right(dllist *next_pointer);
void append_left(dllist *new_node, dllist *left_node);
void append_right(dllist *new_node, dllist *right_node);
void free_memory(dllist *root);
void print_next(dllist *next);
void print_previous(dllist *previous);

// --------------------------------------------------------------------------------------------------------------------

int main()
{
    // Using a function to return a pointer to a list element.
    dllist *root = new_node(10, NULL, NULL);
    if (root == NULL)
    {
        return 1;
    }
    dllist *right = new_node(20, NULL, root);
    if (right == NULL)
    {
        return 1;
    }
    dllist *left = new_node(0, root, NULL);
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
    dllist *new_right = new_node(30, NULL, NULL);
    if (new_right == NULL)
    {
        return 1;
    }
    dllist *new_left = new_node(-10, NULL, NULL);
    if (new_left == NULL)
    {
        return 1;
    }

    // Add new nodes to the end of the chain.
    append_left(new_left, root);
    append_right(new_right, root);
    print_previous(root);
    print_next(root);

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
// Adding nodes

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
// Printing nodes

void print_previous(dllist *previous)
// Prints the previous node in the chain.
{
    // Print the current number
    printf("%i\n", previous->number);

    // Base case
    if (previous->prev == NULL)
    {
        printf("End of chain!\n\n");
        return;
    }
    // Call the function
    print_previous(previous->prev);
}

void print_next(dllist *next_node)
// Prints the next node in the chain.
{
    // Print the current number
    printf("%i\n", next_node->number);

    // Base case
    if (next_node->next == NULL)
    {
        printf("End of chain!\n\n");
        return;
    }
    // Call the function
    print_next(next_node->next);
}

// --------------------------------------------------------------------------------------------------------------------
// Free memory

void free_memory(dllist *root)
{
    int left = free_memory_left(root->prev);
    int right = free_memory_right(root->next);
    if (left == 0 && right == 0)
    {
        free(root);
        printf("Memory has been freed\n.");
        return;
    }
}

int free_memory_left(dllist *prev_pointer)
{
    if (prev_pointer->prev == NULL)
    {
        free(prev_pointer);
        return 0;
    }
    free_memory_left(prev_pointer->prev);
    free(prev_pointer);
    return 0;
}

int free_memory_right(dllist *next_pointer)
{
    // Base case
    if (next_pointer->next == NULL)
    {
        free(next_pointer);
        return 0;
    }
    free_memory_right(next_pointer->next);
    free(next_pointer);
    return 0;
}