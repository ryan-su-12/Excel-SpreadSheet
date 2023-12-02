#ifndef ASSIGNMENT_MODEL_H
#define ASSIGNMENT_MODEL_H

#include "defs.h"
#define EMPTY_CELL_VALUE 0.0
#define MAX_ROWS 11 // maximum rows of the speadsheet
#define MAX_COLS 8


typedef enum _CellType {
    Number,
    Formula,
    Text
} CellType;

 typedef struct _Cell {
        CellType cellType;
        union { // union is used so the data shares the same memory
            double number; // if isNumber = 1, the number is stored here
            char text[128]; // if isNumber = 0, store the text here
        } cellContent;
    } Cell;

// Initializes the data structure.
//
// This is called once, at program start.
void model_init();
int getValue(char *text, double *value );
// Sets the value of a cell based on user input.
//
// The string referred to by 'text' is now owned by this function and/or the
// cell contents data structure; it is its responsibility to ensure it is freed
// once it is no longer needed.
void set_cell_value(ROW row, COL col, char *text);

// Clears the value of a cell.
void clear_cell(ROW row, COL col);

// Gets a textual representation of the value of a cell, for editing.
//
// The returned string must have been allocated using 'malloc' and is now owned
// by the interface. The cell contents data structure must not modify it or
// retain any reference to it after the function returns.
char *get_textual_value(ROW row, COL col);

#endif //ASSIGNMENT_MODEL_H
