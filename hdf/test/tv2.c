#ifdef RCSID
static char RcsId[] = "@(#)$Revision$";
#endif
/*
$Header$

$Log$
Revision 1.5  1993/01/19 05:59:25  koziol
Merged Hyperslab and JPEG routines with beginning of DEC ALPHA
port.  Lots of minor annoyances fixed.

 * Revision 1.4  1992/11/30  22:01:15  chouck
 * Added fix for Vstart() and Vend()
 *
 * Revision 1.3  1992/05/27  21:51:19  chouck
 * Added a few casts to VSwrite() calls
 *
 * Revision 1.2  1992/05/18  22:11:07  sxu
 * modified constants for number types
 *
 * Revision 1.1  1992/03/01  22:29:07  dilg
 * Initial revision
 *
 * Revision 1.1  1992/02/29  19:55:36  likkai
 * Initial revision
 *
*/
/*
	=================================
	HDF VSET TEST PROGRAM
	Jason Ng Feb-28-92 NCSA
	=================================
*/


#include "vg.h"

#define filef "tv2a.hdf"
#define fileg "tv2b.hdf"
#define NVDATAS 3
#define CLASS "FIGURES"

static char * vdatanames[NVDATAS] = { "GNP", "INCOME", "SALESTAX"};

main (ac,av) int ac; char**av;
{
		
    printf("%s: tests creating of vsets in 2 files simultaneously\n", av[0]);
    printf("after the test, files %s and %s will each contain 2 vgroups and %d vdatas\n",
           filef, fileg, NVDATAS); 
    
    doit();
    printf("\nsuccess: files %s and %s created\n", filef, fileg);
    printf("use the utility vshow to examine these files:\n");
    printf("\t\tvshow %s \n",filef);
    printf("\t\tvshow %s \n",fileg);
    printf("results should be as in %s.result\n",av[0]);
    
}

doit() {
    HFILEID 	f, g;
    VGROUP * vg1, * vg2;
    VDATA  * vs1, * vs2;
    
    char* vgname1 = "ALASKA";
    char* vgname2 = "MISSISSIPPI";
    
    int i;
    int bb[500];
    int nelt1 = 20;
    int nelt2 = 15;
    
    
    for(i=0;i<500;i++) bb[i] =i;
    
    if( FAIL==(f=Hopen(filef,DFACC_ALL,0)))
	{ printf(" %s open err\n",filef); exit(0); }
    
    if( FAIL==(g=Hopen(fileg,DFACC_ALL,0)))
	{ printf(" %s open err\n",fileg); exit(0); }

    Vstart(f);
    Vstart(g);
    
    vg1 = (VGROUP*) Vattach(f,-1,"w"); 
    vg2 = (VGROUP*) Vattach(g,-1,"w"); 
    
    Vsetname(vg1,vgname1); Vsetclass (vg1,CLASS);
    Vsetname(vg2,vgname2); Vsetclass (vg2,CLASS);
    
    printf("created vgroup [%s.%s] in file %s ok\n", vgname1, CLASS, filef);
    printf("created vgroup [%s.%s] in file %s ok\n", vgname2, CLASS, fileg);
    
    for(i=0;i<NVDATAS;i++) {
        
        vs1 = (VDATA*) VSattach(f,-1,"w");
        vs2 = (VDATA*) VSattach(g,-1,"w");
        
        VSsetname (vs1,vdatanames[i]); VSsetclass(vs1, CLASS);
        VSsetname (vs2,vdatanames[i]); VSsetclass(vs2, CLASS);
        
        VSfdefine(vs1, "DOLLARS", DFNT_INT32,1);
        VSfdefine(vs2, "DOLLARS", DFNT_INT32,1);
        
        VSsetfields (vs1,"DOLLARS");
        VSsetfields (vs2,"DOLLARS");
        
        VSwrite (vs1, (unsigned char *) &bb[i*50], nelt1, FULL_INTERLACE);
        
        VSwrite (vs2, (unsigned char *) &bb[i*50], nelt2, FULL_INTERLACE);
        
        Vinsert (vg1,vs1);
        Vinsert (vg2,vs2);
        
        VSdetach (vs1);
        VSdetach (vs2);
        
        printf("created vdata %s (%d elts) linked to [%s] in %s\n",
               vdatanames[i], nelt1, vgname1, filef);
        
        printf("created vdata %s (%d elts) linked to [%s] in %s\n",
               vdatanames[i], nelt2, vgname2, fileg);
    }
    
    Vdetach(vg1);
    Vdetach(vg2);
    
    Vend(f);
    Vend(g);
    
    Hclose(f);
    Hclose(g);
    
}

