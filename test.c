/*#include <stdio.h>
#include <string.h>
#include <libelf.h>
#include <gelf.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s ELF_FILE\n", argv[0]);
    return 1;
  }

  char* elf_file = argv[1];

  // Initialize libelf
  elf_version(EV_CURRENT);

  // Open the ELF file
  int fd = open(elf_file, O_RDONLY);
  Elf* elf = elf_begin(fd, ELF_C_READ, NULL);
  if (elf == NULL) {
    fprintf(stderr, "Error: %s is not a valid ELF file.\n", elf_file);
    return 1;
  }

  // Get the symbol table
  Elf_Scn* scn = NULL;
  GElf_Shdr shdr;
  while ((scn = elf_nextscn(elf, scn)) != NULL) {
    gelf_getshdr(scn, &shdr);
    if (shdr.sh_type == SHT_SYMTAB) {
      break;
    }
  }
  if (scn == NULL) {
    fprintf(stderr, "Error: ELF file does not contain a symbol table.\n");
    return 1;
  }

  // Iterate over the symbols in the symbol table
  Elf_Data* data = NULL;
  GElf_Sym sym;
  while ((data = elf_getdata(scn, data)) != NULL) {
    int count = data->d_size / shdr.sh_entsize;
    for (int i = 0; i < count; i++) {
      gelf_getsym(data, i, &sym);
      printf("%s: 0x%llx\n", elf_strptr(elf, shdr.sh_link, sym.st_name),
             (long long)sym.st_value);
    }
  }

  // Clean up
  elf_end(elf);
  close(fd);
  return 0;
}*/






#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s ELF_FILE\n", argv[0]);
    return 1;
  }

  char* elf_file = argv[1];

  // Open the ELF file
  int fd = open(elf_file, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }

  // Read the ELF header
  Elf32_Ehdr ehdr;
  if (read(fd, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
    perror("read");
    return 1;
  }

  // Check the ELF magic number
  if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
    fprintf(stderr, "Error: %s is not a valid ELF file.\n", elf_file);
    return 1;
  }

  // Check the ELF class (32-bit or 64-bit)
  if (ehdr.e_ident[EI_CLASS] == ELFCLASS32) {
    // 32-bit ELF
    Elf32_Shdr shdr;
    for (int i = 0; i < ehdr.e_shnum; i++) {
      // Seek to the section header table entry
      lseek(fd, ehdr.e_shoff + i * ehdr.e_shentsize, SEEK_SET);

      // Read the section header
      if (read(fd, &shdr, sizeof(shdr)) != sizeof(shdr)) {
        perror("read");
        return 1;
      }

      // Check if this is a symbol table
      if (shdr.sh_type == SHT_SYMTAB) {
        // Read the symbol table
        Elf32_Sym sym;
        lseek(fd, shdr.sh_offset, SEEK_SET);
        for (int j = 0; j < shdr.sh_size / shdr.sh_entsize; j++) {
          if (read(fd, &sym, sizeof(sym)) != sizeof(sym)) {
            perror("read");
            return 1;
          }

          // Print the symbol's name and value
          printf("%d: 0x%x\n", sym.st_name, sym.st_value);
        }
      }
    }
  } else if (ehdr.e_ident[EI_CLASS] == ELFCLASS64) {
    // 64-bit ELF
    Elf64_Shdr shdr;
    for (int i = 0; i < ehdr.e_shnum; i++) {
      // Seek to the section header table entry
      lseek(fd, ehdr.e_shoff + i * ehdr.e_shentsize, SEEK_SET);

      // Read the section header
      if (read(fd, &shdr, sizeof(shdr)) != sizeof(shdr)) {
        perror("read");
        return 1;
      }

      // Check if this is a symbol table
      if (shdr.sh_type == SHT_SYMTAB) {
        // Read the symbol table
        Elf32_Sym sym;
        lseek(fd, shdr.sh_offset, SEEK_SET);
        for (int j = 0; j < shdr.sh_size / shdr.sh_entsize; j++) {
          if (read(fd, &sym, sizeof(sym)) != sizeof(sym)) {
            perror("read");
            return 1;
          }

          // Print the symbol's name and value
          printf("%d: 0x%x\n", sym.st_name, sym.st_value);
        }
      }
    }
  }
}
   
