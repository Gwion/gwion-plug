#include "defs.h"
#include "type.h"
#include "lang.h"
#include "import.h"

#include "csv.h"

#define CSV(o) (*(CSV_BUFFER **)(o->d.data + o_csv_buffer))

static m_int o_csv_buffer;
static struct Type_ t_csv = { "Csv", SZ_INT, &t_object };

CTOR(csv_ctor) {
	CSV(o) = csv_create_buffer();
}

DTOR(csv_dtor) {
	csv_destroy_buffer(CSV(o));
}

static MFUN(gw_csv_load) {
	M_Object arg = *(M_Object*)(shred->reg + SZ_INT);
	RETURN->d.v_uint = csv_load(CSV(o), STRING(arg));
}

static MFUN(gw_csv_save) {
	M_Object arg = *(M_Object*)(shred->reg + SZ_INT);
	RETURN->d.v_uint = csv_save(STRING(arg), CSV(o));
}

static MFUN(gw_csv_copy_row) {
	m_int src_row = *(m_int*)   (shred->reg + SZ_INT);
	M_Object dst  = *(M_Object*)(shred->reg + SZ_INT*2);
	m_int dst_row = *(m_int*)   (shred->reg + SZ_INT*3);
	RETURN->d.v_uint = csv_copy_row(CSV(dst), dst_row, CSV(o), src_row);
	release(dst, shred);
}

static MFUN(gw_csv_copy_field) {
	m_int src_row = *(m_int*)   (shred->reg + SZ_INT);
	m_int src_ent = *(m_int*)   (shred->reg + SZ_INT*2);
	M_Object dst  = *(M_Object*)(shred->reg + SZ_INT*3);
	m_int dst_row = *(m_int*)   (shred->reg + SZ_INT*4);
	m_int dst_ent = *(m_int*)   (shred->reg + SZ_INT*4);
	RETURN->d.v_uint = csv_copy_field(CSV(dst), dst_row, dst_ent, CSV(o), src_row, src_ent);
	release(dst, shred);
}

// CHECK ME get field
static MFUN(gw_csv_get_field) {
	char dest[128];
	int ret = csv_get_field(dest, 128, CSV(o), *(m_int*)(shred->reg + SZ_INT), *(m_int*)(shred->reg + SZ_INT*2));
	RETURN->d.v_uint = !ret ? (m_uint)new_String(shred, dest) : 0;
}

static MFUN(gw_csv_clear_field) {
	m_int row = *(m_int*)   (shred->reg + SZ_INT);
    m_int ent = *(m_int*)   (shred->reg + SZ_INT*2);
	RETURN->d.v_uint = csv_clear_field(CSV(o), row, ent);
}

static MFUN(gw_csv_clear_row) {
	RETURN->d.v_uint = csv_clear_row(CSV(o), *(m_int*)(shred->reg + SZ_INT));
}

static MFUN(gw_csv_remove_row) {
	RETURN->d.v_uint = csv_remove_row(CSV(o), *(m_int*)(shred->reg + SZ_INT));
}

static MFUN(gw_csv_get_height) {
	RETURN->d.v_uint = csv_get_height(CSV(o));
}

static MFUN(gw_csv_get_width) {
	RETURN->d.v_uint = csv_get_width(CSV(o), *(m_int*)(shred->reg + SZ_INT));
}

static MFUN(gw_csv_get_field_length) {
    m_int row = *(m_int*)   (shred->reg + SZ_INT);
    m_int ent = *(m_int*)   (shred->reg + SZ_INT*2);
    RETURN->d.v_uint = csv_get_field_length(CSV(o), row, ent);
}

static MFUN(gw_csv_set_field) {
    m_int    row = *(m_int*)   (shred->reg + SZ_INT);
    m_int    ent = *(m_int*)   (shred->reg + SZ_INT*2);
	M_Object arg = *(M_Object*)(shred->reg + SZ_INT*3);
    RETURN->d.v_uint = csv_set_field(CSV(o), row, ent, STRING(arg));
}

static MFUN(gw_csv_insert_field) {
    m_int    row = *(m_int*)   (shred->reg + SZ_INT);
    m_int    ent = *(m_int*)   (shred->reg + SZ_INT*2);
	M_Object arg = *(M_Object*)(shred->reg + SZ_INT*3);
    RETURN->d.v_uint = csv_insert_field(CSV(o), row, ent, STRING(arg));
}

static MFUN(gw_csv_set_text_delim) {
	csv_set_text_delim(CSV(o), *(m_int*)(shred->reg + SZ_INT));
}

static MFUN(gw_csv_set_field_delim) {
	csv_set_field_delim(CSV(o), *(m_int*)(shred->reg + SZ_INT));
}

IMPORT{
	DL_Func fun;

	// start import
	import_class_begin(env, &t_csv, env->curr, csv_ctor, csv_dtor);

	CHECK_BB((o_csv_buffer = import_var(env, "int", "@x",   0, NULL)))

	// import functions
	dl_func_init(&fun, "int", "load", (m_uint)gw_csv_load);
		dl_func_add_arg(&fun, "string", "filename");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "save", (m_uint)gw_csv_save);
		dl_func_add_arg(&fun, "string", "filename");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "copy", (m_uint)gw_csv_copy_row);
		dl_func_add_arg(&fun, "int", "src_row");
		dl_func_add_arg(&fun, "Csv", "dst");
		dl_func_add_arg(&fun, "int", "dst_row");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "copy", (m_uint)gw_csv_copy_field);
		dl_func_add_arg(&fun, "int", "src_row");
		dl_func_add_arg(&fun, "int", "src_ent");
		dl_func_add_arg(&fun, "Csv", "dst");
		dl_func_add_arg(&fun, "int", "dst_row");
		dl_func_add_arg(&fun, "int", "dst_ent");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "string", "get", (m_uint)gw_csv_get_field);
		dl_func_add_arg(&fun, "int", "src_row");
		dl_func_add_arg(&fun, "int", "src_ent");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "clear",  (m_uint)gw_csv_clear_field);
		dl_func_add_arg(&fun, "int",    "row");
		dl_func_add_arg(&fun, "int",    "ent");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "clear",  (m_uint)gw_csv_remove_row);
		dl_func_add_arg(&fun, "int",    "row");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "clear",  (m_uint)gw_csv_clear_row);
		dl_func_add_arg(&fun, "int",    "row");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "height", (m_uint)gw_csv_get_height);
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "width", (m_uint)gw_csv_get_width);
		dl_func_add_arg(&fun, "int",   "row");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "length", (m_uint)gw_csv_get_field_length);
		dl_func_add_arg(&fun, "int",    "row");
		dl_func_add_arg(&fun, "int",    "ent");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "set",    (m_uint)gw_csv_set_field);
		dl_func_add_arg(&fun, "int",    "row");
		dl_func_add_arg(&fun, "int",    "ent");
		dl_func_add_arg(&fun, "string", "field");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "int", "insert", (m_uint)gw_csv_insert_field);
		dl_func_add_arg(&fun, "int",    "row");
		dl_func_add_arg(&fun, "int",    "ent");
		dl_func_add_arg(&fun, "string", "field");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "void", "text_delim", (m_uint)gw_csv_set_text_delim);
		dl_func_add_arg(&fun, "int",    "delim");
	CHECK_BB(import_fun(env, &fun, 0))

	dl_func_init(&fun, "void", "field_delim", (m_uint)gw_csv_set_field_delim);
		dl_func_add_arg(&fun, "int",    "delim");
	CHECK_BB(import_fun(env, &fun, 0))

	// end import
	import_class_end(env);
	return 1;
}
