/****************************************************************************
 * NCSA HDF                                                                 *
 * Software Development Group                                               *
 * National Center for Supercomputing Applications                          *
 * University of Illinois at Urbana-Champaign                               *
 * 605 E. Springfield, Champaign IL 61820                                   *
 *                                                                          *
 * For conditions of distribution and use, see the accompanying             *
 * hdf/COPYING file.                                                        *
 *                                                                          *
 ****************************************************************************/

#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif

/* $Id$ */

/***************************************************************************
*
* Slab test routines slabw(), slabwf64(), slabwui16(), slabwui8(),
*  slab1w(), slab2w(), slab3w(), slab4w()
*
***************************************************************************/

#include "hdf.h"
#include "tutils.h"

/* External variables */
extern int num_errs;
extern int Verbocity;

#ifdef TEST_PC
#define FAR far
#else
#define FAR /* */
#endif

/* Internal variables */

static int32 FAR rank = 3;

static float64 FAR maxf64  = 123.0;
static float64 FAR minf64  = -1.0;
static float64 FAR fillf64 =  1.0;

static float32 FAR maxf32  = (float32)123.0;
static float32 FAR minf32  = (float32)-1.0;
static float32 FAR fillf32 = (float32) 1.0;

#ifndef PC
static intn FAR maxin  = 123;
static intn FAR minin  = -1;
static intn FAR fillin = 1;

static uintn FAR maxuin  = 123;
static uintn FAR minuin  = 2;
static uintn FAR filluin = 1;
#endif

static int32 FAR maxi32  = 123;
static int32 FAR mini32  = -1;
static int32 FAR filli32 = 1;

static uint32 FAR maxui32  = 123;
static uint32 FAR minui32  = 2;
static uint32 FAR fillui32 = 1;

static int16 FAR maxi16  = 123;
static int16 FAR mini16  = -1;
static int16 FAR filli16 = 1;

static uint16 FAR maxui16  = 123;
static uint16 FAR minui16  = 2;
static uint16 FAR fillui16 = 1;

static int8 FAR maxi8    = 123;
static int8 FAR mini8    = -1;
static int8 FAR filli8   = 1;

static uint8 FAR maxui8    = 123;
static uint8 FAR minui8    =  2;
static uint8 FAR fillui8   = 1;

/* Dimensions of slab */
static int32 FAR size_dims[3]  = {2, 3, 4}; /* size of slab dims */
static int32 FAR start_dims[3] = {1, 1, 1}; /* starting dims  */
static int32 FAR stride[3]     = {0, 0, 0};

/* luf for planes, rows and cols  */
static char FAR *lpln = "Time";
static char FAR *upln = "Second";
static char FAR *fpln = "Int32";
static char FAR *lrow = "Line";
static char FAR *urow = "Inch";
static char FAR *frow = "Int16";
static char FAR *lcol = "Column";
static char FAR *ucol = "Cm";
static char FAR *fcol = "Int32";

/* scales for planes, rows, and cols */
static float64 FAR scplnf64[2] = {0.0, 100.0};
static float64 FAR scrowf64[3] = {0.0, 10.0, 20.0};
static float64 FAR sccolf64[4] = {0.0, 1.0, 2.0, 3.0};

static float32 FAR scplnf32[2] = {(float32)0.0, (float32)100.0};
static float32 FAR scrowf32[3] = {(float32)0.0, (float32)10.0, (float32)20.0};
static float32 FAR sccolf32[4] = {(float32)0.0, (float32)1.0, (float32)2.0, (float32)3.0};

#ifndef PC
static intn FAR scplnin[2] = {0, 100};
static intn FAR scrowin[3] = {0, 10, 20};
static intn FAR sccolin[4] = {0, 1, 2, 3};

static uintn FAR scplnuin[2] = {0, 100};
static uintn FAR scrowuin[3] = {0, 10, 20};
static uintn FAR sccoluin[4] = {0, 1, 2, 3};
#endif

static int32 FAR scplni32[2] = {0, 100};
static int32 FAR scrowi32[3] = {0, 10, 20};
static int32 FAR sccoli32[4] = {0, 1, 2, 3};

