#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <unordered_set>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;

 private slots:
  void on_buttonEqual_clicked();
  void on_buttonClear_clicked();
  void on_buttonErase_clicked();
  void on_buttonGraph_clicked();
  void operations();
  void gui_operations();
};
#endif  // MAINWINDOW_H
