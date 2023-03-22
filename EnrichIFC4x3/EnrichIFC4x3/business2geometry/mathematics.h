#pragma once


#include <assert.h>
#include <cmath>


static	const	double		Pi = 3.14159265358979323846;


struct	___MATRIX {
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

struct	___VECTOR2 {
	double			x;
	double			y;
};

struct	___VECTOR3 {
	double			x;
	double			y;
    double			z;
};

struct	___POINT4D {
	___VECTOR3		point;
	___VECTOR3		tangent;
	___VECTOR3		normal;
	double			direction__DERIVED;
};

static	inline	bool	___LinesIntersect(
								___VECTOR2			* intersectionPoint,
								const ___VECTOR2	* lineI_pointI,
								const ___VECTOR2	* lineI_pointII,
								const ___VECTOR2	* lineII_pointI,
								const ___VECTOR2	* lineII_pointII
							)
{
	double	a1 = lineI_pointII->y - lineI_pointI->y,
			b1 = lineI_pointI->x  - lineI_pointII->x,
			c1 = lineI_pointII->x * lineI_pointI->y - lineI_pointI->x * lineI_pointII->y;

	double	a2 = lineII_pointII->y - lineII_pointI->y,
			b2 = lineII_pointI->x  - lineII_pointII->x,
			c2 = lineII_pointII->x * lineII_pointI->y - lineII_pointI->x * lineII_pointII->y;

	double	denom = a1 * b2 - a2 * b1;

	if (std::fabs(denom) < 0.00000000001) {
		return	false;
	}

	intersectionPoint->x = (b1 * c2 - b2 * c1) / denom;
	intersectionPoint->y = (a2 * c1 - a1 * c2) / denom;
	return	true;
}

static	inline	void	___MatrixMultiply(
								___MATRIX		* pOut,
								const ___MATRIX	* pM1,
								const ___MATRIX	* pM2
							)
{
	assert(pOut && pM1 && pM2);

	___MATRIX pTmp;

	if (pOut) {
		pTmp._11 = pM1->_11 * pM2->_11 + pM1->_12 * pM2->_21 + pM1->_13 * pM2->_31;
		pTmp._12 = pM1->_11 * pM2->_12 + pM1->_12 * pM2->_22 + pM1->_13 * pM2->_32;
		pTmp._13 = pM1->_11 * pM2->_13 + pM1->_12 * pM2->_23 + pM1->_13 * pM2->_33;

		pTmp._21 = pM1->_21 * pM2->_11 + pM1->_22 * pM2->_21 + pM1->_23 * pM2->_31;
		pTmp._22 = pM1->_21 * pM2->_12 + pM1->_22 * pM2->_22 + pM1->_23 * pM2->_32;
		pTmp._23 = pM1->_21 * pM2->_13 + pM1->_22 * pM2->_23 + pM1->_23 * pM2->_33;

		pTmp._31 = pM1->_31 * pM2->_11 + pM1->_32 * pM2->_21 + pM1->_33 * pM2->_31;
		pTmp._32 = pM1->_31 * pM2->_12 + pM1->_32 * pM2->_22 + pM1->_33 * pM2->_32;
		pTmp._33 = pM1->_31 * pM2->_13 + pM1->_32 * pM2->_23 + pM1->_33 * pM2->_33;

		pTmp._41 = pM1->_41 * pM2->_11 + pM1->_42 * pM2->_21 + pM1->_43 * pM2->_31 + pM2->_41;
		pTmp._42 = pM1->_41 * pM2->_12 + pM1->_42 * pM2->_22 + pM1->_43 * pM2->_32 + pM2->_42;
		pTmp._43 = pM1->_41 * pM2->_13 + pM1->_42 * pM2->_23 + pM1->_43 * pM2->_33 + pM2->_43;

		pOut->_11 = pTmp._11;
		pOut->_12 = pTmp._12;
		pOut->_13 = pTmp._13;

        pOut->_21 = pTmp._21;
		pOut->_22 = pTmp._22;
		pOut->_23 = pTmp._23;

        pOut->_31 = pTmp._31;
		pOut->_32 = pTmp._32;
		pOut->_33 = pTmp._33;

        pOut->_41 = pTmp._41;
		pOut->_42 = pTmp._42;
		pOut->_43 = pTmp._43;
	}
	else {
		assert(false);
	}
}

static	inline	void	___MatrixIdentity(
								___MATRIX				* matrix
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

static	inline	double	___Vec2Normalize(
								___VECTOR2				* pInOut
							)
{
	double	size = pInOut->x * pInOut->x + pInOut->y * pInOut->y;

	double	sqrtSize = sqrt(size);

	pInOut->x /= sqrtSize;
	pInOut->y /= sqrtSize;

	return	sqrtSize;
}

static	inline	void	___Vec2Subtract(
								___VECTOR2				* pOut,
								const ___VECTOR2		* pV1,
								const ___VECTOR2		* pV2
							)
{
	assert(pOut != pV1);

	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
}

static	inline	void	___Vec3Subtract(
								___VECTOR3				* pOut,
								const ___VECTOR3		* pV1,
								const ___VECTOR3		* pV2
							)
{
	assert(pOut != pV1);

	pOut->x = pV1->x - pV2->x;
	pOut->y = pV1->y - pV2->y;
	pOut->z = pV1->z - pV2->z;
}

static	inline	double	___Vec3Normalize(
								___VECTOR3				* pInOut
							)
{
	double	size = pInOut->x * pInOut->x + pInOut->y * pInOut->y + pInOut->z * pInOut->z;

	double	sqrtSize = sqrt(size);

	pInOut->x /= sqrtSize;
	pInOut->y /= sqrtSize;
	pInOut->z /= sqrtSize;

	return	sqrtSize;
}

static	inline	double	___Vec3Distance(
								const ___VECTOR3		* pV1,
								const ___VECTOR3		* pV2
							)
{
	return	sqrt((pV1->x - pV2->x) * (pV1->x - pV2->x) + (pV1->y - pV2->y) * (pV1->y - pV2->y) + (pV1->z - pV2->z) * (pV1->z - pV2->z));
}

static	inline	double	___Vec2Length(
								const ___VECTOR2		* pV
							)
{
	return	sqrt((pV->x * pV->x) + (pV->y * pV->y));
}

static	inline	double	___Vec3Length(
								const ___VECTOR3		* pV
							)
{
	return	sqrt((pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z));
}

static	inline	double	___Vec3Dot(
								const ___VECTOR3		* pV1,
								const ___VECTOR3		* pV2
							)
{
	assert(pV1 != pV2);

	double	dotProduct = pV1->x * pV2->x + pV1->y * pV2->y + pV1->z * pV2->z;

	return	dotProduct;
}

static	inline	void	___Vec3Cross(
								___VECTOR3				* pOut,
								const ___VECTOR3		* pV1,
								const ___VECTOR3		* pV2
							)
{
	___VECTOR3 v;

	v.x = pV1->y * pV2->z - pV1->z * pV2->y;
	v.y = pV1->z * pV2->x - pV1->x * pV2->z;
	v.z = pV1->x * pV2->y - pV1->y * pV2->x;

	pOut->x = v.x;
	pOut->y = v.y;
	pOut->z = v.z;
}

static	inline	void	___Vec3Transform(
								___VECTOR3				* pOut,
								const ___VECTOR3		* pV,
								const ___MATRIX			* pM
							)
{
	___VECTOR3	pTmp;

	pTmp.x = pV->x * pM->_11 + pV->y * pM->_21 + pV->z * pM->_31 + pM->_41;
	pTmp.y = pV->x * pM->_12 + pV->y * pM->_22 + pV->z * pM->_32 + pM->_42;
	pTmp.z = pV->x * pM->_13 + pV->y * pM->_23 + pV->z * pM->_33 + pM->_43;

	pOut->x = pTmp.x;
	pOut->y = pTmp.y;
	pOut->z = pTmp.z;
}

static	inline	void	___MatrixInverse(
								___MATRIX				* pOut,
								const ___MATRIX			* pM
							)
{
	___MATRIX pTmp;

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

	___VECTOR3	vTmp;
	vTmp.x = -pM->_41;
	vTmp.y = -pM->_42;
	vTmp.z = -pM->_43;
	___Vec3Transform(&vTmp, &vTmp, &pTmp);

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

static	inline	double	___Vec3Dot(
								const ___VECTOR3	* pV
							)
{
	return	(pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z);
}

static	inline	double	___PointLineDistance(
								___VECTOR3			* linePointB,
								const ___VECTOR3	* point,
								const ___VECTOR3	* linePointI,
								const ___VECTOR3	* linePointII
							)
{
	___VECTOR3	v, w;
	v.x = linePointII->x - linePointI->x;
	v.y = linePointII->y - linePointI->y;
	v.z = linePointII->z - linePointI->z;
	w.x = point->x - linePointI->x;
	w.y = point->y - linePointI->y;
	w.z = point->z - linePointI->z;

	double	c1 = ___Vec3Dot(&w, &v),
			c2 = ___Vec3Dot(&v),
			b;
	if (c2 < -0.0000000000000001 || c2 > 0.0000000000000001) {
		b = c1 / c2;
	}
	else {
		b = 0.;
///		assert(false);
	}

	if (linePointB) {
		linePointB->x = linePointI->x + b * v.x;
		linePointB->y = linePointI->y + b * v.y;
		linePointB->z = linePointI->z + b * v.z;
		
		return	___Vec3Distance(
						point,
						linePointB
					);
	}
	else {
		___VECTOR3	pnt;

		pnt.x = linePointI->x + b * v.x;
		pnt.y = linePointI->y + b * v.y;
		pnt.z = linePointI->z + b * v.z;
		
		return	___Vec3Distance(
						point,
						&pnt
					);
	}
}
