#include "ball_data_gui.h"

#include "src/kernel/constants.h"

#include <QPicture>
#include <QPainter>

ball_data_gui::ball_data_gui ()
{

}

QPointF ball_data_gui::get_pos () const
{
  return m_pos;
}

double ball_data_gui::get_vx () const
{
  return m_vx;
}

double ball_data_gui::get_vy () const
{
  return m_vy;
}

void ball_data_gui::set_pos (QPointF pos)
{
  m_pos = pos;
}

void ball_data_gui::set_vx (double vx)
{
  m_vx = vx;
}

void ball_data_gui::set_vy (double vy)
{
  m_vy = vy;
}

void ball_data_gui::paint (QPainter *painter)
{
  QPixmap pixmap;
  pixmap.load (":/resourse/ball.svg");

  QRectF sourse (QPointF (0, 0), QPointF (32, 32));
  QRectF target (QPointF (m_pos.x () - 0.01, m_pos.y () - 0.01),
                 QPointF (m_pos.x () + 0.1, m_pos.y () + 0.1));

  painter->drawPixmap (target, pixmap, sourse);
}

void ball_data_gui::move_ball_at_time (double delta_t)
{
  m_pos.rx () += m_vx * delta_t;
  m_pos.ry () += m_vy * delta_t - ACCG * delta_t * delta_t / 2.;

  m_vy -= ACCG * delta_t;
}

ball_data_gui::~ball_data_gui () = default;

