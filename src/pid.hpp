#ifndef PID_HPP
#define PID_HPP

#include <cstdint>


class PID {
 public:
  PID(double kp, double ki, double kd);

  void UpdateCTE(double cte);

  double GetTotalError() const;

  double CalcSteeringValue(double speed, double angle) const;

 private:
  const double m_kp;
  const double m_ki;
  const double m_kd;

  double m_cte = 0.0;
  double m_diff_cte = 0.0;
  double m_sum_cte = 0.0;

  double m_total_error = 0;
};

#endif  // PID_HPP
