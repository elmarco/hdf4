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

#ifndef __HDP_H
#define __HDP_H

#include "hdf.h"
#include "hfile.h"
#include "vg.h"

/* Global Variables (ick) */
#ifndef HDP_MASTER
extern
#endif							/* !HDP_MASTER */
intn        vinit_done
#ifdef HDP_MASTER
= FALSE							/* indicates Vsets have been init'ed for the current file */
#endif							/* HDP_MASTER */
           ;

/* Global Definitions */
#define MAXCHOICES 50
#ifndef MAXNAMELEN
#define MAXNAMELEN 100
#endif /* !MAXNAMELEN */
#define MAXCLASSLEN 100
#define MAXRANK 100
#define MAXFNLEN 256
#define CONDENSE 1

/* Add enum and string for new commands to both of the variables below. */
/* Preserve the correct/corresponding ordering */
typedef enum
  {
	  HELP, LIST, DUMPSDS, DUMPRIG, DUMPVG, DUMPVD, DUMPGR, NONE
  }
command_t;
#ifndef HDP_MASTER
extern
#endif							/* !HDP_MASTER */
const char *commands[]
#ifdef HDP_MASTER
=
{
	"help",
	"list",
	"dumpsds",
	"dumprig",
	"dumpvg",
	"dumpvd",
	"dumpgr"
}
#endif							/* HDP_MASTER */
           ;

/* Global options structure */
typedef struct
  {
	  intn        help;			/* Print help on this command */
  }
dump_opt_t;

typedef enum
  {
	  OTAG, OFILE, OGROUP, ONAME
  }
sort_t;							/* The order tag/refs are sorted */

/* 'list' command option structure */
typedef struct
  {
	  sort_t      order;		/* The sort order tag/refs are printed in */
	  enum
		{
			VSHORT, VLONG, VDEBUG
		}
	  verbosity;				/* verbosity level of list */
	  enum
		{
			LNONE, LTAGNUM, LTAGNAME, LGROUP
		}
	  limit;					/* How to limit tag/refs */
	  intn        class;		/* Whether to dump class information */
	  intn        name;			/* Whether to dump name information */
	  intn        desc;			/* Whether to dump description information */
	  intn        spec;			/* Whether to dump special element information */
	  intn        group;		/* Whether to dump group information */
	  uint16      limit_tag;	/* tag # to limit search to */
	  char       *limit_name;	/* tag name to limit search to */
  }
list_info_t;

/* Which dataset to dump */
typedef enum
  {
	  DINDEX, DREFNUM, DNAME, DCLASS, DFIELDS, DALL
  }
filter_t;

/* Which contents to dump */
typedef enum
  {
	  DVERBOSE, DHEADER, DDATA
  }
content_t;

/* What kind of data to dump to file */
typedef enum
  {
	  DASCII, DBINARY
  }
file_type_t;

/* 'dumpsds' command option structure */
typedef struct
  {
	  filter_t    filter;		/* Which data object to dump */
	  intn       *filter_num;	/* ref #'s or indices to use as filter */
	  char      **filter_str;	/* names or classes to use as filter */
	  int32       num_chosen;	/* number of items chosen (-1==ALL) */
	  content_t   contents;		/* What contents to dump */
	  intn        dump_to_file;	/* Whether to dump to a filename */
	  file_type_t file_type;	/* What kind of data to dump to file */
	  char        file_name[MAXNAMELEN];	/* Name of file to dump into */
  }
dump_info_t;

/* Filename list structure */
typedef struct
  {
	  intn        max_files;	/* the number of files in the file list */
	  intn        curr_file;	/* the current file */
	  char      **file_arr;		/* pointer to the filename information */
  }
filelist_t;

/* Group info structure */
typedef struct
  {
	  intn        max_dds;		/* the number of DDs in the group */
	  intn        curr_dd;		/* The current DD */
	  DFdi       *dd_arr;		/* array to hold the DDs in the group */
  }
groupinfo_t;

/* DD info structure */
typedef struct
  {
	  uint16      tag, ref;		/* tag and ref of object */
	  int32       offset, length;	/* offset and length of object in file */
	  int32       index;		/* index of the object in the file */
	  uintn       is_group:1,	/* flag to indicate item is a group */
	              is_special:1,	/* flag to indicate item is a special element */
	              has_label:1,	/* flag to indicate item has a label */
	              has_desc:1;	/* flag to indicate item has a desc. */
	  groupinfo_t *group_info;	/* pointer to group information */
	  sp_info_block_t *spec_info;	/* pointer to special element information */
  }
