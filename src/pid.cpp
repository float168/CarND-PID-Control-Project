#include "pid.hpp"


PID::PID(double kp, double ki, double kd)
  : m_kp(kp), m_ki(ki), m_kd(kd)
{}

void PID::UpdateCTE(double cte) {
  m_diff_cte = cte - m_cte;
  m_cte = cte;
  m_sum_cte += cte;

  m_total_error += cte * cte;
}

double PID::GetTotalError() const {
  return m_total_error;
}

double PID::CalcSteeringValue(double speed, double angle) const {
  double steer = - (m_kp * m_cte + m_ki * m_sum_cte + m_kd * m_diff_cte);
  if (steer >  1.0) { steer =  1.0; }
  if (steer < -1.0) { steer = -1.0; }
  return steer;
}
