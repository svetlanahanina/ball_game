#pragma once

#include "src/gui/abstract_geom_object.h"

#include <QLineF>

class line_segment_object : public abstract_geom_object
{
  QLineF m_line;
  double m_push_force = 1;

  bool ball_in_line_segment_at_time (const ball_data_gui &ball, double t) const;
  void calculate_velocity_after_rebound (rebound_data &rebound, const ball_data_gui &ball) const;
  std::pair<double, double> get_n_vector () const; // normal vector for line
  std::pair<double, double> get_t_vector () const; // tangent vector for line
public:
  line_segment_object ();
  ~line_segment_object ();

  virtual rebound_data intersect_with_trajectory_of_ball (const ball_data_gui &ball) const override;
  virtual void paint (QPainter *painter) const override;
  virtual bool check_visibility (double bottom) const override;

  void set_line (QLineF line);
  void set_push_force (double push_force);
};
