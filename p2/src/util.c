#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "util.h"

// Parses a command from stdin
int nextCommand(State* state, char* msg) {
    while (1) {
        scanf("%c", &state->c);
        char c = state->c;
        if (c == ' ' || c == '\t' || c == '\n') {
            continue;
        }
        if (c == 'S' || c == 's') {
            state->shouldPrint = 1;
            strcpy(msg, "S\0");
            printf("COMMAND: S\n");
            return 0;
        }
        if (c == 'W' || c == 'w') {
            strcpy(msg, "W\0");
            return 1;
        }
        if (c == 'P' || c == 'p') {
            int matches = scanf("%d", &state->s);
            matches += scanf("%d", &state->t);
            matches += scanf("%d", &state->f);
            if (matches == 3) {
                state->shouldPrint = 1;
                state->c = 'P';
                sprintf(msg, "P %d %d %d", state->s, state->t, state->f);
                return 1;
            }
        }

        if (c == 'R' || c == 'r') {
            strcpy(msg, "R\0");
            return 1;
        }

        state->c = (char)0;
        strcpy(msg, "INVALID COMMAND!\0");
        return 1;
    }
}

// Reads and parses HEAPInput file
Error ReadFile(Vertex** vertices, int* n_v, int* n_e) {
    // Open the file for reading
    FILE* fptr = fopen("Ginput.txt", "r");

    // Check if file open was successful
    if (fptr == NULL) {
        return CannotOpenFile();
    }

    // Scan first line for number of elements
    int matches = fscanf(fptr, "%d %d\n", n_v, n_e);
    // If we didn't find a number, then return an error
    if (matches == 0) {
        return InputFileEmpty();
    }

    // Allocate an array of the proper size based on first line
    (*vertices) = malloc(sizeof(Vertex) * (*n_v));
    for (int i = 0; i < (*n_v); i++) {
        (*vertices)[i] = (Vertex){-1, INT_MAX, NULL};
    }

    // Scan the number of lines for each edge
    int node, neighbor, weight;
    for (int i = 0; i < (*n_e); i++) {
        int matches = fscanf(fptr, "%d %d %d\n", &node, &neighbor, &weight);
        // If we find a match, add it to the graph; else return an error
        if (matches > 0) {
            node--;
            neighbor--;
            AddEdge((*vertices), node, neighbor, weight);
        } else {
            return TooFewElements();
        }
    }

    // Close the file
    fclose(fptr);

    return Ok();
}