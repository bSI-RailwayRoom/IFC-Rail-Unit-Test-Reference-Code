//  These sources are licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at http ://www.apache.org/licenses/LICENSE-2.0
//  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.

#ifndef __SPIRAL_H
#define __SPIRAL_H


#include	"engdef.h"
#include	"mathematics.h"


double			___IntegralTaylorSeriesCos(
						double		* polynomialConstants,
						uint_t		polynomialConstantCnt,
						double		s
					);

double			___IntegralTaylorSeriesSin(
						double		* polynomialConstants,
						uint_t		polynomialConstantCnt,
						double		s
					);

double			___NumericalIntegrationCos(
						double		* polynomialConstants,
						uint_t		polynomialConstantCnt,
						double		s
					);

double			___NumericalIntegrationSin(
						double		* polynomialConstants,
						uint_t		polynomialConstantCnt,
						double		s
					);


static	inline	double	___XbyAngleDeviationPolynomial(
								double	* polynomialConstants,
								uint_t	polynomialConstantCnt,
								double	s
							)
{
	double	* localPolynomialConstants = new double[polynomialConstantCnt + 1];
	uint_t	localPolynomialConstantCnt = 1;

	localPolynomialConstants[0] = 0.;

	for (uint_t i = 0; i < polynomialConstantCnt; i++) {
		if (polynomialConstants[i]) {
			localPolynomialConstants[i + 1] = polynomialConstants[i] / (i + 1.);
			localPolynomialConstantCnt = i + 2;
		}
		else {
			localPolynomialConstants[i + 1] = 0;
		}
	}

	double	rValue =
		___IntegralTaylorSeriesCos(
				localPolynomialConstants,
				localPolynomialConstantCnt,
				s
			);

#ifdef _DEBUG
	double	rValueNumericalIntegration =
		___NumericalIntegrationCos(
				localPolynomialConstants,
				localPolynomialConstantCnt,
				s
			);
#endif // _DEBUG
	assert(std::fabs(rValueNumericalIntegration - rValue) < 0.000001);

	delete[] localPolynomialConstants;

	return	rValue;
}

static	inline	double	___XbyAngleDeviationPolynomial__NumericalIntegration(
								double	* polynomialConstants,
								uint_t	polynomialConstantCnt,
								double	s
							)
{
	double	* localPolynomialConstants = new double[polynomialConstantCnt + 1];
	uint_t	localPolynomialConstantCnt = 1;

	localPolynomialConstants[0] = 0.;

	for (uint_t i = 0; i < polynomialConstantCnt; i++) {
		if (polynomialConstants[i]) {
			localPolynomialConstants[i + 1] = polynomialConstants[i] / (i + 1.);
			localPolynomialConstantCnt = i + 2;
		}
		else {
			localPolynomialConstants[i + 1] = 0;
		}
	}

	double	rValueNumericalIntegration =
		___NumericalIntegrationCos(
				localPolynomialConstants,
				localPolynomialConstantCnt,
				s
			);

	delete[] localPolynomialConstants;

	return	rValueNumericalIntegration;
}

static	inline	double	___YbyAngleDeviationPolynomial(
								double	* polynomialConstants,
								uint_t	polynomialConstantCnt,
								double	s
							)
{
	double	* localPolynomialConstants = new double[polynomialConstantCnt + 1];
	uint_t	localPolynomialConstantCnt = 1;

	localPolynomialConstants[0] = 0.;

	for (uint_t i = 0; i < polynomialConstantCnt; i++) {
		if (polynomialConstants[i]) {
			localPolynomialConstants[i + 1] = polynomialConstants[i] / (i + 1.);
			localPolynomialConstantCnt = i + 2;
		}
		else {
			localPolynomialConstants[i + 1] = 0.;
		}
	}

	double	rValue =
		___IntegralTaylorSeriesSin(
				localPolynomialConstants,
				localPolynomialConstantCnt,
				s
			);

	delete[] localPolynomialConstants;

	return	rValue;
}

