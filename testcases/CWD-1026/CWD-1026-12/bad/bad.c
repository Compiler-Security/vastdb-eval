#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Standard macros */
#ifndef NULL
#define NULL ((void *)0)
#endif
#define TRUE 1
#define FALSE 0

/* BFD types */
typedef unsigned long long bfd_vma;
typedef int bfd_boolean;
typedef unsigned long long bfd_size_type;
typedef long long file_ptr;

/* ELF internal types */
typedef struct elf_internal_shdr {
  unsigned int sh_name;
  unsigned int sh_type;
  bfd_vma sh_flags;
  bfd_vma sh_addr;
  file_ptr sh_offset;
  bfd_size_type sh_size;
  unsigned int sh_link;
  unsigned int sh_info;
  bfd_vma sh_addralign;
  bfd_size_type sh_entsize;
  void *bfd_section;
  unsigned char *contents;
} Elf_Internal_Shdr;

typedef struct elf_internal_sym {
  bfd_vma st_value;
  bfd_vma st_size;
  unsigned long st_name;
  unsigned char st_info;
  unsigned char st_other;
  unsigned char st_target_internal;
  unsigned int st_shndx;
} Elf_Internal_Sym;

typedef struct elf_internal_rela {
  bfd_vma r_offset;
  bfd_vma r_info;
  bfd_vma r_addend;
} Elf_Internal_Rela;

typedef struct elf_internal_chdr {
  unsigned int ch_type;
  bfd_size_type ch_size;
  bfd_vma ch_addralign;
} Elf_Internal_Chdr;

typedef struct {
  unsigned char ch_type[4];
  unsigned char ch_size[4];
  unsigned char ch_addralign[4];
} Elf32_External_Chdr;

typedef struct {
  unsigned char ch_type[4];
  unsigned char ch_reserved[4];
  unsigned char ch_size[8];
  unsigned char ch_addralign[8];
} Elf64_External_Chdr;

typedef struct elf_internal_ehdr {
  unsigned char e_ident[16];
  bfd_vma e_entry;
  bfd_size_type e_phoff;
  bfd_size_type e_shoff;
  unsigned long e_version;
  unsigned long e_flags;
  unsigned short e_type;
  unsigned short e_machine;
  unsigned int e_ehsize;
  unsigned int e_phentsize;
  unsigned int e_phnum;
  unsigned int e_shentsize;
  unsigned int e_shnum;
  unsigned int e_shstrndx;
} Elf_Internal_Ehdr;

typedef struct elf_section_list {
  Elf_Internal_Shdr *hdr;
  struct elf_section_list *next;
} elf_section_list;

typedef struct elf_internal_syminfo {
  unsigned short si_boundto;
  unsigned short si_flags;
} Elf_Internal_Syminfo;

/* Dwarf types */
typedef unsigned long long dwarf_vma;
typedef long long dwarf_signed_vma;
typedef unsigned long long dwarf_size_type;

enum dwarf_section_display_enum {
  abbrev = 0,
  aranges,
  frame,
  info,
  line,
  pubnames,
  gnu_pubnames,
  eh_frame,
  macinfo,
  macro,
  str,
  loc,
  pubtypes,
  gnu_pubtypes,
  ranges,
  static_func,
  static_vars,
  types,
  weaknames,
  gdb_index,
  trace_info,
  trace_abbrev,
  trace_aranges,
  info_dwo,
  abbrev_dwo,
  types_dwo,
  line_dwo,
  loc_dwo,
  macro_dwo,
  macinfo_dwo,
  str_dwo,
  str_index,
  str_index_dwo,
  debug_addr,
  dwp_cu_index,
  dwp_tu_index,
  max
};

struct dwarf_section {
  const char *uncompressed_name;
  const char *compressed_name;
  const char *name;
  unsigned char *start;
  dwarf_vma address;
  dwarf_size_type size;
  enum dwarf_section_display_enum abbrev_sec;
  void *reloc_info;
  unsigned long num_relocs;
  void *user_data;
};

