//  These sources are licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http ://www.apache.org/licenses/LICENSE-2.0
//  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.


#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<assert.h>


#include	"spiral.h"


#ifdef _DEBUG
extern  int_t   horizontalAlignmentParentCurveI = 0, horizontalAlignmentParentCurveII = 0;
#endif // _DEBUG

int_t	reusedIfcGeometricRepresentationContextInstance = 0;


void	___RecursiveMultiplication(
				double	value,
				double	* polynomialConstants,
				uint_t	polynomialConstantCnt,
				double	* myArray,
				uint_t	myArrayParentIndex,
				uint_t	i_n,
				uint_t	n
			)
{
	assert(value);
	if (i_n < n - 1) {
		assert(polynomialConstantCnt && polynomialConstants[0] == 0.);
		for (uint_t i = 1; i < polynomialConstantCnt; i++) {
			if (polynomialConstants[i]) {
				___RecursiveMultiplication(
						value * polynomialConstants[i],
						polynomialConstants, polynomialConstantCnt,
						myArray, myArrayParentIndex + i,
						i_n + 1, n
					);
			}
		}
	}
	else {
		assert(i_n == n - 1);
		for (uint_t i = 1; i < polynomialConstantCnt; i++) {
			if (polynomialConstants[i]) {
				assert(myArrayParentIndex + i >= n && myArrayParentIndex + i < polynomialConstantCnt* n + 1);
				myArray[myArrayParentIndex + i] += value * polynomialConstants[i];
			}
		}
	}
}

double	___IntegralTaylorSeriesExpansionElement(
				uint_t	n,
				double	* polynomialConstants,
				uint_t	polynomialConstantCnt,
				double	s
			)
{
	//
	//	pC = polynomialConstants
	//	c = polynomialConstantCnt
	// 	x =  indeterminate
	// 	   =>	pC[c-1] * x^(c-1) + pC[c-2] * x^(c-2) + ... + pC[1] * x^(1) + pC[0] * x^(0) where pC[0] == 0
	//
	assert(polynomialConstantCnt && polynomialConstants[0] == 0.);

	if (n) {
		double	* myArray = new double[polynomialConstantCnt * n + 1]();
		___RecursiveMultiplication(
				1.,
				polynomialConstants, polynomialConstantCnt,
				myArray, 0,
				0, n
			);

		double	value = 0., factor = 1.;
		for (uint_t k = 0; k < polynomialConstantCnt * n + 1; k++) {
			factor *= s;
			value += myArray[k] * factor / (k + 1);
		}

		uint_t	tB = n;
		while (tB > 1) {
			value /= (double) tB;
			tB--;
		}
		
		delete[] myArray;

		return	value;
	}
	else {
		return	s;
	}
}

double	___IntegralTaylorSeriesCosExpansion(
				uint_t	i,
				double	* polynomialConstants,
				uint_t	polynomialConstantCnt,
				double	s
			)
{
	double	value =
		___IntegralTaylorSeriesExpansionElement(
				i * 2,
				polynomialConstants, polynomialConstantCnt,
				s
			);

	if (i % 2) {
		return	-value;
	}
	else {
		return	value;
	}
}

double	___IntegralTaylorSeriesSinExpansion(
				uint_t	i,
				double	* polynomialConstants,
				uint_t	polynomialConstantCnt,
				double	s
			)
{
	double	value =
		___IntegralTaylorSeriesExpansionElement(
				i * 2 + 1,
				polynomialConstants, polynomialConstantCnt,
				s
			);

	if (i % 2) {
		return	-value;
	}
	else {
		return	value;
	}
}

double	___IntegralTaylorSeriesCos(
				double	* polynomialConstants,
				uint_t	polynomialConstantCnt,
				double	s
			)
{
	uint_t	minSteps = (polynomialConstantCnt > 7) ? 4 : 6, maxSteps = 8;
	double	borderValue = 0.000001;

	//
	// SUM [0 .. inf]
	//
	double value = 0.;

	uint_t i = 0;
	for (; i < minSteps; i++) {
		value +=
			___IntegralTaylorSeriesCosExpansion(
					i,
					polynomialConstants, polynomialConstantCnt,
					s
				);
	}

	if (polynomialConstantCnt > 7) {
		return	value;
	}

	for (; i < maxSteps; i++) {
		double	deviation =
					___IntegralTaylorSeriesCosExpansion(
							i,
							polynomialConstants, polynomialConstantCnt,
							s
						);
		value += deviation;

		if (std::fabs(deviation) < borderValue) {
			return value;
		}
	}

	value +=
		___IntegralTaylorSeriesCosExpansion(
				i,
				polynomialConstants, polynomialConstantCnt,
				s
			);

	assert(i == maxSteps);
	return value;
}

