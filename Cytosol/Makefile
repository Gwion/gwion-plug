# this should point to the cytosol's source
CYTOSOL_DIR ?=Cytosol/

include ../config.mk

CFLAGS+=-I${CYTOSOL_DIR}/cytosol-c-ffi

# currently linking in a dirty way, but ld wanted to use the .so
LDFLAGS+=${CYTOSOL_DIR}/target/release/libcytosol_c_ffi.a

include ../config_post.mk
