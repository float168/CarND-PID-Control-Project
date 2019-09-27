#ifndef PID_HPP
#define PID_HPP

class PID {
 public:
  PID(double k_p, double k_i, double k_d);

  void UpdateError(double cte);

  double GetTotalError() const;

  double CalcSteeringValue(double speed, double angle) const;

 private:
  const double m_k_p;
  const double m_k_i;
  const double m_k_d;

  double m_cte = 0.0;
  double m_diff_cte = 0.0;
  double m_total_cte = 0.0;
};

#endif  // PID_HPP
