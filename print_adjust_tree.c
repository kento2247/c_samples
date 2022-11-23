// gcc -framework OpenGL -framework GLUT -DGL_SILENCE_DEPRECATION
// print_adjust_tree.c

#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__     // OSがMacでない (= WindowsやLinux)
#include <GL/glut.h>  // Windows, Linuxの場合のヘッダ
#else
#include <GLUT/glut.h>  // Macの場合のヘッダ
#endif

int WINDOW_WIDTH = 1280;  // ウィンドウの横幅
int WINDOW_HEIGHT = 720;  // ウィンドウの高さ
int max_depth = 0;
char typed_key = 0;
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
void debug();
void print_tree(struct data *target, int x, int y);

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
  int num = p->value;          // pはvalueだけ埋めて渡される
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
      if (target == root) {
        return target_state;  // rootまで来た場合
      } else {
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
      p->left = NULL;
      p->right = NULL;
      p->left_count = 0;
      p->right_count = 0;
      p->left_depth = 0;
      p->right_depth = 0;
      tree_erase_flag = insert_leaf(&root, p);
      if (tree_erase_flag) break;
    }
    if (tree_erase_flag) {
      printf(" > erased");  // debug
      continue;
    } else {
      glClear(GL_COLOR_BUFFER_BIT);
      max_depth = max(root.left_depth, root.right_depth);
      int need_height = (2 * (max_depth + 1) - 1) * 24;
      int height_margin = (WINDOW_HEIGHT - need_height) / 2;  //マージン
      glColor3d(0.0, 0.0, 0.0);
      glLineWidth(1.0);
      print_tree(&root, WINDOW_WIDTH / 2, (WINDOW_HEIGHT - height_margin));
      glRasterPos2f(WINDOW_WIDTH / 40, WINDOW_HEIGHT / 20 * 19);
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 40);
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 32);
      for (int c = 0; c < n; c++) {
        char ic = 48 + array[i][c];
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ic);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 32);
      }
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 41);
      int tree_state = check_tree_state(&root);  //木を評価
      char tree_state_name_1[12] = {'t', 'r', 'e', 'e', ' ', 's',
                                    't', 'a', 't', 'e', '=', 48 + tree_state};

      char tree_state_name_2[26] = {'(', '2', '=', 'p', 'e', 'r', 'f', 'e', 'c',
                                    't', ',', '1', '=', 'A', 'V', 'L', ',', '0',
                                    '=', 'n', 'o', 'r', 'm', 'a', 'l', ')'};
      glRasterPos2f(WINDOW_WIDTH / 40, WINDOW_HEIGHT / 20 * 18);
      for (int c = 0; c < 12; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, tree_state_name_1[c]);
      }
      glRasterPos2f(WINDOW_WIDTH / 40, WINDOW_HEIGHT / 20 * 17);
      for (int c = 0; c < 26; c++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, tree_state_name_2[c]);
      }
      glFlush();
      //ここで描画
      printf("...state %d", tree_state);  // debug
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

      //以下デバッグ用
      if (tree_state != 2) continue;
      int d = 0;
      scanf("%d", &d);
      if (d == 0) break;
      //ここまで
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

void print_tree(struct data *target, int x, int y) {
  int num = target->value;
  int str_width = glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, 48 + num);
  glRasterPos2f(x - str_width / 2, y + 3);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48 + num);
  if (target->left != NULL) {
    int shift_x =
        (pow2(max_depth - target->depth + 1) - 1) * 5;  //枝の長さを定義
    glBegin(GL_LINES);
    glVertex2d(x, y - 3);             //数字の上下3pxは空ける
    glVertex2d(x - shift_x, y - 21);  //数字の上下3pxは空ける
    glEnd();
    print_tree(target->left, x - shift_x, y - 48);
  }
  if (target->right != NULL) {
    int shift_x = (pow2(max_depth - target->depth + 1) - 1) * 5;
    glBegin(GL_LINES);
    glVertex2d(x, y - 3);
    glVertex2d(x + shift_x, y - 21);
    glEnd();
    print_tree(target->right, x + shift_x, y - 48);
  }
}
void print_home() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3d(0.0, 0.0, 0.0);
  glLineWidth(5.0);
  glBegin(GL_LINES);
  glVertex2d(0, 0);
  glVertex2d(250, 250);
  glEnd();
  glLineWidth(1.0);
  int str_width = glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, 48);
  glRasterPos2f(WINDOW_WIDTH / 2 - str_width / 2, WINDOW_HEIGHT / 2);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 48);
  glFlush();
}
void init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);  //背景色を白に指定
}
void display() { print_home(); }

