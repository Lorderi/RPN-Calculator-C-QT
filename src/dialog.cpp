#include "dialog.h"

#include "qcustomplot.h"
#include "ui_dialog.h"
extern "C" {
#include "source/polish.h"
}

void Dialog::connect_slots() {
  QSlider *sliders[] = {ui->horizontalSlider, ui->verticalSlider};
  for (QSlider *slider : sliders) {
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(draw_graph()));
  }
}

void Dialog::draw_graph() {
  int size_x = ui->horizontalSlider->value();
  int size_y = ui->verticalSlider->value();
  struct calculator_state state;
  QVector<double> x, y;
  for (int i = -size_x * 10; i < size_x * 10; i += 1) {
    double f_i = i * 0.1f;
    double result = raw_calculate_with_x(equal, &state, f_i);
    if (!state.tkn_error) {
      x.append(i * 0.1f);
      y.append(result);
    }
  }
  // create graph and assign data to it:
  ui->customPlot->addGraph();
  ui->customPlot->graph(0)->setData(x, y);
  // give the axes some labels:
  ui->customPlot->xAxis->setLabel("x");
  ui->customPlot->yAxis->setLabel("y");
  // set axes ranges, so we see all data:
  ui->customPlot->xAxis->setRange(-size_x / 2, size_x / 2);
  ui->customPlot->yAxis->setRange(-size_y, size_y);
  ui->customPlot->replot();
}

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog) {
  ui->setupUi((QDialog *)this);
  connect_slots();
  draw_graph();
}

Dialog::Dialog(const char *value, QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog) {
  this->setModal(true);
  ui->setupUi((QDialog *)this);
  connect_slots();
  setEqualString(value);
  draw_graph();
}

Dialog::~Dialog() { delete ui; }

void Dialog::setEqualString(const char *value) { equal = (char *)value; }
