#include "learn.h"

double** make(int row, int col){
	double** matrix = (double**)malloc(row*sizeof(double*));
	int i;
	for(i = 0; i<row; i++){
		matrix[i] = (double*)malloc(col*sizeof(double));
	}
	return matrix;
}

double** multiply(double** a, double** b, int row1, int col1, int col2){
	double value = 0;
	int i, j, k;
	double** output = make(row1, col2);	
	for(i = 0; i<row1; i++){//1st matrix row
		for(j = 0; j<col2; j++){//2nd matrix column
			for(k = 0; k<col1; k++){//1st matrix column
				value = value + a[i][k]*b[k][j];
			}
				output[i][j] = value;
				value = 0;
		}

	}
	return output;
}

void print(double** a,int row, int col){
	int i, j;
	for(i = 0; i<row; i++){
		for(j = 0; j<col; j++){
			printf("%lf\t", a[i][j]);
		}
		printf("\n");
	}
}
void findInverse(double** aug, int col){
	int i,j,k,a;
	double value;
	for(i = 0; i<col; i++){//first subtract
		for(j = i; j<col; j++){
			if(j != i){
				value = (aug[j][i]/aug[i][i]);
				for(k = i; k<2*col; k++){
					aug[j][k] = aug[j][k] - aug[i][k] * value;//SUBTRACTS THE ENTIRE ROW BY THIS
				}
			}
		}
	}
	for(i = 0; i<col; i++){//divide pivots to 1
		for(j = i; j<col; j++){
			double pivot = aug[i][i];
				for(k = i; k<2*col; k++){
					aug[j][k] = (aug[j][k]/pivot);
				}
		}
	}
	for(j = col-2; j>=0; j--){//now simplify to identity matrix
		for(a = col-1; a>=0; a--){
			if(a == j){
				break;
			}
			value = aug[j][a];
			for(k = 2*col-1; k>=j+1; k--){//for row operations
					aug[j][k] = aug[j][k] - aug[a][k]*value;
			}
		}
	}
}


int main(int argc, char*argv[]){
	FILE* train = fopen(argv[1], "r");
	int col, row;
	fscanf(train, "%d", &col);
	col++;
	//printf("this is col + 1: %d\n", col);
	fscanf(train, "%d", &row);
	//printf("this is row: %d\n", row);
	int i,j;
	int count = 0;
	char comma;
	double fill;
	double** x = make(row, col);
	double** y = make(row, 1);
	double** transpose = make(col, row);
	for(i = 0; i<row; i++){
		for(j = 0; j<col; j++){ //%f is for float, but does same thing for double
			count++;
			//printf("%lf and count is %d\n", fill,count); 
			if(j == 0){
				x[i][j] = 1;
				transpose[j][i] = 1;
			}
			else{
				fscanf(train, "%lf", &fill);
				fscanf(train, "%c", &comma);
				x[i][j] = fill;
				transpose[j][i] = fill;
			}
		}
		fscanf(train, "%lf", &fill);
		fscanf(train, "%c", &comma);
		count++;
		//printf("%lf and count is %d for y\n", fill,count);
		y[i][0] = fill;
	}

	/*printf("This is the x matrix\n");
	print(x, row, col);
	
	printf("This is the transpose matrix\n");
	
	print(transpose, col, row);
	printf("This is the y matrix\n");
	print(y, row, 1);*/

	//multiply first the transpose and x
	double** output = multiply(transpose, x, col, row, col);
	double** aug = make(col, 2*col);
	for(i = 0; i<col; i++){
		aug[i] = (double*)malloc(2*col*sizeof(double));
	}
	for(i = 0; i<col; i++){
		for(j = 0; j<col; j++){
			aug[i][j] = output[i][j];
		}
	}
	for(i = 0; i<col; i++){
		for(j = col; j<2*col; j++){
			if(i == j-col){
				aug[i][j] = 1;
			}
		}
	}

	/*printf("this is the current output\n");
	print(output, col, col);*/

	//now perform gaussian elimination and find the inverse
	findInverse(aug, col);
	/*printf("this is the augmented matrix\n");
	print(aug, col, 2*col);*/
	double** inverse = make(col, col);
	for(i = 0; i<col; i++){
		for(j = col; j<2*col; j++){
			inverse[i][j-col] = aug[i][j];
		}
	}

	/*printf("this is the inverse matrix\n");
	print(inverse, col, col);*/
	double ** output2 = make(col, row);
	output2 = multiply(inverse, transpose, col, col, row);
	/*printf("this is output2\n");
	print(output2, col, row);*/
	double ** weighted = make(col, 1);
	weighted = multiply(output2, y, col, row, 1);
	/*printf("this is the final weighted matrix\n");
	print(weighted, col, 1); */
	//row1, col1, col2

	FILE* test = fopen(argv[2], "r");
	int testrows;
	fscanf(test, "%d", &testrows);
	double** t = make(testrows, col);
	for(i = 0; i<testrows; i++){
		for(j = 0; j<col; j++){ //%f is for float, but does same thing for double
			if(j == 0){
				t[i][j] = 1;
			}
			else{
				fscanf(test, "%lf", &fill);
				fscanf(test, "%c", &comma);
				t[i][j] = fill;
			}
		}
	}
	/*printf("this is t\n");
	print(t, testrows, col);*/
	double** final = make(testrows, 1);
	//3x5, 5x1	
	final = multiply(t, weighted, testrows, col, 1);
	//printf("HERE IT IS!!!!!!\n");
	for(i = 0; i<testrows; i++){
		for(j = 0; j<1; j++){
			printf("%0.0f\n", final[i][j]);
		}
	}
	return 0;
}
