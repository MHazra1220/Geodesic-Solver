#include "world.h"
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;

// Currently returns the metric of a Schwarzschild black hole with a Schwarzschild radius of 1 (normalised units).
Matrix4d World::getMetricTensor(Vector4d &x)
{
    double r_s = 1.;
    double r = x(seq(1, 3)).dot(x(seq(1, 3)));
    double r_squared = r*r;
    double mult_factor = r_s / (r_squared * (r - r_s));
    Matrix4d metric;
    metric.setIdentity();
    metric(0, 0) = -1. + r_s/r;
    metric(1, 1) += x(1)*x(1);
    metric(2, 2) += x(2)*x(2);
    metric(3, 3) += x(3)*x(3);
    metric(1, 2) = x(1)*x(2);
    metric(2, 1) = metric(1, 2);
    metric(1, 3) = x(1)*x(3);
    metric(3, 1) = metric(1, 3);
    metric(2, 3) = x(2)*x(3);
    metric(3, 2) = metric(2, 3);
    metric *= mult_factor;
    metric(0, 0) /= mult_factor;

    return metric;
}

// TODO: Write central difference numerical derivatives to get the Christoffel symbols.
// Consider using GiNaC to do this symbolically.
// WARNING: This is going to be an utter bastard to write.
std::vector<Matrix4d> World::getChristoffelSymbols(Vector4d &x)
{
    // Each entry corresponds to a coordinate of the upper index of the Christoffel symbols.
    std::vector<Matrix4d> symbols (4);

    return symbols;
}
