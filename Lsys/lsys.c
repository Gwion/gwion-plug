/* 
 * LSYS
 * Yet Another Micro-language by Paul Batchelor
 *
 * LSYS is a tiny little language designed to produce l-systems.
 * A grammar for a classic L-System could look like this:
 *
 * a|a:ab|b:a
 *
 * The code is split up into three slices, delimited by the '|'.
 * The first slice dictates the initial axiom, 'a'.
 * The second slice dictates the definition for 'a' to be 'ab'.
 * The third slice dictates the definition for 'b' to be 'a'.
 *
 * Once the code has been parsed, it can be used to generate a
 * list, whose length is determined by the order N:
 *
 * N | output
 * ----------
 * 1 | a
 * 2 | ab 
 * 3 | aba
 * 4 | abaab
 * 5 | abaababa
 * 6 | abaababaabaab
 * 7 | abaababaabaababaababa
 *
 * And so on and so forth...
 *
 * ## LSYS in Sporth
 *
 * LSYS is implemented as a Sporth UGen, which takes in
 * 3 arguments. From left to right, they are:
 *
 * 1. trigger signal, which iterates through the L-System
 * 2. The order N of the L-System (init-time only)
 * 3. The code itself.
 *
 * The signal output by the LSYS ugen a number in
 * the range of 0-35, which correspond to the base-36
 * numbering system:
 *
 * 0123456789abcdefghijklmnopqrstuvwxyz
 *
 * In the example above, the signal would be alternating between
 * 10 and 11.
 *
 * To see the lys ugen in action, see examples/lsys.sp
 *
 * ## LSYS as a standalone
 *
 * LSYS can also be compiled as a standalone application:
 *
 * gcc lsys.c -DLSYS_STANDALONE -o lsys
 *
 * It can be fed in code and the order as command line arguments:
 *
 * ./lsys 5 "01|0:121|1:01|2:1
 *
 * Which will produce the following string:
 *
 * 01101121011210101101121011210101121010110112101
 *
 * Modified To fit Gwion.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lsys.h"


#include "gwion_util.h"

/* static funcs */
static int toint(unsigned char a) {
    if(a >= 97)
        return a - 87;
    else
        return a - 48;
}

static int lsys_list_append(lsys_d *lsys, lsys_list *lst, char c)
{
    lsys_item *new = (lsys_item*)xmalloc(sizeof(lsys_item));
    new->ent = &lsys->ent[toint(c)];
    lst->last->nxt = new;
    lst->last = new;
    lst->size++;
    return 0;
}


/* non static funcs */
int lsys_init(lsys_d *ls)
{
    ls->pos = 0;
    ls->cur = 0;
    ls->mode = MODE_VALUE;
    ls->ent[0].start = 1;
    ls->ent[0].end = 0;
    ls->ent[0].val = -1;
    unsigned int i;
    for(i = 1; i < 37; i++) {
        ls->ent[i].start = 1;
        ls->ent[i].end = 1;
        ls->ent[i].val = i - 1;
    }
    return 0;
}

unsigned int lsys_parse(lsys_d *ls, char *ref, char *str, unsigned int len)
{
    ls->pos++;
    switch(ls->mode) {
        case MODE_KEY:
            switch(str[0]) {
                case ':':
                    ls->cur = toint(ref[ls->pos -2]) + 1;
                    ls->ent[ls->cur].start = ls->pos + 1;
                    ls->mode = MODE_VALUE;
                    break;
                default:
                    break;
            }
            break;
        case MODE_VALUE:
            switch(str[0]) {
                case '|':
                    ls->ent[ls->cur].end = ls->pos;
                    ls->mode = MODE_KEY;
                    break;
                default:
                    break;
            }
            break;
        default:
            gw_err("Unknown mode!\n");
            break;
    }
    if(len > 0) {
        return lsys_parse(ls, ref, (str + 1), len - 1);
    } else {
        ls->ent[ls->cur].end = ls->pos;
        return len;
    }
}

/* static */
int lsys_list_init(lsys_list *lst)
{
    lst->last = &lst->root;
    lst->size = 0;
    return 0;
}

/* static */
int lsys_list_destroy(lsys_list *lst)
{
    lsys_item *itm = lst->root.nxt;
    lsys_item *nxt;
    unsigned int i;
    for(i = 0; i < lst->size; i++) {
        nxt = itm->nxt;
        free(itm);
        itm = nxt;
    }
    return 0;
}

/* static */
int lsys_make_list(lsys_d *lsys,
    lsys_list *lst, char *str, char c, int N)
{
    if(N > 0) {
        int pos;

        if(c == 0) {
            pos = 0;
        } else {
            pos = toint(c) + 1;
        }
        lsys_entry *ent = &lsys->ent[pos];
        for(uint i = ent->start; i < ent->end; i++) {
            lsys_make_list(lsys, lst, str, str[i - 1], N - 1);
        }
    } else {
        lsys_list_append(lsys, lst, c);
    }
    return 0;
}

/* static */
void lsys_list_reset(lsys_list *lst)
{
    lst->last = lst->root.nxt;
}

/* static */
unsigned int lsys_list_iter(lsys_list *lst, lsys_entry **ent, unsigned pos)
{
    if(pos == 0) {
        lsys_list_reset(lst);
    }
    *ent = lst->last->ent;
    lst->last = lst->last->nxt;
    return (pos + 1) % lst->size;
}
