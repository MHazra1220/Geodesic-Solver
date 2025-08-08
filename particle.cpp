#include "particle.h"
#include <Eigen/Dense>

using namespace Eigen;

// Gets the value of the metric tensor at the current position of the particle.
void Particle::updateMetric(Matrix4d new_metric)
{
    metric = new_metric;
}