static uint32 FAR scplnui32[2] = {0, 100};
static uint32 FAR scrowui32[3] = {0, 10, 20};
static uint32 FAR sccolui32[4] = {0, 1, 2, 3};

static int16 FAR scplni16[2] = {0, 100};
static int16 FAR scrowi16[3] = {0, 10, 20};
static int16 FAR sccoli16[4] = {0, 1, 2, 3};

static uint16 FAR scplnui16[2] = {0, 100};
static uint16 FAR scrowui16[3] = {0, 10, 20};
static uint16 FAR sccolui16[4] = {0, 1, 2, 3};

static int8 FAR scplni8[2] = {0, 100};
static int8 FAR scrowi8[3] = {0, 10, 20};
static int8 FAR sccoli8[4] = {0, 1, 2, 3};

static uint8 FAR scplnui8[2] = {0, 100};
static uint8 FAR scrowui8[3] = {0, 10, 20};
static uint8 FAR sccolui8[4] = {0, 1, 2, 3};

/* Slabs for slabw(), slab1w(), slab2w() */
static float32 FAR slabw1[1][1][3] = { { {(float32)110.0, (float32)111.0, (float32)112.0} } };
static float32 FAR slabw2[2][1][3] = { { {(float32)20.0, (float32)21.0, (float32)22.0} },
                                   { {(float32)120.0, (float32)121.0, (float32)122.0} } };
static float32 FAR slabw3[1][2][3] = { { {(float32)0.0, (float32)1.0, (float32)2.0},
                                     {(float32)10.0, (float32)11.0, (float32)12.0} } };
static float32 FAR slabw4[1][1][3] = { { {(float32)100.0, (float32)101.0, (float32)102.0} } };
static float32 FAR slabw5[2][3][1] = { { {(float32)3.0}, {(float32)13.0}, {(float32)23.0} },
                                   { {(float32)103.0}, {(float32)113.0}, {(float32)123.0} } };

static float64 FAR slabw1f64[1][1][3] = { { {110.0, 111.0, 112.0} } };
static float64 FAR slabw2f64[2][1][3] = { { {20.0, 21.0, 22.0} },
                                    { {120.0, 121.0, 122.0} } };
static float64 FAR slabw3f64[1][2][3] = { { {0.0, 1.0, 2.0},
                                        {10.0, 11.0, 12.0} } };
static float64 FAR slabw4f64[1][1][3] = { { {100.0, 101.0, 102.0} } };
static float64 FAR slabw5f64[2][3][1] = { { {3.0}, {13.0}, {23.0} },
                                      { {103.0}, {113.0}, {123.0} } };

#ifndef PC
static intn FAR slabw1in[1][1][3] = { { {110, 111, 112} } };
static intn FAR slabw2in[2][1][3] = { { {20, 21, 22} },
                                  { {120, 121, 122} } };
static intn FAR slabw3in[1][2][3] = { { {0, 1, 2},
                                    {10, 11, 12} } };
static intn FAR slabw4in[1][1][3] = { { {100, 101, 102} } };
static intn FAR slabw5in[2][3][1] = { { {3}, {13}, {23} },
                                  { {103}, {113}, {123} } };

static uintn FAR slabw1uin[1][1][3] = { { {110, 111, 112} } };
static uintn FAR slabw2uin[2][1][3] = { { {20, 21, 22} },
                                    { {120, 121, 122} } };
static uintn FAR slabw3uin[1][2][3] = { { {0, 1, 2},
                                      {10, 11, 12} } };
static uintn FAR slabw4uin[1][1][3] = { { {100, 101, 102} } };
static uintn FAR slabw5uin[2][3][1] = { { {3}, {13}, {23} },
                                    { {103}, {113}, {123} } };
#endif

static int32 FAR slabw1i32[1][1][3] = { { {110, 111, 112} } };
static int32 FAR slabw2i32[2][1][3] = { { {20, 21, 22} },
                                    { {120, 121, 122} } };
static int32 FAR slabw3i32[1][2][3] = { { {0, 1, 2},
                                      {10, 11, 12} } };
static int32 FAR slabw4i32[1][1][3] = { { {100, 101, 102} } };
static int32 FAR slabw5i32[2][3][1] = { { {3}, {13}, {23} },
                                    { {103}, {113}, {123} } };