static	inline	double	___XbyAngleDeviationPolynomial(
								double	a,
								double	b,
								double	c,
								double	d,
								double	e,
								double	f,
								double	g,
								double	h,
								double	s
							)
{
	double	polynomialConstants[9] = { 0., 0., 0., 0., 0., 0., 0., 0., 0. };
	uint_t	polynomialConstantCnt = 1;

	if (h) {
		polynomialConstants[1] = h / 1.;
		polynomialConstantCnt = 2;
	}
	if (g) {
		polynomialConstants[2] = g / 2.;
		polynomialConstantCnt = 3;
	}
	if (f) {
		polynomialConstants[3] = f / 3.;
		polynomialConstantCnt = 4;
	}
	if (e) {
		polynomialConstants[4] = e / 4.;
		polynomialConstantCnt = 5;
	}
	if (d) {
		polynomialConstants[5] = d / 5.;
		polynomialConstantCnt = 6;
	}
	if (c) {
		polynomialConstants[6] = c / 6.;
		polynomialConstantCnt = 7;
	}
	if (b) {
		polynomialConstants[7] = b / 7.;
		polynomialConstantCnt = 8;
	}
	if (a) {
		polynomialConstants[8] = a / 8.;
		polynomialConstantCnt = 9;
	}

	return	___IntegralTaylorSeriesCos(
					polynomialConstants,
					polynomialConstantCnt,
					s
				);
}

static	inline	double	___XbyAngleDeviationPolynomial__NumericalIntegration(
								double	a,
								double	b,
								double	c,
								double	d,
								double	e,
								double	f,
								double	g,
								double	h,
								double	s
							)
{
	double	polynomialConstants[9] = { 0., 0., 0., 0., 0., 0., 0., 0., 0. };
	uint_t	polynomialConstantCnt = 1;

	if (h) {
		polynomialConstants[1] = h / 1.;
		polynomialConstantCnt = 2;
	}
	if (g) {
		polynomialConstants[2] = g / 2.;
		polynomialConstantCnt = 3;
	}
	if (f) {
		polynomialConstants[3] = f / 3.;
		polynomialConstantCnt = 4;
	}
	if (e) {
		polynomialConstants[4] = e / 4.;
		polynomialConstantCnt = 5;
	}
	if (d) {
		polynomialConstants[5] = d / 5.;
		polynomialConstantCnt = 6;
	}
	if (c) {
		polynomialConstants[6] = c / 6.;
		polynomialConstantCnt = 7;
	}
	if (b) {
		polynomialConstants[7] = b / 7.;
		polynomialConstantCnt = 8;
	}
	if (a) {
		polynomialConstants[8] = a / 8.;
		polynomialConstantCnt = 9;
	}

	return	___NumericalIntegrationCos(
					polynomialConstants,
					polynomialConstantCnt,
					s
				);
}

static	inline	double	___XbyAngleDeviationPolynomial(
								double	a,
								double	b,
								double	c,
								double	d,
								double	s
							)
{
	double	polynomialConstants[5] = { 0., 0., 0., 0., 0. };
	uint_t	polynomialConstantCnt = 1;

	if (d) {
		polynomialConstants[1] = d / 1.;
		polynomialConstantCnt = 2;
	}
	if (c) {
		polynomialConstants[2] = c / 2.;
		polynomialConstantCnt = 3;
	}
	if (b) {
		polynomialConstants[3] = b / 3.;
		polynomialConstantCnt = 4;
	}
	if (a) {
		polynomialConstants[4] = a / 4.;
		polynomialConstantCnt = 5;
	}

	return	___IntegralTaylorSeriesCos(
					polynomialConstants,
					polynomialConstantCnt,
					s
				);
}

static	inline	double	___YbyAngleDeviationPolynomial(
								double	a,
								double	b,
								double	c,
								double	d,
								double	e,
								double	f,
								double	g,
								double	h,
								double	s
							)
{
	double	polynomialConstants[9] = { 0., 0., 0., 0., 0., 0., 0., 0., 0. };
	uint_t	polynomialConstantCnt = 1;

	if (h) {
		polynomialConstants[1] = h / 1.;
		polynomialConstantCnt = 2;
	}
	if (g) {
		polynomialConstants[2] = g / 2.;
		polynomialConstantCnt = 3;
	}
	if (f) {
		polynomialConstants[3] = f / 3.;
		polynomialConstantCnt = 4;
	}
	if (e) {
		polynomialConstants[4] = e / 4.;
		polynomialConstantCnt = 5;
	}
	if (d) {
		polynomialConstants[5] = d / 5.;
		polynomialConstantCnt = 6;
	}
	if (c) {
		polynomialConstants[6] = c / 6.;
		polynomialConstantCnt = 7;
	}
	if (b) {
		polynomialConstants[7] = b / 7.;
		polynomialConstantCnt = 8;
	}
	if (a) {
		polynomialConstants[8] = a / 8.;
		polynomialConstantCnt = 9;
	}

	return	___IntegralTaylorSeriesSin(
					polynomialConstants,
					polynomialConstantCnt,
					s
				);
}

