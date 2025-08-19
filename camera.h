#ifndef CAMERA
#define CAMERA

#include <Eigen/Dense>

using namespace Eigen;

class Camera
{
public:
    // Render resolution; 1440p widescreen by default.
    int image_width { 2560 };
    int image_height { 1440 };
    // Store double versions for the sake of calculation.
    double double_width;
    double double_height;
    // Horizontal FoV in degrees.
    // TODO: Make the program assume that the FoV spans across the wider of either the width or height of the image.
    double fov_width { 10. };
    double fov_width_rad;
    // Cartesian location of the camera.
    Vector3d camera_location;
    // Quaternion that determines the orientation of the camera, defined as a rotation from pointing along +x with zero angle.
    Vector4d camera_orientation { 1., 0., 0., 0. };

    void setWidthHeight(int pixels_width, int pixels_height);
    void setFov(double fov);
    void setCameraLocation(Vector3d location);
    void setCameraOrientation(Vector4d orientation);

    // Calculates the starting direction of the photon mapped to pixel x and pixel y.
    // x=0 and y=0 is the bottom left corner.
    Vector3d calculateStartDirection(int x, int y);

private:
    double pi = 3.141592653589793;
};

// Returns the Hamilton product of u with v.
Vector4d hamiltonProduct(Vector4d u, Vector4d v);

// Returns a 3D cartesian vector rotated by the given quaternion.
Vector3d quaternionRotate(Vector3d vec, Vector4d rotation_quat);

#endif
