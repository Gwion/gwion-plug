/* from paul batchemor lsys.c */

enum {
MODE_KEY,
MODE_VALUE,
MODE_ASSIGN,
MODE_AXIOM
};

typedef struct {
    int val;
    unsigned int start;
    unsigned int end;
} lsys_entry;

typedef struct {
    lsys_entry ent[37];
    unsigned int pos;
    int mode;
    int cur;
} lsys_d;

typedef struct lsys_item {
    lsys_entry *ent;
    struct lsys_item *nxt;
} lsys_item;

typedef struct {
    lsys_item root;
    lsys_item *last;
    unsigned int size;
} lsys_list;

int lsys_init(lsys_d *ls);
int lsys_list_destroy(lsys_list *lst);
unsigned int lsys_parse(lsys_d *ls, char *ref, char *str, unsigned int len);
int lsys_list_init(lsys_list *lst);
int lsys_make_list(lsys_d *lsys, lsys_list *lst, char *str, char c, int N);
unsigned int lsys_list_iter(lsys_list *lst, lsys_entry **ent, unsigned pos);
void lsys_list_reset(lsys_list *lst);
