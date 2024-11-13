class RefExample {
    void incrementBy(ref int x, int y) {
        x = x + y;
    }

    void applyIncrements() {
        int base;
        base = 10;
        int values[5];
        int i;
        i = 0;
        while (i < 5) {
            incrementBy(ref base, i);
            values[i] = base;
            i = i + 1;
        }
        print(values[4]);
    }
}
