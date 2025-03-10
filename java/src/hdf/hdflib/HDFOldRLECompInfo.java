/****************************************************************************
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF Java Products. The full HDF Java copyright       *
 * notice, including terms governing use, modification, and redistribution,  *
 * is contained in the file, COPYING.  COPYING can be found at the root of   *
 * the source code distribution tree. You can also access it online  at      *
 * http://www.hdfgroup.org/products/licenses.html.  If you do not have       *
 * access to the file, you may request a copy from help@hdfgroup.org.        *
 ****************************************************************************/

package hdf.hdflib;

/**
 * <p>
 * This class is a generic container for the parameters to the HDF RLE compressed classes, with the
 * ``old'' type encoding.
 * <p>
 * In this case, there is no auxilliary information.
 */

public class HDFOldRLECompInfo extends HDFOldCompInfo {

    /** */
    public HDFOldRLECompInfo() {
        ctype = HDFConstants.COMP_RLE;
    }

}
