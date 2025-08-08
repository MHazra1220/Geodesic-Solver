#include "world.h"
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;

// Currently returns the metric of a Schwarzschild black hole with a mass of one million solar masses.
Matrix4d World::getMetricTensor(Vector4d &x)
{
    double M = 1e6 * 1.989e30;
    Matrix4d metric;
    metric.setIdentity();
    metric(0, 0) = -1.;
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
