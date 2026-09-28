#include <stdio.h>
#define SIZE 13
#define N 7

int keys[N] = {23, 43, 13, 33, 53, 63, 73};

typedef enum {
    LINEAR,
    QUADRATIC,
    DOUBLE_HASH
} Method;

int h1(int key) {
    return key % 10;
}

int h2(int key) {
    return 1 + (key % 11);
}

int probe_index(int key, int i, Method method) {
    int index;

    if (method == LINEAR) {
        index = h1(key) + i;
    } else if (method == QUADRATIC) {
        index = h1(key) + i * i;
    } else {
        index = h1(key) + i * h2(key);
    }

    return index % SIZE;
}

void insert(int table[], int key, Method method) {
    for (int i = 0; i < SIZE; i++) {
        int index = probe_index(key, i, method);

        if (table[index] == -1) {
            table[index] = key;
            return;
        }
    }

    printf("Could not insert %d\n", key);
}

int search(int table[], int key, Method method) {
    for (int i = 0; i < SIZE; i++) {
        int index = probe_index(key, i, method);

        if (table[index] == key) {
            return i + 1;  // Probes include the successful check.
        }

        if (table[index] == -1) {
            return i + 1;  // Empty slot confirms the key is absent.
        }
    }

    return SIZE;  // Probe limit reached.
}

void print_table(const char *name, int table[]) {
    printf("\n%s\n", name);
    printf("Index: ");

    for (int i = 0; i < SIZE; i++) {
        printf("%3d", i);
    }

    printf("\nValue: ");

    for (int i = 0; i < SIZE; i++) {
        if (table[i] == -1) {
            printf("%3s", "-");
        } else {
            printf("%3d", table[i]);
        }
    }

    printf("\n");
}

int main(void) {
    int tables[3][SIZE];
    Method methods[3] = {LINEAR, QUADRATIC, DOUBLE_HASH};
    const char *names[3] = {
        "Linear Probing",
        "Quadratic Probing",
        "Double Hashing"
    };

    for (int m = 0; m < 3; m++) {
        for (int i = 0; i < SIZE; i++) {
            tables[m][i] = -1;
        }

        for (int i = 0; i < N; i++) {
            insert(tables[m], keys[i], methods[m]);
        }

        print_table(names[m], tables[m]);
    }

    int search_keys[] = {23, 73, 80};

    printf("\nSearch probe counts\n");
    printf("Key   Result       Linear  Quadratic  Double Hashing\n");

    for (int i = 0; i < 3; i++) {
        int key = search_keys[i];
        int p1 = search(tables[0], key, LINEAR);
        int p2 = search(tables[1], key, QUADRATIC);
        int p3 = search(tables[2], key, DOUBLE_HASH);

        printf("%-5d %-12s %-7d %-10d %d\n",
               key,
               (key == 23 || key == 73) ? "Found" : "Not found",
               p1, p2, p3);
    }

    printf("\nLoad factor = %d / %d = %.3f (%.1f%%)\n",
           N, SIZE, (double)N / SIZE, 100.0 * N / SIZE);

    return 0;
}