static uint32 FAR slabw1ui32[1][1][3] = { { {110, 111, 112} } };
static uint32 FAR slabw2ui32[2][1][3] = { { {20, 21, 22} },
                                      { {120, 121, 122} } };
static uint32 FAR slabw3ui32[1][2][3] = { { {0, 1, 2},
                                        {10, 11, 12} } };
static uint32 FAR slabw4ui32[1][1][3] = { { {100, 101, 102} } };
static uint32 FAR slabw5ui32[2][3][1] = { { {3}, {13}, {23} },
                                      { {103}, {113}, {123} } };

static int16 FAR slabw1i16[1][1][3] = { { {110, 111, 112} } };
static int16 FAR slabw2i16[2][1][3] = { { {20, 21, 22} },
                                    { {120, 121, 122} } };
static int16 FAR slabw3i16[1][2][3] = { { {0, 1, 2},
                                      {10, 11, 12} } };
static int16 FAR slabw4i16[1][1][3] = { { {100, 101, 102} } };
static int16 FAR slabw5i16[2][3][1] = { { {3}, {13}, {23} },
                                    { {103}, {113}, {123} } };

static uint16 FAR slabw1ui16[1][1][3] = { { {110, 111, 112} } };
static uint16 FAR slabw2ui16[2][1][3] = { { {20, 21, 22} },
                                      { {120, 121, 122} } };
static uint16 FAR slabw3ui16[1][2][3] = { { {0, 1, 2},
                                        {10, 11, 12} } };
static uint16 FAR slabw4ui16[1][1][3] = { { {100, 101, 102} } };
static uint16 FAR slabw5ui16[2][3][1] = { { {3}, {13}, {23} },
                                      { {103}, {113}, {123} } };

static int8 FAR slabw1i8[1][1][3] = { { {110, 111, 112} } };
static int8 FAR slabw2i8[2][1][3] = { { {20, 21, 22} },
                                  { {120, 121, 122} } };
static int8 FAR slabw3i8[1][2][3] = { { {0, 1, 2},
                                    {10, 11, 12} } };
static int8 FAR slabw4i8[1][1][3] = { { {100, 101, 102} } };
static int8 FAR slabw5i8[2][3][1] = { { {3}, {13}, {23} },
                                  { {103}, {113}, {123} } };

static uint8 FAR slabw1ui8[1][1][3] = { { {110, 111, 112} } };
static uint8 FAR slabw2ui8[2][1][3] = { { {20, 21, 22} },
                                    { {120, 121, 122} } };
static uint8 FAR slabw3ui8[1][2][3] = { { {0, 1, 2},
                                      {10, 11, 12} } };
static uint8 FAR slabw4ui8[1][1][3] = { { {100, 101, 102} } };
static uint8 FAR slabw5ui8[2][3][1] = { { {3}, {13}, {23} },
                                    { {103}, {113}, {123} } };
/* Slabs for slab3w() */
static float32 FAR slab1[1][1][1] = { { {(float32)0.0} } };
static float32 FAR slab2[1][1][1] = { { {(float32)1.0} } };
static float32 FAR slab3[1][1][1] = { { {(float32)2.0} } };
static float32 FAR slab4[1][1][1] = { { {(float32)3.0} } };
static float32 FAR slab5[1][1][1] = { { {(float32)10.0} } };
static float32 FAR slab6[1][1][1] = { { {(float32)11.0} } };
static float32 FAR slab7[1][1][1] = { { {(float32)12.0} } };
static float32 FAR slab8[1][1][1] = { { {(float32)13.0} } };
static float32 FAR slab9[1][1][1] = { { {(float32)20.0} } };
static float32 FAR slab10[1][1][1] = { { {(float32)21.0} } };
static float32 FAR slab11[1][1][1] = { { {(float32)22.0} } };
static float32 FAR slab12[1][1][1] = { { {(float32)23.0} } };
static float32 FAR slab13[1][1][1] = { { {(float32)100.0} } };
static float32 FAR slab14[1][1][1] = { { {(float32)101.0} } };
static float32 FAR slab15[1][1][1] = { { {(float32)102.0} } };
static float32 FAR slab16[1][1][1] = { { {(float32)103.0} } };
static float32 FAR slab17[1][1][1] = { { {(float32)110.0} } };
static float32 FAR slab18[1][1][1] = { { {(float32)111.0} } };
static float32 FAR slab19[1][1][1] = { { {(float32)112.0} } };
static float32 FAR slab20[1][1][1] = { { {(float32)113.0} } };
static float32 FAR slab21[1][1][1] = { { {(float32)120.0} } };
static float32 FAR slab22[1][1][1] = { { {(float32)121.0} } };
static float32 FAR slab23[1][1][1] = { { {(float32)122.0} } };
static float32 FAR slab24[1][1][1] = { { {(float32)123.0} } };

