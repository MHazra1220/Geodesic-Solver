#include "particle.h"
#include <Eigen/Dense>

using namespace Eigen;

// Updates particle coordinates.
void Particle::setX(Vector4d new_x)
{
    x = new_x;
}

// Updates particle 4-velocity.
void Particle::setV(Vector4d new_v)
{
    v = new_v;
}

// Gets the value of the metric tensor at the current position of the particle.
void Particle::updateMetric(Matrix4d new_metric)
{
    metric = new_metric;
}
