#include <string.h>
#include "timermap.h"

//*****************************************************************************
//
// Lookup tables used by am_hal_ctimer_output_config().
//
//  CTx_tbl[] relates the padnum and pad funcsel based on a given CTx.
//  Valid pads for CTx are: 4-7, 11-13, 18-19, 22-33, 35, 37, 39, 42-49.
//
//  outcfg_tbl[] contains attributes of the 4 output signal types for each
//  of the 32 CTx signals. Therefore it is indexed by CTnumber 0-31.
//  This table provides only the non-common OUTCFG attributes (2-5, other
//  settings are shown below).
//  OUTCFG 0 = Force output to 0.
//  OUTCFG 1 = Force output to 1.
//  OUTCFG 6 = A6OUT2.
//  OUTCFG 7 = A7OUT2.
//
//*****************************************************************************
static const uint8_t CTx_tbl[32] =
{
    12, 25, 13, 26, 18,
    27, 19, 28,  5, 29,
     6, 30, 22, 31, 23,
    32, 42,  4, 43,  7,
    44, 24, 45, 33, 46,
    39, 47, 35, 48, 37,
    49, 11
};

#define OUTC(seg,num,reg)      ((reg << 4) | (seg << 3) | (num << 0))
#define OUTCNUM(ctx,n)         (outcfg_tbl[ctx][n] & (0x7 << 0))
#define OUTCSEG(ctx,n)         ((outcfg_tbl[ctx][n] & (0x1 << 3)) >> 3)
#define OUTCREG(ctx,n)         ((outcfg_tbl[ctx][n] & (0x1 << 4)) >> 4)
static const uint8_t outcfg_tbl[32][4] =
{
    {OUTC(0,0,0), OUTC(1,2,1), OUTC(0,5,1), OUTC(0,6,0)},     // CTX0:  A0OUT,  B2OUT2, A5OUT2, A6OUT
    {OUTC(0,0,1), OUTC(0,0,0), OUTC(0,5,0), OUTC(1,7,1)},     // CTX1:  A0OUT2, A0OUT,  A5OUT,  B7OUT2
    {OUTC(1,0,0), OUTC(1,1,1), OUTC(1,6,1), OUTC(0,7,0)},     // CTX2:  B0OUT,  B1OUT2, B6OUT2, A7OUT
    {OUTC(1,0,1), OUTC(1,0,0), OUTC(0,1,0), OUTC(0,6,0)},     // CTX3:  B0OUT2, B0OUT,  A1OUT,  A6OUT
    {OUTC(0,1,0), OUTC(0,2,1), OUTC(0,5,1), OUTC(1,5,0)},     // CTX4:  A1OUT,  A2OUT2, A5OUT2, B5OUT
    {OUTC(0,1,1), OUTC(0,1,0), OUTC(1,6,0), OUTC(0,7,0)},     // CTX5:  A1OUT2, A1OUT,  B6OUT,  A7OUT
    {OUTC(1,1,0), OUTC(0,1,0), OUTC(1,5,1), OUTC(1,7,0)},     // CTX6:  B1OUT,  A1OUT,  B5OUT2, B7OUT
    {OUTC(1,1,1), OUTC(1,1,0), OUTC(1,5,0), OUTC(0,7,0)},     // CTX7:  B1OUT2, B1OUT,  B5OUT,  A7OUT
    {OUTC(0,2,0), OUTC(0,3,1), OUTC(0,4,1), OUTC(1,6,0)},     // CTX8:  A2OUT,  A3OUT2, A4OUT2, B6OUT
    {OUTC(0,2,1), OUTC(0,2,0), OUTC(0,4,0), OUTC(1,0,0)},     // CTX9:  A2OUT2, A2OUT,  A4OUT,  B0OUT
    {OUTC(1,2,0), OUTC(1,3,1), OUTC(1,4,1), OUTC(0,6,0)},     // CTX10: B2OUT,  B3OUT2, B4OUT2, A6OUT
    {OUTC(1,2,1), OUTC(1,2,0), OUTC(1,4,0), OUTC(1,5,1)},     // CTX11: B2OUT2, B2OUT,  B4OUT,  B5OUT2
    {OUTC(0,3,0), OUTC(1,1,0), OUTC(1,0,1), OUTC(1,6,1)},     // CTX12: A3OUT,  B1OUT,  B0OUT2, B6OUT2
    {OUTC(0,3,1), OUTC(0,3,0), OUTC(0,6,0), OUTC(1,4,1)},     // CTX13: A3OUT2, A3OUT,  A6OUT,  B4OUT2
    {OUTC(1,3,0), OUTC(1,1,0), OUTC(1,7,1), OUTC(0,7,0)},     // CTX14: B3OUT,  B1OUT,  B7OUT2, A7OUT
    {OUTC(1,3,1), OUTC(1,3,0), OUTC(0,7,0), OUTC(0,4,1)},     // CTX15: B3OUT2, B3OUT,  A7OUT,  A4OUT2
    {OUTC(0,4,0), OUTC(0,0,0), OUTC(0,0,1), OUTC(1,3,1)},     //*CTX16: A4OUT,  A0OUT,  A0OUT2, B3OUT2
    {OUTC(0,4,1), OUTC(1,7,0), OUTC(0,4,0), OUTC(0,1,1)},     // CTX17: A4OUT2, B7OUT,  A4OUT,  A1OUT2
    {OUTC(1,4,0), OUTC(1,0,0), OUTC(0,0,0), OUTC(0,3,1)},     //*CTX18: B4OUT,  B0OUT,  A0OUT,  A3OUT2
    {OUTC(1,4,1), OUTC(0,2,0), OUTC(1,4,0), OUTC(1,1,1)},     // CTX19: B4OUT2, A2OUT,  B4OUT,  B1OUT2
    {OUTC(0,5,0), OUTC(0,1,0), OUTC(0,1,1), OUTC(1,2,1)},     //*CTX20: A5OUT,  A1OUT,  A1OUT2, B2OUT2
    {OUTC(0,5,1), OUTC(0,1,0), OUTC(1,5,0), OUTC(0,0,1)},     // CTX21: A5OUT2, A1OUT,  B5OUT,  A0OUT2
    {OUTC(1,5,0), OUTC(0,6,0), OUTC(0,1,0), OUTC(0,2,1)},     // CTX22: B5OUT,  A6OUT,  A1OUT,  A2OUT2
    {OUTC(1,5,1), OUTC(0,7,0), OUTC(0,5,0), OUTC(1,0,1)},     // CTX23: B5OUT2, A7OUT,  A5OUT,  B0OUT2
    {OUTC(0,6,0), OUTC(0,2,0), OUTC(0,1,0), OUTC(1,1,1)},     // CTX24: A6OUT,  A2OUT,  A1OUT,  B1OUT2
    {OUTC(1,4,1), OUTC(1,2,0), OUTC(0,6,0), OUTC(0,2,1)},     //*CTX25: B4OUT2, B2OUT,  A6OUT,  A2OUT2
    {OUTC(1,6,0), OUTC(1,2,0), OUTC(0,5,0), OUTC(0,1,1)},     //*CTX26: B6OUT,  B2OUT,  A5OUT,  A1OUT2
    {OUTC(1,6,1), OUTC(0,1,0), OUTC(1,6,0), OUTC(1,2,1)},     // CTX27: B6OUT2, A1OUT,  B6OUT,  B2OUT2
    {OUTC(0,7,0), OUTC(0,3,0), OUTC(0,5,1), OUTC(1,0,1)},     // CTX28: A7OUT,  A3OUT,  A5OUT2, B0OUT2
    {OUTC(1,5,1), OUTC(0,1,0), OUTC(0,7,0), OUTC(0,3,1)},     // CTX29: B5OUT2, A1OUT,  A7OUT,  A3OUT2
    {OUTC(1,7,0), OUTC(1,3,0), OUTC(0,4,1), OUTC(0,0,1)},     // CTX30: B7OUT,  B3OUT,  A4OUT2, A0OUT2
    {OUTC(1,7,1), OUTC(0,6,0), OUTC(1,7,0), OUTC(1,3,1)},     // CTX31: B7OUT2, A6OUT,  B7OUT,  B3OUT2
};

static int16_t timer_usage[2][8];

void timermap_ct_init()
{
    memset(timer_usage, -1, sizeof(timer_usage));
}

int16_t timermap_ct_find(PinName pin)
{
    for (int i = 0; i < 32; i++)
    {
        if (CTx_tbl[i] == pin)
        {
            return i;
        }
    }

    return -1;
}

void timermap_ct_assign(uint8_t seg, uint8_t num, int16_t user)
{
    timer_usage[seg][num] = user;
}

void timermap_ct_available(PinName pin, uint32_t *seg, uint32_t *num, uint32_t *reg)
{
    *seg = -1;
    *num = -1;
    *reg = -1;

    int16_t ct = timermap_ct_find(pin);

    if (ct == -1)
    {
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        uint8_t s = OUTCSEG(ct, i);
        uint8_t n = OUTCNUM(ct, i);
        uint8_t r = OUTCREG(ct, i);

        if ((timer_usage[s][n] < 0)
         || (timer_usage[s][n] == pin))
        {
            *seg = s;
            *num = n;
            *reg = r;

            timer_usage[s][n] = pin;
            return;
        }
    }
}