double	___IntegralTaylorSeriesSin(
				double	* polynomialConstants,
				uint_t	polynomialConstantCnt,
				double	s
			)
{
	uint_t	minSteps = (polynomialConstantCnt > 7) ? 4 : 6, maxSteps = 8;
	double	borderValue = 0.0000001;

	//
	// SUM [0 .. inf]
	//
	double value = 0.;

	uint_t i = 0;
	for (; i < minSteps; i++) {
		value +=
			___IntegralTaylorSeriesSinExpansion(
					i,
					polynomialConstants, polynomialConstantCnt,
					s
				);
	}

	if (polynomialConstantCnt > 7) {
		return	value;
	}

	for (; i < maxSteps; i++) {
		double	deviation =
			___IntegralTaylorSeriesSinExpansion(
					i,
					polynomialConstants, polynomialConstantCnt,
					s
				);
		value += deviation;

		if (std::fabs(deviation) < borderValue) {
			return value;
		}
	}

	value +=
		___IntegralTaylorSeriesSinExpansion(
				i,
				polynomialConstants, polynomialConstantCnt,
				s
			);

	assert(i == maxSteps);
	return value;
}


//
//	Numerical Integration
//


double	___NumericalIntegrationCos(
				double		length,
				double		* polynomialConstants,
				uint_t		polynomialConstantCnt
			)
{
	double	factor = 1.,
			value = polynomialConstants[0];
	for (uint_t i = 1; i < polynomialConstantCnt; i++) {
		factor *= length;
		value += polynomialConstants[i] * factor;
	}

	return	std::cos(value);
}

double	___NumericalIntegrationSin(
				double		length,
				double		* polynomialConstants,
				uint_t		polynomialConstantCnt
			)
{
	double	factor = 1.,
			value = polynomialConstants[0];
	for (uint_t i = 1; i < polynomialConstantCnt; i++) {
		factor *= length;
		value += polynomialConstants[i] * factor;
	}

	return	std::sin(value);
}

double	___NumericalIntegrationCos(
				double		offset,
				double		length,
				double		* polynomialConstants,
				uint_t		polynomialConstantCnt,
				double		minWidth,
				double		angle
			)
{
	double	P1y = ___NumericalIntegrationCos(offset + length / 6., polynomialConstants, polynomialConstantCnt);
	double	P2y = ___NumericalIntegrationCos(offset + length / 2., polynomialConstants, polynomialConstantCnt);
	double	P3y = ___NumericalIntegrationCos(offset + length - length / 6., polynomialConstants, polynomialConstantCnt);

	double	distance = 2. * (P2y - P1y) - (P3y - P1y);

	if ((length / 6. > minWidth) &&
		(distance > angle * length)) {
		double	a = ___NumericalIntegrationCos(offset, length / 3., polynomialConstants, polynomialConstantCnt, minWidth, angle),
				b = ___NumericalIntegrationCos(offset + length / 3., length / 3., polynomialConstants, polynomialConstantCnt, minWidth, angle),
				c = ___NumericalIntegrationCos(offset + 2. * length / 3., length / 3., polynomialConstants, polynomialConstantCnt, minWidth, angle);

		assert(a * 3. <= length);
		assert(b * 3. <= length);
		assert(c * 3. <= length);
		return	a + b + c;
	}

	return	(P1y + P2y + P3y) * length / 3.;
}

double	___NumericalIntegrationSin(
				double		offset,
				double		length,
				double		* polynomialConstants,
				uint_t		polynomialConstantCnt,
				double		minWidth,
				double		angle
			)
{
	double	P1y = ___NumericalIntegrationSin(offset + length / 6., polynomialConstants, polynomialConstantCnt);
	double	P2y = ___NumericalIntegrationSin(offset + length / 2., polynomialConstants, polynomialConstantCnt);
	double	P3y = ___NumericalIntegrationSin(offset + length - length / 6., polynomialConstants, polynomialConstantCnt);

	double	distance = 2. * (P2y - P1y) - (P3y - P1y);

	if ((length / 6. > minWidth) &&
		(distance > angle * length)) {
		double	a = ___NumericalIntegrationSin(offset, length / 3., polynomialConstants, polynomialConstantCnt, minWidth, angle),
				b = ___NumericalIntegrationSin(offset + length / 3., length / 3., polynomialConstants, polynomialConstantCnt, minWidth, angle),
				c = ___NumericalIntegrationSin(offset + 2. * length / 3., length / 3., polynomialConstants, polynomialConstantCnt, minWidth, angle);

		assert(a * 3. < length);
		assert(b * 3. < length);
		assert(c * 3. < length);
		return	a + b + c;
	}

	return	(P1y + P2y + P3y) * length / 3.;
}

