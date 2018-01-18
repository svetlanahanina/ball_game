#include "math_functions.h"

#include <math.h>

#include "src/kernel/constants.h"

bool math_equal (double a, double b)
{
  return fabs (b - a) < EPS;
}

std::vector<double> solve_square_equation (double a, double b, double c)
{
  if (math_equal (a, 0) && math_equal (b, 0))
    return {};

  if (math_equal (a, 0))
    return {-c / b};

  double d = b * b - 4 * a * c;
  if (d < 0)
    return {};

  if (math_equal (d, 0))
    return {-b / (2. * a)};

  return {(-b + sqrt (d)) / (2. * a), (-b - sqrt (d)) / (2. * a)};
}

std::vector<double> solve_2x2_matrix (std::vector<double> mat, std::vector<double> rhs)
{
  if (math_equal (mat[0], 0))
    {
      std::swap (mat[0], mat[2]);
      std::swap (mat[1], mat[3]);
      std::swap (rhs[0], rhs[1]);
    }

  // singular matrix
  if (math_equal (mat[0], 0))
    return {};

  mat[3] -= mat[1] * mat[2] / mat[0];
  rhs[1] -= rhs[0] * mat[2] / mat[0];

  // singular matrix
  if (math_equal (mat[3], 0))
    return {};

  std::vector<double> res (2);
  res[1] = rhs[1] / mat[3];
  res[0] = (rhs[0] - res[1] * mat[1]) / mat[0];
  return res;
}
