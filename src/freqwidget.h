#pragma once

#include <QWidget>

class FreqWidget : public QWidget
{
  Q_OBJECT

public:
  explicit FreqWidget(QWidget* parent = 0);

  void paintEvent(QPaintEvent* event) override;
};
