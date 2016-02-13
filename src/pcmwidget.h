#pragma once

#include <QAudioFormat>
#include <QPainter>
#include <QPen>
#include <QWidget>
#include <vector>

class PcmWidget : public QWidget
{
  Q_OBJECT

  struct Item
  {
    std::vector<short> pcm_;
    QPen pen_;
  };

public:
  explicit PcmWidget(QWidget* parent = 0);

  QAudioFormat audioFormat;

  void clear();
  void add( std::vector<short>* pcm, QPen pen);

protected:
  QList<Item> items;

protected:
  void paintEvent(QPaintEvent* event) override;
};
