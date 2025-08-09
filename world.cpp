#include "world.h"
#include <vector>
#include <Eigen/Dense>

using namespace Eigen;

// Currently returns the Schwarzschild metric with a Schwarzschild radius of 1 (normalised units).
// Probably want to store a set of well-known metrics that the user can select.
Matrix4d World::getMetricTensor(Vector4d &x)
{
    const double r_s = 1.;
    double r = x(seq(1, 3)).dot(x(seq(1, 3)));
    const double r_squared = r*r;
    double mult_factor = r_s / (r_squared * (r - r_s));
    Matrix4d metric;
    metric.setIdentity();
    metric(0, 0) = -1. + r_s/r;
    metric(1, 1) += x(1)*x(1);
    metric(2, 2) += x(2)*x(2);
    metric(3, 3) += x(3)*x(3);
    metric(1, 2) = x(1)*x(2);
    metric(2, 1) = metric(1, 2);
    metric(1, 3) = x(1)*x(3);
    metric(3, 1) = metric(1, 3);
    metric(2, 3) = x(2)*x(3);
    metric(3, 2) = metric(2, 3);
    metric *= mult_factor;
    metric(0, 0) /= mult_factor;

    return metric;
}

// TODO: Write central difference numerical derivatives to get the Christoffel symbols.
// Consider using GiNaC to do this symbolically.
std::vector<Matrix4d> World::getChristoffelSymbols(Vector4d x, Matrix4d &metric)
{
    // The metric at x is also passed in because it should already be present in whatever particle this
    // is being used for.
    // Each entry corresponds to a coordinate of the upper index of the Christoffel symbols.
    std::vector<Matrix4d> christoffel_symbols (4);
    // Assumed default step in each coordinate (only really works if c=1).
    // TODO: How do you define this adaptively to not break near areas of extreme distortion?
    // For now, just set it to a small number.
    const double step { 1e-3 };

    // Derivatives of the metric along each component.
    std::vector<Matrix4d> metric_derivs (4);
    for (int i = 0; i < 4; i++)
    {
        // Second-order central difference scheme.
        Matrix4d metric_forward;
        Matrix4d metric_backward;

        // Don't pass x by reference to stop intermediate_x modifying x.
        Vector4d intermediate_x { x };
        intermediate_x(i) += step;
        metric_forward = getMetricTensor(intermediate_x);
        intermediate_x(i) -= 2.*step;
        metric_backward = getMetricTensor(intermediate_x);
        metric_derivs[i] = (metric_forward - metric_backward) / (2.*step);
    }

    Matrix4d metricInverse = metric.inverse();

    // Go through each upper index of the Christoffel symbols and only calculate the 40 independent components.
    for (int alpha = 0; alpha < 4; alpha++)
    {
        Matrix4d christoffel_component;
        for (int mu = 0; mu < 4; mu++)
        {
            for (int nu = mu; nu < 4; nu++)
            {
                Vector4d metric_deriv_components;
                for (int gamma = 0; gamma < 4; gamma++)
                {
                    metric_deriv_components(gamma) = metric_derivs[nu](gamma, mu)
                        + metric_derivs[mu](gamma, nu)
                        - metric_derivs[gamma](mu, nu);
                }
                christoffel_component(mu, nu) = metricInverse.col(alpha).dot(metric_deriv_components);
                // This is redundant when mu = nu, but rather this than putting an if statement in.
                christoffel_component(nu, mu) = christoffel_component(mu, nu);
            }
        }

        christoffel_symbols[alpha] = 0.5*christoffel_component;
    }

    return christoffel_symbols;
}
