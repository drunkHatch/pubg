/* CMPUT379 Winter 2018
   Author: Mark Stevens
*/

#include <stdio.h>
#include <stdlib.h>

// This simple program shows two ways of dynamically creating and allocating
// space for a two dimensional array of integers. This same approach could be
// used for pointers to any other datatype, including pointers to structs.
// Please read the comments and analyze the code carefully.


int main(){
    // Number of rows in our two dimensional arrays
    int rows = 3;
    // Number of colums in our two dimensional arrays
    int cols = 5;

    // sBoard is our single pointer two dimensional array.
    // Declare and allocate space.
    int * sBoard = malloc(sizeof(int) * (rows * cols));

    // dBoard is our double pointer two dimensional array.
    // First you malloc space for your rows, which are essentially pointers
    // to ints. Notice the sizeof(int *). This means "size of integer pointer".
    int ** dBoard = malloc(sizeof(int *) * rows);

    // At this point, dBoard[0], dBoard[1], and dBoard[2] contain integer
    // pointers. They are not actually pointing to anything useful, however. We
    // will handle this next.

    // Now that we have malloced space for our rows (pointers to ints), we
    // loop over each row and allocate enough space for each actual integer
    // in that row. This corresponds to the number of ints in the column.
    int i;
    for (i = 0; i < rows; i++){
        dBoard[i] = malloc(sizeof(int) * cols);
    }

    // It is important to note that at this point we can now index dBoard
    // the same way as a normal two-dimensional array. dBoard[ ][ ].
    // Alternatively, sBoard must be indexed like a one-dimensional array,
    // i.e., sBoard[ ].

    // Load up both arrays
    int j;
    int count = 0;
    for (i = 0; i < rows; i++){
        for (j = 0; j < cols; j++){
            sBoard[(i*cols) + j] = count;  // Notice the funky indexing
            dBoard[i][j] = count;
            count++;
        }
    }

    // Print off values in both arrays
    printf("Number of rows: %d, Columns: %d\n\n",rows,cols);
    for (i = 0; i < rows; i++){
        for (j = 0; j < cols; j++){
            printf("sBoard[(%d*%d) + %d] = %d\n",i,cols,j,
                sBoard[(i*cols) + j]);
            printf("dBoard[%d][%d] = %d\n\n",i,j,dBoard[i][j]);
        }
    }

    // Free up memory for single pointer array
    free(sBoard);

    // Free up memory for double pointer array
    for (i = 0; i < rows; i++){
        free(dBoard[i]);
    }
    free(dBoard);

    return 0;
}
