#ifndef BUILD_ON_WINDOWS
  #define _XOPEN_SOURCE 500
  #define _POSIX_C_SOURCE 200112L
  #include <glob.h>
  #include <dlfcn.h>
  #include <limits.h>
  #include <sys/stat.h>
  #include <stdio.h>
  #include <ftw.h>
  #include <libgen.h>
#endif
#include "unistd.h"
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "array.h"
#include "gwi.h"

static SFUN(core_glob) {
  const m_str name = STRING(*(M_Object*)MEM(0));
  const VM_Code code = *(VM_Code*)REG(SZ_INT);
#ifndef BUILD_ON_WINDOWS
  glob_t results;
  if (glob(name, 0, NULL, &results)) {
     *(M_Object*)RETURN = new_array(shred->info->mp, code->ret_type, 0);
    return;
  }
  const M_Object ret = *(M_Object*)RETURN = new_array(shred->info->mp, code->ret_type, results.gl_pathc);
  const M_Vector array = ARRAY(ret);
  for (m_uint i = 0; i < results.gl_pathc; i++) {
    const M_Object str = new_string(shred->info->vm->gwion, results.gl_pathv[i]);
    m_vector_set(array, i, &str);
  }
  globfree(&results);
#else
  const M_Object ret = new_array(shred->info->mp, code->ret_type, 0);
  WIN32_FIND_DATA filedata;
  HANDLE          file = FindFirstFile(name, &filedata);
  if (file == INVALID_HANDLE_VALUE)
    return;
  do {
    char c[PATH_MAX];
    strcpy(c, name);
    strcpy(c + strlen(name) - 4, filedata.cFileName);
    const M_Object str = new_string(shred->info->vm->gwion, c);
    m_vector_add(array, i, &str);
  } while (FindNextFile(file, &filedata));
  FindClose(file);
  *(M_Object*)RETURN = ret;
#endif
}

static SFUN(core_mv) {
  const m_str old = STRING(*(M_Object*)MEM(0));
  const m_str new = STRING(*(M_Object*)MEM(SZ_INT));
  *(m_int*)RETURN = !rename(old, new);
}

static SFUN(core_touch) {
  const m_str filename = STRING(*(M_Object*)MEM(0));
  FILE *f = fopen(filename, "a+");
  if(f) {
    fclose(f);
    *(m_uint*)RETURN = true;
  } else
    *(m_uint*)RETURN = false;
}

static SFUN(core_rm) {
  const m_str filename = STRING(*(M_Object*)MEM(0));
  *(m_int*)RETURN = !remove(filename);
}

static SFUN(core_rmdir) {
  const m_str filename = STRING(*(M_Object*)MEM(0));
#ifndef BUILD_ON_WINDOWS
  *(m_int*)RETURN = !rmdir(filename);
#else
  *(m_int*)RETURN = RemoveDirectoryA(filename);
#endif
}

static SFUN(core_envget) {
  const m_str varname = STRING(*(M_Object*)MEM(0));
  const m_str value = getenv(varname);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, value ?: "");
}

#ifdef BUILD_ON_WINDOWS
#define setenv(a,b,c) _putenv_s(a,b)
#endif
static SFUN(core_envset) {
  const m_str key = STRING(*(M_Object*)MEM(0));
  const m_str val = STRING(*(M_Object*)MEM(SZ_INT));
  *(m_uint*)RETURN = setenv(key, val, 1);
}
#ifdef BUILD_ON_WINDOWS
#undef setenv
#endif

#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

static SFUN(core_realpath) {
  const m_str path = realpath(STRING(*(M_Object*)REG(0)), NULL);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, path);
}

#ifndef BUILD_ON_WINDOWS

#define PATH_MAX_STRING_SIZE 1024
/* recursive mkdir */
int mkdir_p(const char *dir, const mode_t mode) {
    char tmp[PATH_MAX_STRING_SIZE];
    char *p = NULL;
    struct stat sb;
    size_t len = strlen (dir);
    if (len >PATH_MAX_STRING_SIZE) len = PATH_MAX_STRING_SIZE;
    if (len == 0 || len == PATH_MAX_STRING_SIZE)
        return -1;
    memcpy (tmp, dir, len);
    tmp[len] = '\0';
    if(tmp[len - 1] == '/')
      tmp[len - 1] = '\0';
    if (stat (tmp, &sb) == 0 && S_ISDIR (sb.st_mode))
      return 0;
    for(p = tmp + 1; *p; p++) {
        if(*p == '/') {
            *p = 0;
            if (stat(tmp, &sb) != 0) {
                if (mkdir(tmp, mode) < 0)
                  return -1;
            } else if (!S_ISDIR(sb.st_mode))
                return -1;
            *p = '/';
        }
    }
    if (stat(tmp, &sb) != 0) {
        if (mkdir(tmp, mode) < 0)
            return -1;
    } else if (!S_ISDIR(sb.st_mode))
        return -1;
    return 0;
}
#endif

