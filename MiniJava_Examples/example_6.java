class NumberAnalyzer {
    void analyze(int x, int y) {
        if (x > 0 && y > 0) {
            print("Both numbers are positive");
        } else if (x < 0 || y < 0) {
            print("At least one number is negative");
        } else {
            print("Both numbers are zero or one of them is zero");
        }
    }
}
