#include "particle.h"
#include "world.h"
#include <cmath>
#include <Eigen/Dense>

using namespace Eigen;

void Particle::setX(Vector4d new_x)
{
    x = new_x;
}

void Particle::setV(Vector4d new_v)
{
    v = new_v;
}

/*
 * Modifies the t-component of the 4-velocity to make the vector null.
 * R equires v and the* metric at the current coordinates to be defined.
 * This requires solving a quadratic equation for the t-component; assume
 * that you should take the more positive component because g_00 is
 * probably negative. Note that both solutions are negative in a black hole.
 * The raytracer evolves photons "backwards", as if they are being emitted
 * from the camera/observer. Use this when defining the initial velocities
 * of photons to force them to be null.
 */
void Particle::makeVNull()
{
    Vector3d spatial_v { v(seq(1, 3)) };
    double a { metric(0, 0) };
    double b { 2.*metric(seq(1, 3), 0).dot(spatial_v) };
    Matrix3d spatial_metric { metric(seq(1, 3), seq(1, 3)) };
    double c { spatial_v.dot(spatial_metric*spatial_v) };
    v(0) = (-b - sqrt(b*b - 4.*a*c)) / (2.*a);
}

void Particle::updateMetric(Matrix4d new_metric)
{
    metric = new_metric;
}

// Advances the ray path by a parameter step, dl.
// WARNING: Due to the null constraint, there are only three independent
// velocity components, but it's much simpler to integrate all 4 (and
// probably not slower or any less accurate).
// TODO: Adapt the step-size based on how far the metric at the position
// deviates from the Minkowski metric.
void Particle::advance(World &simulation)
{
    // Calculate parameter step.
    double dl { calculateParameterStep() };

    // Currently advances with RK4.
    Vector4d x_step { 0., 0., 0., 0. };
    Vector4d v_step { 0., 0., 0., 0. };

    Vector4d k_n_minus_1_x;
    Vector4d k_n_x;
    Vector4d k_n_minus_1_v;
    Vector4d k_n_v;

    Vector4d temp_x;
    Vector4d temp_v;
    Matrix4d temp_metric;

    std::vector<Matrix4d> christoffel_symbols;

    // Calculate k_1.
    christoffel_symbols = simulation.getChristoffelSymbols(x, metric);
    k_n_x = v;
    k_n_v = v_derivative(v, christoffel_symbols);
    x_step += k_n_x;
    v_step += k_n_v;

    // Calculate k_2 and k_3.
    for (int i { 0 }; i < 2; i++)
    {
        k_n_minus_1_x = k_n_x;
        k_n_minus_1_v = k_n_v;
        temp_x = x + 0.5*dl*k_n_minus_1_x;
        temp_v = v + 0.5*dl*k_n_minus_1_v;
        temp_metric = simulation.getMetricTensor(temp_x);
        christoffel_symbols = simulation.getChristoffelSymbols(temp_x, temp_metric);
        k_n_x = v + 0.5*dl*k_n_minus_1_v;
        k_n_v = v_derivative(temp_v, christoffel_symbols);
        x_step += 2.*k_n_x;
        v_step += 2.*k_n_v;
    }

    // Calculate k_4.
    k_n_minus_1_x = k_n_x;
    k_n_minus_1_v = k_n_v;
    temp_x = x + dl*k_n_minus_1_x;
    temp_v = v + dl*k_n_minus_1_v;
    temp_metric = simulation.getMetricTensor(temp_x);
    christoffel_symbols = simulation.getChristoffelSymbols(temp_x, temp_metric);
    k_n_x = v + dl*k_n_minus_1_v;
    k_n_v = v_derivative(temp_v, christoffel_symbols);
    x_step += k_n_x;
    v_step += k_n_v;

    // Advance x and v.
    x += (dl/6.)*x_step;
    v += (dl/6.)*v_step;

    updateMetric(simulation.getMetricTensor(x));
}

// Scalar product of the 4-velocity. Primarily for debugging to check the scalar product is conserved.
double Particle::scalarProduct()
{
    return v.dot(metric*v);
}

// Tries to estimate how far the metric deviates from the Minkowski metric as a cheap way
// of estimating how curved the spacetime is without resorting to the Riemann tensor.
double Particle::minkowskiDeviation()
{
    // Try to "normalise" the metric against things that scale the entire metric
    // but don't actually cause any curvature.
    double scale_factor { 0 };
    scale_factor = metric.diagonal().cwiseAbs().sum() / 4.;
    Matrix4d minkowski;
    minkowski.setIdentity();
    minkowski(0, 0) = -1.;
    Matrix4d deviation { metric/scale_factor - minkowski };
    // Sum up the absolute values of this deviation matrix.
    return deviation.cwiseAbs().sum();
}

// Calculates an adaptive step size by estimating how curved the space is.
double Particle::calculateParameterStep()
{
    double deviation { minkowskiDeviation() };
    // Designate a deviation of approximately 3 to give a step size of 0.01 (this produces stability
    // for about 3-4 orbits in a photon ring orbit around a Schwarzschild black hole).
    double step { 1e-2 * (3./deviation) };
    if (step < maxParameterStep)
    {
        return step;
    }
    else
    {
        return maxParameterStep;
    }
}

// Returns the derivative of the given 4-velocity using the provided Christoffel symbols.
Vector4d v_derivative(Vector4d v, std::vector<Matrix4d> &christoffel_symbols)
{
    Vector4d acceleration;
    double sum;
    for (int mu { 0 }; mu < 4; mu++)
    {
        acceleration(mu) = -1.*v.dot(christoffel_symbols[mu]*v);
    }

    return acceleration;
}