static	inline	double	___YbyAngleDeviationPolynomial__NumericalIntegration(
								double	a,
								double	b,
								double	c,
								double	d,
								double	e,
								double	f,
								double	g,
								double	h,
								double	s
							)
{
	double	polynomialConstants[9] = { 0., 0., 0., 0., 0., 0., 0., 0., 0. };
	uint_t	polynomialConstantCnt = 1;

	if (h) {
		polynomialConstants[1] = h / 1.;
		polynomialConstantCnt = 2;
	}
	if (g) {
		polynomialConstants[2] = g / 2.;
		polynomialConstantCnt = 3;
	}
	if (f) {
		polynomialConstants[3] = f / 3.;
		polynomialConstantCnt = 4;
	}
	if (e) {
		polynomialConstants[4] = e / 4.;
		polynomialConstantCnt = 5;
	}
	if (d) {
		polynomialConstants[5] = d / 5.;
		polynomialConstantCnt = 6;
	}
	if (c) {
		polynomialConstants[6] = c / 6.;
		polynomialConstantCnt = 7;
	}
	if (b) {
		polynomialConstants[7] = b / 7.;
		polynomialConstantCnt = 8;
	}
	if (a) {
		polynomialConstants[8] = a / 8.;
		polynomialConstantCnt = 9;
	}

	return	___NumericalIntegrationSin(
					polynomialConstants,
					polynomialConstantCnt,
					s
				);
}

static	inline	double	___YbyAngleDeviationPolynomial(
								double	a,
								double	b,
								double	c,
								double	d,
								double	s
							)
{
	double	polynomialConstants[5] = { 0., 0., 0., 0., 0. };
	uint_t	polynomialConstantCnt = 1;

	if (d) {
		polynomialConstants[1] = d / 1.;
		polynomialConstantCnt = 2;
	}
	if (c) {
		polynomialConstants[2] = c / 2.;
		polynomialConstantCnt = 3;
	}
	if (b) {
		polynomialConstants[3] = b / 3.;
		polynomialConstantCnt = 4;
	}
	if (a) {
		polynomialConstants[4] = a / 4.;
		polynomialConstantCnt = 5;
	}

	return	___IntegralTaylorSeriesSin(
					polynomialConstants,
					polynomialConstantCnt,
					s
				);
}

static	inline	double	___XbyAngleDeviationPolynomialByTerms(
								double	septicTerm,
								double	sexticTerm,
								double	quinticTerm,
								double	quarticTerm,
								double	cubicTerm,
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	double	a = (septicTerm) ?
					septicTerm / std::fabs(septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm) :
					0.,
			b = (sexticTerm) ?
					1. / (sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm) :
					0.,
			c = (quinticTerm) ?
					quinticTerm / std::fabs(quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm) :
					0.,
			d = (quarticTerm) ?
					1. / (quarticTerm * quarticTerm * quarticTerm * quarticTerm * quarticTerm) :
					0.,
			e = (cubicTerm) ?
					cubicTerm / std::fabs(cubicTerm * cubicTerm * cubicTerm * cubicTerm * cubicTerm) :
					0.,
			f = (quadraticTerm) ?
					1. / (quadraticTerm * quadraticTerm * quadraticTerm) :
					0.,
			g = (linearTerm) ?
					linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
					0.,
			h = (constantTerm) ?
					1. / constantTerm :
					0.;

	return	___XbyAngleDeviationPolynomial(
					a, b, c, d, e, f, g, h,
					s
				);
}

