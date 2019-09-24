#include "CcFontConverter.h"
#include "ui_CcFontConverter.h"

CcFontConverter::CcFontConverter(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::CcFontConverter)
{
  ui->setupUi(this);
}

CcFontConverter::~CcFontConverter()
{
  delete ui;
}
