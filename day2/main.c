/*
 * Day 2 of the Advent of Code 2024
 * Author: Grga Palcic
 *
 * Construct linked list from input data and check if levels aare safe and dampaened safe
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAFE                1
#define UNSAFE              0
#define MAX_LINE_LENGTH     1024

typedef struct node_s {
    int data;
    struct node_s *next;
} node_t;


void free_list(node_t *head)
{
    while(head){
        node_t *tmp = head;
        head = head->next;
        free(tmp);
    }
}

static node_t* new_node(int data)
{
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node)
        return NULL;
        
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

static int safety_check(node_t *head)
{
    int decreasing = 0, increasing = 0;
    node_t *tmp_head = head;
    
    while  (tmp_head->next) {
        int diff_decreasing = tmp_head->data - tmp_head->next->data;
        int diff_increasing = tmp_head->next->data - tmp_head->data;
        
        if (diff_decreasing < 1 || diff_decreasing > 3)
            decreasing = 1;

        if (diff_increasing < 1 || diff_increasing > 3)
            increasing = 1;

        if (decreasing && increasing)
            return UNSAFE;
        
        tmp_head = tmp_head->next;
    }

    return (increasing || decreasing) ? SAFE : UNSAFE;
}

static int dampened_safety_check(node_t *head)
{
    if (safety_check(head) == SAFE)
        return SAFE;

    node_t *current = head;
    node_t *prev = NULL;

    while (current) {
        if (prev)
            prev->next = current->next;
        else
            head = current->next;

        int safety = safety_check(head);

        if (prev)
            prev->next = current;
        else
            head = current;

        if (safety == SAFE)
            return SAFE;

        prev = current;
        current = current->next;
    }

    return UNSAFE;
}

int main(void)
{
    FILE *file = fopen("data.in", "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file data.in\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int number_of_safe_reports = 0;
    int number_of_dampened_safe_reports = 0;

    while (fgets(line, sizeof(line), file)) {
        node_t *head_of_levels = NULL;
        node_t *tail_of_levels = NULL;
        
        for (char *token = strtok(line, " "); token; token = strtok(NULL, "  "))  {
            node_t *new =  new_node(atoi(token));

            if (!head_of_levels) {
                head_of_levels = new;
                tail_of_levels = head_of_levels;
            } else {
                tail_of_levels->next = new;
                tail_of_levels = tail_of_levels->next;
            }
        }
        
        if (safety_check(head_of_levels))
            number_of_safe_reports++;

        if (dampened_safety_check(head_of_levels))
            number_of_dampened_safe_reports++;

        free_list(head_of_levels);
    }

    fclose(file);

    printf("Number of safe reports: %d\n", number_of_safe_reports);
    printf("Number of dampened safe reports: %d\n", number_of_dampened_safe_reports);

    return 0;
}