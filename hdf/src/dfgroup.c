#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.5  1993/01/19 05:54:31  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

 * Revision 1.4  1992/12/28  18:18:37  mfolk
 * Changed header doc of DFdiget to make it a little clearer.
 *
 * Revision 1.3  1992/11/02  16:35:41  koziol
 * Updates from 3.2r2 -> 3.3
 *
 * Revision 1.2  1992/10/22  22:53:32  chouck
 * Added group handle to group interface
 *
 * Revision 1.1  1992/08/25  21:40:44  koziol
 * Initial revision
 *
*/
/*-----------------------------------------------------------------------------
 * File:    dfgroup.c
 * Purpose: Low level functions for implementing groups
 * Invokes: df.c df.h
 * Contents:
 *  DFdiread: read in the data identifier list from the group
 *  DFdiget: get next data identifier from list
 *  DFdisetup: get ready to store a list of data identifiers to write out
 *  DFdiput: add a data identifier to the list to be written out
 *  DFdiwrite: write out the list of data identifiers
 * Remarks: A group is a way of associating data elements with each other.
 *          It is a tag whose data is a list of tag/refs
 *          Each tag/ref combination is called a data identifier (DI).
 *---------------------------------------------------------------------------*/

#include "hdf.h"
#include "herr.h"
#include "hfile.h"

#define MAX_GROUPS 8

typedef struct DIlist_struct {
    uint8    *DIlist;
    int32    num;
    int32    current;
} DIlist, *DIlist_ptr;

static DIlist_ptr *Group_list = NULL;

#define GSLOT2ID(s) ((((uint32)GROUPTYPE & 0xffff) << 16) | ((s) & 0xffff))
#define VALIDGID(i) (((((uint32)(i) >> 16) & 0xffff) == GROUPTYPE) && \
                    (((uint32)(i) & 0xffff) < MAX_GROUPS))
#define GID2REC(i)  ((VALIDGID(i) ? (Group_list[(uint32)(i) & 0xffff]) : NULL))

/*-----------------------------------------------------------------------------
 * Name:    setgroupREC
 * Purpose: Add a group list into the internal structure and return an ID
 * Inputs:  list_rec: list to remember
 * Returns: FAIL on failure else a group ID to the list
 * Users:   other group routines
 * Invokes: 
 * Remarks: Allocates internal storeage if necessary
 *---------------------------------------------------------------------------*/
PRIVATE int32
#ifdef PROTOTYPE
setgroupREC(DIlist_ptr list_rec)
#else
setgroupREC(list_rec)
     DIlist_ptr list_rec;
#endif
{
    int32 i;

    if (!Group_list) {
        Group_list = (DIlist_ptr *) HDgetspace((uint32) MAX_GROUPS * 
                                               sizeof(DIlist_ptr));
        if (!Group_list) return FAIL;
        
        for (i = 0; i < MAX_GROUPS; i++)
            Group_list[i] = NULL;

    }

    for (i = 0; i < MAX_GROUPS; i++)
        if (!Group_list[i]) {
            Group_list[i] = list_rec;
            return GSLOT2ID(i);
        }

    return FAIL;

} /* setgroupREC */

/*-----------------------------------------------------------------------------
 * Name:    DFdiread
 * Purpose: Read a list of DIs into memory
 * Inputs:  file_id: HDF file pointer
 *          tag, ref: id of group which is to be read in
 * Returns: FAIL on failure else a group ID to the list
 * Users:   HDF systems programmers, DF8getrig, other routines
 * Invokes: HDvalidfid, DFIfind, DFgetelement
 * Remarks: assumes tag is a group
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int32 DFdiread(int32 file_id, uint16 tag, uint16 ref)
#else
int32 DFdiread(file_id, tag, ref)
     int32 file_id;
     uint16 tag, ref;           /* tag, ref of group */
#endif
{
    DIlist_ptr new_list;
    char *FUNC="DFdiread";
    int32 length;

    HEclear();

    if (!HDvalidfid(file_id)) {
       HERROR(DFE_ARGS);
       return FAIL;
    }

    /* Find the group. */
    length = Hlength(file_id, tag, ref);
    if (length == FAIL) {
       return FAIL;
    }

    /* allocate a new structure to hold the group */
    new_list = (DIlist_ptr) HDgetspace((uint32) sizeof(DIlist));
    if(!new_list) {
        HERROR(DFE_NOSPACE);
        return FAIL;
    }
        
   
    new_list->DIlist = (uint8 *) HDgetspace((uint32)length);
    if (!new_list->DIlist) {
        HDfreespace(new_list);
        HERROR(DFE_NOSPACE);
        return FAIL;
    }

    new_list->num = length / 4;      /* 4==sizeof DFdi */
    new_list->current = 0;           /* no DIs returned so far */

    /* read in group */
    if (Hgetelement(file_id, tag, ref, (uint8 *)new_list->DIlist)<0) {
        HDfreespace(new_list->DIlist);
        HDfreespace(new_list);
        return FAIL;
    }
    return (int32) setgroupREC(new_list);
}