double	___NumericalIntegrationCos__(
				double		* polynomialConstants,
				uint_t		polynomialConstantCnt,
				double		s
			)
{
	int_t	initialPartCnt = 10;
	double	minWidth = 0.0000001, angle = 0.0000001;
	
	double	partSize = s / initialPartCnt, value = 0.;
	for (int_t i = 0; i < initialPartCnt; i++) {
		value +=
			___NumericalIntegrationCos(
					i * partSize,
					partSize,
					polynomialConstants,
					polynomialConstantCnt,
					minWidth * s,
					angle * s
				);
	}

	return	value;
}

double	___NumericalIntegrationSin__(
				double		* polynomialConstants,
				uint_t		polynomialConstantCnt,
				double		s
			)
{
	int_t	initialPartCnt = 1000;
	double	minWidth = 0.0000001, angle = 0.0000001;

	double	partSize = s / initialPartCnt, value = 0.;
	for (int_t i = 0; i < initialPartCnt; i++) {
		value +=
			___NumericalIntegrationSin(
					i * partSize,
					partSize,
					polynomialConstants,
					polynomialConstantCnt,
					minWidth * s,
					angle * s
				);
	}

	return	value;
}

double	___NumericalIntegrationCos(
				double		* polynomialConstants,
				uint_t		polynomialConstantCnt,
				double		s
			)
{
	if (s) {
#ifdef _DEBUG
		double A;

		{
			double	* localPolynomialConstants = new double[polynomialConstantCnt];
			for (uint_t i = 0; i < polynomialConstantCnt; i++) {
				localPolynomialConstants[i] = polynomialConstants[i];
			}

			double	value = ___NumericalIntegrationCos__(localPolynomialConstants, polynomialConstantCnt, s);

			delete[] localPolynomialConstants;

			A = value;
		}
#endif // _DEBUG

		double B;

		{
			double	* localPolynomialConstants = new double[polynomialConstantCnt];
			double	factor = 1.;
			for (uint_t i = 0; i < polynomialConstantCnt; i++) {
				localPolynomialConstants[i] = polynomialConstants[i] * factor;
				factor *= s;
			}

			double	value = ___NumericalIntegrationCos__(localPolynomialConstants, polynomialConstantCnt, 1.);

			delete[] localPolynomialConstants;

			B = s * value;
		}

		assert(std::fabs(A - B) < 0.00001 * std::fmax(std::fabs(A), 1.));

		return	B;
	}

	return	0.;
}

double	___NumericalIntegrationSin(
				double		* polynomialConstants,
				uint_t		polynomialConstantCnt,
				double		s
			)
{
	if (s) {
#ifdef _DEBUG
		double A;

		{
			double	* localPolynomialConstants = new double[polynomialConstantCnt];
			for (uint_t i = 0; i < polynomialConstantCnt; i++) {
				localPolynomialConstants[i] = polynomialConstants[i];
			}

			double	value = ___NumericalIntegrationSin__(localPolynomialConstants, polynomialConstantCnt, s);

			delete[] localPolynomialConstants;

			A = value;
		}
#endif // _DEBUG

		double B;

		{
			double	* localPolynomialConstants = new double[polynomialConstantCnt];
			double	factor = 1.;
			for (uint_t i = 0; i < polynomialConstantCnt; i++) {
				localPolynomialConstants[i] = polynomialConstants[i] * factor;
				factor *= s;
			}

			double	value = ___NumericalIntegrationSin__(localPolynomialConstants, polynomialConstantCnt, 1.);

			delete[] localPolynomialConstants;

			B = s * value;
		}

		assert(std::fabs(A - B) < 0.000001 * std::fmax(std::fabs(A), 1.));

		return	B;
	}

	return	0.;
}