/* data array in memory  */
static float32 FAR fdata[2][3][4] =
        {{{(float32)   0.0,(float32)   1.0,(float32)   2.0,(float32)   3.0},
          {(float32)  10.0,(float32)  11.0,(float32)  12.0,(float32)  13.0},
          {(float32)  20.0,(float32)  21.0,(float32)  22.0,(float32)  23.0}},
         {{(float32) 100.0,(float32) 101.0,(float32) 102.0,(float32) 103.0},
          {(float32) 110.0,(float32) 111.0,(float32) 112.0,(float32) 113.0},
          {(float32) 120.0,(float32) 121.0,(float32) 122.0,(float32) 123.0}}};
static float64 FAR f64data[2][3][4] =
		{{{   0.0,   1.0,   2.0,   3.0},
		  {  10.0,  11.0,  12.0,  13.0},
		  {  20.0,  21.0,  22.0,  23.0}},
		 {{ 100.0, 101.0, 102.0, 103.0},
		  { 110.0, 111.0, 112.0, 113.0},
		  { 120.0, 121.0, 122.0, 123.0}}};
#ifndef PC
static intn FAR indata[2][3][4] =
		{{{   0,   1,   2,   3},
		  {  10,  11,  12,  13},
		  {  20,  21,  22,  23}},
		 {{ 100, 101, 102, 103},
		  { 110, 111, 112, 113},
		  { 120, 121, 122, 123}}};
static uintn FAR uindata[2][3][4] =
		{{{   0,   1,   2,   3},
		  {  10,  11,  12,  13},
		  {  20,  21,  22,  23}},
		 {{ 100, 101, 102, 103},
		  { 110, 111, 112, 113},
		  { 120, 121, 122, 123}}};
#endif
static int32 FAR i32data[2][3][4] =
		{{{   0,   1,   2,   3},
		  {  10,  11,  12,  13},
		  {  20,  21,  22,  23}},
		 {{ 100, 101, 102, 103},
		  { 110, 111, 112, 113},
		  { 120, 121, 122, 123}}};
static uint32 FAR ui32data[2][3][4] =
		{{{   0,   1,   2,   3},
		  {  10,  11,  12,  13},
		  {  20,  21,  22,  23}},
		 {{ 100, 101, 102, 103},
		  { 110, 111, 112, 113},
		  { 120, 121, 122, 123}}};
static int16 FAR i16data[2][3][4] =
		{{{   0,   1,   2,   3},
		  {  10,  11,  12,  13},
		  {  20,  21,  22,  23}},
		 {{ 100, 101, 102, 103},
		  { 110, 111, 112, 113},
		  { 120, 121, 122, 123}}};
static uint16 FAR ui16data[2][3][4] =
		{{{   0,   1,   2,   3},
		  {  10,  11,  12,  13},
		  {  20,  21,  22,  23}},
		 {{ 100, 101, 102, 103},
		  { 110, 111, 112, 113},
		  { 120, 121, 122, 123}}};
static int8 FAR i8data[2][3][4] =
		{{{   0,   1,   2,   3},
		  {  10,  11,  12,  13},
		  {  20,  21,  22,  23}},
		 {{ 100, 101, 102, 103},
		  { 110, 111, 112, 113},
		  { 120, 121, 122, 123}}};
static uint8 FAR ui8data[2][3][4] =
		{{{   0,   1,   2,   3},
		  {  10,  11,  12,  13},
		  {  20,  21,  22,  23}},
		 {{ 100, 101, 102, 103},
		  { 110, 111, 112, 113},
		  { 120, 121, 122, 123}}};
