/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF.  The full HDF copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://www.hdfgroup.org/licenses.               *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 *  This code is the C-interface called by Java programs to access the
 *  HDF 4 library.
 *
 *  Each routine wraps a single HDF entry point, generally with the
 *  analogous arguments and return codes.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <jni.h>
#include "hdf.h"
#include "h4jni.h"

JNIEXPORT jboolean JNICALL
Java_hdf_hdflib_HDFLibrary_DFUfptoimage
(JNIEnv *env, jclass clss, jint hdim, jint vdim,
        jfloat max, jfloat min, jfloatArray hscale, jfloatArray vscale, jfloatArray data,
        jbyteArray palette, jstring outfile, jint ct_method, jint hres, jint vres, jint compress)
{
    H4_UNIMPLEMENTED(ENVONLY, "DFUfptoimage (windows)");

done:
    return JNI_TRUE;
}


#ifdef __cplusplus
} /* end extern "C" */
#endif /* __cplusplus */
