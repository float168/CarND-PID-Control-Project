#include "pid.hpp"


PID::PID(double k_p, double k_i, double k_d)
  : m_k_p(k_p), m_k_i(k_i), m_k_d(k_d)
{}

void PID::UpdateError(double cte) {
  m_diff_cte = cte - m_cte;
  m_cte = cte;
  m_total_cte += cte;
}

double PID::GetTotalError() const {
  return m_total_cte;
}

double PID::CalcSteeringValue(double speed, double angle) const {
  double steer = - (m_k_p * m_cte + m_k_i * m_total_cte + m_k_d * m_diff_cte);
  if (steer >  1.0) { steer =  1.0; }
  if (steer < -1.0) { steer = -1.0; }
  return steer;
}
