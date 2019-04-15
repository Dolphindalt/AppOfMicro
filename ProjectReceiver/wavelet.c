/**
 * @author Dalton Caron
 * @version 4/6/2019
 */
#include "wavelet.h"

/**
 * LeGall 5/3 Inverse Wavelet Transform
 * @param s Some signal
 * @paran n Length of said signal
 */
void ilegall53(float *s, const int n)
{
    float a;
    int i;

    a = 0.70710678118; // invert scale
    for(i = 0; i < n; i++)
    {
        if(i & 0x01)
            s[i] *= a;
        else
            s[i] /= a;
    }

    a = -0.25; // invert update
    for(i = 2; i < n; i += 2)
        s[i] += a * (s[i - 1] + s[i + 1]);
    s[0] += 2 * a * s[1];

    a = 0.5; // invert predict
    for(i = 1; i < n - 2; i += 2)
        s[i] += a * (s[i - 1] + s[i + 1]);
    s[n - 1] += 2 * a * s[n - 2];
}
