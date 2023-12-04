#include "model.h"
#include "interface.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Cell cells[MAX_ROWS][MAX_COLS];

//static Cell cells[MAX_ROWS][MAX_COLS];

void model_init() {
    // TODO: implement this.
    // organizing the contents of the cells in a single datatructure being a typedef struct
    // defining a structure to respresent a cell since it can holdmultiple variable types
    //implemented so that the initial "blank" blank state of the spreadsheet is initilaized with 0.0 and considers them all numbers
    for (int row = 0; row < MAX_ROWS; ++row) {
        for (int col = 0; col < MAX_COLS; ++col) {
            cells[row][col].cellType = Number; // Initially, consider them all as numbers
            cells[row][col].cellContent.number = EMPTY_CELL_VALUE; // Initialize with 0.0
        }
    }
}

void findTypeCell(Cell *cell){
    // take the input text and figure out what it is
    //they point to the first charcter of the string
    if((*cell).cellContent.text[0]== '='){
        (*cell).cellType = Formula;
        return;
    }

    double temp = atof((*cell).cellContent.text); // Gives us the value but no indication of if it is accurate
    
    if (temp == 0.0) {
        // Could be a zero, or invalid text
        int decimalPointCount = 0;
        int i = 0;
        // do while loop iterates through the text 
        do {
            if ((*cell).cellContent.text[i] == '.') decimalPointCount++; // just to see the # of decmials being counted

            else if((*cell).cellContent.text[i] != '0' || decimalPointCount > 1) { // if the index of the text is not equal to 0 we automatically know its text, 
            // if the decimal counter is greater than 1 it also means it is text since there cannont be 2 decimals in a number
                (*cell).cellType = Text;
                return;
            }
            i ++;
        } while((*cell).cellContent.text[i+1] != '\0'); // do this 
    }

    (*cell).cellType = Number;
    (*cell).cellContent.number = temp;

    
}


int computeFormula(Cell *cell, double *value) {
    double sum = 0;  // Variable to store the sum of cell values
    int oldie = 1;    // Index to keep track of the starting position of the current cell value
    int plus = 0;     // Index to find the position of the next '+' character in the cell content

    // Iterate through the cell content until the end is reached
    while ((*cell).cellContent.text[plus] != '\0') {
        char sell[3];  // Array to store the characters representing the current cell reference

        // Find the position of the next '+' character or the end of the cell content
        for (int i = plus + 1; 1; i++) {
            if ((*cell).cellContent.text[i] == '+' || (*cell).cellContent.text[i] == '\0') {
                plus = i;
                break;
            }
        }

        // take the characters representing the current cell reference
        for (int i = oldie; i < plus; i++) {
            if ((*cell).cellContent.text[i] != ' ' && (*cell).cellContent.text[i] != '+') {
                sell[0] = (*cell).cellContent.text[i];
                sell[1] = (*cell).cellContent.text[i + 1];
                sell[2] = (*cell).cellContent.text[i + 2];
                break;
            }
        }

        oldie = plus;  // Update the starting position for the next iteration
        double sellValue;

        // Call the 'getValue' function to get the number value of the current cell "sell"
        if (!getValue(sell, &sellValue))
            return 0;  // Return 0 if 'getValue' fails

        sum += sellValue;  // Add the obtained value to the overall sum
    }

    *value = sum;  // Store the final sum in the 'value' variable
    return 1;      // Return 1 to indicate successful computation
}


int getValue(char *text, double *value ){
    int row, col;

    //columns are assigned to letters on the spreadsheet
    if (text[0] == 'A'){
        col = 0;
    } else if (text[0] == 'B'){
        col = 1;
    } else if (text[0] == 'C'){
        col = 2;
    } else if (text[0] == 'D'){
        col = 3;
    } else if (text[0] == 'E'){
        col = 4;
    } else if (text[0] == 'F'){
        col = 5;
    } else if (text[0] == 'G'){
        col = 6;
    } else{
        return 0;
    }

    // rows are assigned to numbers on the spreadsheet
    if (text[1] == '1' && text[2] == '0'){
        row = 9;
    } else if (text[1] == '1'){
        row = 0;
    } else if (text[1] == '2'){
        row = 1;
    }else if (text[1] == '3'){
        row = 2;
    }else if (text[1] == '4'){
        row = 3;
    }else if (text[1] == '5'){
        row = 4;
    }else if (text[1] == '6'){
        row = 5;
    }else if (text[1] == '7'){
        row = 6;
    }else if (text[1] == '8'){
        row = 7;
    }else if (text[1] == '9'){
        row = 8;
    } else {
        return 0;
    }

    //Determines how to procced based on the textual value of the cell

    if (cells[row][col].cellType == Text) {
        return 0;
    } 
    if (cells[row][col].cellType == Number){
        *value = cells[row][col].cellContent.number;
        return 1;
    }

    
    if  (cells[row][col].cellType = Formula){
        return computeFormula(&(cells[row][col]), value);
    }
    return 0;
}

char *cellText(Cell* cell) {


    if ((*cell).cellType == Text){
        char* copy = (char*)malloc(128*sizeof(char));
        strcpy(copy, (*cell).cellContent.text);
        return copy;
    }
    if ((*cell).cellType == Formula){
        char* string = (char*)malloc(128*sizeof(char));
        double value;

        //calculates the number value of the cell
        if (!computeFormula(cell, &value)) {
            return "ERROR, FORMULA INVALID";
        }
        //converts numbers to a string
        sprintf(string,"%f", value);
        return string;
    }


    char* string = (char*)malloc(128*sizeof(char));
    sprintf(string,"%f", (*cell).cellContent.number);
    return string;



}
void updateFormulas() {
    for (int row = 0; row < MAX_ROWS; ++row) {
        for (int col = 0; col < MAX_COLS; ++col) {
            
            //check if the cell contains a formula
            if(cells[row][col].cellType == Formula) {
                update_cell_display(row,col, cellText(&(cells[row][col])));
            }
        }
    }
}

void set_cell_value(ROW row, COL col, char *text) {
    // TODO: implement this.
    
    if (row >= 0 && row < MAX_ROWS && col >=0 && col < MAX_COLS){
        cells[row][col].cellType = Text;
        strcpy(cells[row][col].cellContent.text, text);
        findTypeCell(&(cells[row][col]));
        
    // This just displays the text without saving it in any data structure. You will need to change this.
        updateFormulas();
        update_cell_display(row, col, cellText(&(cells[row][col])));

    //how to save into data strucutre?
        free(text);
    }
}    


void clear_cell(ROW row, COL col) {
    // TODO: implement this.
     if (row >= 0 && row < MAX_ROWS && col >= 0 && col < MAX_COLS) {
        cells[row][col].cellType = Number;                           // Set as number cell
        cells[row][col].cellContent.number = EMPTY_CELL_VALUE;  // Clears the value of the cell

    // This just clears the display without updating any data structure. You will need to change this.
    updateFormulas();
    update_cell_display(row, col, "");
    }
}

char *get_textual_value(ROW row, COL col) {
    // TODO: implement this.
    char* copy = (char*)malloc(128*sizeof(char));
    if(cells[row][col].cellType == Number) {
        sprintf(copy,"%f", cells[row][col].cellContent.number);
    } else {
        strcpy(copy, cells[row][col].cellContent.text);
    }
    
    return copy;
}
