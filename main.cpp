#include "world.h"
#include "particle.h"
#include <iostream>
#include <Eigen/Dense>

int main()
{
    World world_test;
    Particle particle_test;
    std::cout << particle_test.metric << "\n";
    particle_test.updateMetric(world_test.getMetricTensor(particle_test.x));
    std::cout << particle_test.metric << "\n";

    return 0;
}
