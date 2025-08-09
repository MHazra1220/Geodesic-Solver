#ifndef WORLD
#define WORLD

#include "particle.h"
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;

/*
 *  Everything in the simulation works in coordinates of (ct, x, y, z)
 *  with the assumption that c = 1 is set, so the coordinates are
 *  in terms of just (t, x, y, z).
 */

// World is the internal simulation object and contains the metric and all the rays.
class World
{
    public:
        int num_particles { 0 };

        // Function to retrieve the metric tensor at x.
        // TODO: For now, this function is user-defined.
        Matrix4d getMetricTensor(Vector4d &x);
        // Calculates the 30 required independent Christoffel symbols using central difference numerical derivatives.
        // Returns an array of 4 Eigen::Matrix4d objects, one for each coordinate of the upper index.
        // TODO: Use GiNaC or something to do this symbolically using the form of the metric.
        std::vector<Matrix4d> getChristoffelSymbols(Vector4d &x);
};

#endif
