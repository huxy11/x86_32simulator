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

uint8_t tmp_buf[1000];
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
	uint32_t i = 0;
	//ph = malloc(elf->e_phentsize);
	/* Load each program segment */
	for(; i < elf->e_phnum; i++) {
		/* Scan the program header table, load each segment into memory */
		//ph = (void *)(elf + elf->e_phoff + i * elf->e_phentsize); 
		ph = (void *)(buf + elf->e_phoff + i * elf->e_phentsize);
		//nemu_assert(ph->p_vaddr == 0x800000);
		//nemu_assert(ph->p_memsz == 0x0066c);
		//nemu_assert(ph->p_filesz == 0x0066c);

				
		if(ph->p_type == PT_LOAD) {
			//nemu_assert(*p_magic == elf_magic);
			//nemu_assert(ph->p_vaddr == 0x800000);
			//nemu_assert(ph->p_memsz == 0x0066c);
			//nemu_assert(ph->p_filesz == 0x0066c);
	
			int offset = ph->p_offset, filesz = ph->p_filesz;
			ramdisk_read(tmp_buf, offset, filesz);
			//ramdisk_read(tmp_buf, ph->p_offset, ph->p_filesz);
			
		//	nemu_assert(*p_magic == elf_magic);
		
			//nemu_assert(ph->p_vaddr == 0x800000);
			//nemu_assert(ph->p_memsz == 0x0066c);
			//nemu_assert(ph->p_filesz == 0x0066c);
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
