#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	if(DATA_BYTE==2){
		cpu.esp=cpu.esp-2;
		MEM_W(cpu.esp, op_src->val,SS);
	}
	else{
		cpu.esp=cpu.esp-4;
		swaddr_write(cpu.esp,4,(DATA_TYPE)op_src->val,SS);
	}
//	MEM_R(cpu.esp, op_src->val);
	print_asm_template1();
}

#if DATA_BYTE==1

make_instr_helper(si)

#endif
#if DATA_BYTE==2 ||DATA_BYTE==4
make_instr_helper(i)
make_instr_helper(rm)
make_instr_helper(r)
//make_instr_helper(rm2r)

#endif

#include "cpu/exec/template-end.h"
