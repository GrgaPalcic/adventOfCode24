/*
 * Day 5 of the Advent of Code 2024
 * Author: Grga Palcic
 *
 * Loaded all data into several linked lists, sorted pages and returned middle page from every 'update'.
 * Went overboard with pointers...no idea why
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 100
#define FULL_DATA

#ifdef FULL_DATA
    #define FILENAME "data.in"
#else
    #define FILENAME "example.in"
#endif

typedef struct generic_node_s{
    void *data;
    size_t size;
    struct generic_node_s *next;
} generic_node_t;

static generic_node_t* new_generic_node(void *data, size_t data_size)
{
    generic_node_t *new_node = (generic_node_t*)calloc(1, sizeof(generic_node_t));
    if (!new_node) {
        return NULL;
    }

    new_node->data = malloc(data_size);
    if (!new_node->data) {
        free(new_node);
        return NULL;
    }

    memcpy(new_node->data, data, data_size);
    new_node->size = data_size;
    new_node->next = NULL;

    return new_node;
}

static void free_generic_list(generic_node_t *head)
{
    while(head){
        generic_node_t *temp = head;
        head = head->next;
        free(temp->data);
        free(temp); 
    }
}

typedef struct {
    int pred;
    int succ;
} rule_pair_t;

/* compares two integers against rules, if found to deviate, return 1, otherwise return 0 */
static int compare(int a, int b, generic_node_t *rules_list)
{
    generic_node_t *current = rules_list;
    while (current) {
        rule_pair_t *rule = (rule_pair_t*)current->data;
        if (rule->pred == b && rule->succ == a) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

/* swaps data of two nodes */
static void swap_nodes_data(generic_node_t *a, generic_node_t *b) {
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
}

/**
 * basic bubble sort on the updates list, uses custom compare function
 * return: pointer to the int array of size 2 containing the middle page of the sorted and unsorted list (after sorting)
 */
static int* sort_pages(generic_node_t *updates, generic_node_t *rules_list)
{
    int is_sorted = 0;
    int count = 0;
    int *result = (int*)calloc(2, sizeof(int));

    generic_node_t *outer_node, *inner_node;
    for (outer_node = updates; outer_node; outer_node = outer_node->next) {
        for (inner_node = outer_node->next; inner_node; inner_node = inner_node->next) {
            int outer_val = *(int*)outer_node->data;
            int inner_val = *(int*)inner_node->data;

            if (compare(outer_val, inner_val, rules_list)) {
                swap_nodes_data(outer_node, inner_node);
                is_sorted = 1;
            }
        }
        count++;
    }

    generic_node_t *current = updates;
    for (int i = 0; i < count/2; i++) {
        current = current->next;
    }
    result[is_sorted] = *(int*)(current->data);

    return result;
}

int main(void)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file data.in\n");
        return 1;
    }

    char buffer[BUFFER_SIZE];

    generic_node_t *rules = NULL;
    generic_node_t *rules_tail = NULL;

    /* load rules */
    while (strcmp(fgets(buffer, BUFFER_SIZE, file), "\n")) {
        rule_pair_t rule;
        sscanf(buffer, "%d|%d", &rule.pred, &rule.succ);

        if (!rules_tail) {
            rules = new_generic_node(&rule, sizeof(rule_pair_t));
            rules_tail = rules;
        } else {
            rules_tail->next = new_generic_node(&rule, sizeof(rule_pair_t));
            rules_tail = rules_tail->next;
        }
    }

    int *result = NULL;
    int sorted_sum = 0;
    int unsorted_sum = 0;
    float startTime = (float)clock()/CLOCKS_PER_SEC;

    /* load update, proccess pages, add middle pages */
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        generic_node_t *updates = NULL;
        generic_node_t *updates_tail = NULL;

        for (char *tok = strtok(buffer, ","); tok; tok = strtok(NULL, ",")) {
            int val = strtol(tok, NULL, 10);
            if (!updates_tail) {
                updates = new_generic_node(&val, sizeof(int));
                updates_tail = updates;
            } else {
                updates_tail->next = new_generic_node(&val, sizeof(int));
                updates_tail = updates_tail->next;
                updates->size = updates_tail->size;
            }
        }
        
        result = sort_pages(updates, rules);
        sorted_sum += result[0];
        unsorted_sum += result[1];
        free_generic_list(updates);
    }
    
    free_generic_list(rules);
    fclose(file);

    printf("\n");
    printf("Added sorted updates: %d\n", sorted_sum);
    printf("Added unsorted updates: %d\n", unsorted_sum);

    return 0;
}