#include "particle.h"
#include "world.h"
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

void Particle::updateMetric(Matrix4d new_metric)
{
    metric = new_metric;
}

// Advances the ray path by a parameter step, dl.
// WARNING: Due to the null constraint, there are only three independent
// velocity components, but it's much simpler to integrate all 4 (and
// probably not slower or much less accurate, if at all).
void Particle::advance(double dl, World &simulation)
{
    // Currently defined to advance with RK4.
    Vector4d x_step;
    Vector4d v_step;

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
    for (int i = 0; i < 2; i++)
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
}

// Returns the scalar product of the 4-velocity. Primarily for debugging to check the scalar product is conserved.
double Particle::scalarProduct()
{
    // This should be faster than a simple double for loop.
    Vector4d col_contractions;
    for (int nu = 0; nu < 4; nu++)
    {
        col_contractions(nu) = metric.col(nu).dot(v);
    }

    return col_contractions.dot(v);
}

// Returns the derivative of the given 4-velocity using the provided Christoffel symbols.
Vector4d v_derivative(Vector4d v, std::vector<Matrix4d> &christoffel_symbols)
{
    Vector4d acceleration;
    Vector4d col_contractions;
    // Should be faster than a triple for loop.
    for (int alpha = 0; alpha < 4; alpha++)
    {
        for (int sigma = 0; sigma < 4; sigma++)
        {
            col_contractions(sigma) = christoffel_symbols[alpha].col(sigma).dot(v);
        }
        acceleration(alpha) = col_contractions.dot(v);
    }

    return acceleration;
}