struct dwarf_section_display {
  struct dwarf_section section;
  int relocate;
  int (*display)(struct dwarf_section *, void *);
};

/* ELF macros */
#define EM_PARISC 15
#define EM_SH 42
#define EM_D30V 86
#define EM_MN10300 89
#define EM_PJ 91
#define EM_XTENSA 94
#define EM_MSP430 105
#define EM_RL78 197
#define EM_PJ_OLD 99
#define EM_MSP430_OLD 0x1059
#define EM_CYGNUS_D30V 0x7676
#define EM_CYGNUS_MN10300 0xbeef

#define ET_NONE 0        /* No file type */
#define ET_REL 1         /* Relocatable file */
#define ET_EXEC 2        /* Executable file */
#define ET_DYN 3         /* Shared object file */
#define ET_CORE 4        /* Core file */
#define ET_LOOS 0xFE00   /* Operating system-specific */
#define ET_HIOS 0xFEFF   /* Operating system-specific */
#define ET_LOPROC 0xFF00 /* Processor-specific */
#define ET_HIPROC 0xFFFF /* Processor-specific */

#define SHT_SYMTAB 2
#define SHT_RELA 4
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_DYNSYM 11

#define SHF_COMPRESSED (1 << 11)

#define ELFCOMPRESS_ZLIB 1

#define STT_SECTION 3
#define STT_COMMON 5

#define ELF_ST_BIND(val) (((unsigned int)(val)) >> 4)
#define ELF_ST_TYPE(val) ((val) & 0xF)

#define ELF32_R_SYM(i) ((i) >> 8)
#define ELF64_R_SYM(i) ((i) >> 32)

/* Dump flags */
#define HEX_DUMP (1 << 0)
#define DISASS_DUMP (1 << 1)
#define DEBUG_DUMP (1 << 2)
#define STRING_DUMP (1 << 3)
#define RELOC_DUMP (1 << 4)

typedef unsigned char dump_type;

/* String comparison macros */
#define streq(a, b) (strcmp((a), (b)) == 0)
#define const_strneq(a, b) (strncmp((a), (b), sizeof(b) - 1) == 0)

/* Internationalization */
#define _(String) (String)

/* External variables */
extern int is_32bit_elf;
extern Elf_Internal_Ehdr elf_header;
extern Elf_Internal_Shdr *section_headers;
extern struct dwarf_section_display debug_displays[];
extern unsigned int *section_subset;
extern int do_dump;
extern unsigned int num_dump_sects;
extern dump_type *dump_sects;
extern char *string_table;
extern unsigned long string_table_length;

/* SECTION_NAME macro */
#define SECTION_NAME(X)                                                        \
  ((X) == NULL ? _("<none>")                                                   \
   : string_table == NULL                                                      \
       ? _("<no-name>")                                                        \
       : ((X)->sh_name >= string_table_length ? _("<corrupt>")                 \
                                              : string_table + (X)->sh_name))

/* GET_ELF_SYMBOLS macro */
extern Elf_Internal_Sym *get_32bit_elf_symbols(FILE *, Elf_Internal_Shdr *,
                                               unsigned long *);
extern Elf_Internal_Sym *get_64bit_elf_symbols(FILE *, Elf_Internal_Shdr *,
                                               unsigned long *);
#define GET_ELF_SYMBOLS(file, section, sym_count)                              \
  (is_32bit_elf ? get_32bit_elf_symbols(file, section, sym_count)              \
                : get_64bit_elf_symbols(file, section, sym_count))

