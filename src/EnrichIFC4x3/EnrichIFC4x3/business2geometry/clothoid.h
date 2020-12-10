#pragma once

#include "mathematics.h"


static	inline	double	EulerSpiralX(
								double	RL,
								double	l
							)
{
	//
	//	x = l - l^5 / (40 * RL^2) + l^9 / (3456 * RL^4) - l^13 / (599040 * RL^6)
	//
	double	ll_div_RL = l * l / RL,
			ll_div_RL_0 = 1.,
			ll_div_RL_2 = ll_div_RL * ll_div_RL,
			ll_div_RL_4 = ll_div_RL_2 * ll_div_RL * ll_div_RL,
			ll_div_RL_6 = ll_div_RL_4 * ll_div_RL * ll_div_RL,
			ll_div_RL_8 = ll_div_RL_6 * ll_div_RL * ll_div_RL,
			ll_div_RL_10 = ll_div_RL_8 * ll_div_RL * ll_div_RL,
			ll_div_RL_12 = ll_div_RL_10 * ll_div_RL * ll_div_RL;

	return	l * (ll_div_RL_0 / 1. -
				 ll_div_RL_2 / 40. +
				 ll_div_RL_4 / 3456. -
				 ll_div_RL_6 / 599040. +
				 ll_div_RL_8 / 175472640. -
				 ll_div_RL_10 / 78033715200. +
				 ll_div_RL_12 / 49049763840000.);
}

static	inline	double	EulerSpiralY(
								double	RL,
								double	l
							)
{
	//
	//	y = l^3 / (6 * RL) - l^7 / (336 * RL^3) + l^11 / (42240 * RL^5) - l^15 / (9676800 * RL^7)
	//
	double	ll_div_RL = l * l / RL,
			ll_div_RL_3 = ll_div_RL * ll_div_RL * ll_div_RL,
			ll_div_RL_5 = ll_div_RL_3 * ll_div_RL * ll_div_RL,
			ll_div_RL_7 = ll_div_RL_5 * ll_div_RL * ll_div_RL,
			ll_div_RL_9 = ll_div_RL_7 * ll_div_RL * ll_div_RL,
			ll_div_RL_11 = ll_div_RL_9 * ll_div_RL * ll_div_RL,
			ll_div_RL_13 = ll_div_RL_11 * ll_div_RL * ll_div_RL;

	return	l * (ll_div_RL   / 6. -
				 ll_div_RL_3 / 336. +
				 ll_div_RL_5 / 42240. -
				 ll_div_RL_7 / 9676800. + 
				 ll_div_RL_9 / 3530096640. -
				 ll_div_RL_11 / 1880240947200. +
				 ll_div_RL_13 / 1377317368627200.);
}

static	inline	void    CalculateClothoidPosition(
								double  clothoidConstant,
								double  offset,
								VECTOR3 * vector
							)
{
    double	RL = clothoidConstant * clothoidConstant;

    vector->x = EulerSpiralX(RL, offset);
    vector->y =
		(clothoidConstant < 0.) ?
			-EulerSpiralY(RL, offset) :
			EulerSpiralY(RL, offset);
    vector->z = 0.;
}

static	inline	void    CalculateClothoidDirection(
								double  clothoidConstant,
								double  offset,
								VECTOR3 * vector
							)
{
    double	RL = clothoidConstant * clothoidConstant,
            epsilon = 0.001;

    VECTOR3 posI = {
                        EulerSpiralX(RL, offset - epsilon),
                        (clothoidConstant < 0.) ?
		                    -EulerSpiralY(RL, offset - epsilon) :
		                    EulerSpiralY(RL, offset - epsilon),
                        0.
                    },
            posII = {
                        EulerSpiralX(RL, offset + epsilon),
                        (clothoidConstant < 0.) ?
		                    -EulerSpiralY(RL, offset + epsilon) :
		                    EulerSpiralY(RL, offset + epsilon),
                        0.
                    };

    vector->x = posII.x - posI.x;
    vector->y = posII.y - posI.y;
    vector->z = 0.;

    Vec3Normalize(vector);
}
