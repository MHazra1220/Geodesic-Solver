#include "world.h"
#include "particle.h"
#include <iostream>
#include <vector>
#include <Eigen/Dense>

int main()
{
    World world_test;
    Particle particle_test;
    particle_test.setX(Vector4d { 40., -5., 10.4, 8. });
    particle_test.updateMetric(world_test.getMetricTensor(particle_test.x));
    std::vector<Matrix4d> christoffel = world_test.getChristoffelSymbols(particle_test.x, particle_test.metric);
    std::cout << christoffel[0] << "\n";
    std::cout << christoffel[2] << "\n";

    return 0;
}
