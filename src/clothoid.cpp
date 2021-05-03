//
//  Author:  Peter Bonsma
//  $Date: 1999-12-31 23:59:59 +0000 (Wed, 31 Jan 1999) $
//  $Revision: 3999 $
//  Project: Geometry Kernel
//
//  In case of use of the DLL:
//  be aware of license fee for use of this DLL when used commercially
//  more info for commercial use:  peter.bonsma@rdf.bg
//

#include	"../stdafx.h"

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>


#include	"clothoid.h"

#include	"engineControlStatic.h"


double	TaylorSeriesCosQubic(
				int_t	i,
				double	s
			)
{
	double	value = s / (8 * i + 1);

	{
		double	sq = s * s * s * s * s * s * s * s;
		int_t	tA = i,
				tB = 2 * i;
		while (tA) {
			value *= sq;
			while (value > sq && tB > 1) {
				value /= (double) tB;
				tB--;
			}
			tA--;
		}

		while (tB > 1) {
			value /= (double) tB;
			tB--;
		}
	}

	return	value;
}

double	TaylorSeriesCosQuadratic(
				int_t	i,
				double	s
			)
{
	double	value = s / (6 * i + 1);

	{
		double	sq = s * s * s * s * s * s;
		int_t	tA = i,
				tB = 2 * i;
		while (tA) {
			value *= sq;
			while (value > sq && tB > 1) {
				value /= (double) tB;
				tB--;
			}
			tA--;
		}

		while (tB > 1) {
			value /= (double) tB;
			tB--;
		}
	}

	return	value;
}

double	TaylorSeriesCosLinear(
				int_t	i,
				double	s
			)
{
	double	value = s / (4 * i + 1);

	{
		double	sq = s * s * s * s;
		int_t	tA = i,
				tB = 2 * i;
		while (tA) {
			value *= sq;
			while (value > sq && tB > 1) {
				value /= (double) tB;
				tB--;
			}
			tA--;
		}

		while (tB > 1) {
			value /= (double) tB;
			tB--;
		}
	}

	return	value;
}

double	TaylorSeriesCosConstant(
				int_t	i,
				double	s
			)
{
	double	value = s / (2 * i + 1);

	{
		double	sq = s * s;
		int_t	tA = i,
				tB = 2 * i;
		while (tA) {
			value *= sq;
			while (value > sq && tB > 1) {
				value /= (double) tB;
				tB--;
			}
			tA--;
		}

		while (tB > 1) {
			value /= (double) tB;
			tB--;
		}
	}

	return	value;
}

double	TaylorSeriesCos(
				int_t	i,
				double	qubicTerm,
				double	quadraticTerm,
				double	linearTerm,
				double	constantTerm,
				double	s
			)
{
	double	value =
					qubicTerm ?
						qubicTerm *
							TaylorSeriesCosQubic(
									i,
									s / std::fabs(qubicTerm)
								) :
						0.
				+
					quadraticTerm ?
						quadraticTerm *
							TaylorSeriesCosQuadratic(
									i,
									s / std::fabs(quadraticTerm)
								) :
						0.
				+
					linearTerm ?
						linearTerm *
							TaylorSeriesCosLinear(
									i,
									s / std::fabs(linearTerm)
								) :
						0.
				+
					constantTerm ?
						constantTerm *
							TaylorSeriesCosConstant(
									i,
									s / std::fabs(constantTerm)
								) :
						0.;

	if (i % 2) {
		return	-value;
	}
	else {
		return	value;
	}
}

double	TaylorSeriesSinQubic(
				int_t	i,
				double	s
			)
{
	double	value = s * s * s * s * s / (8 * i + 5);

	{
		double	sq = s * s * s * s * s * s * s * s;
		int_t	tA = i,
				tB = 2 * i + 1;
		while (tA) {
			value *= sq;
			while (value > sq && tB > 1) {
				value /= (double) tB;
				tB--;
			}
			tA--;
		}

		while (tB > 1) {
			value /= (double) tB;
			tB--;
		}
	}

	return	value;
}

double	TaylorSeriesSinQuadratic(
				int_t	i,
				double	s
			)
{
	double	value = s * s * s * s / (6 * i + 4);

	{
		double	sq = s * s * s * s * s * s;
		int_t	tA = i,
				tB = 2 * i + 1;
		while (tA) {
			value *= sq;
			while (value > sq && tB > 1) {
				value /= (double) tB;
				tB--;
			}
			tA--;
		}

		while (tB > 1) {
			value /= (double) tB;
			tB--;
		}
	}

	return	value;
}

double	TaylorSeriesSinLinear(
				int_t	i,
				double	s
			)
{
	double	value = s * s * s / (4 * i + 3);

	{
		double	sq = s * s * s * s;
		int_t	tA = i,
				tB = 2 * i + 1;
		while (tA) {
			value *= sq;
			while (value > sq && tB > 1) {
				value /= (double) tB;
				tB--;
			}
			tA--;
		}

		while (tB > 1) {
			value /= (double) tB;
			tB--;
		}
	}

	return	value;
}