objinfo_t;
#define CHECK_SPECIAL   0x0001	/* Look for spec. elem. when building dd list */
#define CHECK_GROUP     0x0002	/* Look for groups when building dd list */
#define CHECK_LABEL     0x0004	/* Look for anno. labels when building dd list */
#define CHECK_DESC      0x0008	/* Look for anno. desc. when building dd list */

/* DD list structure */
typedef struct
  {
	  intn        max_obj;		/* the number of dds in the obj list */
	  intn        curr_obj;		/* the current obj */
	  uint32      options;		/* storage for options used to build the dd list */
	  objinfo_t **srt_obj_arr;	/* the sorted dd/object information array */
	  objinfo_t  *raw_obj_arr;	/* the raw dd/object information array */
  }
objlist_t;

/* hdp_list.c */
extern intn print_data_labels(const char *fname, int32 an_id, uint16 tag, uint16 ref);
extern intn print_data_descs(const char *fname, int32 an_id, uint16 tag, uint16 ref);
extern intn print_all_data_labels(const char *fname, int32 an_id);
extern intn print_all_data_descs(const char *fname, int32 an_id);
extern intn do_list(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts);

/* hdp_sds.c */
extern intn do_dumpsds(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts);

/* hdp_rig.c */
extern intn do_dumprig(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts);

/* hdp_vg.c */
extern intn do_dumpvg(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts);

/* hdp_vd.c */
extern intn do_dumpvd(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_opts);
extern intn parse_dumpvd_opts(dump_info_t * dumpvd_opts, intn *curr_arg, intn argc,
              char *argv[], char *flds_chosen[MAXCHOICES], int *dumpallfields);

/* hdp_gr.c */
extern intn do_dumpgr(intn curr_arg, intn argc, char *argv[], dump_opt_t * glob_ops);


/* hdp_dump.c */
extern intn fmtchar(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtuchar8(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtbyte(unsigned char *x, file_type_t ft, FILE * ofp);
extern intn fmtint(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtshort(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtint8(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtuint8(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtint16(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtuint16(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtint32(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtuint32(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtfloat32(VOIDP x, file_type_t ft, FILE * ofp);
extern intn fmtfloat64(VOIDP x, file_type_t ft, FILE * ofp);
extern int32 dumpfull(int32 nt, file_type_t ft, int32 cnt, VOIDP databuf, intn indent, FILE * ofp);

/* show.c */
extern int32 dumpvd(int32 vd, file_type_t ft, int data_only, FILE *fp, 
                    char separater[2],int32 flds_indices[VSFIELDMAX], 
                    int dumpallfields);
extern intn dumpattr(int32 vid, int32 findex, intn isvs, file_type_t ft, FILE *fp);

/* hdp_util.c */
	/* misc. functions */
extern char *tagnum_to_name(intn num);
extern intn tagname_to_num(const char *name);
extern void sort(int32 *chosen, int32 choices);
int sort_obj_list_by_tag(const void *, const void *);
int int32_compare(const void *, const void *);
	/* filename list functions */
extern filelist_t *make_file_list(intn curr_arg, intn argc, char *argv[]);
extern char *get_next_file(filelist_t * f_list, intn advance);
extern void free_file_list(filelist_t * f_list);
	/* group list functions */
extern groupinfo_t *make_group_list(int32 fid, uint16 tag, uint16 ref);
extern DFdi *get_next_group(groupinfo_t * g_list, intn advance);
extern int32 get_group_max(groupinfo_t * g_list);
extern void free_group_list(groupinfo_t * g_list);
	/* object list functions */
extern objlist_t *make_obj_list(int32 fid, uint32 options);
extern objinfo_t *get_next_obj(objlist_t * o_list, intn advance);
extern objinfo_t *goto_nth_obj(objlist_t * o_list, intn n);
extern void reset_obj_list(objlist_t * o_list);
extern void free_obj_list(objlist_t * o_list);
extern void sort_obj_list(objlist_t * o_list, sort_t sort_type);

#endif /* __HDP_H */