/* Function declarations */
extern unsigned int get_reloc_type(bfd_vma);
extern int uses_msp430x_relocs(void);
extern int is_none_reloc(unsigned int);
extern int is_32bit_abs_reloc(unsigned int);
extern int is_32bit_pcrel_reloc(unsigned int);
extern int is_64bit_abs_reloc(unsigned int);
extern int is_64bit_pcrel_reloc(unsigned int);
extern int is_24bit_abs_reloc(unsigned int);
extern int is_16bit_abs_reloc(unsigned int);
extern int slurp_rela_relocs(FILE *, bfd_size_type, bfd_size_type,
                             Elf_Internal_Rela **, unsigned long *);
extern int slurp_rel_relocs(FILE *, bfd_size_type, bfd_size_type,
                            Elf_Internal_Rela **, unsigned long *);
extern const char *get_symbol_type(unsigned int);
extern const char *printable_section_name(const Elf_Internal_Shdr *);
typedef unsigned long elf_vma;
extern elf_vma (*byte_get)(unsigned char *, int);
extern void (*byte_put)(unsigned char *, elf_vma, int);
extern void *get_data(void *, FILE *, bfd_size_type, bfd_size_type,
                      bfd_size_type, const char *);
extern unsigned int get_compression_header(Elf_Internal_Chdr *,
                                           unsigned char *);
extern int uncompress_section_contents(unsigned char **, dwarf_size_type,
                                       bfd_size_type *);
extern Elf_Internal_Shdr *find_section(const char *);
extern void free_debug_section(enum dwarf_section_display_enum);
extern unsigned int *find_cu_tu_set(FILE *, int);
extern void initialise_dumps_byname(void);
extern void dump_section_as_bytes(Elf_Internal_Shdr *, FILE *, int);
extern void dump_section_as_strings(Elf_Internal_Shdr *, FILE *);
extern void warn(const char *, ...);
extern void error(const char *, ...);
#ifdef SUPPORT_DISASSEMBLY
extern void disassemble_section(Elf_Internal_Shdr *, FILE *);
#else
static void disassemble_section(Elf_Internal_Shdr *section, FILE *file) {}
#endif

/* Forward declarations for functions defined in this file */
static bfd_vma get_reloc_symindex(bfd_vma reloc_info);
static bfd_boolean target_specific_reloc_handling(Elf_Internal_Rela *reloc,
                                                  unsigned char *start,
                                                  Elf_Internal_Sym *symtab);
static void apply_relocations(void *file, const Elf_Internal_Shdr *section,
                              unsigned char *start, bfd_size_type size,
                              void **relocs_return,
                              unsigned long *num_relocs_return);
static int load_specific_debug_section(enum dwarf_section_display_enum debug,
                                       const Elf_Internal_Shdr *sec,
                                       void *file);
static int display_debug_section(int shndx, Elf_Internal_Shdr *section,
                                 FILE *file);
static void process_section_contents(FILE *file);
static bfd_vma get_reloc_symindex(bfd_vma reloc_info) {
  return is_32bit_elf ? ELF32_R_SYM(reloc_info) : ELF64_R_SYM(reloc_info);
}

