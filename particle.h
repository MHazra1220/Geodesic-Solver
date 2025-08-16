#ifndef PARTICLE
#define PARTICLE

#include "world.h"
#include <Eigen/Dense>

/*
 *  Everything in the simulation works in coordinates of (ct, x, y, z)
 *  with the assumption that c = 1 is set, so that coordinates are
 *  in terms of just (t, x, y, z).
 */

using namespace Eigen;

class Particle
{
    public:
        // 4-position in (t, x, y, z) coordinates of the particle.
        Vector4d x{ 0., 0., 0., 0. };
        // TODO: 4-velocity of the particle; default components depend on whether the particle is light-like or time-like.
        Vector4d v{ 1., 0., 0., 0. };
        // Metric tensor at the particle's coordinates, assumed to be symmetric (torsion-free).
        Matrix4d metric;

        void setX(Vector4d new_x);
        void setV(Vector4d new_v);
        void makeVNull();
        void updateMetric(Matrix4d new_metric);
        // Advances the simulation using RK4 by a parameter step, dl.
        // TODO: Consider other, potentially more stable ODE integrators, e.g. a symplectic integrator.
        void advance(double dl, World &simulation);

        // Returns the scalar product of the 4-velocity; primarily for debugging purposes.
        double scalarProduct();
};

// Return the parameter derivative of a 4-velocity under the provided Christoffel symbols.
Vector4d v_derivative(Vector4d v, std::vector<Matrix4d> &christoffel_symbols);

#endif
