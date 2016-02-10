#include "pcmwidget.h"

PcmWidget::PcmWidget(QWidget *parent) : QWidget(parent)
{
}

void PcmWidget::clear()
{
  items.clear();
}

void PcmWidget::add(QByteArray* pcm, QPen pen)
{
  Item item;
  item.pcm_ = *pcm;
  item.pen_ = pen;
  items.push_back(item);
}

void PcmWidget::paintEvent(QPaintEvent *event)
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
    int size = item.pcm_.size() / sizeof(short);
    short* p = (short*)item.pcm_.data();
    int h = height();
    QPoint from(0, h / 2);
    for (int i = 0; i < size; i++)
    {
      short s = *p++;
      int y = h / 2 - s * h / 2 / 32768;
      QPoint to(i, y);
      painter.drawLine(from, to);
      from = to;
    }
  }
}
