#include "freqwidget.h"

#include <QPainter>

FreqWidget::FreqWidget(QWidget* parent) : QWidget(parent)
{
}

void FreqWidget::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event)
  QPainter painter(this);
  QRect rect(0, 0, width(), height());
  QBrush brush(Qt::blue);
  painter.fillRect(rect, brush);
}
