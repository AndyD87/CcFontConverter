#include "CcFontConverter.h"
#include <QApplication>

// Test if examples are working
#include "CFontExample.h"
#include "CFontExampleCpp.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  CcFontConverter w;
  w.show();

  return a.exec();
}
