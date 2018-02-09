#include "simplexnoisegenerator.h"

SimplexNoiseGenerator::SimplexNoiseGenerator()
{
	for (int i = 0; i < 256; i++) {
		p[256 + i] = permutation[i];
		p[i] = permutation[i];
	}
}

SimplexNoiseGenerator::SimplexNoiseGenerator(const SimplexNoiseGenerator &)
{
}

SimplexNoiseGenerator::~SimplexNoiseGenerator()
{
}

double SimplexNoiseGenerator::GenerateNoise(double x_in, double y_in, double z_in)
{
	//Find unit cube that contains point
	int X = FastFloor(x_in) & 255;								
	int	Y = FastFloor(y_in) & 255;								
	int	Z = FastFloor(z_in) & 255;

	//Find relative x, y, z of point in cube
	x_in -= FastFloor(x_in);									
	y_in -= FastFloor(y_in);									
	z_in -= FastFloor(z_in);

	//Compute fade curves for each of x, y, z
	double u = Fade(x_in);
	double v = Fade(y_in);
	double w = Fade(z_in);

	// Hash co-ordinates
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x_in, y_in, z_in), Grad(p[BA], x_in - 1, y_in, z_in)), Lerp(u, Grad(p[AB], x_in, y_in- 1, z_in),						
			Grad(p[BB], x_in - 1, y_in - 1, z_in))), Lerp(v, Lerp(u, Grad(p[AA + 1], x_in, y_in, z_in - 1),	Grad(p[BA + 1], x_in - 1, y_in, z_in - 1)),					
				Lerp(u, Grad(p[AB + 1], x_in, y_in - 1, z_in - 1), Grad(p[BB + 1], x_in - 1, y_in - 1, z_in - 1))));
}

int SimplexNoiseGenerator::FastFloor(double x)
{
	return x>0 ? (int)x : (int)x - 1;
}

double SimplexNoiseGenerator::Fade(double t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double SimplexNoiseGenerator::Lerp(double t, double a, double b)
{
	return a + t * (b - a);
}

double SimplexNoiseGenerator::Grad(int hash, double x, double y, double z)
{
	int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
	double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
		v = h<4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}
