#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

void dividerow(double **matrix, int n, int row, double divisor);
void showmatrix(double **matrix, int n);
void getmatrix(double **matrix, int n);

//display entire augmented matrix (calls are commented out now)
void allmatrix(double **matrix, int n){
    for(int i=0; i<n; i++){
    printf("|");
    for(int j=0; j<(2*n); j++){
        printf("(%d,%d)",i,j);
        printf("%lf ", matrix[i][j]);
    }
    printf("|\n");
    }
}

//create an empty augmented matrix for a n nxn matrix
double **newmatrix(int n) {
double **matrix;
matrix = (double**) malloc(n*sizeof(double*));
for (int i=0; i<n; i++)
   matrix[i] = (double*) malloc(2*n*sizeof(double));
   return matrix;
} 

void freematrix(double **matrix, int n){
    for (int i=0; i<n; i++){
        free(matrix[i]);
    }
    free(matrix);
}

//initialise right half of augmented matrix to nxn identity matrix
void initialise(double **matrix,int n){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            matrix[i][n+j]=0;
        }
        //allmatrix(matrix, n);
        matrix[i][n+i] = 1;
    }
    
}

//swap two rows
void swap(double **matrix, int n, int row1, int row2){
    double temp;
    for(int i=0; i<(2*n); i++){
        temp = matrix[row1][i];
        matrix[row1][i] = matrix[row2][i];
        matrix[row2][i] = temp;
    }
}

//perform a single row operation
void rowop(double **matrix, int n, int row1, int row2){
    if(matrix[row1][row1] == 0) swap(matrix, n, row1, row2);
    else{
        double multiplier = matrix[row2][row1]/matrix[row1][row1];
        for(int i=row1; i<(2*n); i++){
            matrix[row2][i]= matrix[row2][i] - multiplier*(matrix[row1][i]);
        }
    }
}

//perfomorm all row operations where the given row is the highest row (or lowest in backsub phase)
void rowloop(double **matrix, int n, int row, bool down){
    if(down){
        for(int i=(row+1); i<n; i++){
            rowop(matrix, n, row, i);
        }
    }
    else{
        for(int i=(row-1); i>=0; i--){
            dividerow(matrix, n, row, matrix[row][row]);
            rowop(matrix, n, row, i);
        }
        dividerow(matrix, n, 0, matrix[0][0]);
    }
}
    
//transform a matrix to triangular form    
void triangle(double **matrix, int n){
    for(int i=0; i<n; i++){
        rowloop(matrix, n, i, true);
    }
}

void dividerow(double **matrix, int n, int row, double divisor){
    if(divisor == 0){
        printf("matrix is not invertble%lf\n", divisor);
        exit(1);
    }
    for(int i=0; i<(2*n); i++){
        matrix[row][i] = (matrix[row][i]/divisor);
    }
} 

//transform a triangle matrix to identity (back substitution phase)
void backsub(double **matrix, int n){
    for(int i=(n-1); i>=0; i--){
        rowloop(matrix, n, i, false);
    }
}     

bool isdouble(char *array){
    int i = 1;
    bool decimal = false;
    if(!((array[0]=='-') || (((array[0] -'0')>=0) && ((array[0]-'0') < 10)))) return false;
    while(array[i] != '\n'){
        if(array[i] == '.'){
            if(decimal == true) return false;
            decimal = true;
        }
        else if(!((array[i] -'0')>=0) && (array[0]-'0' < 10)) return false;
        i++;
    }
    return true;
}
        

//find the inverse of a user given nxn matrix
void run(int n){
    double **matrix = newmatrix(n);
    initialise(matrix, n);
    getmatrix(matrix, n);
    triangle(matrix, n);
    backsub(matrix, n);
    showmatrix(matrix, n);
    //allmatrix(matrix, n);
    freematrix(matrix, n);
}

//IO
//-------------------------------------------------------------------------------------

//insert elements specified by user into matrix
void getmatrix(double **matrix, int n){
    char *array = malloc(sizeof(double)+1);
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("(%d,%d):\n", (i+1), j+1);
            fgets(array, sizeof(double)+1, stdin);
            assert(isdouble(array));
            sscanf(array, "%lf", &matrix[i][j]);
            //allmatrix(matrix, n);
        }
    }
}

void showmatrix(double **matrix, int n){
    for(int i=0; i<n; i++){
        printf("|");
        for(int j=0; j<n; j++){
            printf("%7.3lf ", matrix[i][j+n]);
        }
        printf("|\n");
    }
}

//testing and main
//-------------------------------------------------------------------------------------
void testinitialise(){
    double **matrix = newmatrix(3);
    initialise(matrix, 3);
    assert(matrix[0][3] == 1);
    assert(matrix[0][4] == 0);
    assert(matrix[1][4] == 1);
    assert(matrix[2][4] == 0);
    assert(matrix[2][5] == 1);
    freematrix(matrix, 3);
}

double **testmatrix(){
    double **matrix = newmatrix(2);
    initialise(matrix, 2);
    matrix[0][0] = 1;
    matrix[0][1] = 2;
    matrix[1][0] = 2;
    matrix[1][1] = 6;
    return matrix;
}

void testrowop(){
    double **matrix = testmatrix();
    rowop(matrix, 2, 0, 1);
    assert(matrix[0][1] == 2);
    assert(matrix[0][0] == 1);
    assert(matrix[1][0] == 0);
    assert(matrix[1][1] == 2);
    freematrix(matrix, 2);
}

void testtriangle(){
    double **matrix = testmatrix();
    triangle(matrix, 2);
    assert(matrix[0][1] == 2);
    assert(matrix[0][0] == 1);
    assert(matrix[1][0] == 0);
    assert(matrix[1][1] == 2);
    freematrix(matrix, 2);
}
    
void testbacksub(){
    double **matrix = testmatrix();
    triangle(matrix, 2);
    backsub(matrix, 2);
    assert(matrix[0][1] == 0);
    assert(matrix[0][0] == 1);
    assert(matrix[1][0] == 0);
    assert(matrix[1][1] == 1);
    assert(matrix[0][2] == 3);
    assert(matrix[0][3] == -1);
    assert(matrix[1][2] == -1);
    assert(matrix[1][3] == 0.5);
    freematrix(matrix, 2);
}

void test(){
    testinitialise();
    testrowop();
    testtriangle();
    testbacksub();
    printf("Tests Pass\n");
}


int main(int n, char **argv){
    if(n==1) test();
    else if(n==2){
        int n = atoi(argv[1]);
        assert(n != 0);//note:atoi of a non-numeric character returns zero
        run(n);
    }
    else{
        fprintf(stderr, "use: \"./matrix n\" to specify size of the matrix you want inverted\n");
        exit(1);
    }
    return 0;
}
