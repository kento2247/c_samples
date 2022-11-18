#ifndef __APPLE__     // OSがMacでない (= WindowsやLinux)
#include <GL/glut.h>  // Windows, Linuxの場合のヘッダ
#else
#include <GLUT/glut.h>  // Macの場合のヘッダ
#endif

int WINDOW_WIDTH = 500;   // ウィンドウの横幅
int WINDOW_HEIGHT = 500;  // ウィンドウの高さ

void init() { glClearColor(0.0, 0.0, 1.0, 1.0); }

void display() {
  // ウィンドウ表示内容のクリア
  glClear(GL_COLOR_BUFFER_BIT);

  // 直線の描画 (glFlushを忘れないこと)
  glBegin(GL_LINES);
  glVertex2d(150.0, 150.0);
  glVertex2d(350.0, 350.0);
  glEnd();
  glFlush();
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
  // ウィンドウのサイズ変更時に呼ばれる関数の登録
  glutReshapeFunc(reshape);
  // OpenGLの初期化処理  (これはコールバック関数ではないので直接呼び出す)
  init();
  // 描画ループの開始
  glutMainLoop();
}