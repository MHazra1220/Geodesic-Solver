#include "world.h"
#include <cmath>
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;

// Currently returns the Schwarzschild metric with a Schwarzschild radius of 1 (normalised units).
// Keep in mind that, for a Schwarzschild metric, there is no need to simulate a photon that crosses inside
// the photon sphere; any photon that does so it guaranteed to cross the horizon and should render
// a black pixel.
// TODO: Probably want to store a set of common metrics that the user can select.
Matrix4d World::getMetricTensor(Vector4d x)
{
    const double r_s = 1.;
    double r_squared = x(seq(1, 3)).dot(x(seq(1, 3)));
    double r = sqrt(r_squared);
    double mult_factor = r_s / (r_squared * (r - r_s));
    Matrix4d metric;
    metric.setZero();
    for (int j { 1 }; j < 4; j++)
    {
        for (int i { j }; i < 4; i++)
        {
            metric(i, j) = x(i)*x(j);
            metric(j, i) = metric(i, j);
        }
    }
    metric *= mult_factor;
    metric(0, 0) = -1. + r_s/r;
    metric(1, 1) += 1.;
    metric(2, 2) += 1.;
    metric(3, 3) += 1.;

    return metric;
}

// Calculates the Euclidean "distance"; this is useful for some metrics (e.g.
// calculating if a photon has crossed inside the photon sphere of the Schwarzschild metric)
// and for checking if a photon has escaped sufficiently far to infinity (i.e. hit the
// background skybox/skysphere).
double World::getEuclideanDistance(Vector4d x)
{
    return sqrt(x(seq(1, 3)).dot(x(seq(1, 3))));
}

// TODO: Consider using GiNaC to do this symbolically (though I doubt this will be beneficial for complicated metrics).
// TODO: Consider writing a Hamiltonian raytracer that doesn't require the Christoffel symbols (but does require multiple matrix inversions).
std::vector<Matrix4d> World::getChristoffelSymbols(Vector4d x, Matrix4d &metric)
{
    // The metric at x is also passed in because it should already be present in whatever particle this
    // is being used for.
    // Each entry corresponds to a coordinate of the upper index of the Christoffel symbols.
    std::vector<Matrix4d> christoffel_symbols (4);
    // Assumed default step in each coordinate (only really works if c=1).
    // TODO: How do you define this adaptively to not break near areas of extreme distortion?
    // For now, just set it to a small number.
    double step { 1e-4 };

    // Second-order accurate central-difference derivatives of the metric along each component.
    std::vector<Matrix4d> metric_derivs (4);
    Matrix4d metric_forward;
    Matrix4d metric_backward;
    Vector4d intermediate_x { x };
    for (int mu { 0 }; mu < 4; mu++)
    {
        intermediate_x(mu) += step;
        metric_forward = getMetricTensor(intermediate_x);
        intermediate_x(mu) -= 2.*step;
        metric_backward = getMetricTensor(intermediate_x);
        metric_derivs[mu] = (metric_forward - metric_backward) / (2.*step);
        intermediate_x(mu) = x(mu);
    }

    Matrix4d metricInverse { metric.inverse() };

    // Go through each upper index of the Christoffel symbols and only calculate the 40 independent components.
    for (int alpha { 0 }; alpha < 4; alpha++)
    {
        Matrix4d christoffel_component;
        for (int nu { 0 }; nu < 4; nu++)
        {
            for (int mu = nu; mu < 4; mu++)
            {
                Vector4d metric_deriv_components;
                for (int gamma { 0 }; gamma < 4; gamma++)
                {
                    metric_deriv_components(gamma) = metric_derivs[nu](gamma, mu)
                    + metric_derivs[mu](gamma, nu)
                    - metric_derivs[gamma](mu, nu);
                }
                christoffel_component(mu, nu) = 0.5*metricInverse.col(alpha).dot(metric_deriv_components);
                // This is redundant when mu = nu, but probably faster than an if statement.
                christoffel_component(nu, mu) = christoffel_component(mu, nu);
            }
        }
        christoffel_symbols[alpha] = christoffel_component;
    }

    return christoffel_symbols;
}
