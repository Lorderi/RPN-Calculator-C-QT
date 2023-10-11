#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
  Q_OBJECT

 public:
  explicit Dialog(QWidget *parent = nullptr);
  Dialog(const char *value, QWidget *parent = nullptr);
  ~Dialog();
  void setEqualString(const char *value);

 private slots:
  void draw_graph();

 private:
  Ui::Dialog *ui;
  char *equal;
  void connect_slots();
};

#endif  // DIALOG_H
