#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];

#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;

	nemu_assert(*p_magic == elf_magic);
	//TODO:tmp
	nemu_assert(sizeof(Elf32_Phdr) == elf->e_phentsize);
	nemu_assert(3 == elf->e_phnum);

	uint32_t i = 0;
	//ph = malloc(elf->e_phentsize);
	/* Load each program segment */
	for(; i < elf->e_phnum; i++) {
		/* Scan the program header table, load each segment into memory */
		//ramdisk_read(ph, elf->e_phoff, elf->e_phentsize);
		ph = (void *)(elf + elf->e_phoff + i * elf->e_phentsize);
		uint8_t tmp_buf[1024];
		if(ph->p_type == PT_LOAD) {

			/* TODO: read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			ramdisk_read(tmp_buf, ph->p_offset, ph->p_filesz);
			memset((void*)ph->p_vaddr, 0, ph->p_memsz);
			memcpy((void*)ph->p_vaddr, tmp_buf, ph->p_filesz);
			 
			/* TODO: zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */


#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t cur_brk, max_brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(cur_brk < new_brk) { max_brk = cur_brk = new_brk; }
#endif
		}
	}

	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
#endif

	write_cr3(get_ucr3());
#endif
	return entry;
}
