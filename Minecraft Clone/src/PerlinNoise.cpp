#include "PerlinNoise.h"

PerlinNoise::PerlinNoise(int seed) {
    p.resize(2 * permutationSize);
    std::iota(p.begin(), p.end(), 0);

    std::default_random_engine engine(seed);
    std::shuffle(p.begin(), p.end(), engine);
    p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y) const {
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);

    double u = fade(x);
    double v = fade(y);

    int A = p[X] + Y;
    int AA = p[A];
    int AB = p[A + 1];

    int B = p[X + 1] + Y;
    int BA = p[B];
    int BB = p[B + 1];

    return lerp(v, lerp(u, grad(AA, x, y), grad(BA, x - 1, y)),
        lerp(u, grad(AB, x, y - 1), grad(BB, x - 1, y - 1)));
}

double PerlinNoise::octaveNoise(double x, double y, int octaves, double persistence) const {
    double total = 0.0;
    double frequency = 1.0;
    double amplitude = 1.0;
    double maxValue = 0.0;  // Used for normalizing result to 0.0 - 1.0

    for (int i = 0; i < octaves; ++i) {
        total += noise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2.0;
    }

    return total / maxValue;
}

double PerlinNoise::fade(double t) const {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) const {
    return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y) const {
    int h = hash & 15;
    double grad = 1.0 + (h & 7);  // Gradient value 1-8
    if (h & 8) grad = -grad;  // Randomly invert half of them
    return (h & 1) ? x : y;
}