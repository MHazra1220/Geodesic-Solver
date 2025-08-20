#include "camera.h"
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Eigen;

void Camera::setWidthHeight(int pixels_width, int pixels_height)
{
    image_width = pixels_width;
    image_height = pixels_height;
    double_width = image_width;
    double_height = image_height;
    // Set the size of the array of the camera view pixel array.
    // 3 bytes per pixel for RGB.
    camera_view.resize(image_width*image_height*3);
}

void Camera::setFov(double fov)
{
    fov_width = fov;
    // Convert to radians.
    fov_width_rad = (fov_width/180.)*pi;
}

void Camera::setCameraLocation(Vector3d location)
{
    camera_location = location;
}

// Used to rotate the starting 4-velocities of photons.
void Camera::setCameraOrientation(Vector4d orientation)
{
    camera_orientation = orientation / orientation.norm();
}

// Calculates the starting direction of the photon mapped to pixel x and pixel y.
// x=0 and y=0 are the bottom-left corner of the camera.
Vector3d Camera::calculateStartDirection(int x, int y)
{
    double conversion_factor { fov_width_rad/double_width };
    double phi { (x-0.5*double_width)*conversion_factor };
    double theta { (y-0.5*double_height)*conversion_factor + 0.5*pi };

    // Convert to a Cartesian unit vector in (x, y, z).
    Vector3d start_direction;
    start_direction(1) = sin(theta)*cos(phi);
    start_direction(2) = sin(theta)*sin(phi);
    start_direction(3) = cos(theta);
    // Rotate to align with the camera orientation.
    return quaternionRotate(start_direction, camera_orientation);
    // From here, the Particle/Photon object this is sent to needs
    // to normalise the direction to a null 4-velocity.
}

void Camera::traceImage()
{
    // Go through each ray.
    for (int x { 0 }; x < image_width; x++)
    {
        for (int y { 0 }; y < image_height; y++)
        {

        }
    }
}

// Output the pixel array of the camera's view to an image file.
void Camera::writeCameraImage(char* image_path)
{
    unsigned char* data { &camera_view.data()[0] };
    stbi_write_jpg(image_path, image_width, image_height, 3, data, 100);
}

// Returns the Hamilton (quaternionic) product of u with v.
Vector4d hamiltonProduct(Vector4d u, Vector4d v)
{
    Vector4d result;
    Vector3d u_vec { u(seq(1, 3)) };
    Vector3d v_vec { v(seq(1, 3)) };
    result(0) = u(0)*v(0) - u_vec.dot(v_vec);
    result(seq(1, 3)) = u(0)*v_vec + v(0)*u_vec + u_vec.cross(v_vec);

    return result;
}

// Returns a 3D cartesian vector rotated by the given quaternion.
Vector3d quaternionRotate(Vector3d vec, Vector4d rotation_quat)
{
    // Assume that rotation_quat is normalised.
    Vector4d rotation_quat_inverse { rotation_quat };
    rotation_quat_inverse(seq(1, 3)) *= -1.;
    Vector4d vec_as_quat;
    vec_as_quat(0) = 0.;
    vec_as_quat(seq(1, 3)) = vec;

    return hamiltonProduct(rotation_quat, hamiltonProduct(vec_as_quat, rotation_quat_inverse))(seq(1, 3));
}
