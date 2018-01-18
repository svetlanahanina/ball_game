#pragma once

#include "src/gui/abstract_geom_object.h"

#include <QTimer>
#include <QWidget>
class QPainter;

class geometry_data_gui;

class game_process_view : public QWidget
{
  Q_OBJECT

  QTimer *m_timer = nullptr;
  geometry_data_gui *m_geom_data;
  double m_delta_time = 1.e-3;
  double m_current_time = 0;
  double m_bottom = 0;
  double m_max_y = 0;
  double m_width = 5;
  double m_height = 5;
  rebound_data m_next_rebound;
  QLineF m_creating_line;
  bool m_line_is_creating = false;

  bool return_ball_in_bounds ();
  void calculate_rebound ();

  QPointF map_to_scene (QPointF p);
public:
  game_process_view (QWidget *parent, geometry_data_gui *geom_data);
  ~game_process_view ();
  QSize minimumSizeHint () const;
  QSize sizeHint () const;

public slots:
 void update_time ();

protected:
  void paintEvent (QPaintEvent *event);

  void mousePressEvent (QMouseEvent * event);
  void mouseMoveEvent (QMouseEvent * event);
  void mouseReleaseEvent (QMouseEvent * event);

};
