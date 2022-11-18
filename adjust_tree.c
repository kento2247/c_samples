#include <stdio.h>
#include <stdlib.h>
struct data {
  int value;
  struct data *parent;
  struct data *left;
  struct data *right;
  int left_count;
  int right_count;
  int left_depth;  //葉からの相対距離
  int right_depth;
  int depth;
};

float pow2(float base) {
  float result = 1;
  for (int i = 0; i < 2; i++) {
    result *= base;
  }
  return result;
}
int max(int a, int b) {
  if (a > b)
    return a;
  else
    return b;
}
int min(int a, int b) {
  if (a < b)
    return a;
  else
    return b;
}
int abs(int a) {
  if (a < 0)
    return -a;
  else
    return a;
}
float varp(float ave, int *array, int length) {
  float result = 0;
  for (int i = 0; i < length; i++) {
    result += pow2((ave - array[i]));
  }
  return result / length;
}
int get_fact(int num) {    //階乗を求める
  if (num == 0) return 1;  // 0!=1にする
  int m = 1;
  for (int i = 1; i <= num; i++) {
    m *= i;
  }
  return m;
}

int insert_leaf(struct data *root, struct data *p) {
  int num = p->value;  // pはvalueだけ埋めて渡される
  p->left = NULL;
  p->right = NULL;
  p->left_count = 0;
  p->right_count = 0;
  p->left_depth = 0;
  p->right_depth = 0;
  struct data *target = root;  //探索対象
  int depth = 1;
  while (1) {
    int target_num = target->value;
    if (num < target_num) {  //探索対象よりもnumが小さい場合、左部分木を探す
      if (target->right != NULL)
        return 1;  //全て左から順に挿入する手法でない場合
      if (target->left == NULL) {  //葉だった場合、pを葉として付け足す
        target->left = p;
        p->parent = target;
        p->depth = depth;
        // printf("inserted %d left under %d...depth %d\n", num, target_num,
        //        depth);  // debug
        for (struct data *x = p; x != root;
             x = x->parent) {            //追加による変化を親に反映
          if (x->parent->value > num) {  //ノードが左部分木だった場合
            x->parent->left_count += 1;  //親から見た左のノード数を一個増やす
            x->parent->left_depth = max(x->left_depth, x->right_depth) +
                                    1;  //親から見た左の深さを一個増やす
          } else {
            x->parent->right_count += 1;
            x->parent->right_depth = max(x->left_depth, x->right_depth) + 1;
          }
        }
        return 0;
      } else {
        target = target->left;  //探索範囲を左へ
        depth++;
        continue;
      }
    } else {  //探索対象よりもnumが大きい場合、右部分木を探す
      if (target->right == NULL) {  //葉だった場合、pを葉として付け足す
        target->right = p;
        p->parent = target;
        p->depth = depth;
        // printf("inserted %d right under %d...depth %d\n", num, target_num,
        //        depth);  // debug
        for (struct data *x = p; x != root;
             x = x->parent) {  //追加による変化を親に反映
          if (x->parent->value > num) {
            x->parent->left_count += 1;  //親から見た左のノード数を一個増やす
            x->parent->left_depth = max(x->left_depth, x->right_depth) +
                                    1;  //親から見た左の深さを一個増やす
          } else {
            x->parent->right_count += 1;
            x->parent->right_depth = max(x->left_depth, x->right_depth) + 1;
          }
        }
        return 0;
      } else {
        target = target->right;  //探索範囲を右へ
        depth++;
        continue;
      }
    }
  }
}
int check_tree_state(struct data *root) {
  // 0=tree,1=AVL,2=perfect
  struct data *target = root;
  while (1) {
    if (target->left != NULL) {
      target = target->left;
      continue;
    } else if (target->right != NULL) {
      target = target->right;
      continue;
    } else {  //葉の場合
      //ノードの木状態を判定
      int target_state = 0;
      if (abs(target->left_count - target->right_count) <= 1)
        target_state = 2;  // perfectの場合
      else if (abs(target->left_depth - target->right_depth) <= 1)
        target_state = 1;  // AVLの場合
      else  // treeであれば、強制的に判定をtreeで終了する
        return 0;
      if (target == root)
        return target_state;  // rootまで来た場合
      else {
        if (target->value < target->parent->value)
          target->parent->left = NULL;  //親の左ノードだった場合
        else
          target->parent->right = NULL;
        struct data *child = target;
        target = target->parent;
        free(child);
      }
      continue;
    }
  }
}

