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

#ifndef __RDF_LTD__MATHEMATICS_GEOMETRY_DOUBLE_PRECISION_H
#define __RDF_LTD__MATHEMATICS_GEOMETRY_DOUBLE_PRECISION_H


#include <cmath>


struct VECTOR3
{
	double	x, y, z;
};

static	double	Sqr(
						double		value
					)
{
	return	value * value;
}

static	double	Vec3DistanceSqr(
						const VECTOR3				* pV1,
						const VECTOR3				* pV2
					)
{
	return	Sqr(pV1->x - pV2->x) + Sqr(pV1->y - pV2->y) + Sqr(pV1->z - pV2->z);
}

static	double	Vec3Distance(
						const VECTOR3				* pV1,
						const VECTOR3				* pV2
					)
{
	return	sqrt(
					Vec3DistanceSqr(
							pV1,
							pV2
						)
				);
}


#endif
