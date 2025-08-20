#include "world.h"
#include "particle.h"
#include "camera.h"
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include <omp.h>

using namespace Eigen;

int main()
{
    World world_test;
    Camera camera_test;
    camera_test.setWidthHeight(1920, 1080);
    camera_test.setFov(15.);
    camera_test.setCameraLocation(Vector3d { -10., 0., 0. });
    // Point along +x (No change in orientation required).
    camera_test.setCameraOrientation(Vector4d { 1., 0., 0., 0. });

    char file[] { "/media/mh2001/SSD2/Programming/General Relativity/Geodesic_Solver/sky_box_samples/Internet Explorer hent-1.bmp" };
    char* ref { file };
    world_test.readSkyMap(ref);
    world_test.readPixelFromSkyMap(1200, 0);

    Particle particle_test;
    particle_test.setX(Vector4d { 0., -1.55, 0., 0. });
    particle_test.setV(Vector4d { 1., 0., 1., 0. });
    particle_test.updateMetric(world_test.getMetricTensor(particle_test.x));
    particle_test.makeVNull();

    double product;

    // for (int i { 0 }; i < 1000; i++)
    // {
    //     // product = particle_test.scalarProduct();
    //     // std::cout << product << "\n";
    //     // std::cout << particle_test.x << "\n";
    //     std::cout << particle_test.getEuclideanDistance() << "\n";
    //     // std::cout << particle_test.minkowskiDeviation() << "\n";
    //     particle_test.advance(world_test);
    // }

    return 0;
}