static	inline	double	___XbyAngleDeviationPolynomialByTerms__NumericalIntegration(
								double	septicTerm,
								double	sexticTerm,
								double	quinticTerm,
								double	quarticTerm,
								double	cubicTerm,
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	double	a = (septicTerm) ?
					septicTerm / std::fabs(septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm) :
					0.,
			b = (sexticTerm) ?
					1. / (sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm) :
					0.,
			c = (quinticTerm) ?
					quinticTerm / std::fabs(quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm) :
					0.,
			d = (quarticTerm) ?
					1. / (quarticTerm * quarticTerm * quarticTerm * quarticTerm * quarticTerm) :
					0.,
			e = (cubicTerm) ?
					cubicTerm / std::fabs(cubicTerm * cubicTerm * cubicTerm * cubicTerm * cubicTerm) :
					0.,
			f = (quadraticTerm) ?
					1. / (quadraticTerm * quadraticTerm * quadraticTerm) :
					0.,
			g = (linearTerm) ?
					linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
					0.,
			h = (constantTerm) ?
					1. / constantTerm :
					0.;

	return	___XbyAngleDeviationPolynomial__NumericalIntegration(
					a, b, c, d, e, f, g, h,
					s
				);
}

static	inline	double	___XbyAngleDeviationPolynomialByTerms(
								double	cubicTerm,
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	double	a = (cubicTerm) ?
					cubicTerm / std::fabs(cubicTerm * cubicTerm * cubicTerm * cubicTerm * cubicTerm) :
					0.,
			b = (quadraticTerm) ?
					1. / (quadraticTerm * quadraticTerm * quadraticTerm) :
					0.,
			c = (linearTerm) ?
					linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
					0.,
			d = (constantTerm) ?
					1. / constantTerm :
					0.;

	return	___XbyAngleDeviationPolynomial(
					a, b, c, d,
					s
				);
}

static	inline	double	___YbyAngleDeviationPolynomialByTerms(
								double	septicTerm,
								double	sexticTerm,
								double	quinticTerm,
								double	quarticTerm,
								double	cubicTerm,
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	double	a = (septicTerm) ?
					septicTerm / std::fabs(septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm) :
					0.,
			b = (sexticTerm) ?
					1. / (sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm) :
					0.,
			c = (quinticTerm) ?
					quinticTerm / std::fabs(quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm) :
					0.,
			d = (quarticTerm) ?
					1. / (quarticTerm * quarticTerm * quarticTerm * quarticTerm * quarticTerm) :
					0.,
			e = (cubicTerm) ?
					cubicTerm / std::fabs(cubicTerm * cubicTerm * cubicTerm * cubicTerm * cubicTerm) :
					0.,
			f = (quadraticTerm) ?
					1. / (quadraticTerm * quadraticTerm * quadraticTerm) :
					0.,
			g = (linearTerm) ?
					linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
					0.,
			h = (constantTerm) ?
					1. / constantTerm :
					0.;

	return	___YbyAngleDeviationPolynomial(
					a, b, c, d, e, f, g, h,
					s
				);
}

static	inline	double	___YbyAngleDeviationPolynomialByTerms__NumericalIntegration(
								double	septicTerm,
								double	sexticTerm,
								double	quinticTerm,
								double	quarticTerm,
								double	cubicTerm,
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	double	a = (septicTerm) ?
					septicTerm / std::fabs(septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm) :
					0.,
			b = (sexticTerm) ?
					1. / (sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm) :
					0.,
			c = (quinticTerm) ?
					quinticTerm / std::fabs(quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm) :
					0.,
			d = (quarticTerm) ?
					1. / (quarticTerm * quarticTerm * quarticTerm * quarticTerm * quarticTerm) :
					0.,
			e = (cubicTerm) ?
					cubicTerm / std::fabs(cubicTerm * cubicTerm * cubicTerm * cubicTerm * cubicTerm) :
					0.,
			f = (quadraticTerm) ?
					1. / (quadraticTerm * quadraticTerm * quadraticTerm) :
					0.,
			g = (linearTerm) ?
					linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
					0.,
			h = (constantTerm) ?
					1. / constantTerm :
					0.;

	return	___YbyAngleDeviationPolynomial__NumericalIntegration(
					a, b, c, d, e, f, g, h,
					s
				);
}

