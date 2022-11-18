// gcc -framework OpenGL -framework GLUT -DGL_SILENCE_DEPRECATION print_test.c
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__     // OSがMacでない (= WindowsやLinux)
#include <GL/glut.h>  // Windows, Linuxの場合のヘッダ
#else
#include <GLUT/glut.h>  // Macの場合のヘッダ
#endif

int WINDOW_WIDTH = 500;   // ウィンドウの横幅
int WINDOW_HEIGHT = 500;  // ウィンドウの高さ

int type_num = 0;
void init() {
  glClearColor(1.0, 1.0, 1.0, 1.0);  //背景色を白に指定
}

void shape_sample() {
  // 直線の描画 (glFlushを忘れないこと)
  glBegin(GL_LINES);
  glColor3d(1.0, 0.0, 0.0);
  glVertex2d(150, 150);
  glColor3d(0.0, 1.0, 0.0);
  glVertex2d(350, 350);
  glEnd();

  // 太い線 (太さを戻すのを忘れないこと)
  glColor3d(0.0, 0.0, 0.0);
  glLineWidth(5.0);
  glBegin(GL_LINES);
  glVertex2d(200, 400);
  glVertex2d(400, 200);
  glEnd();
  glLineWidth(1.0);

  // 太い線 (太さを戻すのを忘れないこと)
  glColor3d(0.0, 0.0, 0.0);
  glLineWidth(5.0);
  glBegin(GL_LINES);
  glVertex2d(250, 100);
  glVertex2d(250, 240);
  glEnd();
  glLineWidth(1.0);

  // 太い線 (太さを戻すのを忘れないこと)
  glColor3d(0.0, 0.0, 0.0);
  glLineWidth(5.0);
  glBegin(GL_LINES);
  glVertex2d(100, 250);
  glVertex2d(230, 250);
  glEnd();
  glLineWidth(1.0);

  int str_width = glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, 49);
  int str_height = 12;
  glRasterPos2f(250 - str_width / 2, 250 - str_height / 2);
  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 49);
  printf("hello world\n");
}

void display() {
  // ウィンドウ表示内容のクリア
  glClear(GL_COLOR_BUFFER_BIT);
  // shape_sample();
  glFlush();
}

void keybord(unsigned char key, int x, int y) {
  printf("\ntype->%hhu\n", key);
  type_num = key - 48;
  // if (key == 49) glutPostRedisplay();  // 1を入力で画面更新
  if (key == 48) exit(0);  // 0入力でプログラム終了
  if (key == 49) {
    glClear(GL_COLOR_BUFFER_BIT);
    shape_sample();
    glFlush();
  }
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

int main(int argc, char **argv) {
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