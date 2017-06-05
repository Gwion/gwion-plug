#include "map.h"
// commented before doc move
// also , outputs rst (should be markdown.)
void operator_doc(Vector v, FILE* file)
{
#ifdef DEBUG_OPERATOR
  debug_msg(" op", "making doc");
#endif

  m_uint i;

  fprintf(file, "@itemize @bullet\n");
  for(i = 0; i < vector_size(v); i++) {
    M_Operator* mo = (M_Operator*)vector_at(v, i);
    fprintf(file, "@item (%s) '%s' %s '%s'\n", mo->ret->name, mo->lhs->name, op2str(i), mo->rhs->name);
    if(mo->doc)
      fprintf(file, "'%s'\n", mo->doc);
  }
  fprintf(file, "@end itemize\n");
}