static	inline	double	___YbyAngleDeviationPolynomialByTerms(
								double	cubicTerm,
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	double	a = (cubicTerm) ?
					cubicTerm / std::fabs(cubicTerm * cubicTerm * cubicTerm * cubicTerm * cubicTerm) :
					0.,
			b = (quadraticTerm) ?
					1. / (quadraticTerm * quadraticTerm * quadraticTerm) :
					0.,
			c = (linearTerm) ?
					linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
					0.,
			d = (constantTerm) ?
					1. / constantTerm :
					0.;

	return	___YbyAngleDeviationPolynomial(
					a, b, c, d,
					s
				);
}

static	inline	double	___XbyAngleDeviationPolynomialByTerms(
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	return	___XbyAngleDeviationPolynomialByTerms(
					0.,
					quadraticTerm,
					linearTerm,
					constantTerm,
					s
				);
}

static	inline	double	___YbyAngleDeviationPolynomialByTerms(
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	return	___YbyAngleDeviationPolynomialByTerms(
					0.,
					quadraticTerm,
					linearTerm,
					constantTerm,
					s
				);
}

static	inline	double	___XclothoidByConstant(
								double	clothoidConstant,
								double	s
							)
{
	return	___XbyAngleDeviationPolynomialByTerms(
					0.,
					0.,
					clothoidConstant,
					0.,
					s
				);
}

static	inline	double	___YclothoidByConstant(
								double	clothoidConstant,
								double	s
							)
{
	return	___YbyAngleDeviationPolynomialByTerms(
					0.,
					0.,
					clothoidConstant,
					0.,
					s
				);
}


//
//	CalculateAngle
//


static	inline	double	___AngleByAngleDeviationPolynomial(
								double	* polynomialConstants,
								uint_t	polynomialConstantCnt,
								double	s
							)
{
	double	factor = 1., angle = 0.;
	for (uint_t i = 0; i < polynomialConstantCnt; i++) {
		factor *= s;
		angle +=
			polynomialConstants[i] ?
				(polynomialConstants[i] / (i + 1)) * factor:
				0.;
	}
	return	angle;
}

static	inline	double	___AngleByAngleDeviationPolynomial(
								double	a,
								double	b,
								double	c,
								double	d,
								double	e,
								double	f,
								double	g,
								double	h,
								double	s
							)
{
	return	(a ? (a / 8.) * s * s * s * s * s * s * s * s : 0.) +
			(b ? (b / 7.) * s * s * s * s * s * s * s : 0.) +
			(c ? (c / 6.) * s * s * s * s * s * s : 0.) +
			(d ? (d / 5.) * s * s * s * s * s : 0.) +
			(e ? (e / 4.) * s * s * s * s : 0.) +
			(f ? (f / 3.) * s * s * s : 0.) +
			(g ? (g / 2.) * s * s : 0.) +
			(h ? (h / 1.) * s : 0.);
}

static	inline	double	___AngleByAngleDeviationPolynomial(
								double	a,
								double	b,
								double	c,
								double	d,
								double	s
							)
{
	return	(a ? (a / 4.) * s * s * s * s : 0.) +
			(b ? (b / 3.) * s * s * s : 0.) +
			(c ? (c / 2.) * s * s : 0.) +
			(d ? (d / 1.) * s : 0.);
}

static	inline	double	___DirectAngleByAngleDeviationPolynomial(
								double	a,
								double	b,
								double	c,
								double	d,
								double	s
							)
{
	return	(a ? a * s * s * s : 0.) +
			(b ? b * s * s : 0.) +
			(c ? c * s : 0.) +
			(d ? d : 0.);
}

