#include "dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::Dialog) {
  ui->setupUi(this);
}

Dialog::~Dialog() { delete ui; }

void Dialog::accept() {
  emit getIP(ui->lineEdit->text());
}