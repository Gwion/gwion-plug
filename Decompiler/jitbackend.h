//typedef struct JitBackend;

struct JitBackend_* jb_ini(const Gwion);
void jb_run(struct JitBackend_*, const VM_Code);
void jb_end(const Gwion, struct JitBackend_*);
