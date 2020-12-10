#pragma once

#include <assert.h>
#include <cmath>


struct	MATRIX {
	double			_11;
	double			_12;
	double			_13;
	double			_21;
	double			_22;
	double			_23;
	double			_31;
	double			_32;
	double			_33;
	double			_41;
	double			_42;
	double			_43;
};

struct	VECTOR2 {
	double			x;
	double			y;
};

struct	VECTOR3 {
	double			x;
	double			y;
    double			z;
};

void	MatrixIdentity(
                MATRIX  * matrix
            )
{
	matrix->_11 = 1.;
	matrix->_12 = 0.;
	matrix->_13 = 0.;
	matrix->_21 = 0.;
	matrix->_22 = 1.;
	matrix->_23 = 0.;
	matrix->_31 = 0.;
	matrix->_32 = 0.;
	matrix->_33 = 1.;
	matrix->_41 = 0.;
	matrix->_42 = 0.;
	matrix->_43 = 0.;
}

static	inline	double	Vec2Normalize(
								VECTOR2						* pInOut
							)
{
	double	size = pInOut->x * pInOut->x + pInOut->y * pInOut->y;

	double	sqrtSize = sqrt(size);

	pInOut->x /= sqrtSize;
	pInOut->y /= sqrtSize;

	return	sqrtSize;
}

static	inline	void	Vec3Subtract(
								VECTOR3						* pOut,
								const VECTOR3				* pV1,
								const VECTOR3				* pV2
							)
{
	assert(pOut != pV1);

	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
}

static	inline	double	Vec3Normalize(
								VECTOR3						* pInOut
							)
{
	double	size = pInOut->x * pInOut->x + pInOut->y * pInOut->y + pInOut->z * pInOut->z;

	double	sqrtSize = sqrt(size);

	pInOut->x /= sqrtSize;
	pInOut->y /= sqrtSize;
	pInOut->z /= sqrtSize;

	return	sqrtSize;
}

static	inline	double	Vec3Length(
								const VECTOR3				* pV
							)
{
	return	sqrt((pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z));
}

static	inline	double	Vec3Dot(
								const VECTOR3				* pV1,
								const VECTOR3				* pV2
							)
{
	assert(pV1 != pV2);

	double	dotProduct = pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;

	return	dotProduct;
}

static	inline	void	Vec3Cross(
								VECTOR3						* pOut,
								const VECTOR3				* pV1,
								const VECTOR3				* pV2
							)
{
	VECTOR3 v;

	v.x = pV1->y * pV2->z - pV1->z * pV2->y;
	v.y = pV1->z * pV2->x - pV1->x * pV2->z;
	v.z = pV1->x * pV2->y - pV1->y * pV2->x;

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;
}

void	Vec3Transform(
				VECTOR3			* pOut,
				const VECTOR3	* pV,
				const MATRIX	* pM
			)
{
	VECTOR3	pTmp;

	pTmp.x = pV->x * pM->_11 + pV->y * pM->_21 + pV->z * pM->_31 + pM->_41;
	pTmp.y = pV->x * pM->_12 + pV->y * pM->_22 + pV->z * pM->_32 + pM->_42;
	pTmp.z = pV->x * pM->_13 + pV->y * pM->_23 + pV->z * pM->_33 + pM->_43;

	pOut->x = pTmp.x;
	pOut->y = pTmp.y;
	pOut->z = pTmp.z;
}

void	MatrixInverse(
				MATRIX			* pOut,
				const MATRIX	* pM
			)
{
	MATRIX pTmp;

	double	a, b, c, d, e, f, determinant;
	a = pM->_11 * pM->_22;
	b = pM->_12 * pM->_23;
	c = pM->_13 * pM->_21;
	d = pM->_22 * pM->_31;
	e = pM->_21 * pM->_33;
	f = pM->_23 * pM->_32;

	determinant = a * pM->_33 +
				  b * pM->_31 +
				  c * pM->_32 -
				  pM->_13 * d -
				  pM->_12 * e -
				  pM->_11 * f;

	pTmp._11 = (pM->_22 * pM->_33 - f) / determinant;
	pTmp._12 = (pM->_13 * pM->_32 - pM->_12 * pM->_33) / determinant;
	pTmp._13 = (b - pM->_13 * pM->_22) / determinant;
	pTmp._21 = (pM->_23 * pM->_31 - e) / determinant;
	pTmp._22 = (pM->_11 * pM->_33 - pM->_13 * pM->_31) / determinant;
	pTmp._23 = (c - pM->_11 * pM->_23) / determinant;
	pTmp._31 = (pM->_21 * pM->_32 - d) / determinant;
	pTmp._32 = (pM->_12 * pM->_31 - pM->_11 * pM->_32) / determinant;
	pTmp._33 = (a - pM->_12 * pM->_21) / determinant;
	pTmp._41 = 0.;
	pTmp._42 = 0.;
	pTmp._43 = 0.;

	VECTOR3	vTmp;
	vTmp.x = -pM->_41;
	vTmp.y = -pM->_42;
	vTmp.z = -pM->_43;
	Vec3Transform(&vTmp, &vTmp, &pTmp);

	pOut->_11 = pTmp._11;
	pOut->_12 = pTmp._12;
	pOut->_13 = pTmp._13;

	pOut->_21 = pTmp._21;
	pOut->_22 = pTmp._22;
	pOut->_23 = pTmp._23;

	pOut->_31 = pTmp._31;
	pOut->_32 = pTmp._32;
	pOut->_33 = pTmp._33;

	pOut->_41 = vTmp.x;
	pOut->_42 = vTmp.y;
	pOut->_43 = vTmp.z;
}