static bfd_boolean target_specific_reloc_handling(Elf_Internal_Rela *reloc,
                                                  unsigned char *start,
                                                  Elf_Internal_Sym *symtab) {
  unsigned int reloc_type = get_reloc_type(reloc->r_info);

  switch (elf_header.e_machine) {
  case EM_MSP430:
  case EM_MSP430_OLD: {
    static Elf_Internal_Sym *saved_sym = NULL;

    switch (reloc_type) {
    case 10: /* R_MSP430_SYM_DIFF */
      if (uses_msp430x_relocs())
        break;
      /* Fall through.  */
    case 21: /* R_MSP430X_SYM_DIFF */
      saved_sym = symtab + get_reloc_symindex(reloc->r_info);
      return TRUE;

    case 1: /* R_MSP430_32 or R_MSP430_ABS32 */
    case 3: /* R_MSP430_16 or R_MSP430_ABS8 */
      goto handle_sym_diff;

    case 5: /* R_MSP430_16_BYTE */
    case 9: /* R_MSP430_8 */
      if (uses_msp430x_relocs())
        break;
      goto handle_sym_diff;

    case 2:  /* R_MSP430_ABS16 */
    case 15: /* R_MSP430X_ABS16 */
      if (!uses_msp430x_relocs())
        break;
      goto handle_sym_diff;

    handle_sym_diff:
      if (saved_sym != NULL) {
        bfd_vma value;

        value = reloc->r_addend +
                (symtab[get_reloc_symindex(reloc->r_info)].st_value -
                 saved_sym->st_value);

        byte_put(start + reloc->r_offset, value, reloc_type == 1 ? 4 : 2);

        saved_sym = NULL;
        return TRUE;
      }
      break;

    default:
      if (saved_sym != NULL)
        error(_("Unhandled MSP430 reloc type found after SYM_DIFF reloc\n"));
      break;
    }
    break;
  }

  case EM_MN10300:
  case EM_CYGNUS_MN10300: {
    static Elf_Internal_Sym *saved_sym = NULL;

    switch (reloc_type) {
    case 34: /* R_MN10300_ALIGN */
      return TRUE;
    case 33: /* R_MN10300_SYM_DIFF */
      saved_sym = symtab + get_reloc_symindex(reloc->r_info);
      return TRUE;
    case 1: /* R_MN10300_32 */
    case 2: /* R_MN10300_16 */
      if (saved_sym != NULL) {
        bfd_vma value;

        value = reloc->r_addend +
                (symtab[get_reloc_symindex(reloc->r_info)].st_value -
                 saved_sym->st_value);

        byte_put(start + reloc->r_offset, value, reloc_type == 1 ? 4 : 2);

        saved_sym = NULL;
        return TRUE;
      }
      break;
    default:
      if (saved_sym != NULL)
        error(_("Unhandled MN10300 reloc type found after SYM_DIFF reloc\n"));
      break;
    }
    break;
  }

  case EM_RL78: {
    static bfd_vma saved_sym1 = 0;
    static bfd_vma saved_sym2 = 0;
    static bfd_vma value;

    switch (reloc_type) {
    case 0x80: /* R_RL78_SYM.  */
      saved_sym1 = saved_sym2;
      saved_sym2 = symtab[get_reloc_symindex(reloc->r_info)].st_value;
      saved_sym2 += reloc->r_addend;
      return TRUE;

    case 0x83: /* R_RL78_OPsub.  */
      value = saved_sym1 - saved_sym2;
      saved_sym2 = saved_sym1 = 0;
      return TRUE;
      break;

    case 0x41: /* R_RL78_ABS32.  */
      byte_put(start + reloc->r_offset, value, 4);
      value = 0;
      return TRUE;

    case 0x43: /* R_RL78_ABS16.  */
      byte_put(start + reloc->r_offset, value, 2);
      value = 0;
      return TRUE;

    default:
      break;
    }
    break;
  }
  }

  return FALSE;
}

