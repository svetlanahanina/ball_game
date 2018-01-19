#include "line_segment_object.h"

#include "src/gui/ball_data_gui.h"

#include "src/kernel/constants.h"
#include "src/kernel/math_functions.h"

#include <iostream>
#include <QPainter>

line_segment_object::line_segment_object ()
{

}

bool line_segment_object::ball_in_line_segment_at_time (const ball_data_gui &ball, double t) const
{
  if (t < 0)
    return false;

  double x = ball.get_pos ().x () + ball.get_vx () * t;
  double y = ball.get_pos ().y () + ball.get_vy () * t - ACCG * t * t / 2.;

  return x <= std::max (m_line.x1 (), m_line.x2 ())
      && x >= std::min (m_line.x1 (), m_line.x2 ())
      && y <= std::max (m_line.y1 (), m_line.y2 ())
      && y >= std::min (m_line.y1 (), m_line.y2 ());
}

void line_segment_object::calculate_velocity_after_rebound (rebound_data &rebound, const ball_data_gui &ball) const
{
  auto t_vect = get_t_vector ();
  auto n_vect = get_n_vector ();

  double vx = ball.get_vx ();
  double vy = ball.get_vy () - ACCG * rebound.m_t_befor_rebound;

  auto result_v = solve_2x2_matrix ({-n_vect.first, -n_vect.second, t_vect.first, t_vect.second},
                                    {vx * n_vect.first + vy * n_vect.second,
                                     vx * t_vect.first + vy * t_vect.second});
  if (result_v.empty ())
    {
      std::cout << "ERROR: singular matrix for new velocity equations\n";
      return;
    }

  rebound.m_new_vx = result_v[0] * m_push_force;
  rebound.m_new_vy = result_v[1] * m_push_force;
}

std::pair<double, double> line_segment_object::get_n_vector () const
{
  return {-m_line.dy (), m_line.dx ()};
}

std::pair<double, double> line_segment_object::get_t_vector() const
{
  return {m_line.dx (), m_line.dy ()};
}

rebound_data line_segment_object::intersect_with_trajectory_of_ball (const ball_data_gui &ball) const
{
  rebound_data rebound;
  // case vertical line segment
  if (math_equal (m_line.x1 (), m_line.x2 ()))
    {
      // case vertical ball trajectory
      if (math_equal (ball.get_vx (), 0))
        return {};

      // case not vertical ball trajectory
      double t = (m_line.x1 () - ball.get_pos ().x ()) / ball.get_vx ();

      if (!ball_in_line_segment_at_time (ball, t))
        return {};

      rebound.m_rebound_exist = true;
      rebound.m_t_befor_rebound = t;
      calculate_velocity_after_rebound (rebound, ball);
      return rebound;
    }

  // compute coefficients for line equation y = kx + l
  double k = m_line.dy () / m_line.dx ();
  double l = m_line.y1 () - k * m_line.x1 ();

  // compute coefficients for square equation at^2 + bt + c = 0
  double a = ACCG / 2.;
  double b = k * ball.get_vx () - ball.get_vy ();
  double c = k * ball.get_pos ().x () - ball.get_pos ().y () + l;

  auto solutions = solve_square_equation (a, b, c);

  for (const auto &t : solutions)
    {
      if (!ball_in_line_segment_at_time (ball, t))
        continue;

      if (rebound.m_rebound_exist == false || rebound.m_t_befor_rebound > t)
        {
          rebound.m_rebound_exist = true;
          rebound.m_t_befor_rebound = t;
          calculate_velocity_after_rebound (rebound, ball);
        }
    }
  return rebound;
}

void line_segment_object::paint (QPainter *painter) const
{
  QPen pen;
  pen.setCosmetic (true);
  painter->setPen (pen);
  painter->drawLine (m_line);
}

bool line_segment_object::check_visibility (double bottom) const
{
  return !(m_line.y1 () < bottom && m_line.y2 () < bottom);
}

void line_segment_object::set_line (QLineF line)
{
  m_line = line;
}

void line_segment_object::set_push_force (double push_force)
{
  m_push_force = push_force;
}

line_segment_object::~line_segment_object () = default;

