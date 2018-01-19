#include "geometry_data_gui.h"

#include "src/gui/ball_data_gui.h"
#include "src/gui/abstract_geom_object.h"
#include "src/gui/line_segment_object.h"
#include "src/kernel/future_cpp_features.h"

#include <algorithm>

geometry_data_gui::geometry_data_gui ()
{
  create_default_ball ();
}

void geometry_data_gui::create_default_ball()
{
  m_ball = std::make_unique<ball_data_gui> ();
  m_ball->set_pos (QPointF (0, 0));
  m_ball->set_vx (4);
  m_ball->set_vy (10);
}

ball_data_gui *geometry_data_gui::get_ball ()
{
  return m_ball.get ();
}

const ball_data_gui *geometry_data_gui::get_ball () const
{
  return m_ball.get ();
}

std::vector<std::unique_ptr<abstract_geom_object>> &geometry_data_gui::get_geom_objects ()
{
  return m_geom_objects;
}

const std::vector<std::unique_ptr<abstract_geom_object>> &geometry_data_gui::get_geom_objects () const
{
  return m_geom_objects;
}

void geometry_data_gui::move_geom_at_time (double delta_t)
{
  m_ball->move_ball_at_time (delta_t);
}

rebound_data geometry_data_gui::get_first_rebound () const
{
  rebound_data first_rebound;
  for (const auto &obj : m_geom_objects)
    {
      rebound_data rebound = obj->intersect_with_trajectory_of_ball (*m_ball);
      if (rebound.m_rebound_exist && (!first_rebound.m_rebound_exist || first_rebound.m_t_befor_rebound > rebound.m_t_befor_rebound))
        first_rebound = rebound;
    }

  return first_rebound;
}

void geometry_data_gui::paint_all_objects (QPainter *painter) const
{
  for (const auto &obj : m_geom_objects)
    obj->paint (painter);

  m_ball->paint (painter);
}

void geometry_data_gui::clear_all_objects ()
{
  m_geom_objects.clear ();
  create_default_ball ();
}

void geometry_data_gui::delete_not_visibility_objects (double bottom)
{
  m_geom_objects.erase (std::remove_if (m_geom_objects.begin (), m_geom_objects.end (),
                                        [bottom] (const std::unique_ptr<abstract_geom_object> &obj) {
      return !obj->check_visibility (bottom);
    }), m_geom_objects.end ());
}

geometry_data_gui::~geometry_data_gui () = default;

