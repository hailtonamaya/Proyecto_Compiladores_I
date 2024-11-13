class PrimeChecker {
    int isPrime(int num) {
        if (num <= 1) {
            return 0;
        }
        int i;
        i = 2;
        while (i < num) {
            if (num % i == 0) {
                return 0;
            }
            i = i + 1;
        }
        return 1;
    }
}
