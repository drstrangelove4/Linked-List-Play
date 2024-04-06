// Headers

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// --------------------------------------------------------------------------------------------------------------------

// Custom Types

typedef struct dllist
{
    int number;
    bool is_root;
    struct dllist *prev;
    struct dllist *next;
} dllist;

typedef struct
{
    int return_int;
    int return_code;
    dllist *node;
} return_object;

// --------------------------------------------------------------------------------------------------------------------

// Signatures

dllist *change_root(dllist *current_root, int new_root_value);
dllist *new_node(int value, dllist *next_pointer, dllist *previous_pointer);

int free_memory_left(dllist *prev_pointer);
int free_memory_right(dllist *next_pointer);

return_object search(dllist *root, int search);
return_object search_left(dllist *root, int search);
return_object search_right(dllist *root, int search);

void append_left(dllist *new_node, dllist *left_node);
void append_right(dllist *new_node, dllist *right_node);
void delete_node(int node_number, dllist *current_root);
void edit_node(dllist *root, int get, int edit);
void free_memory(dllist *root);
void insert_node(dllist *before_node, dllist *after_node);
void print_next(dllist *next);
void print_previous(dllist *previous);

// --------------------------------------------------------------------------------------------------------------------

int main()
{
    // Demo of my linked list functionality.

    // Create a root node.
    dllist *root = new_node(0, NULL, NULL);
    if (root == NULL)
    {
        return 1;
    }
    root->is_root = true;

    // Create and prepend some nodes to the chain.
    append_left(new_node(-10, NULL, NULL), root);
    append_left(new_node(-20, NULL, NULL), root);

    // Print the current chain (prev).
    print_previous(root);

    // Create and append some nodes to the chain..
    append_right(new_node(10, NULL, NULL), root);
    append_right(new_node(20, NULL, NULL), root);

    // Print the current chain (next).
    print_next(root);

    // Insert new nodes into the chain
    insert_node(root->prev, new_node(-5, NULL, NULL));
    insert_node(root, new_node(5, NULL, NULL));

    printf("Chains after inserting nodes:\n");
    print_previous(root);
    print_next(root);

    // Edit a node
    edit_node(root, 20, 25);
    edit_node(root, -20, -25);

    printf("Chains after editing nodes:\n");
    print_previous(root);
    print_next(root);

    // Delete a node
    delete_node(-25, root);
    delete_node(-10, root);
    delete_node(25, root);

    printf("Chains after deleting nodes:\n");
    print_previous(root);
    print_next(root);

    // Change the root
    root = change_root(root, -5);
    printf("The new root is: %i\n\n", root->number);

    printf("Chains after changing root\n");
    print_previous(root);
    print_next(root);

    // Clear malloc'd memory
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
    new->is_root = false;

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

void insert_node(dllist *before_node, dllist *new_node)
// Inserts a nodes between elements into the list.
{
    // Check for NULL nodes.
    if (before_node == NULL || new_node == NULL)
    {
        printf("Error: One of the nodes is NULL.\n");
        return;
    }

    // Set pointers of the new node
    new_node->next = before_node->next;
    new_node->prev = before_node;

    // Set pointers of before node to point at inserted node
    before_node->next = new_node;

    // Set pointer of next node's prev to new node
    new_node->next->prev = new_node;
}

// --------------------------------------------------------------------------------------------------------------------

// Edit node properties

dllist *change_root(dllist *current_root, int new_root_value)
{
    return_object new_root = search(current_root, new_root_value);
    // Check for errors.
    if (new_root.return_code != 0)
    {
        printf("There was no node with that value found!\n");
        printf("The root has not been changed.\n");
        return current_root;
    }
    else
    {
        // Change the root node.
        current_root->is_root = false;
        new_root.node->is_root = true;
        dllist *ptr_root = new_root.node;
        return ptr_root;
    }
}

void edit_node(dllist *root, int get, int edit)
// Searches for a node based upon the value held in that node and edits it.
{
    return_object results = search(root, get);
    if (results.return_code != 0)
    {
        printf("Node not found.\n");
        return;
    }
    results.node->number = edit;
    return;
}

// --------------------------------------------------------------------------------------------------------------------

// Search for X value

return_object search(dllist *root, int search)
// Base function. Calls for searches in both directions of the list starting from the root.
{
    // Search linked list in either direction.
    return_object left = search_left(root, search);
    return_object right = search_right(root, search);

    // If we find the object return its value and success code.
    if (left.return_int == search && left.return_code == 0)
    {
        return left;
    }
    else if (right.return_int == search && right.return_code == 0)
    {
        return right;
    }
    // If we don't find the object return an error code.
    else
    {
        return_object new_return;
        new_return.return_code = 1;
        new_return.return_int = 0;

        return new_return;
    }
}

return_object search_left(dllist *root, int search)
// Searchs the left or previous side of the list for the search term.
{
    // Base cases. Finding the number or encountering null.
    return_object new_return;
    if (root->number == search)
    {
        new_return.return_code = 0;
        new_return.return_int = root->number;
        new_return.node = root;
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
        new_return.return_int = root->number;
        new_return.node = root;
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

// Remove a singular node from the linked list.

void delete_node(int node_number, dllist *current_root)
{
    return_object search_results = search(current_root, node_number);

    // Check for valid node to delete.
    if (search_results.return_code != 0)
    {
        printf("No node with that value has been found\n");
        return;
    }

    // Check if the node is root
    if (search_results.node->is_root)
    {
        printf("Cannot delete the root node.\n");
        return;
    }

    // Set pointers
    // If we are deleting a node at the end of the chain, set the next/prev node to point to NULL.
    if (search_results.node->prev == NULL)
    {
        search_results.node->next->prev = NULL;
    }
    else if (search_results.node->next == NULL)
    {
        search_results.node->prev->next = NULL;
    }
    else
    // Change pointers of to the found nodes next/prev to each other.
    {
        // Current next's previous should point to current previous instead of self.
        search_results.node->next->prev = search_results.node->prev;
        // Current previous's next should point to current next instead of self.
        search_results.node->prev->next = search_results.node->next;
    }

    // Free the found node
    free(search_results.node);

    return;
}

// --------------------------------------------------------------------------------------------------------------------

// Free memory

void free_memory(dllist *root)
// Searches for nodes down each side of the list and frees memory used by malloc.
{
    /* We need the recursive list to return something to signal root to call free on itself.
    We need to free the list from null pointer, backwards to root to avoid breaking the chain
    and leaving nodes floating in memory.*/

    int left, right;

    // Free left side of the chain
    if (root->prev != NULL)
    {
        left = free_memory_left(root->prev);
    }
    else
    {
        // If previous is NULL then we don't need to free anything.
        left = 0;
    }

    // free right side of the chain
    if (root->next != NULL)
    {
        right = free_memory_right(root->next);
    }
    else
    {
        // If next is NULL then we don't need to free anything.
        right = 0;
    }

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

// --------------------------------------------------------------------------------------------------------------------
