#pragma once
#include <QDialog>
#include "ui_dialog.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {

  Q_OBJECT

public:
  explicit Dialog(QWidget *parent = nullptr);
  ~Dialog();

private:
  Ui::Dialog *ui;

public slots:
  void accept();

signals:
  void getIP(QString);
};