static SFUN(core_mkdir) {
  const m_str dirname = STRING(*(M_Object*)MEM(0));
#ifndef BUILD_ON_WINDOWS
  *(m_uint*)RETURN = !mkdir_p(dirname, 0777);
#else
  *(m_uint*)RETURN = CreateDirectoryA(dirname, NULL);
#endif
}

#ifndef BUILD_ON_WINDOWS
ANN static int unlink_cb(const char *fpath NUSED, const struct stat *sb NUSED, int typeflag NUSED, struct FTW *ftwbuf NUSED) {
  const int rv = remove(fpath);
  if (rv)
    perror(fpath);
  return rv;
}
#else
ANN static inline BOOL IsDots(const TCHAR* str) {
    if(_tcscmp(str,".") && _tcscmp(str,"..")) return FALSE;
    return TRUE;
}

ANN static BOOL DeleteDirectory(const TCHAR* sPath) {
    HANDLE hFind;  // file handle
    WIN32_FIND_DATA FindFileData;

    TCHAR DirPath[MAX_PATH];
    TCHAR FileName[MAX_PATH];

    _tcscpy(DirPath,sPath);
    _tcscat(DirPath,"\\*");    // searching all files
    _tcscpy(FileName,sPath);
    _tcscat(FileName,"\\");

    hFind = FindFirstFile(DirPath,&FindFileData); // find the first file
    if(hFind == INVALID_HANDLE_VALUE) return FALSE;
    _tcscpy(DirPath,FileName);

    bool bSearch = true;
    while(bSearch) { // until we finds an entry
        if(FindNextFile(hFind,&FindFileData)) {
            if(IsDots(FindFileData.cFileName)) continue;
            _tcscat(FileName,FindFileData.cFileName);
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

                // we have found a directory, recurse
                if(!DeleteDirectory(FileName)) {
                    FindClose(hFind);
                    return FALSE; // directory couldn't be deleted
                }
                RemoveDirectory(FileName); // remove the empty directory
                _tcscpy(FileName,DirPath);
            }
            else {
                if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
                    _chmod(FileName, _S_IWRITE); // change read-only file mode
                if(!DeleteFile(FileName)) {  // delete the file
                    FindClose(hFind);
                    return FALSE;
                }
                _tcscpy(FileName,DirPath);
            }
        }
        else {
            if(GetLastError() == ERROR_NO_MORE_FILES) // no more files there
            bSearch = false;
            else {
                // some error occured, close the handle and return FALSE
                FindClose(hFind);
                return FALSE;
            }

        }

    }
    FindClose(hFind);  // closing file handle

    return RemoveDirectory(sPath); // remove the empty directory

}
#endif

static SFUN(core_rmdirr) {
  const m_str dirname = STRING(*(M_Object*)MEM(0));
#ifndef BUILD_ON_WINDOWS
  *(m_int*)RETURN = !nftw(dirname, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
#else
  *(m_int*)RETURN = DeleteDirectory(String, Boolean)
#endif
}

static SFUN(core_cp) {
  *(m_uint*)RETURN = false;
  FILE *fileIn = fopen(STRING(*(M_Object*)MEM(0)), "rb");
  if(!fileIn)
    return;
  FILE *fileOut = fopen(STRING(*(M_Object*)MEM(SZ_INT)), "wb");
  if (fileOut) {
    int ch;
    while ((ch=fgetc(fileIn))!=EOF)
	    fputc(ch, fileOut);
		fclose(fileOut);
  *(m_uint*)RETURN = true;
  }
  fclose(fileIn);
}

static SFUN(core_link) {
  const m_str old = STRING(*(M_Object*)MEM(0));
  const m_str new = STRING(*(M_Object*)MEM(SZ_INT));
#ifndef BUILD_ON_WINDOWS
  *(m_int*)RETURN = !link(old, new);
#else
  const DWORD attr = GetFileAttributesA(old);
  const DWORD flag = FileAttributes & FILE_ATTRIBUTE_DIRECTORY;
  *(m_int*)RETURN = CreateSymbolicLinkA(old, new, attr);
#endif
}

static SFUN(core_unlink) {
  const m_str filename = STRING(*(M_Object*)MEM(0));
#ifndef BUILD_ON_WINDOWS
  *(m_int*)RETURN = !unlink(filename);
#else
  *(m_int*)RETURN = !remove(filename);
#endif
}

static SFUN(core_dirname) {
  const m_str filename = STRING(*(M_Object*)MEM(0));
#ifndef BUILD_ON_WINDOWS
  const m_str dir = dirname(filename);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, dir);
#else
  xfun_handle(shred, "UnImplementedFunction");
#endif
}

static SFUN(core_separator) {
#ifndef BUILD_ON_WINDOWS
  *(m_int*)RETURN = '/';
#else
  *(m_int*)RETURN = '\\';
#endif
}

static SFUN(core_delimiter) {
#ifndef BUILD_ON_WINDOWS
  *(m_int*)RETURN = ':';
#else
  *(m_int*)RETURN = ';';
#endif
}

