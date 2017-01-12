#include "common.h"
#include <stdlib.h>
#include <elf.h>

char *exec_file = NULL;

static char *strtab = NULL;
static Elf32_Sym *symtab = NULL;
static int nr_symtab_entry;
static int nr_objtab_entry;
static struct objtab {
	swaddr_t addr;
	char *name;
}*objtab;

void load_elf_tables(int argc, char *argv[]) {
	int ret;
	Assert(argc == 2, "run NEMU with format 'nemu [program]'");
	exec_file = argv[1];

	//Log("exec_file = %s\n", exec_file);

	FILE *fp = fopen(exec_file, "rb");
	Assert(fp, "Can not open '%s'", exec_file);

	uint8_t buf[sizeof(Elf32_Ehdr)];
	ret = fread(buf, sizeof(Elf32_Ehdr), 1, fp);
	assert(ret == 1);

	/* The first several bytes contain the ELF header. */
	Elf32_Ehdr *elf = (void *)buf;
	char magic[] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	/* Check ELF header */
	assert(memcmp(elf->e_ident, magic, 4) == 0);		// magic number
	assert(elf->e_ident[EI_CLASS] == ELFCLASS32);		// 32-bit architecture
	assert(elf->e_ident[EI_DATA] == ELFDATA2LSB);		// littel-endian
	assert(elf->e_ident[EI_VERSION] == EV_CURRENT);		// current version
	assert(elf->e_ident[EI_OSABI] == ELFOSABI_SYSV || 	// UNIX System V ABI
			elf->e_ident[EI_OSABI] == ELFOSABI_LINUX); 	// UNIX - GNU
	assert(elf->e_ident[EI_ABIVERSION] == 0);			// should be 0
	assert(elf->e_type == ET_EXEC);						// executable file
	assert(elf->e_machine == EM_386);					// Intel 80386 architecture
	assert(elf->e_version == EV_CURRENT);				// current version

	objtab = malloc(4 * sizeof(struct objtab));

	/* Load symbol table and string table for future use */

	/* Load section header table */
	uint32_t sh_size = elf->e_shentsize * elf->e_shnum;
	Elf32_Shdr *sh = malloc(sh_size);
	fseek(fp, elf->e_shoff, SEEK_SET);
	ret = fread(sh, sh_size, 1, fp);
	assert(ret == 1);

	/* Load section header string table */
	char *shstrtab = malloc(sh[elf->e_shstrndx].sh_size);
	fseek(fp, sh[elf->e_shstrndx].sh_offset, SEEK_SET);
	ret = fread(shstrtab, sh[elf->e_shstrndx].sh_size, 1, fp);
	assert(ret == 1);

	int i;
	for(i = 0; i < elf->e_shnum; i ++) {
		if(sh[i].sh_type == SHT_SYMTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".symtab") == 0) {
			/* Load symbol table from exec_file */
			symtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(symtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
			nr_symtab_entry = sh[i].sh_size / sizeof(symtab[0]);
		}
		else if(sh[i].sh_type == SHT_STRTAB && 
				strcmp(shstrtab + sh[i].sh_name, ".strtab") == 0) {
			/* Load string table from exec_file */
			strtab = malloc(sh[i].sh_size);
			fseek(fp, sh[i].sh_offset, SEEK_SET);
			ret = fread(strtab, sh[i].sh_size, 1, fp);
			assert(ret == 1);
		}
	}
	free(sh);
	free(shstrtab);

	//TODO:optimize this
	objtab = malloc(nr_symtab_entry * sizeof(struct objtab));
	int j;
	for (j = 0;j < nr_symtab_entry; j++) {
		//Log("name offset = %#x\n", symtab[j].st_name);
		//Log("info = %#x\n", symtab[j].st_info);
		//Log("name = %s\n", &strtab[symtab[j].st_name]);

		if (symtab[j].st_info == 0x11) {
			objtab[nr_objtab_entry].name = (strtab + symtab[j].st_name);
			objtab[nr_objtab_entry].addr = symtab[j].st_value;
			//Log("objtab[nr_objtab_entry].name = %s\n", objtab[nr_objtab_entry].name);
			//Log("objtab[nr_objtab_entry].addr = %#x\n", objtab[nr_objtab_entry].addr);
			nr_objtab_entry++;
		}
	}		
	assert(strtab != NULL && symtab != NULL);
	fclose(fp);
}

swaddr_t sym_eval(char* sym) {
	int i = 0;
	for(; i < nr_objtab_entry; i++) 
		if (strcmp(objtab[i].name, sym) == 0)
			return objtab[i].addr;
	Warn("Incorrect var name!\n");
	return 0;
}
