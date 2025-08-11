#include "world.h"
#include "particle.h"
#include <iostream>
#include <vector>
#include <Eigen/Dense>

int main()
{
    World world_test;
    Particle particle_test;
    particle_test.setX(Vector4d { 40., -200, 300, 400 });
    particle_test.setV(Vector4d { 1., 1., 0., 0. });
    particle_test.updateMetric(world_test.getMetricTensor(particle_test.x));

    for (int i = 0; i < 100; i++)
    {
        std::cout << particle_test.scalarProduct() << "\n";
        std::cout << particle_test.x << "\n";
        particle_test.advance(1e-3, world_test);
    }

    return 0;
}
