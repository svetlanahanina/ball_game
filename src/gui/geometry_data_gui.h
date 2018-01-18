#pragma once

#include <memory>
#include <vector>

#include "src/gui/abstract_geom_object.h"

class QPainter;

class ball_data_gui;

class geometry_data_gui
{
  std::unique_ptr<ball_data_gui> m_ball;
  std::vector<std::unique_ptr<abstract_geom_object>> m_geom_objects;

  void create_default_ball ();
public:
  geometry_data_gui ();
  ~geometry_data_gui ();

  ball_data_gui *get_ball ();
  const ball_data_gui *get_ball () const;

  std::vector<std::unique_ptr<abstract_geom_object>> &get_geom_objects ();
  const std::vector<std::unique_ptr<abstract_geom_object>> &get_geom_objects () const;

  void move_geom_at_time (double delta_t);
  rebound_data get_first_rebound () const;

  void paint_all_objects (QPainter *painter) const;

  void clear_all_objects ();
};
