class MatrixOperations {
    int[9] matrix;
    int i, j;
    int total;
    int i, j;

    void initializeMatrix() {
        ii = 0;
        while (i < 3) {
            j = 0;
            while (j < 3) {
                matrix[i * 3 + j] = i * j;
                j = j + 1;
            }
            i = i + 1;
        }
    }

    int getSum() {
        
        total = 0;
        i = 0;
        while (i < 3) {
            j = 0;
            while (j < 3) {
                total = total + matrix[i * 3 + j];
                j = j + 1;
            }
            i = i + 1;
        }
        return total;
    }
}
