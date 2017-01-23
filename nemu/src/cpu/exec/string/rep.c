#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(rep) {
	int len;
	int count = 0;
	if(instr_fetch(eip + 1, 1) == 0xc3) {
		/* repz ret */
		exec(eip + 1);
		len = 0;
	}
	else {
		while(cpu.ecx) {
			exec(eip + 1);
			count ++;
			cpu.ecx --;
			//Log("cpu.ecx = %#x\n", cpu.ecx);
			//Log("ops_decoded.opcode == %#x\n", ops_decoded.opcode);
			assert(ops_decoded.opcode == 0xa4	// movsb
				|| ops_decoded.opcode == 0xa5	// movsw
				|| ops_decoded.opcode == 0xaa	// stosb
				|| ops_decoded.opcode == 0xab	// stosw
				|| ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
				);

			/* TODO: Jump out of the while loop if necessary. */
			Assert(cpu.ecx < 0x10000000, "ecx to large(>10000000)");

		}
		len = 1;
	}

#ifdef DEBUG
	char temp[80];
	sprintf(temp, "rep %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif
	
	return len + 1;
}

make_helper(repnz) {
	int count = 0;
	while(cpu.ecx) {
		exec(eip + 1);
		count ++;
		cpu.ecx --;
		assert(ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
			  );

		//Log("cpu.ecx = %d\n", cpu.ecx);
		if (cpu._zf == 1)
			break;
	}

#ifdef DEBUG
	char temp[80];
	sprintf(temp, "repnz %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif

	return 1 + 1;
}
