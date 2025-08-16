#include "world.h"
#include "particle.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <Eigen/Dense>

int main()
{
    World world_test;
    Particle particle_test;
    particle_test.setX(Vector4d { 0., -1.5, 0., 0. });
    particle_test.setV(Vector4d { 1., 0., 1., 0. });
    particle_test.updateMetric(world_test.getMetricTensor(particle_test.x));
    particle_test.makeVNull();

    double product;

    for (int i { 0 }; i < 1000; i++)
    {
        product = particle_test.scalarProduct();
        std::cout << product << "\n";
        std::cout << particle_test.x << "\n";
        std::cout << particle_test.x(seq(1, 3)).dot(particle_test.x(seq(1, 3))) << "\n";
        particle_test.advance(1e-2, world_test);
        particle_test.updateMetric(world_test.getMetricTensor(particle_test.x));
    }

    return 0;
}
