/*
 * Day 6 of the Advent of Code 2024
 * Author: Grga Palcic
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define LARGE_MATRIX

#ifdef LARGE_MATRIX
    #define MATRIX_ROWS 130
    #define MATRIX_COLS 131
    #define FILENAME "data.in"
#else
    #define MATRIX_ROWS 10
    #define MATRIX_COLS 11
    #define FILENAME "example.in"
#endif

typedef enum {
    INVALID,
    INITIAL,
    VALID,
    OBSTACLE,
    VISITED
} state_t;

typedef struct {
    int x;
    int y;
} coordinates_t;

coordinates_t directions[] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

static int is_valid_position(coordinates_t position)
{
    return position.x >= 0 && position.x < MATRIX_ROWS && position.y >= 0 && position.y < MATRIX_COLS;
}

static int check_position(char matrix[MATRIX_ROWS][MATRIX_COLS], coordinates_t position)
{
    if (!is_valid_position(position)) {
        return INVALID;
    } else if (matrix[position.x][position.y] == '.') {
        return VALID;
    } else if (matrix[position.x][position.y] == '^') {
        return INITIAL;
    } else if (matrix[position.x][position.y] == 'X') {
        return VISITED;
    } else {
        return OBSTACLE;
    }
}

static void do_step(coordinates_t *position, coordinates_t dir)
{
    position->x += dir.x;
    position->y += dir.y;
}

static void undo_step(coordinates_t *position, coordinates_t dir)
{
    position->x -= dir.x;
    position->y -= dir.y;
}

static int count_positions(char matrix[MATRIX_ROWS][MATRIX_COLS], coordinates_t current_position)
{
    int dir = 0;
    int unique_positions = 0;
    state_t state;
    coordinates_t position = current_position;
    coordinates_t current_direction = directions[dir];

    while (state = check_position(matrix, position)) {
        switch (state) {
            case VALID:
            case INITIAL:
                matrix[position.x][position.y] = 'X';
                do_step(&position, current_direction);
                unique_positions++;
                break;
            case OBSTACLE:
                undo_step(&position, current_direction);
                dir = (dir + 1) % 4;
                current_direction = directions[dir];
                break;
            case VISITED:
                do_step(&position, current_direction);
                break;
            case INVALID:
                return unique_positions;
        }
    }

    return unique_positions;
}

int main(void)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file data.in\n");
        return 1;
    }

    char buffer[MATRIX_ROWS][MATRIX_COLS];
    coordinates_t position = {0, 0};

    for (int i = 0; i < MATRIX_ROWS; i++) {
        fgets(buffer[i], MATRIX_COLS+1, file);
        for (int j = 0; j < MATRIX_COLS; j++) {
            if (buffer[i][j] == '^') {
                position = (coordinates_t){i, j};
            }
        }
    }

    fclose(file);

    printf("\nUnique: %d\n", count_positions(buffer, position));

    return 0;
}