#pragma once

#include <QPointF>
#include <QPainter>

class ball_data_gui
{
  QPointF m_pos = {0, 0};
  double m_vx = 0;
  double m_vy = 0;
public:
  ball_data_gui ();
  ~ball_data_gui ();

  QPointF get_pos () const;
  double get_vx () const;
  double get_vy () const;

  void set_pos (QPointF pos);
  void set_vx (double vx);
  void set_vy (double vy);

  void paint (QPainter *painter);

  void move_ball_at_time (double delta_t);
};