void keybord(unsigned char key, int x, int y) {
  // printf("\ntype->%hhu\n", key);
  typed_key = key;
  if (key == 51) glutPostRedisplay();  // 3を入力でホーム画面
  if (key == 50) {
    printf("\ntype tree depth num : ");
    int d = 0;
    scanf("%d", &d);
    generate_tree(d);  // 2を入力でgenerate tree
  }
  if (key == 49) debug();  // 1を入力で木を描画
  if (key == 48) exit(0);  // 0入力でプログラム終了
}

void reshape(int width, int height) {
  // OpenGLウィンドウの描画範囲を設定
  // 下記は描画範囲が[0, width] x [0, height]となるように設定している
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (double)width, 0.0, (double)height);
  WINDOW_WIDTH = width;
  WINDOW_HEIGHT = height;
}

#define length 8
void debug() {
  // int arr[4] = {3, 1, 2, 4};  // length_size
  // int arr[5] = {3, 1, 2, 4, 5};           // length_size
  int arr[length] = {3, 1, 2, 4, 5, 6, 7, 8};  // length_size
  struct data root;
  root.value = arr[0];
  root.depth = 0;
  root.left_count = 0;
  root.left_depth = 0;
  root.right_count = 0;
  root.right_depth = 0;
  root.left = NULL;
  root.right = NULL;
  int insert_tree_flag = 0;
  for (int i = 1; i < length; i++) {
    struct data *p;
    p = (struct data *)malloc(sizeof(struct data));
    p->value = arr[i];
    p->left = NULL;
    p->right = NULL;
    p->left_count = 0;
    p->right_count = 0;
    p->left_depth = 0;
    p->right_depth = 0;
    insert_tree_flag = insert_leaf(&root, p);  //木の形状を保存
    if (insert_tree_flag) break;
  }

  if (insert_tree_flag) printf("\ntree was erased\n");
  //ここで描画
  glClear(GL_COLOR_BUFFER_BIT);
  max_depth = max(root.left_depth, root.right_depth);
  int need_height = (2 * (max_depth + 1) - 1) * 24;
  int height_margin = (WINDOW_HEIGHT - need_height) / 2;  //マージン
  glColor3d(0.0, 0.0, 0.0);
  glLineWidth(1.0);
  print_tree(&root, WINDOW_WIDTH / 2, (WINDOW_HEIGHT - height_margin));

  glRasterPos2f(WINDOW_WIDTH / 20, WINDOW_HEIGHT / 20 * 19);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 40);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 32);
  for (int i = 0; i < length; i++) {
    char ic = 48 + arr[i];
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ic);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 32);
  }
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 41);
  glFlush();

  int tree_state = check_tree_state(&root);
  printf(
      "root\nl_count=%d\nr_count=%d\nl_depth=%d\nr_depth=%d\nstate=%d\n "
      "(0=tree,1=AVL,2=perfect)\n",
      root.left_count, root.right_count, root.left_depth, root.right_depth,
      tree_state);
}

int main(int argc, char **argv) {
  //以下デバッグ
  printf(
      "keybord\n 3->back to home\n 2->generate tree\n 1->print debug tree\n "
      "0->exit\n");
  // GLUTの初期化
  glutInit(&argc, argv);
  // ウィンドウのサイズを設定
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  // ウィンドウの作成 (引数はウィンドウのタイトル)
  glutCreateWindow("First GLUT program");
  // 描画に使う関数の登録
  glutDisplayFunc(display);
  // キーそうさ
  glutKeyboardFunc(keybord);
  // ウィンドウのサイズ変更時に呼ばれる関数の登録
  glutReshapeFunc(reshape);
  // OpenGLの初期化処理  (これはコールバック関数ではないので直接呼び出す)
  init();
  // 描画ループの開始
  glutMainLoop();
}
