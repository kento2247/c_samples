#include <stdio.h>
#include <time.h>
// #define len 8388608
#define len 100000

unsigned int array[len][2];

void set_chain() {
  for (unsigned int i = 0; i < len; i++) {
    unsigned int n = (1 + i) * 2 - 1;
    array[i][0] = n;
    unsigned int count = 1;
    while (1) {
      if (n == 1) break;
      if (n % 2 == 1) {
        n = n * 3 + 1;
        count++;
      } else
        n /= 2;
    }
    array[i][1] = count;
  }
}

void print_2darray() {
  for (unsigned int i = 0; i < len; i++) {
    for (unsigned int j = 0; j < 2; j++) {
      printf(" %u", array[i][j]);
    }
    printf("\n");
  }
}

void quick_sort(unsigned int in_left, unsigned int in_right) {
  if (in_left < in_right) {
    unsigned int left = in_left;
    unsigned int right = in_right;
    do {
      while (array[left][1] < array[(in_right + in_left) / 2][1]) left++;
      while (array[right][1] > array[(in_right + in_left) / 2][1])
        if (right != 0) right--;
      if (left <= right) {
        unsigned int buf[2] = {array[left][0], array[left][1]};
        array[left][0] = array[right][0];
        array[left][1] = array[right][1];
        array[right][0] = buf[0];
        array[right][1] = buf[1];
        left++;
        if (right != 0) right--;
      }
    } while (left <= right);
    // printf("sort(%u,%u)\n", in_left, right);
    quick_sort(in_left, right);
    // printf("sort(%u,%u)\n", left, in_right);
    quick_sort(left, in_right);
  }
}

void selection_sort() {
  for (unsigned int i = 0; i < len; i++) {
    // printf("progress: %d/%d\n", i, len);
    unsigned int minpos = i;
    for (unsigned int j = i; j < len; j++) {
      if (array[minpos][1] > array[j][1]) minpos = j;
    }
    unsigned int buf[2] = {array[i][0], array[i][1]};
    array[i][0] = array[minpos][0];
    array[i][1] = array[minpos][1];
    array[minpos][0] = buf[0];
    array[minpos][1] = buf[1];
  }
}

unsigned int get_minpos() {
  unsigned int minpos = 0;
  float min = array[0][1] / (float)array[0][0];
  for (unsigned int i = 1; i < len; i++) {
    if ((array[i][1] / (float)array[i][0]) < min) minpos = i;
  }
  return minpos;
}

void print_chain(unsigned int n) {
  while (1) {
    if (n == 1) break;
    if (n % 2 == 1) {
      printf(" %u", n);
      n = n * 3 + 1;
    } else
      n /= 2;
  }
  printf(" 1\n");
}

int main() {
  set_chain();
  //   printf("\n\nBefore: \n");
  print_2darray();
  double start_clock = (double)clock();
  //   quick_sort(0, len - 1);
  //   selection_sort();
  double end_clock = (double)clock();
  //   printf("\n\nAfter: \n");
  //   print_2darray();
  //   printf("Time : %.8lf s\n", (end_clock - start_clock) / CLOCKS_PER_SEC);
  //   return 0;
  //   unsigned int minpos = get_minpos();
  //   printf(
  //       "minpos=%u\nchainlength(%u)/%u=%f\ni=%u\nchainlength=%u\nchain_array\n",
  //       minpos, array[minpos][0], array[minpos][0],
  //       array[minpos][1] / (float)array[minpos][0], array[minpos][0],
  //       array[minpos][1]);
  //   print_chain(array[minpos][0]);
}