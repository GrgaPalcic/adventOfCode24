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

static int xmas_check(char matrix[MATRIX_ROWS][MATRIX_COLS], int row, int col, char *str, int x_pos, int y_pos)
{
    int str_len = strlen(str);
    int count = 0;

    for (int dir = 0; dir < 8; dir++) {
        int x = x_pos;
        int y = y_pos;
        int char_count;
        
        for (char_count = 0; char_count < str_len; char_count++) {
            if (x < 0 || x > row || y < 0 || y > col) {
                break;
            }

            if (matrix[x][y] != str[char_count]) {
                break;
            }

            x += directions[dir].x;
            y += directions[dir].y;
        }

        if (char_count == str_len) {
            count++;
        }
    }

    return count;
}

const int dx[] = {-1, -1, 1, 1};
const int dy[] = {-1, 1, -1, 1};

static int x_mas_check(char matrix[MATRIX_ROWS][MATRIX_COLS], int row, int col, int x_pos, int y_pos)
{
    int m_count = 0;
    int s_count = 0;
    direction_t m_pos[2];

    if (matrix[x_pos][y_pos] == 'A') {
        for (int i = 0; i < 4; i++) {
            if (matrix[x_pos + dx[i]][y_pos + dy[i]] == 'M') {
                m_pos[m_count].x = dx[i];
                m_pos[m_count].y = dy[i];
                m_count++;
            } else if (matrix[x_pos + dx[i]][y_pos + dy[i]] == 'S') {
                s_count++;
            }
        }
    }

    if (m_count == 2 && s_count == 2 && (m_pos[0].x * -1 != m_pos[1].x || m_pos[0].y * -1 != m_pos[1].y)) {
        return 1;
    }

    return 0;
}

int main(void)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file data.in\n");
        return 1;
    }

    char buffer[MATRIX_ROWS][MATRIX_COLS];

    for (int i = 0; i < MATRIX_ROWS; i++) {
        fgets(buffer[i], MATRIX_COLS+1, file);
    }

    fclose(file);

    printf("\nXMAS: %d\n", xmas(buffer, MATRIX_ROWS, MATRIX_COLS));
    printf("X-MAS: %d\n", x_mas(buffer, MATRIX_ROWS, MATRIX_COLS));

    return 0;
}