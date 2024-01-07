#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>

class PerlinNoise {
public:
    PerlinNoise() {}
    PerlinNoise(int seed);

    double noise(double x, double y) const;
    double octaveNoise(double x, double y, int octaves, double persistence) const;

private:
    std::vector<int> p;
    static constexpr int permutationSize = 256;

    double fade(double t) const;
    double lerp(double t, double a, double b) const;
    double grad(int hash, double x, double y) const;
};