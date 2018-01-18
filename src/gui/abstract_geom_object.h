#pragma once

class ball_data_gui;
class QPainter;

struct rebound_data
{
  bool m_rebound_exist = false;
  double m_t_befor_rebound = 0;
  double m_new_vx = 0;
  double m_new_vy = 0;
};

class abstract_geom_object
{

public:
  abstract_geom_object ();
  virtual ~abstract_geom_object ();

  virtual rebound_data intersect_with_trajectory_of_ball (const ball_data_gui &ball) const = 0;
  virtual void paint (QPainter *painter) const = 0;
};