static void apply_relocations(void *file, const Elf_Internal_Shdr *section,
                              unsigned char *start, bfd_size_type size,
                              void **relocs_return,
                              unsigned long *num_relocs_return) {
  Elf_Internal_Shdr *relsec;
  unsigned char *end = start + size;

  if (relocs_return != NULL) {
    *(Elf_Internal_Rela **)relocs_return = NULL;
    *num_relocs_return = 0;
  }

  if (elf_header.e_type != ET_REL)
    return;

  /* Find the reloc section associated with the section.  */
  for (relsec = section_headers; relsec < section_headers + elf_header.e_shnum;
       ++relsec) {
    bfd_boolean is_rela;
    unsigned long num_relocs;
    Elf_Internal_Rela *relocs;
    Elf_Internal_Rela *rp;
    Elf_Internal_Shdr *symsec;
    Elf_Internal_Sym *symtab;
    unsigned long num_syms;
    Elf_Internal_Sym *sym;

    if ((relsec->sh_type != SHT_RELA && relsec->sh_type != SHT_REL) ||
        relsec->sh_info >= elf_header.e_shnum ||
        section_headers + relsec->sh_info != section || relsec->sh_size == 0 ||
        relsec->sh_link >= elf_header.e_shnum)
      continue;

    is_rela = relsec->sh_type == SHT_RELA;

    if (is_rela) {
      if (!slurp_rela_relocs((FILE *)file, relsec->sh_offset, relsec->sh_size,
                             &relocs, &num_relocs))
        return;
    } else {
      if (!slurp_rel_relocs((FILE *)file, relsec->sh_offset, relsec->sh_size,
                            &relocs, &num_relocs))
        return;
    }

    /* SH uses RELA but uses in place value instead of the addend field.  */
    if (elf_header.e_machine == EM_SH)
      is_rela = FALSE;

    symsec = section_headers + relsec->sh_link;
    if (symsec->sh_type != SHT_SYMTAB && symsec->sh_type != SHT_DYNSYM)
      return;
    symtab = GET_ELF_SYMBOLS((FILE *)file, symsec, &num_syms);

    for (rp = relocs; rp < relocs + num_relocs; ++rp) {
      bfd_vma addend;
      unsigned int reloc_type;
      unsigned int reloc_size;
      unsigned char *rloc;
      unsigned long sym_index;

      reloc_type = get_reloc_type(rp->r_info);

      if (target_specific_reloc_handling(rp, start, symtab))
        continue;
      else if (is_none_reloc(reloc_type))
        continue;
      else if (is_32bit_abs_reloc(reloc_type) ||
               is_32bit_pcrel_reloc(reloc_type))
        reloc_size = 4;
      else if (is_64bit_abs_reloc(reloc_type) ||
               is_64bit_pcrel_reloc(reloc_type))
        reloc_size = 8;
      else if (is_24bit_abs_reloc(reloc_type))
        reloc_size = 3;
      else if (is_16bit_abs_reloc(reloc_type))
        reloc_size = 2;
      else {
        static unsigned int prev_reloc = 0;
        if (reloc_type != prev_reloc)
          warn(_("unable to apply unsupported reloc type %d to section %s\n"),
               reloc_type, printable_section_name(section));
        prev_reloc = reloc_type;
        continue;
      }

      rloc = start + rp->r_offset;
      if ((rloc + reloc_size) > end || (rloc < start)) {
        warn(_("skipping invalid relocation offset 0x%lx in section %s\n"),
             (unsigned long)rp->r_offset, printable_section_name(section));
        continue;
      }

      sym_index = (unsigned long)get_reloc_symindex(rp->r_info);
      if (sym_index >= num_syms) {
        warn(
            _("skipping invalid relocation symbol index 0x%lx in section %s\n"),
            sym_index, printable_section_name(section));
        continue;
      }
      sym = symtab + sym_index;

      /* If the reloc has a symbol associated with it,
         make sure that it is of an appropriate type.

         Relocations against symbols without type can happen.
         Gcc -feliminate-dwarf2-dups may generate symbols
         without type for debug info.

         Icc generates relocations against function symbols
         instead of local labels.

         Relocations against object symbols can happen, eg when
         referencing a global array.  For an example of this see
         the _clz.o binary in libgcc.a.  */
      if (sym != symtab && ELF_ST_TYPE(sym->st_info) != STT_COMMON &&
          ELF_ST_TYPE(sym->st_info) > STT_SECTION) {
        warn(_("skipping unexpected symbol type %s in %ld'th relocation in "
               "section %s\n"),
             get_symbol_type(ELF_ST_TYPE(sym->st_info)),
             (long int)(rp - relocs), printable_section_name(relsec));
        continue;
      }

      addend = 0;
      if (is_rela)
        addend += rp->r_addend;
      /* R_XTENSA_32, R_PJ_DATA_DIR32 and R_D30V_32_NORMAL are
         partial_inplace.  */
      if (!is_rela || (elf_header.e_machine == EM_XTENSA && reloc_type == 1) ||
          ((elf_header.e_machine == EM_PJ ||
            elf_header.e_machine == EM_PJ_OLD) &&
           reloc_type == 1) ||
          ((elf_header.e_machine == EM_D30V ||
            elf_header.e_machine == EM_CYGNUS_D30V) &&
           reloc_type == 12))
        addend += byte_get(rloc, reloc_size);

      if (is_32bit_pcrel_reloc(reloc_type) ||
          is_64bit_pcrel_reloc(reloc_type)) {
        /* On HPPA, all pc-relative relocations are biased by 8.  */
        if (elf_header.e_machine == EM_PARISC)
          addend -= 8;
        byte_put(rloc, (addend + sym->st_value) - rp->r_offset, reloc_size);
      } else
        byte_put(rloc, addend + sym->st_value, reloc_size);
    }

    free(symtab);

    if (relocs_return) {
      *(Elf_Internal_Rela **)relocs_return = relocs;
      *num_relocs_return = num_relocs;
    } else
      free(relocs);

    break;
  }
}