void generate_tree(int n) {
  int max_height = get_fact(n);  // n!を求める
  int array[max_height][n];      //結果を格納する
  int target_height = 0;  //二次元配列の縦方向のポインタを設定
  // n!*n要素の二次元配列に、順列を全て記録
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
  int tree_count = 0, avl_count = 0,
      perfect_count = 0;  // 0startでavl,perfect treeの個数を記録。
  int tree_total_dep = 0, avl_total_dep = 0,
      perfect_total_dep = 0;       //各treeの高さの和を記録
  int tree_height[3][max_height];  // tree,avl,perfectの高さを記録
  for (int i = 0; i < max_height;
       i++) {  //各順列に対する木を作成。高さと数を記録

    printf("\n( ");                // debug
    for (int z = 0; z < n; z++) {  // debug
      printf("%d ", array[i][z]);
    }
    printf(")");  // debug

    struct data root;
    root.value = array[i][0];
    root.depth = 0;
    root.left_count = 0;
    root.left_depth = 0;
    root.right_count = 0;
    root.right_depth = 0;
    root.left = NULL;
    root.right = NULL;
    int tree_erase_flag = 0;
    //(すでに存在するtreeの場合)==全て左から順に挿入する手法でない場合
    for (int j = 1; j < n; j++) {
      struct data *p;
      p = (struct data *)malloc(sizeof(struct data));
      p->value = array[i][j];
      tree_erase_flag = insert_leaf(&root, p);
      if (tree_erase_flag) break;
    }
    if (tree_erase_flag) {
      printf(" > erased");  // debug
      continue;
    } else {
      int tree_state = check_tree_state(&root);  //木を評価
      printf("...state %d", tree_state);         // debug
      tree_height[0][tree_count] =
          max(root.left_depth, root.right_depth);  //木の高さを記録
      tree_total_dep += tree_height[0][tree_count];
      if (tree_state >= 1) {  // AVL,Perfectの場合
        tree_height[1][avl_count] =
            tree_height[0][tree_count];  //木の高さを記録
        avl_total_dep += tree_height[0][tree_count];
        avl_count += 1;
      }
      if (tree_state == 2) {  // Perfectの場合
        tree_height[2][perfect_count] =
            tree_height[0][tree_count];  //木の高さを記録
        perfect_total_dep += tree_height[0][tree_count];
        perfect_count += 1;
      }
      tree_count += 1;
    }
  }
  float tree_ave = tree_total_dep / (float)tree_count;
  float avl_ave = avl_count ? avl_total_dep / (float)avl_count : 0;
  float perfect_ave =
      perfect_count ? perfect_total_dep / (float)perfect_count : 0;
  float tree_varp = varp(tree_ave, tree_height[0], tree_count);
  float avl_varp = avl_count ? varp(avl_ave, tree_height[1], avl_count) : 0;
  float perfect_varp =
      perfect_count ? varp(perfect_ave, tree_height[2], perfect_count) : 0;
  printf("\n\n(n = %d)", n);
  printf("\n%d trees\n  ave height = %f\n  var height = %f", tree_count,
         tree_ave, tree_varp);
  printf("\n%d AVL trees\n  ave height = %f\n  var height = %f", avl_count,
         avl_ave, avl_varp);
  printf("\n%d perfect trees\n  ave height = %f\n  var height = %f",
         perfect_count, perfect_ave, perfect_varp);
}

void debug() {
  int length = 4;
  int arr[4] = {2, 4, 3, 1};  // length_size
  struct data root;
  root.value = arr[0];
  root.depth = 0;
  root.left_count = 0;
  root.left_depth = 0;
  root.right_count = 0;
  root.right_depth = 0;
  root.left = NULL;
  root.right = NULL;
  for (int i = 1; i < length; i++) {
    struct data *p;
    p = (struct data *)malloc(sizeof(struct data));
    p->value = arr[i];
    insert_leaf(&root, p);  //木の形状を保存
  }
  int tree_state = check_tree_state(&root);
  printf(
      "root\nl_count=%d\nr_count=%d\nl_depth=%d\nr_depth=%d\nstate=%d\n "
      "(0=tree,1=AVL,2=perfect)\n",
      root.left_count, root.right_count, root.left_depth, root.right_depth,
      tree_state);
}

int main() {
  // debug();
  generate_tree(4);
}