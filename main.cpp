#include "world.h"
#include "particle.h"
#include <iostream>
#include <Eigen/Dense>

int main()
{
    World world_test;
    Particle particle_test;
    particle_test.setX(Vector4d {0., -0.1, 0.5, -0.8});
    std::cout << particle_test.metric << "\n";
    particle_test.updateMetric(world_test.getMetricTensor(particle_test.x));
    std::cout << particle_test.metric << "\n";

    return 0;
}