static int load_specific_debug_section(enum dwarf_section_display_enum debug,
                                       const Elf_Internal_Shdr *sec,
                                       void *file) {
  struct dwarf_section *section = &debug_displays[debug].section;
  char buf[64];

  /* If it is already loaded, do nothing.  */
  if (section->start != NULL)
    return 1;

  snprintf(buf, sizeof(buf), _("%s section data"), section->name);
  section->address = sec->sh_addr;
  section->user_data = NULL;
  section->start = (unsigned char *)get_data(NULL, (FILE *)file, sec->sh_offset,
                                             1, sec->sh_size, buf);
  if (section->start == NULL)
    section->size = 0;
  else {
    unsigned char *start = section->start;
    dwarf_size_type size = sec->sh_size;
    dwarf_size_type uncompressed_size = 0;

    if ((sec->sh_flags & SHF_COMPRESSED) != 0) {
      Elf_Internal_Chdr chdr;
      unsigned int compression_header_size;

      if (size < (is_32bit_elf ? sizeof(Elf32_External_Chdr)
                               : sizeof(Elf64_External_Chdr))) {
        warn(_("compressed section %s is too small to contain a compression "
               "header"),
             section->name);
        return 0;
      }

      compression_header_size = get_compression_header(&chdr, start);

      if (chdr.ch_type != ELFCOMPRESS_ZLIB) {
        warn(_("section '%s' has unsupported compress type: %d\n"),
             section->name, chdr.ch_type);
        return 0;
      } else if (chdr.ch_addralign != sec->sh_addralign) {
        warn(_("compressed section '%s' is corrupted\n"), section->name);
        return 0;
      }
      uncompressed_size = chdr.ch_size;
      start += compression_header_size;
      size -= compression_header_size;
    } else if (size > 12 && streq((char *)start, "ZLIB")) {
      /* Read the zlib header.  In this case, it should be "ZLIB"
         followed by the uncompressed section size, 8 bytes in
         big-endian order.  */
      uncompressed_size = start[4];
      uncompressed_size <<= 8;
      uncompressed_size += start[5];
      uncompressed_size <<= 8;
      uncompressed_size += start[6];
      uncompressed_size <<= 8;
      uncompressed_size += start[7];
      uncompressed_size <<= 8;
      uncompressed_size += start[8];
      uncompressed_size <<= 8;
      uncompressed_size += start[9];
      uncompressed_size <<= 8;
      uncompressed_size += start[10];
      uncompressed_size <<= 8;
      uncompressed_size += start[11];
      start += 12;
      size -= 12;
    }

    if (uncompressed_size &&
        uncompress_section_contents(&start, uncompressed_size, &size)) {
      /* Free the compressed buffer, update the section buffer
         and the section size if uncompress is successful.  */
      free(section->start);
      section->start = start;
    }
    section->size = size;
  }

  if (section->start == NULL)
    return 0;

  if (debug_displays[debug].relocate)
    apply_relocations((FILE *)file, sec, section->start, section->size,
                      &section->reloc_info, &section->num_relocs);
  else {
    section->reloc_info = NULL;
    section->num_relocs = 0;
  }

  return 1;
}

