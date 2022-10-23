#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

const int size = 17;

typedef struct thread_arguments{
    int number_of_threads;
    int *matrix;
    int *new_matrix;
}thread_arguments;


void print_matrix(int matrix[size][size]) {
    for (int i = 1; i < size - 1; ++i) {
        for (int j = 1; j < size - 1; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void fill_matrix(int matrix[size][size]) {
    srand(time(NULL));
    for (int i = 1; i < size - 1; ++i) {
        for (int j = 1; j < size - 1; ++j) {
            matrix[i][j] = rand() % 10;
        }
    }
    for (int i = 1; i < size - 1; ++i) {
        matrix[i][0] = matrix[i][1];
    }
    for (int i = 1; i < size - 1; ++i) {
        matrix[i][size - 1] = matrix[i][size - 2];
    }
    for (int i = 0; i < size; ++i) {
        matrix[0][i] = matrix[1][i];
    }
    for (int i = 0; i < size; ++i) {
        matrix[size - 1][i] = matrix[size - 2][i];
    }
}

void result_to_matrix(int matrix[size][size], int new_matrix[size][size]) {
    for (int i = 1; i < size - 1; ++i) {
        for (int j = 1; j < size - 1; ++j) {
            matrix[i][j] = new_matrix[i][j];
        }
    }
    for (int i = 1; i < size - 1; ++i) {
        matrix[i][0] = matrix[i][1];
    }
    for (int i = 1; i < size - 1; ++i) {
        matrix[i][size - 1] = matrix[i][size - 2];
    }
    for (int i = 0; i < size; ++i) {
        matrix[0][i] = matrix[1][i];
    }
    for (int i = 0; i < size; ++i) {
        matrix[size - 1][i] = matrix[size - 2][i];
    }
}

void sort(int array[], int length) {
    int tmp;
    for (int i = 0; i < length - 1; ++i) {
        for (int j = 0; j < length - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                tmp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = tmp;
            }
        }
    }
}

int median(int i, int j, int matrix[size][size]) {
    int numbers[9];
    int count = 0;
    for (int l = i - 1; l <= i + 1; ++l) {
        for (int k = j - 1; k <= j + 1; ++k) {
            numbers[count] = matrix[l][k];
            ++count;
        }
    }
    sort(numbers, 9);
    return numbers[4];
}

void filter_for_string(int matrix[size][size], int new_matrix[size][size], int number_of_string) {
    for (int j = 1; j < size - 1; ++j) {
        new_matrix[size][j] = median(size, j, matrix);
    }
}

// void* thread_filter(void* arg) {
//     thread_arguments data = *((thread_arguments*) arg);
//     for (int i = 0; i < size; i += data.number_of_threads) {
//         filter_for_string(data.martix, data.new_matrix, i);
//     }
// }

int main(int argc, const char *argv[]) {
    int array[size][size], new_array[size][size];
    int overlays, number_of_threads;

    printf("number of filter overlays = ");
    scanf("%d", &overlays);
    printf("number of threads = ");
    scanf("%d", &number_of_threads);

    fill_matrix(array);
    
    printf("array:\n");
    print_matrix(array);

    for (int k = 0; k < overlays; ++k) {
        for (int i = 1; i < size - 1; ++i) {
            for (int j = 1; j < size - 1; ++j) {
                new_array[i][j] = median(i, j, array);
            }
        } 
    
        result_to_matrix(array, new_array);
    }

    printf("new array:\n");
    print_matrix(array);

    return 0;
}