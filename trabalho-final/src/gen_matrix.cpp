#include "../include/matrix.h"

int main(int argc, char *argv[]) {

  Matrix matrix250(250, 250);
  Matrix matrix500(500, 500);
  Matrix matrix1000(1000, 1000);

  matrix250.fillMatrix();
  matrix500.fillMatrix();
  matrix1000.fillMatrix();

  matrix250.dumpMatrix("../matrix/matrix250.bin");
  matrix500.dumpMatrix("../matrix/matrix500.bin");
  matrix1000.dumpMatrix("../matrix/matrix1000.bin");

  return 0;
}