/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

int main(int argc, char** argv) {
if (argc < 2) {
fprintf(stderr, "Usage: %s ELF_FILE\n", argv[0]);
return 1;
}

char* elf_file = argv[1];

// Open the ELF file
FILE* fp = fopen(elf_file, "r");
if (fp == NULL) {
perror("fopen");
return 1;
}

// Read the ELF header
Elf32_Ehdr ehdr;
if (fread(&ehdr, sizeof(ehdr), 1, fp) != 1) {
perror("fread");
return 1;
}

// Check the ELF magic number
if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
fprintf(stderr, "Error: %s is not a valid ELF file.\n", elf_file);
return 1;
}

// Check the ELF class (32-bit or 64-bit)
if (ehdr.e_ident[EI_CLASS] == ELFCLASS32) {
// 32-bit ELF
Elf32_Shdr shdr;
for (int i = 0; i < ehdr.e_shnum; i++) {
// Seek to the section header table entry
fseek(fp, ehdr.e_shoff + i * ehdr.e_shentsize, SEEK_SET);

// Read the section header
  if (fread(&shdr, sizeof(shdr), 1, fp) != 1) {
    perror("fread");
    return 1;
  }

  if (shdr.sh_type == SHT_SYMTAB) {
    // This is a symbol table section
    break;
  }
}
if (shdr.sh_type != SHT_SYMTAB) {
  fprintf(stderr, "Error: ELF file does not contain a symbol table.\n");
  return 1;
}

// Iterate over the symbols in the symbol table
Elf32_Sym sym;
for (int i = 0; i < shdr.sh_size / shdr.sh_entsize; i++) {
  // Seek to the symbol table entry
  fseek(fp, shdr.sh_offset + i * shdr.sh_entsize, SEEK_SET);

  // Read the symbol table entry
  if (fread(&sym, sizeof(sym), 1, fp) != 1) {
    perror("fread");
    return 1;
  }

  printf("%d: 0x%x\n", sym.st_name, sym.st_value);
}

} else if (ehdr.e_ident[EI_CLASS] == ELFCLASS64) {
// 64-bit ELF
Elf64_Shdr shdr;
for (int i = 0; i < ehdr.e_shnum; i++) {
// Seek to the section header table entry
fseek(fp, ehdr.e_shoff + i * ehdr.e_shentsize, SEEK_SET);

 // Read the section header
  if (fread(&shdr, sizeof(shdr), 1, fp) != 1) {
    perror("fread");
    return 1;
  }

  if (shdr.sh_type == SHT_SYMTAB) {
    // This is a symbol table section
    break;
  }
}
if (shdr.sh_type != SHT_SYMTAB) {
  fprintf(stderr, "Error: ELF file does not contain a symbol table.\n");
  return 1;
}

// Iterate over the symbols in the symbol table
Elf64_Sym sym;
for (int i = 0; i < shdr.sh_size / shdr.sh_entsize; i++) {
  // Seek to the symbol table entry
  fseek(fp, shdr.sh_offset + i * shdr.sh_entsize, SEEK_SET);

  // Read the symbol table entry
  if (fread(&sym, sizeof(sym), 1, fp) != 1) {
    perror("fread");
    return 1;
  }

  printf("%d: 0x%lx\n", sym.st_name, sym.st_value);
}

} else {
fprintf(stderr, "Error: %s is not a 32-bit or 64-bit ELF file.\n",
elf_file);
return 1;
}

// Close the ELF file
fclose(fp);

return 0;
}*/