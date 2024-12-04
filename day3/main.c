/*
 * Day 3 of the Advent of Code 2024
 * Author: Grga Palcic
 *
 * Regex who? I don't know her. I'm just gonna parse this input manually.
 * 
 * Jokes aside, I'm gonna read the file line by line and parse the input, should be easy.
 * Using strstr in combination with sscanf to find the mul function, confirm it's validity and then multiply the two numbers.
 * 
 * It was easy, until part 2, turns out whole file should be treated as one "continuous corrupted memory space",
 * instead of every line being a separate memory space. Kept getting "your result is too high" :(
 * 
 * Just load it whole in a buffer and parse it. fread ballz to the wall
 * VERY UNSAFE CODE INCOMING...
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    DO = 1,
    DO_NOT = 0
} condition;

/* why not lol */
static int multiply(int a, int b)
{
    return a * b;
}

/* part 1 */
static int parse_input(char *line)
{
    char *processing = line;
    int a, b;
    int result = 0;
    
    while ((processing = strstr(processing, "mul("))) {
        char open_bracket, close_bracket, comma;
        if (sscanf(processing, "mul%c%d%c%d%c", &open_bracket, &a, &comma, &b, &close_bracket) == 5 && 
            open_bracket == '(' && comma == ',' && close_bracket == ')') {
                result += multiply(a, b);
        }
        processing++;
    }
    return result;
}

/* part 2 */
static int conditioned_parse_input(char *line)
{
    char *last_processed = line;
    char *processing = NULL;
    char *dont_pos = NULL;
    char *do_pos = NULL;
    int condition = DO;
    int a, b;
    int result = 0;
    
    while ((processing = strstr(last_processed, "mul("))) {
        char open_bracket, close_bracket, comma;
        
        do_pos = strstr(last_processed, "do()");
        dont_pos = strstr(last_processed, "don't()");

        if (do_pos && do_pos < processing) {
            condition = DO;
        }

        if (dont_pos && dont_pos < processing) {
            condition = DO_NOT;
        }

        if (sscanf(processing, "mul%c%d%c%d%c", &open_bracket, &a, &comma, &b, &close_bracket) == 5 &&
            open_bracket == '(' && comma == ',' && close_bracket == ')') {
            if (condition) {
                result += multiply(a, b);
            }
        }
        last_processed = ++processing;
    }
    return result;
}

/* I KNOW ITS UNSAFE */
int main(void)
{
    FILE *file = fopen("data.in", "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file data.in\n");
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    char *content = (char*)malloc((fileSize + 1) * sizeof(char));
    /* check if allocated, i wont */

    /* read file and terminate */
    fread(content, sizeof(char), fileSize, file);
    content[fileSize] = '\0';

    printf("Result: %d\n", parse_input(content));
    printf("Conditioned result: %d\n", conditioned_parse_input(content));

    fclose(file);

    return 0;
}