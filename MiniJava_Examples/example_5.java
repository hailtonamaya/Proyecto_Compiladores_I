class FibonacciCalculator {
    void printFibonacci(int count) {
        int a, b, temp;
        int i;
        a = 0;
        b = 1;

        i = 0;
        while (i < count) {
            print(a);
            print(" ");
            temp = a + b;
            a = b;
            b = temp;
            i = i + 1;
        }
    }
    void main(){
        printFibonacci(10);
    }
}
