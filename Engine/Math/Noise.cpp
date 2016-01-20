//==============================================================================================================
//Noise.cpp
//by Squirrel Eiserloh
//modified by Albert Chen
//==============================================================================================================

#include "Noise.hpp"

//---------------------------------------------------------------------------
// Computes a random Perlin noise value based on a 2D input <position> and
//	Perlin noise parameters.  Recursive (for additional octaves).
//
//	<perlinNoiseGridCellSize>: Noise density.  Larger values produce longer
//		wavelength noise (e.g. gentle, sweeping hills).
//	<numOctaves>: 0 is flat, 1 is simple smoothed noise. Values of 2+ add one
//		or more additional "octave" harmonics.  Each additional octave has
//		double the frequency/density but only a fraction of the amplitude of
//		the base noise.
//	<baseAmplitude>: The minimum (-amplitude) and maximum (+amplitude) values
//		produced by the first octave of the noise.  Note that adding
//		additional octaves can push the final total Perlin noise values above
//		or below the maximum base amplitude; the noise can be "normalized" by
//		the caller (omitted from this function for optimization purposes) via:
//				noise *= A / (A + (A * P))
//		...where A is the <baseAmplitude> and P is the <persistance>.
//	<persistance>: The fraction of amplitude of each subsequent octave, based on the amplitude of the previous octave.  For
//		example, with a persistance of 0.3, each octave is only 30% as strong as the previous octave.
//
float ComputePerlinNoiseValueAtPosition2D( const Vector2& position, float perlinNoiseGridCellSize,
	int numOctaves, float baseAmplitude, float persistance, int randomSeed )
{
	int numOctavesRemaining = numOctaves;
	float amplitude = baseAmplitude;
	float gridSize = perlinNoiseGridCellSize;
	float totalPerlinNoise = 0.0f;

	while( numOctavesRemaining > 0 )
	{
		Vector2 perlinPosition = position / gridSize;
		Vector2 perlinPositionFloor( floor( perlinPosition.x ), floor( perlinPosition.y ) );
		IntVec2 perlinCell( (int) perlinPositionFloor.x, (int) perlinPositionFloor.y );
		Vector2 perlinPositionUV = perlinPosition - perlinPositionFloor;
		Vector2 perlinPositionAntiUV( perlinPositionUV.x - 1.f, perlinPositionUV.y - 1.f );
		float eastWeight = SmoothStep( perlinPositionUV.x );
		float northWeight = SmoothStep( perlinPositionUV.y );
		float westWeight = 1.f - eastWeight;
		float southWeight = 1.f - northWeight;

		Vector2 southwestNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x, perlinCell.y, randomSeed );
		Vector2 southeastNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x + 1, perlinCell.y, randomSeed );
		Vector2 northeastNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x + 1, perlinCell.y + 1, randomSeed );
		Vector2 northwestNoiseGradient = GetPseudoRandomNoiseUnitVector2D( perlinCell.x, perlinCell.y + 1, randomSeed );

		float southwestDot = DotProduct( southwestNoiseGradient, perlinPositionUV );
		float southeastDot = DotProduct( southeastNoiseGradient, Vector2( perlinPositionAntiUV.x, perlinPositionUV.y ) );
		float northeastDot = DotProduct( northeastNoiseGradient, perlinPositionAntiUV );
		float northwestDot = DotProduct( northwestNoiseGradient, Vector2( perlinPositionUV.x, perlinPositionAntiUV.y ) );

		float southBlend = (eastWeight * southeastDot) + (westWeight * southwestDot);
		float northBlend = (eastWeight * northeastDot) + (westWeight * northwestDot);
		float fourWayBlend = (southWeight * southBlend) + (northWeight * northBlend);
		float perlinNoiseAtThisOctave = amplitude * fourWayBlend;

		-- numOctavesRemaining;
		totalPerlinNoise += perlinNoiseAtThisOctave;
		amplitude *= persistance;
		gridSize *= 0.5f;
	}

	return totalPerlinNoise;
}


