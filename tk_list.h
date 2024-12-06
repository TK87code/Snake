/**
 * @file tk_list.h
* @brief This is a implementation of singly linked list in C.
* @author Takuya Kimura (https://github.com/TK87code)
 * @date December 4th 2024 7:11 pm 
*/
#ifndef TK_LIST_H
#define TK_LIST_H

#ifndef NULL
#define NULL 0
#endif //NULL

typedef struct tk_node_t{
    void *data;
    struct tk_node_t *next;
}tk_node_t;

/**
* @brief Create a head node of a singly linked list.
 * @param data A ptr to the data to hold in the list node.
* @return A ptr to the head node of the list.
*/
extern tk_node_t* tk_list_create(void *data);

/**
* @brief Delete the list. This function will free all allocated memory for the list, and set the list ptr to NULL.
* @param list A double ptr to the head node of the list.
*/
extern void tk_list_delete(tk_node_t **list);

/**
* @brief Insert new node at the front of the list.
* @param list A double ptr to the head node.
* @param data A ptr to the data to insert.
* @return return 0 on success, -1 on malloc error.
*/
extern int tk_list_push_front(tk_node_t **list, void *data);

/**
* @brief Delete the node at the front.
* @param list A double ptr to the head node.
*/
extern void tk_list_pop_front(tk_node_t **list);

/**
* @brief Insert a new node at the end of the list.
* @param list A ptr to the head node.
* @param data A ptr to the data to insert.
* @return return 0 on success, -1 on malloc error.
*/
extern int tk_list_push_back(tk_node_t *list, void* data);

/**
* @brief Delete the node at the end of the list.
* @param list A ptr to the head node.
*/
extern void tk_list_pop_back(tk_node_t *list);

/**
* @brief Return the node at the end of the list.
* @param list A ptr to the head node.
* @return A ptr to the node at the end of the list.
*/
extern tk_node_t* tk_list_get_back(tk_node_t *list);

/**
* @brief Find a node that contains the data passed in the param.
* @param list A ptr to the head node.
* @param data A ptr to the data to find.
* @return A ptr to the node that contains the data passed in the param. If the function could not find the node, returns NULL.
*/
extern tk_node_t* tk_list_find(tk_node_t *list, void *data);

/**
* @brief Insert a new node after the node which contains the data passed in the param.
* @param list A ptr to the head node.
* @param data_to_insert A ptr to the data to insert.
* @param data_to_find A ptr to the data to find.
* @return Return 0 on success, -1 on malloc error.
*/
extern int tk_list_insert_after(tk_node_t *list, void* data_to_insert, void *data_to_find);

#endif //TK_LIST_H