double	TaylorSeriesSinConstant(
				int_t	i,
				double	s
			)
{
	double	value = s * s / (2 * i + 2);

	{
		double	sq = s * s;
		int_t	tA = i,
				tB = 2 * i + 1;
		while (tA) {
			value *= sq;
			while (value > sq && tB > 1) {
				value /= (double) tB;
				tB--;
			}
			tA--;
		}

		while (tB > 1) {
			value /= (double) tB;
			tB--;
		}
	}

	return	value;
}

double	TaylorSeriesSin(
				int_t	i,
				double	qubicTerm,
				double	quadraticTerm,
				double	linearTerm,
				double	constantTerm,
				double	s
			)
{
	double	value =
					qubicTerm ?
						qubicTerm *
							TaylorSeriesSinQubic(
									i,
									s / std::fabs(qubicTerm)
								) :
						0.
				+
					quadraticTerm ?
						quadraticTerm *
							TaylorSeriesSinQuadratic(
									i,
									s / std::fabs(quadraticTerm)
								) :
						0.
				+
					linearTerm ?
						linearTerm *
							TaylorSeriesSinLinear(
									i,
									s / std::fabs(linearTerm)
								) :
						0.
				+
					constantTerm ?
						constantTerm *
							TaylorSeriesSinConstant(
									i,
									s / std::fabs(constantTerm)
								) :
						0.;

	if (i % 2) {
		return	-value;
	}
	else {
		return	value;
	}
}

double	TaylorSeriesCos(
				double	qubicTerm,
				double	quadraticTerm,
				double	linearTerm,
				double	constantTerm,
				double	s
			)
{
	int_t	minSteps = 6, maxSteps = 500;
	double	borderValue = 0.00001;

	//
	// SUM [0 .. inf]
	//
	double value = 0.;

	int_t i = 0;
	for (; i < minSteps; i++) {
		value +=
			TaylorSeriesCos(
					i,
					qubicTerm,
					quadraticTerm,
					linearTerm,
					constantTerm,
					s
				);
	}

	for (; i < maxSteps; i++) {
		double	deviation =
					TaylorSeriesCos(
							i,
							qubicTerm,
							quadraticTerm,
							linearTerm,
							constantTerm,
							s
						);
		value += deviation;

		if (std::fabs(deviation) < borderValue) {
			return value;
		}
	}

	value +=
		TaylorSeriesCos(
				i,
				qubicTerm,
				quadraticTerm,
				linearTerm,
				constantTerm,
				s
			);

	assert(i == maxSteps);
	return value;
}

double	TaylorSeriesSin(
				double	qubicTerm,
				double	quadraticTerm,
				double	linearTerm,
				double	constantTerm,
				double	s
			)
{
	int_t	minSteps = 6, maxSteps = 500;
	double	borderValue = 0.00001;

	//
	// SUM [0 .. inf]
	//
	double value = 0.;

	int_t i = 0;
	for (; i < minSteps; i++) {
		value +=
			TaylorSeriesSin(
					i,
					qubicTerm,
					quadraticTerm,
					linearTerm,
					constantTerm,
					s
				);
	}

	for (; i < maxSteps; i++) {
		double	deviation =
					TaylorSeriesSin(
							i,
							qubicTerm,
							quadraticTerm,
							linearTerm,
							constantTerm,
							s
						);
		value += deviation;

		if (std::fabs(deviation) < borderValue) {
			return value;
		}
	}

	value +=
		TaylorSeriesSin(
				i,
				qubicTerm,
				quadraticTerm,
				linearTerm,
				constantTerm,
				s
			);

	assert(i == maxSteps);
	return value;
}

double	SpiralX(
				double	qubicTerm,
				double	quadraticTerm,
				double	linearTerm,
				double	constantTerm,
				double	s
			)
{
	return	TaylorSeriesCos(
					qubicTerm,
					quadraticTerm,
					linearTerm,
					constantTerm,
					s
				);
}

double	SpiralY(
				double	qubicTerm,
				double	quadraticTerm,
				double	linearTerm,
				double	constantTerm,
				double	s
			)
{
	return	TaylorSeriesSin(
					qubicTerm,
					quadraticTerm,
					linearTerm,
					constantTerm,
					s
				);
}

double	ClothoidX(
				double	clothoidConstant,
				double	s
			)
{
	return	TaylorSeriesCos(
					0.,
					0.,
					clothoidConstant * sqrt(2.),
					0.,
					s
				);
}

double	ClothoidY(
				double	clothoidConstant,
				double	s
			)
{
	return	TaylorSeriesSin(
					0.,
					0.,
					clothoidConstant * sqrt(2.),
					0.,
					s
				);
}