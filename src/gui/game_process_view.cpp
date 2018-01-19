#include "game_process_view.h"

#include "src/gui/geometry_data_gui.h"
#include "src/gui/ball_data_gui.h"
#include "src/gui/line_segment_object.h"
#include "src/kernel/future_cpp_features.h"

#include <QPainter>
#include <QMessageBox>
#include <QMouseEvent>

game_process_view::game_process_view (QWidget *parent, geometry_data_gui *geom_data)
  : QWidget (parent), m_geom_data (geom_data)
{
  m_next_rebound = m_geom_data->get_first_rebound ();

  m_timer = new QTimer (this);
  m_timer->setInterval (1);
  connect (m_timer, SIGNAL (timeout ()), this, SLOT (update_time ()));
  m_timer->start ();
}

game_process_view::~game_process_view () = default;

bool game_process_view::return_ball_in_bounds ()
{
  auto pos = m_geom_data->get_ball ()->get_pos ();
  if (pos.x () < 0)
    {
      pos.rx () += m_width;
      m_geom_data->get_ball ()->set_pos (pos);
      return true;
    }

  if (pos.x () > m_width)
    {
      pos.rx () -= m_width;
      m_geom_data->get_ball ()->set_pos (pos);
      return true;
    }

  return false;
}

void game_process_view::calculate_rebound ()
{
  m_current_time = 0;
  m_next_rebound = m_geom_data->get_first_rebound ();
}

void game_process_view::update_time ()
{
  if (m_next_rebound.m_rebound_exist && m_current_time + m_delta_time > m_next_rebound.m_t_befor_rebound)
    {
      m_geom_data->get_ball ()->set_vx (m_next_rebound.m_new_vx);
      m_geom_data->get_ball ()->set_vy (m_next_rebound.m_new_vy);

      calculate_rebound ();
    }

  m_current_time += m_delta_time;
  m_geom_data->move_geom_at_time (m_delta_time);

  if (m_geom_data->get_ball ()->get_pos ().y () > m_max_y)
    {
      m_max_y = m_geom_data->get_ball ()->get_pos ().y ();
      m_bottom = m_max_y - 0.5 * m_height;
      m_geom_data->delete_not_visibility_objects (m_bottom);
    }

  if (return_ball_in_bounds ())
    calculate_rebound ();

  if (m_geom_data->get_ball ()->get_pos ().y () < m_bottom)
    {
      QString str = QString("You climbed %1 meters").arg (m_max_y, 0, 'g', 2);
      if (QMessageBox::information (this, "You Lose", str, QMessageBox::Reset, QMessageBox::Cancel)
          == QMessageBox::Reset)
        {
          m_geom_data->clear_all_objects ();
          calculate_rebound ();
          m_max_y = 0;
        }
      else
        exit (1);

      return;
    }

  update ();
}

QSize game_process_view::minimumSizeHint () const
{
  return QSize (100, 100);
}

QSize game_process_view::sizeHint () const
{
  return QSize (1000, 1000);
}

QPointF game_process_view::map_to_scene (QPointF p)
{
  p.rx () -= 0.5 * width ();
  p.rx () /= (width () / m_width);
  p.rx () += m_width / 2.;

  p.ry () -= 0.5 * height ();
  p.ry () /= (-height () / m_height);
  p.ry () += m_max_y;
  return p;
}

void game_process_view::paintEvent (QPaintEvent * /* event */)
{
  QPainter painter (this);

  // save current Coordinate System
  painter.save ();

  // make Coordinate Transformations
  painter.translate (0.5 * width (), 0.5 * height ());
  painter.scale (width () / m_width, -height () / m_height);
  painter.translate (-m_width / 2., -m_max_y);

  m_geom_data->paint_all_objects (&painter);

  QPen pen (Qt::red);
  pen.setCosmetic (true);
  painter.setPen (pen);
  if (m_line_is_creating)
    painter.drawLine (m_creating_line);

  // restore previously saved Coordinate System
  painter.restore ();

  constexpr double indent = 20;

  QString str = QString("Y: %1").arg(m_max_y, 0, 'g', 2);
  painter.drawText (5, indent, str);
}

void game_process_view::mousePressEvent (QMouseEvent *event)
{
  m_creating_line.setP1 (map_to_scene (event->windowPos ()));
  m_creating_line.setP2 (map_to_scene (event->windowPos ()));
  m_line_is_creating = true;
}

void game_process_view::mouseMoveEvent (QMouseEvent *event)
{
  m_creating_line.setP2 (map_to_scene (event->windowPos ()));
}

void game_process_view::mouseReleaseEvent (QMouseEvent *event)
{
  m_creating_line.setP2 (map_to_scene (event->windowPos ()));

  m_line_is_creating = false;
  auto line_segment = std::make_unique<line_segment_object> ();
  line_segment->set_line (m_creating_line);
  line_segment->set_push_force (1.5);
  m_geom_data->get_geom_objects ().push_back (std::move (line_segment));
  calculate_rebound ();
}


























