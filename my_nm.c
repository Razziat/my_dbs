#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <elf.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    FILE *fd;
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s Elf_file\n", argv[0]);
        return 1;
    }

    fd = fopen(argv[1], "rb");
    if (fd == NULL)
    {
        fprintf(stderr, "Cannot open file %s\n", argv[1]);
        return 1;
    }

    int fp = open(argv[1], O_RDONLY);

    char *magic_number;
    long fsize;
    fsize = ftell(fd);

    magic_number=(char *)malloc(fsize);
    fread(magic_number, sizeof(char), 4, fd);

    if (magic_number[0] == 0x7f && magic_number[1] == 'E' && magic_number[2] == 'L' && magic_number[3] == 'F') {
        printf("File is an ELF file:%x \n", magic_number[0]);
        //return 0;
    } else {
        printf("File is not an ELF file\n");
        //return 0;
    }

    struct stat st;
    stat(argv[1], &st);
    int size = st.st_size;
    void *map_start = mmap(0, size, PROT_READ | PROT_WRITE , MAP_PRIVATE, fp, 0);
    Elf64_Ehdr *header = NULL;
    header = (Elf64_Ehdr *) map_start;
    Elf64_Shdr *sections = (Elf64_Shdr *)((char *)map_start + header->e_shoff);
    Elf64_Sym *symtab = NULL;

    int size2 = 0;
    for (int i = 0; i < header->e_shnum; i++)
    {
        if (sections[i].sh_type == SHT_SYMTAB)
        {
            symtab = (Elf64_Sym *)((char *)map_start + sections[i].sh_offset);
            size2 = sections[i].sh_size / sections[i].sh_entsize;
            //printf("%c, %d\n", symtab->st_name, size2);
            break; 
        }
    }

    for(int i = 0; i <= size2 + 1000; i++)
    {
        printf("%lx, ", symtab[i].st_value);
    }
    
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
       
        /*Elf64_Shdr *shdr;
        for (int i = 0; i < shdr->e_shnum; i++)
        {
            // Seek to the section header table entry
            fseek(fd, shdr->sh_offset + i * shdr->sh_entsize, SEEK_SET);
            if (fread(&shdr, sizeof(shdr), 1, fd) != 1)
            {
                perror("fread");
                return 1;
            }
            if (shdr->sh_type == SHT_SYMTAB)
            {
                // This is a symbol table section
                break;
            }
        }

        if (shdr->sh_type != SHT_SYMTAB)
        {
            fprintf(stderr, "Error: ELF file does not contain a symbol table.\n");
            return 1;
        }

        // Read the string table section containing the symbol names
        Elf64_Shdr *strtab_shdr;
        fseek(fd, shdr->sh_offset + shdr->sh_link * shdr->sh_entsize, SEEK_SET);
        if (fread(&strtab_shdr, sizeof(strtab_shdr), 1, fd) != 1) 
        {
            perror("fread");
            return 1;
        }

        // Iterate over the symbols in the symbol table
        Elf64_Sym sym;
        for (int i = 0; i < (int)(shdr->sh_size / shdr->sh_entsize); i++)
        {
            // Seek to the symbol table entry
            fseek(fd, shdr->sh_offset + i * shdr->sh_entsize, SEEK_SET);

            // Read the symbol table entry
            if (fread(&sym, sizeof(sym), 1, fd) != 1)
            {
                perror("fread");
                return 1;
            }

            // Read the symbol name from the string table
            char sym_name[256];
            fseek(fd, strtab_shdr->sh_offset + sym.st_name, SEEK_SET);
            if (fgets(sym_name, sizeof(sym_name), fd) == NULL)
            {
                perror("fgets");
                return 1;
            }

            sym_name[strcspn(sym_name, "\n")] = '\0';
            printf("début\n");
            printf("%s: 0x%llx\n", sym_name, (long long)sym.st_value);
            printf("fin\n");
        }
    printf("réussi\n");*/
    fclose(fd);
    return 0;
}