/* Output files */
static char FAR *swf32  = "swf32.hdf";
static char FAR *swf64  = "swf64.hdf";
static char FAR *swin = "swin.hdf";
static char FAR *swuin = "swuin.hdf";
static char FAR *swi32 = "swi32.hdf";
static char FAR *swui32 = "swui32.hdf";
static char FAR *swi16 = "swi16.hdf";
static char FAR *swui16 = "swui16.hdf";
static char FAR *swi8  = "swi8.hdf";
static char FAR *swui8  = "swui8.hdf";
static char FAR *sw1 = "s1w.hdf";
static char FAR *sw3 = "s3w.hdf";
static char FAR *sw4 = "s4w.hdf";

/*
** Write data set to slabw.hdf as 5 hyperslabs.
*/
int
slabwf32()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  float32 sdata[2][3][4]; /* Data array read from from file */
  float32 lfill;

    MESSAGE(10,printf("\n slabwf32:  Writing 5 slabs to slabwf32.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccolf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* 
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxf32, (VOIDP)&minf32);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&fillf32);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swf32);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swf32, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillf32) 
       num_err ++;
    MESSAGE(10,printf("\n       fill value =: %f \n", lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwf32:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != fdata[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%f, ",sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwf32 <<< \n");)
    else
       MESSAGE(10,printf("\n       slabwf32:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
** Write float64 data set to slabw.hdf as 5 hyperslabs.
*/
int
slabwf64()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  float64 sdata[2][3][4]; /* Data array read from from file */
  float64 lfill;

    MESSAGE(10,printf("\n slabwf64:  Writing 5 slabs to slabwf64.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_FLOAT64);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnf64);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowf64);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccolf64);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxf64, (VOIDP)&minf64);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&fillf64);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swf64);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5f64);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swf64, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillf64)
       num_err += 1;
    MESSAGE(10,printf("\n       fill value =: %f \n", (float)lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwf64:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != f64data[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%f, ",(float)sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwf64 <<< \n");)
    else
       MESSAGE(10,printf("\n       slabwf64:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}


#ifndef PC
/*
** Write intn data set to slabwin.hdf as 5 hyperslabs.
*/
int
slabwin()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  intn sdata[2][3][4]; /* Data array read from from file */
  intn lfill;

    MESSAGE(10,printf("\n slabwin:  Writing 5 slabs to slabwin.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_NINT32);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccolin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxin, (VOIDP)&minin);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&fillin);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swin);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5in);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swin, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillin)
       num_err += 1;
    MESSAGE(10,printf("\n       fill value =: %d \n", lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwin:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != indata[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%u, ",sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwin <<< \n");)
    else
       MESSAGE(10,printf("\n       slabwin:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}


/*
** Write uintn data set to slabwuin.hdf as 5 hyperslabs.
*/
int
slabwuin()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  uintn sdata[2][3][4]; /* Data array read from from file */
  uintn lfill;

    MESSAGE(10,printf("\n slabwuin:  Writing 5 slabs to slabwuin.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_NUINT32);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnuin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowuin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccoluin);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* 
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxuin, (VOIDP)&minuin);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&filluin);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swuin);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5uin);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swuin, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != filluin)
       num_err += 1;
    MESSAGE(10,printf("\n       fill value =: %u \n", lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwin:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != uindata[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%u, ",sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwuin <<< \n");)
    else
       MESSAGE(10,printf("\n       slabwuin:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}
#endif


/*
** Write int32 data set to slabwi32.hdf as 5 hyperslabs.
*/
int
slabwi32()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  int32 sdata[2][3][4]; /* Data array read from from file */
  int32 lfill;

    MESSAGE(10,printf("\n slabwi32:  Writing 5 slabs to slabwi32.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_INT32);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplni32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowi32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccoli32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxi32, (VOIDP)&mini32);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&filli32);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swi32);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5i32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swi32, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != filli32)
       num_err += 1;
    MESSAGE(10,printf("\n       fill value =: %d \n", (int)lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwi32:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != i32data[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%d, ",(int)sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwi32 <<< \n");)
    else
       MESSAGE(10,printf("\n       slabwi32:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}


/*
** Write uint32 data set to slabwui32.hdf as 5 hyperslabs.
*/
int
slabwui32()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  uint32 sdata[2][3][4]; /* Data array read from from file */
  uint32 lfill;

    MESSAGE(10,printf("\n slabwui32:  Writing 5 slabs to slabwui32.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_UINT32);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnui32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowui32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccolui32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* 
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxui32, (VOIDP)&minui32);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&fillui32);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swui32);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5ui32);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swui32, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillui32) 
       num_err += 1;
    MESSAGE(10,printf("\n       fill value =: %u \n", (unsigned)lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwui32:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != ui32data[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%u, ",(unsigned)sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwui32 <<< \n");)
    else
       MESSAGE(10,printf("\n       slabwui32:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}


/*
** Write int16 data set to slabwi16.hdf as 5 hyperslabs.
*/
int
slabwi16()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  int16 sdata[2][3][4]; /* Data array read from from file */
  int16 lfill;

    MESSAGE(10,printf("\n slabwi16:  Writing 5 slabs to slabwi16.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_INT16);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplni16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowi16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccoli16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* 
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxi16, (VOIDP)&mini16);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&filli16);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swi16);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5i16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swi16, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != filli16)
       num_err += 1;
    MESSAGE(10,printf("\n       fill value =: %d \n", lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwi16:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != i16data[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%d, ",sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwi16 <<< \n");)
    else
      MESSAGE(10,printf("\n       slabwi16:  %d wrong values in slab.  \n", (int)num_err);)

    return (int)num_err;
}

/*
** Write uint16 data set to slabwui16.hdf as 5 hyperslabs.
*/
int
slabwui16()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  uint16 sdata[2][3][4]; /* Data array read from from file */
  uint16 lfill;

    MESSAGE(10,printf("\n slabwui16:  Writing 5 slabs to slabwui16.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_UINT16);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnui16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowui16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccolui16);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /*
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxui16, (VOIDP)&minui16);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&fillui16);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swui16);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5ui16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swui16, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillui16)
       num_err += 1;
    MESSAGE(10,printf("\n       fill value =: %u \n", lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwui16:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != ui16data[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%u, ",sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwui16 <<< \n");)
    else
       MESSAGE(10,printf("\n       slabwui16:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/*
** Write int8 data set to slabwi8.hdf as 5 hyperslabs.
*/
int
slabwi8()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  int8 sdata[2][3][4]; /* Data array read from from file */
  int8 lfill;

    MESSAGE(10,printf("\n slabwi8:  Writing 5 slabs to slabwi8.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_INT8);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplni8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowi8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccoli8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* 
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxi8, (VOIDP)&mini8);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&filli8);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swi8);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5i8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swi8, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != filli8)
       num_err += 1;
    MESSAGE(10,printf("\n       fill value =: %d \n", (int)lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwi8:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != i8data[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%d, ",(int)sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwi8 <<< \n");)
    else
       MESSAGE(10,printf("\n       slabwi8:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

/* 
** Write uint8 data set to slabw.hdf as 5 hyperslabs.
*/
int
slabwui8()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  uint8 sdata[2][3][4]; /* Data array read from from file */
  uint8 lfill;

    MESSAGE(10,printf("\n slabwui8:  Writing 5 slabs to slabwui8.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    ret = DFSDsetNT(DFNT_UINT8);
    CHECK(ret, FAIL, "DFSDsetNT");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnui8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowui8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccolui8);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* 
    ** write data out using slabs with
    ** each slab in different order to the file "slab.hdf"
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxui8, (VOIDP)&minui8);
    CHECK(ret, FAIL, "DFSDsetrange");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&fillui8);
    CHECK(ret, FAIL, "DFSDsetfillvalue");

    ret = DFSDstartslab(swui8);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5ui8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(swui8, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");
    if (lfill != fillui8)
       num_err += 1;
    MESSAGE(10,printf("\n       fill value =: %u \n", (unsigned)lfill););

    if (num_err != 0)
        MESSAGE(10,printf("\n      slabwui8:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n  Verifying data  \n"););
    MESSAGE(10,printf("sdata = "););
    num_err = 0;
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
       	for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != ui8data[i][j][k])
               num_err += 1;
            MESSAGE(10,printf("%u, ",(unsigned)sdata[i][j][k]););
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n       >>> All tests passed for slabwui8 <<< \n");)
    else
       MESSAGE(10,printf("\n       slabwui8:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}


int
slab1w()
{
  int32 ret = 0;
  int32 num_err = 0;

    MESSAGE(10,printf("\n slab1w: Writing the first 3 of 5 slabs to slab1w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccolf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* Set fill value */
    ret = DFSDsetfillvalue((VOIDP)&fillf32);
    CHECK(ret, FAIL, "DFSDsetfillvalue");
    MESSAGE(10,printf("\n        slab1w: Setting fill value =%f \n", fillf32););

    /*
    ** write each slab in different order
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxf32, (VOIDP)&minf32);
    CHECK(ret, FAIL, "DFSDsetrange");

    ret = DFSDstartslab(sw1);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw1);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 2; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw3);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw5);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    if (num_err == 0)
       MESSAGE(10,printf("\n        >>> All tests passed for slab1w, now run slab2w <<< \n");)
    else
      MESSAGE(10,printf("\n         slab1w:  %d failures.  \n", (int)num_err););

    return (int)num_err;
}

int
slab2w()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  float32 sdata[2][3][4]; /* Data array read from from file */
  float32 lfill;
  intn trank;

    MESSAGE(10,printf("\n slab2w:  Writing the last 2 of 5 slabs to slab1w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* Get dimensions */
    ret = DFSDgetdims(sw1, &trank, size_dims, 3);
    CHECK(ret, FAIL, "DFSDgetdims");

    /* Get fill value */
    ret = DFSDgetfillvalue((VOIDP)&lfill);
    CHECK(ret, FAIL, "DFSDgetfillvalue");

    MESSAGE(10,printf("\n       fill value =: %f \n", lfill););

    /* Call Writeref() first */
    ret = DFSDwriteref(sw1, 2);
    CHECK(ret, FAIL, "DFSDwriteref");

    /*
    ** write each slab in different order
    */
    ret = DFSDstartslab(sw1);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw2);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 3;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slabw4);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0]  = 2; size_dims[1]  = 3; size_dims[2]  = 4;
    stride[0]     = 2; stride[1]     = 3; stride[2]     = 4;
    ret = DFSDgetslice(sw1, start_dims, size_dims, (VOIDP)sdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    if (num_err != 0)
      MESSAGE(10,printf("\n        slab2w:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n         Verifying data \n"););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
        for (k = 0; k < stride[2]; k++)
          {
            if (sdata[i][j][k] != fdata[i][j][k])
               num_err ++;
          }

    if (num_err == 0)
       MESSAGE(10,printf("\n        >>> All tests passed for slab2w <<< \n");)
    else
       MESSAGE(10,printf("\n        slab2w:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

int
slab3w()

{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  float32 adata[2][3][4]; /* Data array read from from file */

    MESSAGE(10,printf("\n slab3w: Writing all 24 elements of data as slabs to slab3w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccolf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    /* 
    ** write each element in different order 
    */

    /* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxf32, (VOIDP)&minf32);
    CHECK(ret, FAIL, "DFSDsetrange");

    ret = DFSDstartslab(sw3);
    CHECK(ret, FAIL, "DFSDstartslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab20);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab21);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab22);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab23);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 3; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab24);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab6);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab7);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab8);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab9);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab10);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab16);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab17);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab18);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 2; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab19);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab11);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 3; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab12);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab13);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab14);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 2; start_dims[1] = 1; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab15);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab1);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 2;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab2);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 3;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab3);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 4;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab4);
    CHECK(ret, FAIL, "DFSDwriteslab");

    start_dims[0] = 1; start_dims[1] = 2; start_dims[2] = 1;
    size_dims[0] = 1; size_dims[1] = 1; size_dims[2] = 1;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)slab5);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(sw3, start_dims, size_dims, (VOIDP)adata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    if (num_err != 0)
      MESSAGE(10,printf("\n        slab3w:  %d failures.  \n", (int)num_err););

    MESSAGE(10,printf("\n        Verifying data  \n"););
    MESSAGE(10,printf("adata = "););
     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
        for (k = 0; k < stride[2]; k++)
          {
            if (adata[i][j][k] != fdata[i][j][k])
               num_err ++;
            MESSAGE(10,printf("%f, ",adata[i][j][k]););
          }

    if (num_err == 0)
       MESSAGE(10,printf("\n        >>> All tests passed for slab3w <<< \n");)
    else
       MESSAGE(10,printf("\n       slab3w:  %d wrong values in slab.  \n", (int)num_err););

    return (int)num_err;
}

int
slab4w()
{
  int32 i, j, k;
  int32 ret = 0;
  int32 num_err = 0;
  float32 bdata[2][3][4]; /* Data array read from from file */

    MESSAGE(10,printf("\n slab4w: Writing data as 1 slab to slab4w.hdf \n"););

    ret = DFSDclear();
    CHECK(ret, FAIL, "DFSDclear");

    /* First set dimensions */
    ret = DFSDsetdims((int16)rank, size_dims);
    CHECK(ret, FAIL, "DFSDsetdims");

    /* Set dimension strings */
    ret = DFSDsetdimstrs(1, lpln, upln, fpln);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(2, lrow, urow, frow);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    ret = DFSDsetdimstrs(3, lcol, ucol, fcol);
    CHECK(ret, FAIL, "DFSDsetdimstrs");

    /* Set dimension scales */
    ret = DFSDsetdimscale(1, size_dims[0], (VOIDP)scplnf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(2, size_dims[1], (VOIDP)scrowf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

    ret = DFSDsetdimscale(3, size_dims[2], (VOIDP)sccolf32);
    CHECK(ret, FAIL, "DFSDsetdimscale");

	/* Set max, min range */
    ret = DFSDsetrange((VOIDP)&maxf32, (VOIDP)&minf32);
    CHECK(ret, FAIL, "DFSDsetrange");

    ret = DFSDstartslab(sw4);
    CHECK(ret, FAIL, "DFSDstartslab");

    /* write out all the data to hdf file */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    ret = DFSDwriteslab(start_dims, stride, size_dims, (VOIDP)fdata);
    CHECK(ret, FAIL, "DFSDwriteslab");

    ret = DFSDendslab();
    CHECK(ret, FAIL, "DFSDendslab");

    /* Verify correctness of slab written */
    start_dims[0] = 1; start_dims[1] = 1; start_dims[2] = 1;
    size_dims[0] = 2; size_dims[1] = 3; size_dims[2] = 4;
    stride[0] = 2; stride[1] = 3; stride[2] = 4;
    ret = DFSDgetslice(sw4, start_dims, size_dims, (VOIDP)bdata, stride);
    CHECK(ret, FAIL, "DFSDgetslice");

    if (num_err != 0)
        MESSAGE(10,printf("\n        slab4w:  %d failures.  \n", (int)num_err););

   MESSAGE(10,printf("\n          Verifying data  \n"););

     for (i = 0; i < stride[0]; i++)
       for (j = 0; j < stride[1]; j++)
        for (k = 0; k < stride[2]; k++)
          {
            if (bdata[i][j][k] != fdata[i][j][k])
               num_err ++;
          }
    if (num_err == 0)
       MESSAGE(10,printf("\n          >>> All tests passed for slab4w <<< \n");)
    else
       MESSAGE(10,printf("\n          slab4w:  %d wrong values in slab.  \n", (int)num_err););

    return (int)(num_err);
}

/*
** Main slab call to all other slab functions
*/
void
test_slab()
{
    num_errs += slabwf32();
    num_errs += slabwf64();
#ifndef PC
    num_errs += slabwin();
    num_errs += slabwuin();
#endif
    num_errs += slabwi32();
    num_errs += slabwui32();
    num_errs += slabwi16();
    num_errs += slabwui16();
    num_errs += slabwi8();
    num_errs += slabwui8();
    num_errs += slab1w();
    num_errs += slab2w();
    num_errs += slab3w();
    num_errs += slab4w();
}