/*-----------------------------------------------------------------------------
 * Name:    DFdiget
 * Purpose: return next DI from the list of DIs in a group
 * Inputs:  list: handle to group (which is list of DIs)
 * Outputs: ptag: pointer to tag part of DI to be returned
 *          pref: pointer to ref part of DI to be returned
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF systems programmers, DF8getrig, other routines
 * Invokes: none
 * Remarks: frees Dilist space when all DIs returned
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFdiget(int32 list, uint16 *ptag, uint16 *pref)
#else
int DFdiget(list, ptag, pref)
     int32  list;
     uint16 *ptag;
     uint16 *pref;
#endif
{
    char       *FUNC="DFdiget";
    uint8      *p;
    DIlist_ptr list_rec;

    list_rec = GID2REC(list);

    if (!list_rec) return FAIL;
    if (list_rec->current >= list_rec->num) return FAIL;

    /* compute address of Ndi'th di */
    p = (uint8 *) list_rec->DIlist + 4 * list_rec->current++;
    UINT16DECODE(p, *ptag);
    UINT16DECODE(p, *pref);

    if (list_rec->current == list_rec->num) {
        HDfreespace(list_rec->DIlist);     /* if all returned, free storage */
        HDfreespace(list_rec);
        Group_list[list & 0xffff] = NULL;  /* YUCK! BUG! */
    }
    return SUCCEED;
}


/*-----------------------------------------------------------------------------
 * Name:    DFdisetup
 * Purpose: setup space for storing a list of DIs to be written out
 * Inputs:  maxsize: maximum number of DIs expected in the list
 * Returns: FAIL on failure with error set
 *          else a group ID
 * Users:   HDF systems programmers, DF8putrig, other routines
 * Invokes: none
 * Remarks: This call should go away sometime.  Need better way to allocate
 *          space, possibly just use a big block of static space
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int32 DFdisetup(int maxsize)
#else
int32 DFdisetup(maxsize)
    int maxsize;
#endif
{
    char *FUNC="DFdisetup";
    DIlist_ptr new_list;

    new_list = (DIlist_ptr) HDgetspace((uint32) sizeof(DIlist));

    if (!new_list) {
        HERROR(DFE_NOSPACE);
        return FAIL;
    }

    new_list->DIlist = (uint8 *) HDgetspace((uint32)(maxsize * 4));
                               /* 4==sizeof(DFdi) */
    if (!new_list->DIlist) {
        HDfreespace(new_list);
        HERROR(DFE_NOSPACE);
        return FAIL;
    }

    new_list->num     = maxsize;
    new_list->current = 0;

    return setgroupREC(new_list);
}

/*-----------------------------------------------------------------------------
 * Name:    DFdiput
 * Purpose: add a DI to the list to be written out
 * Inputs:  tag, ref: DI to add
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF systems programmers, DF8putrig, other routines
 * Invokes: none
 * Remarks: arg is tag/ref rather than DI for convenience
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFdiput(int32 list, uint16 tag, uint16 ref)
#else
int DFdiput(list, tag, ref)
     int32 list;
     uint16 tag, ref;
#endif
{
    char *FUNC="DFdiput";
    uint8 *p;
    DIlist_ptr list_rec;
    
    list_rec = GID2REC(list);

    if (!list_rec) return FAIL;
    if (list_rec->current >= list_rec->num) return FAIL;

    /* compute address of Ndi'th di to put tag/ref in */
    p = (uint8 *) list_rec->DIlist + 4 * list_rec->current++;
    UINT16ENCODE(p, tag);
    UINT16ENCODE(p, ref);

    return SUCCEED;
}

/*-----------------------------------------------------------------------------
 * Name:    DFdiwrite
 * Purpose: Write DI list out to HDF file
 * Inputs:  file_id: HDF file pointer
 *          tag, ref: tag and ref of group whose contents is the list
 * Returns: 0 on success, -1 on failure with error set
 * Users:   HDF systems programmers, DF8putrig, other routines
 * Invokes: none
 * Remarks: frees storage for Dilist
 *---------------------------------------------------------------------------*/

#ifdef PROTOTYPE
int DFdiwrite(int32 file_id, int32 list, uint16 tag, uint16 ref)
#else
int DFdiwrite(file_id, list, tag, ref)
     int32 file_id;
     int32 list;
     uint16 tag, ref;
#endif
{
    char *FUNC="DFdiwrite";
    int ret;                   /* return value */
    DIlist_ptr list_rec;

    if (!HDvalidfid(file_id)) {
        HERROR(DFE_ARGS);
        return FAIL;
    }
    
    list_rec = GID2REC(list);
    
    if (!list_rec) return FAIL;

    ret = Hputelement(file_id, tag, ref, list_rec->DIlist, 
                      (int32)list_rec->current * 4);   /* 4 == sizeof(DFdi) */
    HDfreespace(list_rec->DIlist);
    HDfreespace(list_rec);
    Group_list[list & 0xffff] = NULL;  /* YUCK! BUG! */
    return ret;
}