static int display_debug_section(int shndx, Elf_Internal_Shdr *section,
                                 FILE *file) {
  char *name = SECTION_NAME(section);
  const char *print_name = printable_section_name(section);
  bfd_size_type length;
  int result = 1;
  int i;

  length = section->sh_size;
  if (length == 0) {
    printf(_("\nSection '%s' has no debugging data.\n"), print_name);
    return 0;
  }
  if (section->sh_type == SHT_NOBITS) {
    /* There is no point in dumping the contents of a debugging section
       which has the NOBITS type - the bits in the file will be random.
       This can happen when a file containing a .eh_frame section is
       stripped with the --only-keep-debug command line option.  */
    printf(
        _("section '%s' has the NOBITS type - its contents are unreliable.\n"),
        print_name);
    return 0;
  }

  if (const_strneq(name, ".gnu.linkonce.wi."))
    name = ".debug_info";

  /* See if we know how to display the contents of this section.  */
  for (i = 0; i < max; i++)
    if (streq(debug_displays[i].section.uncompressed_name, name) ||
        (i == line && const_strneq(name, ".debug_line.")) ||
        streq(debug_displays[i].section.compressed_name, name)) {
      struct dwarf_section *sec = &debug_displays[i].section;
      int secondary = (section != find_section(name));

      if (secondary)
        free_debug_section((enum dwarf_section_display_enum)i);

      if (i == line && const_strneq(name, ".debug_line."))
        sec->name = name;
      else if (streq(sec->uncompressed_name, name))
        sec->name = sec->uncompressed_name;
      else
        sec->name = sec->compressed_name;
      if (load_specific_debug_section((enum dwarf_section_display_enum)i,
                                      section, file)) {
        /* If this debug section is part of a CU/TU set in a .dwp file,
           restrict load_debug_section to the sections in that set.  */
        section_subset = find_cu_tu_set(file, shndx);

        result &= debug_displays[i].display(sec, file);

        section_subset = NULL;

        if (secondary || (i != info && i != abbrev))
          free_debug_section((enum dwarf_section_display_enum)i);
      }

      break;
    }

  if (i == max) {
    printf(_("Unrecognized debug section: %s\n"), print_name);
    result = 0;
  }

  return result;
}

static void process_section_contents(FILE *file) {
  Elf_Internal_Shdr *section;
  unsigned int i;

  if (!do_dump)
    return;

  initialise_dumps_byname();

  for (i = 0, section = section_headers;
       i < elf_header.e_shnum && i < num_dump_sects; i++, section++) {
#ifdef SUPPORT_DISASSEMBLY
    if (dump_sects[i] & DISASS_DUMP)
      disassemble_section(section, file);
#endif
    if (dump_sects[i] & HEX_DUMP)
      dump_section_as_bytes(section, file, FALSE);

    if (dump_sects[i] & RELOC_DUMP)
      dump_section_as_bytes(section, file, TRUE);

    if (dump_sects[i] & STRING_DUMP)
      dump_section_as_strings(section, file);

    if (dump_sects[i] & DEBUG_DUMP)
      display_debug_section(i, section, file);
  }

  /* Check to see if the user requested a
     dump of a section that does not exist.  */
  while (i++ < num_dump_sects)
    if (dump_sects[i])
      warn(_("Section %d was not dumped because it does not exist!\n"), i);
}
