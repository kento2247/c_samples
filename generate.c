#include <stdio.h>

int get_fact(int num) {
  if (num == 0) return 1;
  int m = 1;
  for (int i = 1; i <= num; i++) {
    m *= i;
  }
  return m;
}

void generate(int n) {
  int max_height = get_fact(n);  // n!を求める
  int array[max_height][n];      //結果を格納する
  int target_height = 0;  //二次元配列の縦方向のポインタを設定
  for (int i = 0; i < n; i++) {  //二次元配列の横方向の位置を定義
    target_height = 0;           //ターゲットを最上位にリセット
    int repeat = get_fact(n - i - 1);  //数字のリピート回数を定義
    while (target_height != max_height) {
      for (int j = 1; j <= n; j++) {  //埋める数字を定義
        if (target_height == max_height) break;
        int pass_flag = 0;  //すでに埋めた数字ならスキップする
        for (int x = 0; x < i; x++) {  //すでに埋めた数字を確認。フラグたて
          if (array[target_height][x] == j) {
            pass_flag = 1;
            break;
          }
        }
        if (pass_flag) continue;  //埋める数字を更新
        for (int y = 0; y < repeat; y++) {
          //リピート回数分、数字を縦方向に埋める。
          array[target_height][i] = j;
          target_height++;
        }
      }
    }
  }

  for (int i = 0; i < max_height; i++) {  //デバッグ用
    printf("\n( ");
    for (int j = 0; j < n; j++) {
      printf("%d ", array[i][j]);
    }
    printf(")");
  }
}

int main() { generate(5); }