#include "mainwindow.h"

#include <QDir>
#include <QInputDialog>
#include <QMessageBox>

#include "./ui_mainwindow.h"
#include "dialog.h"
extern "C" {
#include "source/polish.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  QPushButton *buttons[] = {
      ui->buttonZero,         ui->buttonOne,   ui->buttonTwo,
      ui->buttonThree,        ui->buttonFour,  ui->buttonFive,
      ui->buttonSix,          ui->buttonSeven, ui->buttonEight,
      ui->buttonNine,         ui->buttonDiv,   ui->buttonMinus,
      ui->buttonPlus,         ui->buttonMul,   ui->buttonMod,
      ui->buttonPow,          ui->buttonCos,   ui->buttonSin,
      ui->buttonTan,          ui->buttonLn,    ui->buttonSqrt,
      ui->buttonAcos,         ui->buttonAsin,  ui->buttonAtan,
      ui->buttonLog,          ui->buttonX,     ui->buttonLeftBracket,
      ui->buttonRightBracket, ui->buttonDot,
  };
  for (QPushButton *button : buttons) {
    connect(button, SIGNAL(clicked()), this, SLOT(operations()));
  }
  QPushButton *additional_buttons[] = {ui->buttonPercent, ui->buttonNeg};
  for (QPushButton *button : additional_buttons) {
    connect(button, SIGNAL(clicked()), this, SLOT(gui_operations()));
  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_buttonEqual_clicked() {
  QString equal = ui->labelResult->text();
  if (equal.contains('x')) {
    bool ok;
    QString text = QInputDialog::getText(
        this, tr("Input window"), tr("X value:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
      QString equal_new = ui->labelResult->text();
      QByteArray byteEqual = equal_new.toUtf8();
      char *equal_str = byteEqual.data();
      bool ok_new;
      double x_value = text.toDouble(&ok_new);
      struct calculator_state state;
      if (ok_new) {
        double result = raw_calculate_with_x(equal_str, &state, x_value);
        if (!state.tkn_error) {
          ui->labelResult->setText(QString::number(result));
        }
      }
      if (!ok_new || state.tkn_error) {
        QMessageBox::warning(this, "Error window", "Result error");
      }
    }
  } else {
    struct calculator_state state;
    QByteArray equalArray = equal.toLocal8Bit();
    std::string equalStr = equalArray.toStdString();
    const char *ch = equalStr.c_str();
    double result = raw_calculate((char *)ch, &state);
    if (!state.tkn_error) {
      ui->labelResult->setText(QString::number(result));
    } else {
      QMessageBox::warning(this, "Error window", "Result error");
    }
  }
}

void MainWindow::operations() {
  QPushButton *button = (QPushButton *)sender();
  QString equal = ui->labelResult->text();
  if (ui->labelResult->text() == "0") {
    equal = equal.chopped(1);
  }
  equal += button->text();

  std::unordered_set<QString> validFunctions = {
      "cos", "sin", "tan", "ln", "log", "sqrt", "pow", "acos", "asin", "atan"};

  if (validFunctions.find(button->text()) != validFunctions.end()) {
    equal.append("(");
  }
  ui->labelResult->setText(equal);
}

void MainWindow::on_buttonClear_clicked() { ui->labelResult->setText("0"); }

void MainWindow::on_buttonErase_clicked() {
  if (!ui->labelResult->text().isEmpty()) {
    if (ui->labelResult->text().length() == 1) {
      ui->labelResult->setText("0");
    } else {
      ui->labelResult->setText(ui->labelResult->text().chopped(1));
    }
  }
}

void MainWindow::on_buttonGraph_clicked() {
  QByteArray byteArray = ui->labelResult->text().toUtf8();
  char *c_str = byteArray.data();
  Dialog window(c_str, nullptr);
  window.setModal(true);
  window.exec();
}

void MainWindow::gui_operations() {
  QPushButton *button = (QPushButton *)sender();
  QString equal = ui->labelResult->text();
  QRegExp re("[+-]?\\d*\\.?\\d+");
  if (re.exactMatch(equal)) {
    if (button->text() == "+/-" || button->text() == "%") {
      double all_numbers;
      all_numbers = equal.toDouble();
      if (button->text() == "+/-") {
        all_numbers *= -1;
      } else if (button->text() == "%") {
        all_numbers *= 0.01;
      }
      equal = QString::number(all_numbers, 'g', 15);
      ui->labelResult->setText(equal);
    }
  }
}
