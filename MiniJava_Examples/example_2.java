class FactorialCalculator {
    int factorial(int n) {
        if (n <= 1) {
            return 1;
        } else {
            return n * factorial(n - 1);
        }
    }

    void main() {
        int n;
        n = 5;
        print(factorial(n));
    }
}
