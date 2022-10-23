#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

const int size = 17;
int matrix[17][17], new_matrix[17][17];
int window;

typedef struct thread_arguments{
    int number_of_threads;
    int current_thread;
}thread_arguments;


void print_matrix(int matrix[size][size]) {
    for (int i = window / 2; i < size - window / 2; ++i) {
        for (int j = window / 2; j < size - window / 2; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void fill_matrix() {
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

void result_to_matrix() {
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
    int swap;
    for (int i = 0; i < length - 1; ++i) {
        for (int j = 0; j < length - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                swap = array[j];
                array[j] = array[j + 1];
                array[j + 1] = swap;
            }
        }
    }
}

int median(int i, int j) {
    int size_sort = window * window - 1;
    int numbers[size_sort];
    int count = 0;
    for (int l = i - window / 2; l <= i + window / 2; ++l) {
        for (int k = j - window / 2; k <= j + window / 2; ++k) {
            numbers[count] = matrix[l][k];
            ++count;
        }
    }
    sort(numbers, size_sort);
    return numbers[size_sort / 2];
}

void filter_for_string(int number_of_string) {
    for (int j = 1; j < size - 1; ++j) {
        new_matrix[number_of_string][j] = median(number_of_string, j);
    }
}

void* thread_filter(void* arg) {
    thread_arguments data = *((thread_arguments*) arg);
    for (int i = data.current_thread; i < size; i += data.number_of_threads) {
        filter_for_string(i);
    }
}

int main(int argc, const char *argv[]) {
    int overlays, number_of_threads;
    number_of_threads = atoi(argv[1]);
    printf("number of threads = %d\n", number_of_threads);
    printf("number of overlays = ");
    scanf("%d", &overlays);
    printf("size of window(odd number) = ");
    scanf("%d", &window);
    
    fill_matrix();
    
    printf("matrix:\n");
    print_matrix(matrix);

    for (int k = 0; k < overlays; ++k) {
        pthread_t threads[number_of_threads];
        thread_arguments* data = malloc(sizeof(thread_arguments) * number_of_threads);
        for (int i = 0; i < number_of_threads; ++i) {
            data[i].current_thread = i;
            data[i].number_of_threads = number_of_threads;
        }
        for (int i = 0; i < number_of_threads; ++i) {
            if (pthread_create(&threads[i], NULL, &thread_filter, &data[i]) != 0) {
                perror("Failed to create thread");
            }
        }
        for (int i = 0; i < number_of_threads; ++i) {
            if (pthread_join(threads[i], NULL) != 0) {
                perror("Failed to join thread");
            }
        }
        result_to_matrix();
    }

    printf("new matrix:\n");
    print_matrix(matrix);

    return 0;
}