static	inline	double	___AngleByAngleDeviationPolynomialByTerms__(
								double	septicTerm,
								double	sexticTerm,
								double	quinticTerm,
								double	quarticTerm,
								double	cubicTerm,
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	double	a = (septicTerm) ?
					septicTerm / std::fabs(septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm * septicTerm) :
					0.,
			b = (sexticTerm) ?
					1. / (sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm * sexticTerm) :
					0.,
			c = (quinticTerm) ?
					quinticTerm / std::fabs(quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm * quinticTerm) :
					0.,
			d = (quarticTerm) ?
					1. / (quarticTerm * quarticTerm * quarticTerm * quarticTerm * quarticTerm) :
					0.,
			e = (cubicTerm) ?
					cubicTerm / std::fabs(cubicTerm * cubicTerm * cubicTerm * cubicTerm * cubicTerm) :
					0.,
			f = (quadraticTerm) ?
					1. / (quadraticTerm * quadraticTerm * quadraticTerm) :
					0.,
			g = (linearTerm) ?
					linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
					0.,
			h = (constantTerm) ?
					1. / constantTerm :
					0.;

	return	___AngleByAngleDeviationPolynomial(
					a, b, c, d, e, f, g, h,
					s
				);
}

static	inline	double	___AngleByAngleDeviationPolynomialByTerms(
								double	cubicTerm,
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	double	a = (cubicTerm) ?
					cubicTerm / std::fabs(cubicTerm * cubicTerm * cubicTerm * cubicTerm * cubicTerm) :
					0.,
			b = (quadraticTerm) ?
					1. / (quadraticTerm * quadraticTerm * quadraticTerm) :
					0.,
			c = (linearTerm) ?
					linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
					0.,
			d = (constantTerm) ?
					1. / constantTerm :
					0.;

	return	___AngleByAngleDeviationPolynomial(
					a, b, c, d,
					s
				);
}

static	inline	double	___DirectAngleByAngleDeviationPolynomialByTerms(
								double	cubicTerm,
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	double	a = (cubicTerm) ?
					cubicTerm / std::fabs(cubicTerm * cubicTerm * cubicTerm * cubicTerm * cubicTerm) :
					0.,
			b = (quadraticTerm) ?
					1. / (quadraticTerm * quadraticTerm * quadraticTerm) :
					0.,
			c = (linearTerm) ?
					linearTerm / std::fabs(linearTerm * linearTerm * linearTerm) :
					0.,
			d = (constantTerm) ?
					1. / constantTerm :
					0.;

	return	___DirectAngleByAngleDeviationPolynomial(
					a, b, c, d,
					s
				);
}

static	inline	double	___AngleByAngleDeviationPolynomialByTerms(
								double	quadraticTerm,
								double	linearTerm,
								double	constantTerm,
								double	s
							)
{
	return	___AngleByAngleDeviationPolynomialByTerms(
					0.,
					quadraticTerm,
					linearTerm,
					constantTerm,
					s
				);
}

static	inline	double	___AngleClothoidByConstant(
								double	clothoidConstant,
								double	s
							)
{
	return	___AngleByAngleDeviationPolynomialByTerms(
					0.,
					0.,
					clothoidConstant,
					0.,
					s
				);
}

static	inline	double	TaylorSerieCosineElement(
								int_t	cnt,
								double	scale
							)
{
	double	value = 1.;
	while (cnt) {
		value *= scale / cnt;
		cnt--;
	}
	return	value;
}

static	inline	void	PopulateTaylorSerieCosine(
								double	* myArray,
								int_t	myArrayCnt,
								double	scale
							)
{
	int_t a = 0;
	for (int_t i = 0; i < myArrayCnt; i++) {
		if (a == 2) {
			myArray[i] = -TaylorSerieCosineElement(i, scale);
			a = -2;
		}
		else if (a == 0) {
			myArray[i] = TaylorSerieCosineElement(i, scale);
		}
		else {
			myArray[i] = 0.;
		}
		a++;
	}
}

static	inline	double	TaylorSeriesSineElement(
								int_t	cnt,
								double	scale
							)
{
	double	value = 1.;
	while (cnt) {
		value *= scale / cnt;
		cnt--;
	}
	return	value;
}

static	inline	void	PopulateTaylorSeriesSine(
								double	* myArray,
								int_t	myArrayCnt,
								double	scale
							)
{
	int_t a = -1;
	for (int_t i = 0; i < myArrayCnt; i++) {
		if (a == 2) {
			myArray[i] = -TaylorSeriesSineElement(i, scale);
			a = -2;
		}
		else if (a == 0) {
			myArray[i] = TaylorSeriesSineElement(i, scale);
		}
		else {
			myArray[i] = 0.;
		}
		a++;
	}
}


#endif
