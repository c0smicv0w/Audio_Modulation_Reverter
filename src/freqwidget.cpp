#include "freqwidget.h"

FreqWidget::FreqWidget(QWidget* parent) : QWidget(parent)
{
}

void FreqWidget::clear()
{
  items.clear();
}

void FreqWidget::add(std::vector<complex>* freq, QPen pen)
{
  Item item;
  item.freq_ = *freq;
  item.pen_ = pen;
  items.push_back(item);
}

void FreqWidget::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event)
  QPainter painter(this);
  QRect rect(0, 0, width(), height());
  QBrush brush(Qt::black);
  painter.fillRect(rect, brush);

  int count = items.count();
  for (int i = 0; i < count; i++)
  {
    const Item& item = items.at(i);
    painter.setPen(item.pen_);
    int size = item.freq_.size() / sizeof(short);
    int h = height();
    QPoint from(0, h / 2);
    for (int i = 0; i < size; i++)
    {
      int amplitude = item.freq_.at(i).norm();
      int y = h / 2 - amplitude * h  / 2 / 32768;
      QPoint to(i, y);
      painter.drawLine(from, to);
      from = to;
    }
  }
}
