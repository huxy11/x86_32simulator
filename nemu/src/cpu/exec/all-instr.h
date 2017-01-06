#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/movzx.h"
#include "data-mov/movsx.h"

#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"

#include "logic/and.h"
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"

#include "stack/push.h"
#include "stack/pop.h"

#include "call/call.h"
#include "call/ret.h"
#include "leave/leave.h"

#include "test/test.h"

#include "jcc/je.h"
#include "jcc/js.h"
#include "jcc/jns.h"
#include "jcc/jne.h"
#include "jcc/jbe.h"
#include "jcc/jle.h"
#include "jcc/ja.h"
#include "jcc/jg.h"
#include "jcc/jl.h"
#include "jcc/jge.h"
#include "jmp/jmp.h"

#include "cmp/cmp.h"

#include "sub/sub.h"
#include "sub/sbb.h"
#include "add/add.h"
#include "add/adc.h"

#include "lea/lea.h"

#include "nop/nop.h"

#include "set/setne.h"
#include "set/sete.h"

#include "cltd/cltd.h"

#include "stos/stos.h"

#include "rep/rep.h"

#include "scas/scas.h"