GWION_IMPORT(CoreUtil) {
  gwidoc(gwi, "Provide file system utilities");
  DECL_OB(const Type, t_coreutil, = gwi_struct_ini(gwi, "CoreUtil"));

  gwidoc(gwi, "list files using globbing");
  GWI_BB(gwi_func_ini(gwi, "string[]", "glob"));
  GWI_BB(gwi_func_arg(gwi, "string", "arg"));
  GWI_BB(gwi_func_end(gwi, core_glob, ae_flag_static));

  gwidoc(gwi, "rename a file");
  GWI_BB(gwi_func_ini(gwi, "bool", "mv"));
  GWI_BB(gwi_func_arg(gwi, "string", "old"));
  GWI_BB(gwi_func_arg(gwi, "string", "new"));
  GWI_BB(gwi_func_end(gwi, core_mv, ae_flag_static));

  gwidoc(gwi, "create a file");
  GWI_BB(gwi_func_ini(gwi, "bool", "touch"));
  GWI_BB(gwi_func_arg(gwi, "string", "file"));
  GWI_BB(gwi_func_end(gwi, core_touch, ae_flag_static));

  gwidoc(gwi, "remove a file");
  GWI_BB(gwi_func_ini(gwi, "bool", "rm"));
  GWI_BB(gwi_func_arg(gwi, "string", "file"));
  GWI_BB(gwi_func_end(gwi, core_rm, ae_flag_static));

  gwidoc(gwi, "remove a directory (must be empty)");
  GWI_BB(gwi_func_ini(gwi, "bool", "rmdir"));
  GWI_BB(gwi_func_arg(gwi, "string", "file"));
  GWI_BB(gwi_func_end(gwi, core_rmdir, ae_flag_static));

  gwidoc(gwi, "get an environment variable");
  GWI_BB(gwi_func_ini(gwi, "string", "env"));
  GWI_BB(gwi_func_arg(gwi, "string", "key"));
  GWI_BB(gwi_func_end(gwi, core_envget, ae_flag_static));

  gwidoc(gwi, "set an environment variable");
  GWI_BB(gwi_func_ini(gwi, "string", "env"));
  GWI_BB(gwi_func_arg(gwi, "string", "key"));
  GWI_BB(gwi_func_arg(gwi, "string", "val"));
  GWI_BB(gwi_func_end(gwi, core_envset, ae_flag_static));

  gwidoc(gwi, "returns the realpath from a file");
  GWI_BB(gwi_func_ini(gwi, "string", "realpath"));
  GWI_BB(gwi_func_arg(gwi, "string", "path"));
  GWI_BB(gwi_func_end(gwi, core_realpath, ae_flag_static));

  gwidoc(gwi, "create a directory");
  GWI_BB(gwi_func_ini(gwi, "bool", "mkdir"));
  GWI_BB(gwi_func_arg(gwi, "string", "dir"));
  GWI_BB(gwi_func_end(gwi, core_mkdir, ae_flag_static));

  gwidoc(gwi, "remove a directory recursively");
  GWI_BB(gwi_func_ini(gwi, "bool", "rmdirr"));
  GWI_BB(gwi_func_arg(gwi, "string", "dir"));
  GWI_BB(gwi_func_end(gwi, core_rmdirr, ae_flag_static));

  gwidoc(gwi, "copy a file");
  GWI_BB(gwi_func_ini(gwi, "int", "cp"));
  GWI_BB(gwi_func_arg(gwi, "string", "old"));
  GWI_BB(gwi_func_arg(gwi, "string", "new"));
  GWI_BB(gwi_func_end(gwi, core_cp, ae_flag_static));

  gwidoc(gwi, "link a file");
  GWI_BB(gwi_func_ini(gwi, "int", "link"));
  GWI_BB(gwi_func_arg(gwi, "string", "old"));
  GWI_BB(gwi_func_arg(gwi, "string", "new"));
  GWI_BB(gwi_func_end(gwi, core_link, ae_flag_static));

  gwidoc(gwi, "unlink a file");
  GWI_BB(gwi_func_ini(gwi, "int", "unlink"));
  GWI_BB(gwi_func_arg(gwi, "string", "file"))
  GWI_BB(gwi_func_end(gwi, core_unlink, ae_flag_static));

  gwidoc(gwi, "get the directory path of a file");
  GWI_BB(gwi_func_ini(gwi, "string", "dirname"));
  GWI_BB(gwi_func_arg(gwi, "string", "file"));
  GWI_BB(gwi_func_end(gwi, core_dirname, ae_flag_static));

  gwidoc(gwi, "get the path separator depending on os");
  GWI_BB(gwi_func_ini(gwi, "char", "separator"));
  GWI_BB(gwi_func_end(gwi, core_separator, ae_flag_static));

  gwidoc(gwi, "get the path delimiter depending on os");
  GWI_BB(gwi_func_ini(gwi, "char", "delimiter"));
  GWI_BB(gwi_func_end(gwi, core_delimiter, ae_flag_static));

  GWI_BB(gwi_struct_end(gwi));
  return GW_OK;
}
