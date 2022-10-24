#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

const int size = 20;
int matrix[20][20], new_matrix[20][20];
int window, frame;

typedef struct thread_arguments{
    int number_of_threads;
    int current_thread;
}thread_arguments;


void print_matrix() {
    for (int i = frame; i < size - frame; ++i) {
        for (int j = frame; j < size - frame; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_without_frame() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void fill_matrix() {
    srand(time(NULL));
    for (int i = frame; i < size - frame; ++i) {
        for (int j = frame; j < size - frame; ++j) {
            matrix[i][j] = rand() % 10;
        }
    }

    for (int i = 1; i < size - 1; ++i) {
        for (int j = 0; j < frame; ++j) {
            matrix[i][j] = matrix[i][frame];
            matrix[i][size - j - 1] = matrix[i][size - frame - 1];
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < frame; ++j) {
            matrix[j][i] = matrix[frame][i];
            matrix[size - j -1][i] = matrix[size - frame - 1][i];  
        }
    }
}

void result_to_matrix() {
    for (int i = frame; i < size - frame; ++i) {
        for (int j = frame; j < size - frame; ++j) {
            matrix[i][j] = new_matrix[i][j];
        }
    }

    for (int i = 1; i < size - 1; ++i) {
        for (int j = 0; j < frame; ++j) {
            matrix[i][j] = matrix[i][frame];
            matrix[i][size - j - 1] = matrix[i][size - frame - 1];
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < frame; ++j) {
            matrix[j][i] = matrix[frame][i];
            matrix[size - j -1][i] = matrix[size - frame - 1][i];  
        }
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

    for (int l = i - frame; l <= i + frame; ++l) {
        for (int k = j - frame; k <= j + frame; ++k) {
            numbers[count] = matrix[l][k];
            ++count;
        }
    }

    sort(numbers, size_sort);
    return numbers[size_sort / 2];
}

void filter_for_string(int number_of_string) {
    for (int j = frame; j < size - frame; ++j) {
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
    frame = window / 2;

    fill_matrix();
    
    printf("matrix:\n");
    print_matrix();

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
                return 1;
            }
        }

        for (int i = 0; i < number_of_threads; ++i) {
            if (pthread_join(threads[i], NULL) != 0) {
                perror("Failed to join thread");
                return 1;
            }
        }
        result_to_matrix();
    }

    printf("new matrix:\n");
    print_matrix();

    return 0;
}