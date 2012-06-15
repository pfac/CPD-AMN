#include "matrix.h"

void matfprintul (FILE * fp, unsigned long * matrix, unsigned long rows, unsigned long cols) {
	unsigned long i,j;
	for (i = 0; i < rows; ++i) {
		if (cols) {
			fprintf(fp, "%lu", matrix[i * cols]);
			for (j = 1; j < cols; ++j) {
				fprintf(fp, " %lu", matrix[i * cols + j]);
			}
			fprintf(fp, "\n");
		}
	}
}