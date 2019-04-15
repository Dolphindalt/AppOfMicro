/**
 * @author Dalton Caron
 * @version 4/6/2019
 */
#include "wavelet.h"

/**
 * LeGall 5/3 Forward Wavelet Transform
 * @param s Some signal
 * @paran n Length of said signal
 */
void flegall53(float *s, const int n)
{
    float a;
    int i;

    a = -0.5; // predict step
    for(i = 1; i < n - 2; i += 2)
        s[i] += a * (s[i - 1] + s[i + 1]);
    s[n - 1] += 2 * a * s[n - 2]; // this trouble is due to the odd amount of wavelet coefficients

    a = 0.25; // update step
    for(i = 2; i < n; i += 2)
        s[i] += a * (s[i - 1] + s[i + 1]);
    s[0] += 2 * a * s[1];

    a = 1.41421356237; // scale step
    for(i = 0; i < n; i++)
    {
        if(i & 0x01)
            s[i] *= a;
        else
            s[i] /= a;
    }
}
