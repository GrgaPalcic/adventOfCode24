/*
 * Day 1 of the Advent of Code 2024
 * Author: Grga Palcic
 *
 * Constructs two lists of integers baased on data from a file, sorts them,
 * and computes the 'distance' and 'similarity score' between the lists.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct node_s {
    int data;
    struct node_s *next;
} node_t;

/* just freeing memory allocated for a linked list */
void free_list(node_t *head)
{
    while(head){
        node_t *tmp = head;
        head = head->next;
        free(tmp);
    }
}

/* create new node so i dont repeat myself */
static node_t* new_node(int data)
{
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node)
        return NULL;
        
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

/* sorts a linked list in ascending order */
static node_t *sort(node_t* head)
{
    node_t *outer, *inner;
    int temp;
    
    if (!head)
        return head;

    for (outer = head; outer->next; outer = outer->next) {
        for (inner = outer->next; inner; inner = inner->next) {
            if (outer->data > inner->data) {
                temp = outer->data;
                outer->data = inner->data;
                inner->data = temp;
            }
        }
    }

    return head;
}

/* computes abs distance (difference) between two linked lists */
static long int distance (node_t *left_head, node_t *right_head)
{
    node_t *tmp_left = left_head;
    node_t *tmp_right = right_head;
    long int diff = 0;
    
    while (tmp_left && tmp_right) {
        diff += labs(tmp_left->data - tmp_right->data);
        tmp_left = tmp_left->next;
        tmp_right = tmp_right->next;
    }

    return diff;
}

/* computes similarity score between two linked lists */
static long int similarity(node_t *left_head, node_t *right_head)
{
    node_t *tmp_left, *tmp_right;
    long int score = 0;
    
    for (tmp_left = left_head; tmp_left; tmp_left = tmp_left->next) {
        int count = 0;

        for (tmp_right = right_head; tmp_right; tmp_right = tmp_right->next) {
            if (tmp_left->data == tmp_right->data)
                count++;
        }

        score += tmp_left->data * count;
    }

    return score;
}

int main(void)
{    
    node_t *left_tail = NULL;
    node_t *left_head = NULL;
    node_t *right_tail = NULL;
    node_t *right_head = NULL;
    int a, b;
    int first_read = 1;

    FILE *file = fopen("data.in", "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file data.in\n");
        return 1;
    }

    while (fscanf(file, "%d %d", &a, &b) == 2) {
        node_t *left_new = new_node(a);
        node_t *right_new = new_node(b);
        /* pretend to check if malloc failed */

        if (first_read) {
            left_head = left_new;
            right_head = right_new;
            left_tail = left_head;
            right_tail = right_head;
            first_read = 0;
        } else {
            left_tail->next = left_new;
            left_tail = left_tail->next;

            right_tail->next = right_new;
            right_tail = right_tail->next;
        }
    }

    fclose(file);

    /* is anything reda? */
    if (first_read) {
        fprintf(stderr, "Error: No data read from file\n");
        return 1;
    }

    /* sort the lists */
    left_head = sort(left_head);
    right_head = sort(right_head);

    /* compute and print results */
    printf("Difference: %ld\n", distance(left_head, right_head));
    printf("Similarity: %ld\n", similarity(left_head, right_head));

    /* free the memory */
    free_list(left_head);
    free_list(right_head);

    return 0;
}