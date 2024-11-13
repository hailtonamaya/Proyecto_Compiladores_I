class FibonacciCalculator {
    void printFibonacci(int count) {
        int a, b, temp;
        a = 0;
        b = 1;
        int i;
        i = 0;
        while (i < count) {
            print(a);
            temp = a + b;
            a = b;
            b = temp;
            i = i + 1;
        }
    }
}
