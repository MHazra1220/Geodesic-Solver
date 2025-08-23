#include "world.h"
#include "particle.h"
#include "camera.h"
#include <iostream>
#include <string>
#include <Eigen/Dense>

using namespace Eigen;

int main()
{
    World world_test;
    char file[] { "/media/mh2001/SSD2/Programming/General Relativity/Geodesic_Solver/sky_box_samples/full_milky_way.jpg" };
    char* ref { file };
    world_test.importSkyMap(ref);
    Camera camera_test;
    camera_test.setWidthHeight(2560, 1440);
    camera_test.setFov(90.);
    camera_test.setCameraLocation(Vector4d { 0., 20., 0., 0. });
    // Point along +x (No change in orientation required).
    camera_test.setCameraOrientation(Vector4d { 0., 0., 0., 1. });
    camera_test.traceImage(world_test);
    char output_image[] { "/media/mh2001/SSD2/Programming/General Relativity/Geodesic_Solver/output_images/test_pointer.jpg" };
    char* output_image_ref { output_image };
    camera_test.writeCameraImage(output_image_ref);

    return 0;
}
