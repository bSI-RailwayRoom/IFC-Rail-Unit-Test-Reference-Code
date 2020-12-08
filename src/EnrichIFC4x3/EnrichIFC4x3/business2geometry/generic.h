#pragma once


static	inline	bool    equals(
                                char    * txtI,
                                char    * txtII
                            )
{
    if (txtI && txtII) {
        int_t   i = 0;
        while (txtI[i]) {
            if (txtI[i] != txtII[i]) {
                return  false;
            }
            i++;
        }
        if (txtII[i] == 0) {
            return  true;
        }
    }
    return  false;
}

