/*
 * Day 5 of the Advent of Code 2024
 * Author: Grga Palcic
 *
 * Loaded rules in hash map and data into several linked lists, sorted pages using stdlib qsort,
 * custom compare function, added middle page from every 'update'.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FULL_DATA

#ifdef FULL_DATA
    #define FILENAME "data.in"
    #define BUFFER_SIZE 128
    #define HASH_MAP_SIZE 1181 /* prime number */
#else
    #define FILENAME "example.in"
    #define BUFFER_SIZE 128
    #define HASH_MAP_SIZE 21
#endif

typedef struct hash_entry_s {
    int key;
    struct hash_entry_s* next;
} hash_entry_t;

typedef struct {
    hash_entry_t* entries[HASH_MAP_SIZE];
} hash_map_t;

static hash_map_t rules_map;

static void hash_map_init(hash_map_t* map)
{
    memset(map->entries, 0, sizeof(map->entries));
}

/* cantor pairing, providing unique key per pair */
static int calculate_key(int pred, int succ)
{
    return (pred + succ) * (pred + succ + 1) / 2 + succ;
}

static void hash_map_insert(hash_map_t* map, int key)
{
    int index = key % HASH_MAP_SIZE;
    hash_entry_t* new_entry = (hash_entry_t*)calloc(1, sizeof(hash_entry_t));
    new_entry->key = key;
    new_entry->next = map->entries[index];
    map->entries[index] = new_entry;
}

static int hash_map_search(hash_map_t* map, int key)
{
    int index = key % HASH_MAP_SIZE;
    hash_entry_t* current = map->entries[index];

    while (current) {
        if (current->key == key) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

static void free_hash_map(hash_map_t* map)
{
    for (int i = 0; i < HASH_MAP_SIZE; i++) {
        hash_entry_t* current = map->entries[i];
        while (current) {
            hash_entry_t* temp = current;
            current = current->next;
            free(temp);
        }
        map->entries[i] = NULL;
    }
}

/* compares two integers against rules, if found to deviate, return 1, otherwise return -1 */
static int compare(const void* a, const void* b)
{
    int int_a = *(int*)a;
    int int_b = *(int*)b;

    int key = calculate_key(int_a, int_b); // Check if rule b -> a exists
    return hash_map_search(&rules_map, key) ? -1 : 1;
}

/* checks if the array is ordered */
static int is_ordered(int* updates, int size, int (*comp)(const void*, const void*))
{
    for (int i = 0; i < size - 1; i++) {
        if (comp(&updates[i], &updates[i+1]) == 1) {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file data.in\n");
        return 1;
    }

    char buffer[BUFFER_SIZE];

    hash_map_init(&rules_map);

    /* load rules */
    while (strcmp(fgets(buffer, BUFFER_SIZE, file), "\n")) {
        int pred, succ;
        sscanf(buffer, "%d|%d", &pred, &succ);
        hash_map_insert(&rules_map, calculate_key(pred, succ));
    }

    int sorted_sum = 0;
    int unsorted_sum = 0;

    /* load update, proccess pages, add middle pages */
    while (fgets(buffer, BUFFER_SIZE, file)) {
        char *temp = strdup(buffer);
        int updates_size = 0;

        /* count number of updates */
        for (char *tok = strtok(temp, ","); tok; tok = strtok(NULL, ",")) {
            updates_size++;
        }

        free(temp);

        /* allocate array for updates */
        int *updates = (int*)calloc(updates_size, sizeof(int));
        int tmp_count = 0;

        for (char *tok = strtok(buffer, ","); tok; tok = strtok(NULL, ",")) {
            updates[tmp_count++] = strtol(tok, NULL, 10);
        }
        
        /* sort updates if not sorted and add middle value to the result */
        if (!is_ordered(updates, updates_size, compare)) {
            qsort(updates, updates_size, sizeof(int), compare);
            unsorted_sum += updates[updates_size/2];
        } else {
            sorted_sum += updates[updates_size/2];
        }

        free(updates);
    }

    free_hash_map(&rules_map);
    fclose(file);

    printf("\n");
    printf("Added sorted updates: %d\n", sorted_sum);
    printf("Added unsorted updates: %d\n", unsorted_sum);

    return 0;
}