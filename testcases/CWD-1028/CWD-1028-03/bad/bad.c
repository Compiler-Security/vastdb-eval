#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========================================================================
 * Type definitions
 * ======================================================================== */

/* Basic types */
typedef int bfd_boolean;
#undef FALSE
#undef TRUE
#define FALSE 0
#define TRUE 1
#define NULL ((void *)0)
#define SEEK_SET 0

typedef unsigned long bfd_vma;
typedef long bfd_signed_vma;
typedef unsigned long bfd_size_type;
typedef unsigned long symvalue;
typedef long file_ptr;
typedef unsigned int flagword;
typedef unsigned char bfd_byte;
typedef unsigned long bfd_hostptr_t;

/* BFD_ASSERT macro */
#define BFD_ASSERT(x)                                                          \
  do {                                                                         \
    if (!(x))                                                                  \
      __builtin_abort();                                                       \
  } while (0)

/* BFD_ALIGN macro */
#define BFD_VMA_FMT "l"
#define BFD_ALIGN(this, boundary)                                              \
  ((((bfd_vma)(this) + (boundary) - 1) >= (bfd_vma)(this))                     \
       ? (((bfd_vma)(this) + ((boundary) - 1)) & ~(bfd_vma)((boundary) - 1))   \
       : ~(bfd_vma)0)

/* CONST_STRNEQ macro */
#define CONST_STRNEQ(STR1, STR2)                                               \
  (strncmp((STR1), (STR2), sizeof(STR2) - 1) == 0)

/* Translation macros */
#define _(x) (x)
#define N_(x) (x)

/* Forward declarations */
typedef struct bfd bfd;
typedef struct bfd_section asection;
typedef struct bfd_symbol asymbol;
typedef struct reloc_cache_entry reloc_cache_entry;
typedef struct relax_table relax_table;
typedef struct lineno_cache_entry lineno_cache_entry;
typedef struct bfd_link_order bfd_link_order;
typedef struct htab *htab_t;

/* ========================================================================
 * BFD error types
 * ======================================================================== */

typedef enum bfd_error {
  bfd_error_no_error = 0,
  bfd_error_system_call,
  bfd_error_invalid_target,
  bfd_error_wrong_format,
  bfd_error_wrong_object_format,
  bfd_error_invalid_operation,
  bfd_error_no_memory,
  bfd_error_no_symbols,
  bfd_error_no_armap,
  bfd_error_no_more_archived_files,
  bfd_error_malformed_archive,
  bfd_error_missing_dso,
  bfd_error_file_not_recognized,
  bfd_error_file_ambiguously_recognized,
  bfd_error_no_contents,
  bfd_error_nonrepresentable_section,
  bfd_error_no_debug_section,
  bfd_error_bad_value,
  bfd_error_file_too_big,
  bfd_error_on_input,
  bfd_error_invalid_error_code
} bfd_error_type;

/* ========================================================================
 * BFD direction enum
 * ======================================================================== */

enum bfd_direction {
  no_direction = 0,
  read_direction = 1,
  write_direction = 2,
  both_direction = 3
};

/* ========================================================================
 * BFD format enum
 * ======================================================================== */

enum bfd_format {
  bfd_unknown = 0,
  bfd_object,
  bfd_archive,
  bfd_core,
  bfd_type_end
};

/* ========================================================================
 * BFD architecture types
 * ======================================================================== */

enum bfd_architecture {
  bfd_arch_unknown,
  bfd_arch_obscure,
  bfd_arch_m68k,
  bfd_arch_vax,
  bfd_arch_i960,
  bfd_arch_or1k,
  bfd_arch_sparc,
  bfd_arch_a29k,
  bfd_arch_sh,
  bfd_arch_mips,
  bfd_arch_i386,
  bfd_arch_l1om,
  bfd_arch_k1om,
  bfd_arch_m88k,
  bfd_arch_hppa,
  bfd_arch_arm,
  bfd_arch_ns32k,
  bfd_arch_pdp11,
  bfd_arch_powerpc,
  bfd_arch_rs6000,
  bfd_arch_d10v,
  bfd_arch_z8k,
  bfd_arch_h8300,
  bfd_arch_xgate,
  bfd_arch_mcore,
  bfd_arch_ia64,
  bfd_arch_s390,
  bfd_arch_crx,
  bfd_arch_frv,
  bfd_arch_iq2000,
  bfd_arch_m32c,
  bfd_arch_m32r,
  bfd_arch_mn10200,
  bfd_arch_mn10300,
  bfd_arch_avr,
  bfd_arch_bfin,
  bfd_arch_cr16,
  bfd_arch_cris,
  bfd_arch_xtensa,
  bfd_arch_microblaze,
  bfd_arch_tilepro,
  bfd_arch_tilegx,
  bfd_arch_spu,
  bfd_arch_last
};

#define bfd_mach_ppc 0
#define bfd_mach_ppc64 1

typedef struct bfd_arch_info {
  int bits_per_word;
  int bits_per_address;
  int bits_per_byte;
  enum bfd_architecture arch;
  unsigned long mach;
  const char *arch_name;
  const char *printable_name;
  unsigned int section_align_power;
  bfd_boolean the_default;
  const struct bfd_arch_info *(*compatible)(const struct bfd_arch_info *,
                                            const struct bfd_arch_info *);
  bfd_boolean (*scan)(const struct bfd_arch_info *, const char *);
  void *(*fill)(bfd_size_type count, bfd_boolean is_bigendian,
                bfd_boolean code);
  const struct bfd_arch_info *next;
} bfd_arch_info_type;

/* ========================================================================
 * BFD target types
 * ======================================================================== */

enum bfd_flavour {
  bfd_target_unknown_flavour,
  bfd_target_aout_flavour,
  bfd_target_coff_flavour,
  bfd_target_ecoff_flavour,
  bfd_target_elf_flavour,
  bfd_target_ieee_flavour,
  bfd_target_nlm_flavour,
  bfd_target_oasys_flavour,
  bfd_target_tekhex_flavour,
  bfd_target_srec_flavour,
  bfd_target_ihex_flavour,
  bfd_target_som_flavour,
  bfd_target_os9k_flavour,
  bfd_target_versados_flavour,
  bfd_target_msdos_flavour,
  bfd_target_evax_flavour,
  bfd_target_mmo_flavour,
  bfd_target_mach_o_flavour,
  bfd_target_pef_flavour,
  bfd_target_pef_xlib_flavour,
  bfd_target_sym_flavour,
  bfd_target_xcoff_flavour
};

enum bfd_endian { BFD_ENDIAN_BIG, BFD_ENDIAN_LITTLE, BFD_ENDIAN_UNKNOWN };

typedef struct bfd_target {
  char *name;
  enum bfd_flavour flavour;
  enum bfd_endian byteorder;
  enum bfd_endian header_byteorder;
  flagword object_flags;
  flagword section_flags;
  char symbol_leading_char;
  char ar_pad_char;
  unsigned char ar_max_namelen;
  unsigned char match_priority;
  const void *backend_data;
  bfd_boolean (*_bfd_write_contents[5])(bfd *);
} bfd_target;

/* ========================================================================
 * Lineno cache entry
 * ======================================================================== */

typedef struct lineno_cache_entry {
  unsigned int line_number;
  union {
    struct bfd_symbol *sym;
    bfd_vma offset;
  } u;
} lineno_cache_entry;

/* ========================================================================
 * BFD symbol types and macros
 * ======================================================================== */

#define BSF_NO_FLAGS 0
#define BSF_LOCAL (1 << 0)
#define BSF_GLOBAL (1 << 1)
#define BSF_EXPORT BSF_GLOBAL
#define BSF_DEBUGGING (1 << 2)
#define BSF_FUNCTION (1 << 3)
#define BSF_KEEP (1 << 5)
#define BSF_ELF_COMMON (1 << 6)
#define BSF_WEAK (1 << 7)
#define BSF_SECTION_SYM (1 << 8)
#define BSF_OLD_COMMON (1 << 9)
#define BSF_NOT_AT_END (1 << 10)
#define BSF_CONSTRUCTOR (1 << 11)
#define BSF_WARNING (1 << 12)
#define BSF_INDIRECT (1 << 13)
#define BSF_FILE (1 << 14)
#define BSF_DYNAMIC (1 << 15)
#define BSF_OBJECT (1 << 16)
#define BSF_DEBUGGING_RELOC (1 << 17)
#define BSF_THREAD_LOCAL (1 << 18)
#define BSF_RELC (1 << 19)
#define BSF_SRELC (1 << 20)
#define BSF_SYNTHETIC (1 << 21)
#define BSF_GNU_INDIRECT_FUNCTION (1 << 22)
#define BSF_GNU_UNIQUE (1 << 23)

typedef struct bfd_symbol {
  struct bfd *the_bfd;
  const char *name;
  symvalue value;
  flagword flags;
  struct bfd_section *section;
  union {
    void *p;
    int i;
  } udata;
} asymbol;

/* ========================================================================
 * BFD section types and macros
 * ======================================================================== */

#define SEC_NO_FLAGS 0x0
#define SEC_ALLOC 0x1
#define SEC_LOAD 0x2
#define SEC_RELOC 0x4
#define SEC_READONLY 0x8
#define SEC_CODE 0x10
#define SEC_DATA 0x20
#define SEC_ROM 0x40
#define SEC_CONSTRUCTOR 0x80
#define SEC_HAS_CONTENTS 0x100
#define SEC_NEVER_LOAD 0x200
#define SEC_THREAD_LOCAL 0x400
#define SEC_HAS_GOT_REF 0x800
#define SEC_IS_COMMON 0x1000
#define SEC_DEBUGGING 0x2000
#define SEC_IN_MEMORY 0x4000
#define SEC_EXCLUDE 0x8000
#define SEC_SORT_ENTRIES 0x10000
#define SEC_LINK_ONCE 0x20000
#define SEC_LINK_DUPLICATES 0xc0000
#define SEC_LINK_DUPLICATES_DISCARD 0x0
#define SEC_LINK_DUPLICATES_ONE_ONLY 0x40000
#define SEC_LINK_DUPLICATES_SAME_SIZE 0x80000
#define SEC_LINK_DUPLICATES_SAME_CONTENTS                                      \
  (SEC_LINK_DUPLICATES_ONE_ONLY | SEC_LINK_DUPLICATES_SAME_SIZE)
#define SEC_LINKER_CREATED 0x100000
#define SEC_KEEP 0x200000
#define SEC_SMALL_DATA 0x400000
#define SEC_MERGE 0x800000
#define SEC_STRINGS 0x1000000
#define SEC_GROUP 0x2000000
#define SEC_COFF_SHARED_LIBRARY 0x4000000
#define SEC_COFF_SHARED 0x8000000

/* ========================================================================
 * Relax table
 * ======================================================================== */

struct relax_table {
  bfd_vma addr;
  int size;
};

/* ========================================================================
 * BFD section struct
 * ======================================================================== */

struct bfd_section {
  const char *name;
  unsigned int id;
  unsigned int index;
  struct bfd_section *next;
  struct bfd_section *prev;
  flagword flags;
  unsigned int user_set_vma : 1;
  unsigned int linker_mark : 1;
  unsigned int linker_has_input : 1;
  unsigned int gc_mark : 1;
  unsigned int compress_status : 2;
  unsigned int segment_mark : 1;
  unsigned int sec_info_type : 3;
  unsigned int use_rela_p : 1;
  unsigned int sec_flg0 : 1;
  unsigned int sec_flg1 : 1;
  unsigned int sec_flg2 : 1;
  unsigned int sec_flg3 : 1;
  unsigned int sec_flg4 : 1;
  unsigned int sec_flg5 : 1;
  bfd_vma vma;
  bfd_vma lma;
  bfd_size_type size;
  bfd_size_type rawsize;
  bfd_size_type compressed_size;
  struct relax_table *relax;
  int relax_count;
  bfd_vma output_offset;
  struct bfd_section *output_section;
  unsigned int alignment_power;
  struct reloc_cache_entry *relocation;
  struct reloc_cache_entry **orelocation;
  unsigned reloc_count;
  file_ptr filepos;
  file_ptr rel_filepos;
  file_ptr line_filepos;
  void *userdata;
  unsigned char *contents;
  struct lineno_cache_entry *lineno;
  unsigned int lineno_count;
  unsigned int entsize;
  struct bfd_section *kept_section;
  file_ptr moving_line_filepos;
  int target_index;
  void *used_by_bfd;
  struct reloc_cache_entry *constructor_chain;
  bfd *owner;
  struct bfd_symbol *symbol;
  struct bfd_symbol **symbol_ptr_ptr;
  union {
    struct bfd_link_order *link_order;
    struct bfd_section *s;
  } map_head, map_tail;
};

/* Global sections */
extern asection _bfd_std_section[4];
#define bfd_com_section_ptr (&_bfd_std_section[0])
#define bfd_und_section_ptr (&_bfd_std_section[1])
#define bfd_abs_section_ptr (&_bfd_std_section[2])
#define bfd_ind_section_ptr (&_bfd_std_section[3])

/* ========================================================================
 * BFD main struct
 * ======================================================================== */

#define BFD_NO_FLAGS 0x0
#define HAS_RELOC 0x1
#define EXEC_P 0x2
#define HAS_LINENO 0x4
#define HAS_DEBUG 0x08
#define HAS_SYMS 0x10
#define HAS_LOCALS 0x20
#define DYNAMIC 0x40
#define WP_TEXT 0x80
#define D_PAGED 0x100

struct bfd {
  const char *filename;
  const struct bfd_target *xvec;
  void *iostream;
  const void *iovec;
  struct bfd *lru_prev, *lru_next;
  unsigned long where;
  long mtime;
  unsigned int id;
  enum bfd_format format : 3;
  enum bfd_direction direction : 2;
  unsigned int flags : 20;
  unsigned int cacheable : 1;
  unsigned int target_defaulted : 1;
  unsigned int opened_once : 1;
  unsigned int mtime_set : 1;
  unsigned int no_export : 1;
  unsigned int output_has_begun : 1;
  unsigned int has_armap : 1;
  unsigned int is_thin_archive : 1;
  unsigned int selective_search : 1;
  unsigned int is_linker_output : 1;
  unsigned int is_linker_input : 1;
  unsigned long origin;
  unsigned long proxy_origin;
  struct bfd_section *sections;
  struct bfd_section *section_last;
  unsigned int section_count;
  int archive_pass;
  bfd_vma start_address;
  struct bfd_symbol **outsymbols;
  unsigned int symcount;
  unsigned int dynsymcount;
  const struct bfd_arch_info *arch_info;
  void *arelt_data;
  struct bfd *my_archive;
  struct bfd *archive_next;
  struct bfd *archive_head;
  struct bfd *nested_archives;
  union {
    struct bfd *next;
    void *hash;
  } link;
  union {
    void *coff_obj_data;
    void *pe_obj_data;
    void *xcoff_obj_data;
    void *elf_obj_data;
    void *any;
  } tdata;
  void *usrdata;
  void *memory;
};

/* ========================================================================
 * COFF internal types (from include/coff/internal.h)
 * ======================================================================== */

#define SCNNMLEN (8)
#define SYMNMLEN 8
#define FILNMLEN 14
#define DIMNUM 4

struct internal_extra_pe_filehdr {
  unsigned short e_magic;
  unsigned short e_cblp;
  unsigned short e_cp;
  unsigned short e_crlc;
  unsigned short e_cparhdr;
  unsigned short e_minalloc;
  unsigned short e_maxalloc;
  unsigned short e_ss;
  unsigned short e_sp;
  unsigned short e_csum;
  unsigned short e_ip;
  unsigned short e_cs;
  unsigned short e_lfarlc;
  unsigned short e_ovno;
  unsigned short e_res[4];
  unsigned short e_oemid;
  unsigned short e_oeminfo;
  unsigned short e_res2[10];
  bfd_vma e_lfanew;
  unsigned long dos_message[16];
  bfd_vma nt_signature;
};

#define GO32_STUBSIZE 2048

struct internal_filehdr {
  struct internal_extra_pe_filehdr pe;
  char go32stub[GO32_STUBSIZE];
  unsigned short f_magic;
  unsigned int f_nscns;
  long f_timdat;
  bfd_vma f_symptr;
  long f_nsyms;
  unsigned short f_opthdr;
  unsigned short f_flags;
  unsigned short f_target_id;
};

/* File header flags */
#define F_RELFLG (0x0001)
#define F_EXEC (0x0002)
#define F_LNNO (0x0004)
#define F_LSYMS (0x0008)
#define F_AR16WR (0x0080)
#define F_AR32WR (0x0100)
#define F_AR32W (0x0200)
#define F_DYNLOAD (0x1000)
#define F_SHROBJ (0x2000)
#define F_DLL (0x2000)
#define F_GO32STUB (0x4000)

struct internal_extra_pe_aouthdr {
  short Magic;
  char MajorLinkerVersion;
  char MinorLinkerVersion;
  long SizeOfCode;
  long SizeOfInitializedData;
  long SizeOfUninitializedData;
  bfd_vma AddressOfEntryPoint;
  bfd_vma BaseOfCode;
  bfd_vma BaseOfData;
  bfd_vma ImageBase;
  bfd_vma SectionAlignment;
  bfd_vma FileAlignment;
  short MajorOperatingSystemVersion;
  short MinorOperatingSystemVersion;
  short MajorImageVersion;
  short MinorImageVersion;
  short MajorSubsystemVersion;
  short MinorSubsystemVersion;
  long Reserved1;
  long SizeOfImage;
  long SizeOfHeaders;
  long CheckSum;
  short Subsystem;
  unsigned short DllCharacteristics;
  bfd_vma SizeOfStackReserve;
  bfd_vma SizeOfStackCommit;
  bfd_vma SizeOfHeapReserve;
  bfd_vma SizeOfHeapCommit;
  long LoaderFlags;
  long NumberOfRvaAndSizes;
};

struct internal_aouthdr {
  short magic;
  short vstamp;
  bfd_vma tsize;
  bfd_vma dsize;
  bfd_vma bsize;
  bfd_vma entry;
  bfd_vma text_start;
  bfd_vma data_start;
  unsigned long tagentries;
  bfd_vma o_toc;
  short o_snentry;
  short o_sntext;
  short o_sndata;
  short o_sntoc;
  short o_snloader;
  short o_snbss;
  short o_algntext;
  short o_algndata;
  short o_modtype;
  short o_cputype;
  bfd_vma o_maxstack;
  bfd_vma o_maxdata;
  bfd_vma bss_start;
  bfd_vma gp_value;
  unsigned long gprmask;
  unsigned long cprmask[4];
  unsigned long fprmask;
  long o_inlib;
  long o_sri;
  long vid[2];
  struct internal_extra_pe_aouthdr pe;
};

struct internal_syment {
  union {
    char _n_name[SYMNMLEN];
    struct {
      bfd_hostptr_t _n_zeroes;
      bfd_hostptr_t _n_offset;
    } _n_n;
    char *_n_nptr[2];
  } _n;
  bfd_vma n_value;
  int n_scnum;
  unsigned short n_flags;
  unsigned short n_type;
  unsigned char n_sclass;
  unsigned char n_numaux;
};

#define n_name _n._n_name
#define n_zeroes _n._n_n._n_zeroes
#define n_offset _n._n_n._n_offset

/* Section header */
struct internal_scnhdr {
  char s_name[SCNNMLEN];
  bfd_vma s_paddr;
  bfd_vma s_vaddr;
  bfd_vma s_size;
  bfd_vma s_scnptr;
  bfd_vma s_relptr;
  bfd_vma s_lnnoptr;
  unsigned long s_nreloc;
  unsigned long s_nlnno;
  long s_flags;
  long s_align;
  unsigned char s_page;
};

/* Storage classes */
#define C_EFCN 0xff
#define C_NULL 0
#define C_AUTO 1
#define C_EXT 2
#define C_STAT 3
#define C_REG 4
#define C_EXTDEF 5
#define C_LABEL 6
#define C_ULABEL 7
#define C_MOS 8
#define C_ARG 9
#define C_STRTAG 10
#define C_MOU 11
#define C_UNTAG 12
#define C_TPDEF 13
#define C_USTATIC 14
#define C_ENTAG 15
#define C_MOE 16
#define C_REGPARM 17
#define C_FIELD 18
#define C_AUTOARG 19
#define C_LASTENT 20
#define C_BLOCK 100
#define C_FCN 101
#define C_EOS 102
#define C_FILE 103
#define C_LINE 104
#define C_ALIAS 105
#define C_HIDDEN 106
#define C_WEAKEXT 127
#define C_UEXT 19
#define C_STATLAB 20
#define C_EXTLAB 21
#define C_SYSTEM 23
#define C_SECTION 104
#define C_NT_WEAK 105
#define C_LEAFPROC 108
#define C_SCALL 107
#define C_LEAFEXT 108
#define C_LEAFSTAT 113
#define C_OPTVAR 109
#define C_DEFINE 110
#define C_PRAGMA 111
#define C_SEGMENT 112
#define C_SHADOW 107
#define C_VERSION 108
#define C_HIDEXT 107
#define C_BINCL 108
#define C_EINCL 109
#define C_AIX_WEAKEXT 111
#define C_NULL_VALUE 0x00de1e00
#define C_GSYM (0x80)
#define C_LSYM (0x81)
#define C_PSYM (0x82)
#define C_RSYM (0x83)
#define C_RPSYM (0x84)
#define C_STSYM (0x85)
#define C_TCSYM (0x86)
#define C_BCOMM (0x87)
#define C_ECOML (0x88)
#define C_ECOMM (0x89)
#define C_DECL (0x8c)
#define C_ENTRY (0x8d)
#define C_FUN (0x8e)
#define C_BSTAT (0x8f)
#define C_ESTAT (0x90)
#define C_GTLS (0x97)
#define C_STTLS (0x98)
#define C_THUMBEXT (128 + C_EXT)
#define C_THUMBSTAT (128 + C_STAT)
#define C_THUMBLABEL (128 + C_LABEL)
#define C_THUMBEXTFUNC (C_THUMBEXT + 20)
#define C_THUMBSTATFUNC (C_THUMBSTAT + 20)

/* Section number constants */
#define N_UNDEF ((int)0)
#define N_ABS ((int)-1)
#define N_DEBUG ((int)-2)
#define N_TV ((int)-3)
#define P_TV ((int)-4)

/* Symbol types */
#define T_NULL 0
#define T_VOID 1
#define T_CHAR 2
#define T_SHORT 3
#define T_INT 4
#define T_LONG 5
#define T_FLOAT 6
#define T_DOUBLE 7
#define T_STRUCT 8
#define T_UNION 9
#define T_ENUM 10
#define T_MOE 11
#define T_UCHAR 12
#define T_USHORT 13
#define T_UINT 14
#define T_ULONG 15
#define T_LNGDBL 16

#define DT_NON (0)
#define DT_PTR (1)
#define DT_FCN (2)
#define DT_ARY (3)

#define N_BTMASK 0xf
#define N_TMASK 0x30
#define N_BTSHFT 4
#define N_TSHIFT 2

#define BTYPE(x) ((x) & N_BTMASK)
#define DTYPE(x) (((x) & N_TMASK) >> N_BTSHFT)
#define ISPTR(x)                                                               \
  (((unsigned long)(x) & N_TMASK) == ((unsigned long)DT_PTR << N_BTSHFT))
#define ISFCN(x)                                                               \
  (((unsigned long)(x) & N_TMASK) == ((unsigned long)DT_FCN << N_BTSHFT))
#define ISARY(x)                                                               \
  (((unsigned long)(x) & N_TMASK) == ((unsigned long)DT_ARY << N_BTSHFT))
#define ISTAG(x) ((x) == C_STRTAG || (x) == C_UNTAG || (x) == C_ENTAG)
#define DECREF(x) ((((x) >> N_TSHIFT) & ~N_BTMASK) | ((x) & N_BTMASK))

/* Forward declaration for coff_ptr_struct */
struct coff_ptr_struct;

/* Union internal_auxent */
union internal_auxent {
  struct {
    union {
      long l;
      struct coff_ptr_struct *p;
    } x_tagndx;
    union {
      struct {
        unsigned short x_lnno;
        unsigned short x_size;
      } x_lnsz;
      long x_fsize;
    } x_misc;
    union {
      struct {
        bfd_signed_vma x_lnnoptr;
        union {
          long l;
          struct coff_ptr_struct *p;
        } x_endndx;
      } x_fcn;
      struct {
        unsigned short x_dimen[DIMNUM];
      } x_ary;
    } x_fcnary;
    unsigned short x_tvndx;
  } x_sym;
  union {
    char x_fname[20];
    struct {
      long x_zeroes;
      long x_offset;
    } x_n;
  } x_file;
  struct {
    long x_scnlen;
    unsigned short x_nreloc;
    unsigned short x_nlinno;
    unsigned long x_checksum;
    unsigned short x_associated;
    unsigned char x_comdat;
  } x_scn;
  struct {
    long x_tvfill;
    unsigned short x_tvlen;
    unsigned short x_tvran[2];
  } x_tv;
  struct {
    union {
      bfd_signed_vma l;
      struct coff_ptr_struct *p;
    } x_scnlen;
    long x_parmhash;
    unsigned short x_snhash;
    unsigned char x_smtyp;
    unsigned char x_smclas;
    long x_stab;
    unsigned short x_snstab;
  } x_csect;
  struct {
    long x_stindx;
  } x_sc;
  struct {
    unsigned long x_balntry;
  } x_bal;
  struct {
    unsigned long x_timestamp;
    char x_idstring[20];
  } x_ident;
};

/* combined_entry_type (from libcoff.h) */
typedef struct coff_ptr_struct {
  unsigned int offset;
  unsigned int fix_value : 1;
  unsigned int fix_tag : 1;
  unsigned int fix_end : 1;
  unsigned int fix_scnlen : 1;
  unsigned int fix_line : 1;
  union {
    union internal_auxent auxent;
    struct internal_syment syment;
  } u;
  bfd_boolean is_sym;
} combined_entry_type;

/* coff_symbol_type (from libcoff.h) - now asymbol is fully defined */
typedef struct coff_symbol_struct {
  asymbol symbol;
  combined_entry_type *native;
  struct lineno_cache_entry *lineno;
  bfd_boolean done_lineno;
} coff_symbol_type;

/* ========================================================================
 * External COFF types (from include/coff/external.h)
 * ======================================================================== */

struct external_scnhdr {
  char s_name[8];
  char s_paddr[4];
  char s_vaddr[4];
  char s_size[4];
  char s_scnptr[4];
  char s_relptr[4];
  char s_lnnoptr[4];
  char s_nreloc[2];
  char s_nlnno[2];
  char s_flags[4];
};

#define SCNHDR struct external_scnhdr

/* Special section names */
#define _TEXT ".text"
#define _DATA ".data"
#define _BSS ".bss"
#define _COMMENT ".comment"
#define _LIB ".lib"

/* ========================================================================
 * PE specific (from include/coff/pe.h)
 * ======================================================================== */

#define IMAGE_FILE_RELOCS_STRIPPED 0x0001
#define IMAGE_FILE_EXECUTABLE_IMAGE 0x0002
#define IMAGE_FILE_LINE_NUMS_STRIPPED 0x0004
#define IMAGE_FILE_LOCAL_SYMS_STRIPPED 0x0008
#define IMAGE_FILE_AGGRESSIVE_WS_TRIM 0x0010
#define IMAGE_FILE_LARGE_ADDRESS_AWARE 0x0020
#define IMAGE_FILE_16BIT_MACHINE 0x0040
#define IMAGE_FILE_BYTES_REVERSED_LO 0x0080
#define IMAGE_FILE_32BIT_MACHINE 0x0100
#define IMAGE_FILE_DEBUG_STRIPPED 0x0200
#define IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP 0x0400
#define IMAGE_FILE_NET_RUN_FROM_SWAP 0x0800
#define IMAGE_FILE_SYSTEM 0x1000
#define IMAGE_FILE_DLL 0x2000
#define IMAGE_FILE_UP_SYSTEM_ONLY 0x4000
#define IMAGE_FILE_BYTES_REVERSED_HI 0x8000

#define IMAGE_SCN_ALIGN_POWER_BIT_POS 20
#define IMAGE_SCN_ALIGN_POWER_BIT_MASK 0x00f00000
#define IMAGE_SCN_ALIGN_POWER_NUM(val)                                         \
  (((val) >> IMAGE_SCN_ALIGN_POWER_BIT_POS) - 1)
#define IMAGE_SCN_ALIGN_POWER_CONST(val)                                       \
  (((val) + 1) << IMAGE_SCN_ALIGN_POWER_BIT_POS)

#define IMAGE_SCN_ALIGN_1BYTES IMAGE_SCN_ALIGN_POWER_CONST(0)
#define IMAGE_SCN_ALIGN_16BYTES IMAGE_SCN_ALIGN_POWER_CONST(4)

#define COFF_ENCODE_ALIGNMENT(SECTION, ALIGNMENT_POWER)                        \
  ((SECTION).s_flags |= IMAGE_SCN_ALIGN_POWER_CONST(                           \
       (ALIGNMENT_POWER) <= 13 ? (ALIGNMENT_POWER) : 13))
#define COFF_DECODE_ALIGNMENT(X)                                               \
  IMAGE_SCN_ALIGN_POWER_NUM((X) & IMAGE_SCN_ALIGN_POWER_BIT_MASK)

#define IMAGE_COMDAT_SELECT_NODUPLICATES (1)
#define IMAGE_COMDAT_SELECT_ANY (2)
#define IMAGE_COMDAT_SELECT_SAME_SIZE (3)
#define IMAGE_COMDAT_SELECT_EXACT_MATCH (4)
#define IMAGE_COMDAT_SELECT_ASSOCIATIVE (5)

#define IMAGE_NT_OPTIONAL_HDR_MAGIC 0x10b
#define IMAGE_NT_OPTIONAL_HDR64_MAGIC 0x20b

#define DOT_RELOC ".reloc"

/* ========================================================================
 * XCOFF specific (from include/coff/xcoff.h)
 * ======================================================================== */

#define _LOADER ".loader"
#define STYP_OVRFLO 0x8000
#define RS6K_AOUTHDR_OMAGIC 0x0107
#define RS6K_AOUTHDR_NMAGIC 0x0108
#define RS6K_AOUTHDR_ZMAGIC 0x010B

/* ========================================================================
 * COFF tdata types (from bfd/libcoff.h)
 * ======================================================================== */

typedef struct coff_tdata {
  struct coff_symbol_struct *symbols;
  unsigned int *conversion_table;
  int conv_table_size;
  file_ptr sym_filepos;
  struct coff_ptr_struct *raw_syments;
  unsigned long raw_syment_count;
  unsigned long int relocbase;
  unsigned local_n_btmask;
  unsigned local_n_btshft;
  unsigned local_n_tmask;
  unsigned local_n_tshift;
  unsigned local_symesz;
  unsigned local_auxesz;
  unsigned local_linesz;
  void *external_syms;
  bfd_boolean keep_syms;
  char *strings;
  bfd_size_type strings_len;
  bfd_boolean keep_strings;
  bfd_boolean strings_written;
  int pe;
  void *sym_hashes;
  int *local_toc_sym_map;
  void *link_info;
  void *line_info;
  void *dwarf2_find_line_info;
  long timestamp;
  flagword flags;
  char *go32stub;
} coff_data_type;

typedef struct pe_tdata {
  coff_data_type coff;
  struct internal_extra_pe_aouthdr pe_opthdr;
  int dll;
  int has_reloc_section;
  int dont_strip_reloc;
  bfd_boolean insert_timestamp;
  bfd_boolean (*in_reloc_p)(bfd *, void *);
  flagword real_flags;
  struct {
    bfd_boolean (*after_write_object_contents)(bfd *);
    const char *style;
    asection *sec;
  } build_id;
} pe_data_type;

struct xcoff_tdata {
  coff_data_type coff;
  bfd_boolean xcoff64;
  bfd_boolean full_aouthdr;
  bfd_vma toc;
  int sntoc;
  int snentry;
  int text_align_power;
  int data_align_power;
  short modtype;
  short cputype;
  bfd_vma maxdata;
  bfd_vma maxstack;
  asection **csects;
  long *debug_indices;
  unsigned int *lineno_counts;
  unsigned int import_file_id;
};

/* Accessor macros */
#define coff_data(bfd) ((coff_data_type *)((bfd)->tdata.coff_obj_data))
#define obj_pe(bfd) (coff_data(bfd)->pe)
#define obj_symbols(bfd) (coff_data(bfd)->symbols)
#define obj_sym_filepos(bfd) (coff_data(bfd)->sym_filepos)
#define obj_relocbase(bfd) (coff_data(bfd)->relocbase)
#define obj_raw_syments(bfd) (coff_data(bfd)->raw_syments)
#define obj_raw_syment_count(bfd) (coff_data(bfd)->raw_syment_count)
#define obj_convert(bfd) (coff_data(bfd)->conversion_table)
#define obj_coff_external_syms(bfd) (coff_data(bfd)->external_syms)
#define obj_coff_strings(bfd) (coff_data(bfd)->strings)
#define obj_coff_strings_len(bfd) (coff_data(bfd)->strings_len)
#define obj_coff_keep_strings(bfd) (coff_data(bfd)->keep_strings)

#define pe_data(bfd) ((pe_data_type *)((bfd)->tdata.pe_obj_data))
#define xcoff_data(abfd) ((struct xcoff_tdata *)((abfd)->tdata.xcoff_obj_data))

/* ========================================================================
 * COFF section data types
 * ======================================================================== */

struct coff_section_tdata {
  void *tdata;
};

#define coff_section_data(abfd, sec)                                           \
  ((struct coff_section_tdata *)(sec)->used_by_bfd)

struct pei_section_tdata {
  bfd_size_type virt_size;
  long pe_flags;
};

#define pei_section_data(abfd, sec)                                            \
  ((struct pei_section_tdata *)coff_section_data((abfd), (sec))->tdata)

/* ========================================================================
 * COFF symbol classifications
 * ======================================================================== */

enum coff_symbol_classification {
  COFF_SYMBOL_GLOBAL,
  COFF_SYMBOL_COMMON,
  COFF_SYMBOL_UNDEFINED,
  COFF_SYMBOL_LOCAL,
  COFF_SYMBOL_PE_SECTION
};

/* ========================================================================
 * String table size
 * ======================================================================== */

#define STRING_SIZE_SIZE 4

/* ========================================================================
 * COFF backend data (from bfd/libcoff.h)
 * ======================================================================== */

typedef struct {
  void (*_bfd_coff_swap_aux_in)(bfd *, void *, int, int, int, int, void *);
  void (*_bfd_coff_swap_sym_in)(bfd *, void *, void *);
  void (*_bfd_coff_swap_lineno_in)(bfd *, void *, void *);
  unsigned int (*_bfd_coff_swap_aux_out)(bfd *, void *, int, int, int, int,
                                         void *);
  unsigned int (*_bfd_coff_swap_sym_out)(bfd *, void *, void *);
  unsigned int (*_bfd_coff_swap_lineno_out)(bfd *, void *, void *);
  unsigned int (*_bfd_coff_swap_reloc_out)(bfd *, void *, void *);
  unsigned int (*_bfd_coff_swap_filehdr_out)(bfd *, void *, void *);
  unsigned int (*_bfd_coff_swap_aouthdr_out)(bfd *, void *, void *);
  unsigned int (*_bfd_coff_swap_scnhdr_out)(bfd *, void *, void *);
  unsigned int _bfd_filhsz;
  unsigned int _bfd_aoutsz;
  unsigned int _bfd_scnhsz;
  unsigned int _bfd_symesz;
  unsigned int _bfd_auxesz;
  unsigned int _bfd_relsz;
  unsigned int _bfd_linesz;
  unsigned int _bfd_filnmlen;
  bfd_boolean _bfd_coff_long_filenames;
  bfd_boolean _bfd_coff_long_section_names;
  bfd_boolean (*_bfd_coff_set_long_section_names)(bfd *, int);
  unsigned int _bfd_coff_default_section_alignment_power;
  bfd_boolean _bfd_coff_force_symnames_in_strings;
  unsigned int _bfd_coff_debug_string_prefix_length;
  unsigned int _bfd_coff_max_nscns;
  void (*_bfd_coff_swap_filehdr_in)(bfd *, void *, void *);
  void (*_bfd_coff_swap_aouthdr_in)(bfd *, void *, void *);
  void (*_bfd_coff_swap_scnhdr_in)(bfd *, void *, void *);
  void (*_bfd_coff_swap_reloc_in)(bfd *, void *, void *);
  bfd_boolean (*_bfd_coff_bad_format_hook)(bfd *, void *);
  bfd_boolean (*_bfd_coff_set_arch_mach_hook)(bfd *, void *);
  void *(*_bfd_coff_mkobject_hook)(bfd *, void *, void *);
  bfd_boolean (*_bfd_styp_to_sec_flags_hook)(bfd *, void *, const char *,
                                             asection *, flagword *);
  void (*_bfd_set_alignment_hook)(bfd *, asection *, void *);
  bfd_boolean (*_bfd_coff_slurp_symbol_table)(bfd *);
  bfd_boolean (*_bfd_coff_symname_in_debug)(bfd *, struct internal_syment *);
  bfd_boolean (*_bfd_coff_pointerize_aux_hook)(bfd *, combined_entry_type *,
                                               combined_entry_type *,
                                               unsigned int,
                                               combined_entry_type *);
  bfd_boolean (*_bfd_coff_print_aux)(bfd *, void *, combined_entry_type *,
                                     combined_entry_type *,
                                     combined_entry_type *, unsigned int);
  void (*_bfd_coff_reloc16_extra_cases)(bfd *, void *, void *, void *,
                                        bfd_byte *, unsigned int *,
                                        unsigned int *);
  int (*_bfd_coff_reloc16_estimate)(bfd *, asection *, void *, unsigned int,
                                    void *);
  enum coff_symbol_classification (*_bfd_coff_classify_symbol)(
      bfd *, struct internal_syment *);
  bfd_boolean (*_bfd_coff_compute_section_file_positions)(bfd *);
  bfd_boolean (*_bfd_coff_start_final_link)(bfd *, void *);
  bfd_boolean (*_bfd_coff_relocate_section)(bfd *, void *, bfd *, asection *,
                                            bfd_byte *, void *,
                                            struct internal_syment *,
                                            asection **);
  void *(*_bfd_coff_rtype_to_howto)(bfd *, asection *, void *, void *,
                                    struct internal_syment *, bfd_vma *);
  bfd_boolean (*_bfd_coff_adjust_symndx)(bfd *, void *, bfd *, asection *,
                                         void *, bfd_boolean *);
  bfd_boolean (*_bfd_coff_link_add_one_symbol)(void *, bfd *, const char *,
                                               flagword, asection *, bfd_vma,
                                               const char *, bfd_boolean,
                                               bfd_boolean, void **);
  bfd_boolean (*_bfd_coff_link_output_has_begun)(bfd *, void *);
  bfd_boolean (*_bfd_coff_final_link_postscript)(bfd *, void *);
  bfd_boolean (*_bfd_coff_print_pdata)(bfd *, void *);
} bfd_coff_backend_data;

#define coff_backend_info(abfd)                                                \
  ((bfd_coff_backend_data *)(abfd)->xvec->backend_data)

#define bfd_coff_swap_aux_in(a, e, t, c, ind, num, i)                          \
  ((coff_backend_info(a)->_bfd_coff_swap_aux_in)(a, e, t, c, ind, num, i))
#define bfd_coff_swap_sym_in(a, e, i)                                          \
  ((coff_backend_info(a)->_bfd_coff_swap_sym_in)(a, e, i))
#define bfd_coff_swap_scnhdr_out(abfd, i, o)                                   \
  ((coff_backend_info(abfd)->_bfd_coff_swap_scnhdr_out)(abfd, i, o))
#define bfd_coff_swap_filehdr_out(abfd, i, o)                                  \
  ((coff_backend_info(abfd)->_bfd_coff_swap_filehdr_out)(abfd, i, o))
#define bfd_coff_swap_aouthdr_out(abfd, i, o)                                  \
  ((coff_backend_info(abfd)->_bfd_coff_swap_aouthdr_out)(abfd, i, o))
#define bfd_coff_filhsz(abfd) (coff_backend_info(abfd)->_bfd_filhsz)
#define bfd_coff_aoutsz(abfd) (coff_backend_info(abfd)->_bfd_aoutsz)
#define bfd_coff_scnhsz(abfd) (coff_backend_info(abfd)->_bfd_scnhsz)
#define bfd_coff_symesz(abfd) (coff_backend_info(abfd)->_bfd_symesz)
#define bfd_coff_auxesz(abfd) (coff_backend_info(abfd)->_bfd_auxesz)
#define bfd_coff_relsz(abfd) (coff_backend_info(abfd)->_bfd_relsz)
#define bfd_coff_linesz(abfd) (coff_backend_info(abfd)->_bfd_linesz)
#define bfd_coff_filnmlen(abfd) (coff_backend_info(abfd)->_bfd_filnmlen)
#define bfd_coff_long_section_names(abfd)                                      \
  (coff_backend_info(abfd)->_bfd_coff_long_section_names)
#define bfd_coff_swap_filehdr_in(abfd, i, o)                                   \
  ((coff_backend_info(abfd)->_bfd_coff_swap_filehdr_in)(abfd, i, o))
#define bfd_coff_swap_aouthdr_in(abfd, i, o)                                   \
  ((coff_backend_info(abfd)->_bfd_coff_swap_aouthdr_in)(abfd, i, o))
#define bfd_coff_swap_scnhdr_in(abfd, i, o)                                    \
  ((coff_backend_info(abfd)->_bfd_coff_swap_scnhdr_in)(abfd, i, o))
#define bfd_coff_bad_format_hook(abfd, filehdr)                                \
  ((coff_backend_info(abfd)->_bfd_coff_bad_format_hook)(abfd, filehdr))
#define bfd_coff_set_arch_mach_hook(abfd, filehdr)                             \
  ((coff_backend_info(abfd)->_bfd_coff_set_arch_mach_hook)(abfd, filehdr))
#define bfd_coff_mkobject_hook(abfd, filehdr, aouthdr)                         \
  ((coff_backend_info(abfd)->_bfd_coff_mkobject_hook)(abfd, filehdr, aouthdr))
#define bfd_coff_styp_to_sec_flags_hook(abfd, scnhdr, name, section,           \
                                        flags_ptr)                             \
  ((coff_backend_info(abfd)->_bfd_styp_to_sec_flags_hook)(abfd, scnhdr, name,  \
                                                          section, flags_ptr))
#define bfd_coff_set_alignment_hook(abfd, sec, scnhdr)                         \
  ((coff_backend_info(abfd)->_bfd_set_alignment_hook)(abfd, sec, scnhdr))
#define bfd_coff_slurp_symbol_table(abfd)                                      \
  ((coff_backend_info(abfd)->_bfd_coff_slurp_symbol_table)(abfd))
#define bfd_coff_symname_in_debug(abfd, sym)                                   \
  ((coff_backend_info(abfd)->_bfd_coff_symname_in_debug)(abfd, sym))
#define bfd_coff_force_symnames_in_strings(abfd)                               \
  (coff_backend_info(abfd)->_bfd_coff_force_symnames_in_strings)
#define bfd_coff_debug_string_prefix_length(abfd)                              \
  (coff_backend_info(abfd)->_bfd_coff_debug_string_prefix_length)
#define bfd_coff_print_aux(abfd, file, base, symbol, aux, indaux)              \
  ((coff_backend_info(abfd)->_bfd_coff_print_aux)(abfd, file, base, symbol,    \
                                                  aux, indaux))
#define bfd_coff_reloc16_extra_cases(abfd, link_info, link_order, reloc, data, \
                                     src_ptr, dst_ptr)                         \
  ((coff_backend_info(abfd)->_bfd_coff_reloc16_extra_cases)(                   \
      abfd, link_info, link_order, reloc, data, src_ptr, dst_ptr))
#define bfd_coff_reloc16_estimate(abfd, section, reloc, shrink, link_info)     \
  ((coff_backend_info(abfd)->_bfd_coff_reloc16_estimate)(abfd, section, reloc, \
                                                         shrink, link_info))
#define bfd_coff_classify_symbol(abfd, sym)                                    \
  ((coff_backend_info(abfd)->_bfd_coff_classify_symbol)(abfd, sym))
#define bfd_coff_compute_section_file_positions(abfd)                          \
  ((coff_backend_info(abfd)->_bfd_coff_compute_section_file_positions)(abfd))
#define bfd_coff_start_final_link(obfd, info)                                  \
  ((coff_backend_info(obfd)->_bfd_coff_start_final_link)(obfd, info))
#define bfd_coff_relocate_section(obfd, info, ibfd, o, con, rel, isyms, secs)  \
  ((coff_backend_info(ibfd)->_bfd_coff_relocate_section)(                      \
      obfd, info, ibfd, o, con, rel, isyms, secs))
#define bfd_coff_rtype_to_howto(abfd, sec, rel, h, sym, addendp)               \
  ((coff_backend_info(abfd)->_bfd_coff_rtype_to_howto)(abfd, sec, rel, h, sym, \
                                                       addendp))
#define bfd_coff_adjust_symndx(obfd, info, ibfd, sec, rel, adjustedp)          \
  ((coff_backend_info(abfd)->_bfd_coff_adjust_symndx)(obfd, info, ibfd, sec,   \
                                                      rel, adjustedp))
#define bfd_coff_link_add_one_symbol(info, abfd, name, flags, section, value,  \
                                     string, cp, coll, hashp)                  \
  ((coff_backend_info(abfd)->_bfd_coff_link_add_one_symbol)(                   \
      info, abfd, name, flags, section, value, string, cp, coll, hashp))
#define bfd_coff_link_output_has_begun(a, p)                                   \
  ((coff_backend_info(a)->_bfd_coff_link_output_has_begun)(a, p))
#define bfd_coff_final_link_postscript(a, p)                                   \
  ((coff_backend_info(a)->_bfd_coff_final_link_postscript)(a, p))
#define bfd_coff_have_print_pdata(a)                                           \
  (coff_backend_info(a)->_bfd_coff_print_pdata)
#define bfd_coff_print_pdata(a, p)                                             \
  ((coff_backend_info(a)->_bfd_coff_print_pdata)(a, p))

#define bfd_pei_p(abfd) (CONST_STRNEQ((abfd)->xvec->name, "pei-"))

/* ========================================================================
 * BFD flags and accessors
 * ======================================================================== */

#define bfd_get_file_flags(abfd) ((abfd)->flags)
#define bfd_get_symcount(abfd) ((abfd)->symcount)
#define bfd_get_outsymbols(abfd) ((abfd)->outsymbols)
#define bfd_get_start_address(abfd) ((abfd)->start_address)
#define bfd_get_arch(abfd) ((abfd)->arch_info->arch)
#define bfd_get_mach(abfd) ((abfd)->arch_info->mach)
#define bfd_get_arch_info(abfd) ((abfd)->arch_info)
#define bfd_get_target(abfd) ((abfd)->xvec->name)
#define bfd_get_filename(abfd) ((abfd)->filename)
#define bfd_get_flavour(abfd) ((abfd)->xvec->flavour)
#define bfd_little_endian(abfd) ((abfd)->xvec->byteorder == BFD_ENDIAN_LITTLE)
#define bfd_big_endian(abfd) ((abfd)->xvec->byteorder == BFD_ENDIAN_BIG)
#define bfd_get_section_by_name(abfd, name) ((asection *)0)
#define bfd_get_section_alignment(abfd, sec) ((sec)->alignment_power)
#define bfd_write_p(abfd)                                                      \
  ((abfd)->direction == write_direction || (abfd)->direction == both_direction)
#define bfd_read_p(abfd)                                                       \
  ((abfd)->direction == read_direction || (abfd)->direction == both_direction)
#define bfd_get_format(abfd) ((abfd)->format)

#define bfd_target_coff_flavour bfd_target_coff_flavour
#define bfd_target_xcoff_flavour bfd_target_xcoff_flavour
#define bfd_target_elf_flavour bfd_target_elf_flavour

/* ========================================================================
 * Function declarations
 * ======================================================================== */

/* Undef macros that would conflict with function declarations */
#undef bfd_get_file_flags
#undef bfd_get_symcount
#undef bfd_get_outsymbols
#undef bfd_get_start_address
#undef bfd_get_arch
#undef bfd_get_mach
#undef bfd_get_arch_info
#undef bfd_get_target
#undef bfd_get_filename
#undef bfd_get_flavour
#undef bfd_little_endian
#undef bfd_big_endian
#undef bfd_get_section_by_name
#undef bfd_get_section_alignment
#undef bfd_write_p
#undef bfd_read_p
#undef bfd_get_format

/* Undef macros that would conflict with function declarations */
#undef obj_raw_syment_count
#undef coff_data
#undef obj_symbols
#undef obj_sym_filepos
#undef obj_relocbase
#undef obj_raw_syments
#undef obj_convert
#undef obj_coff_external_syms
#undef obj_coff_strings
#undef obj_coff_strings_len
#undef obj_coff_keep_strings
#undef pe_data
#undef xcoff_data
#undef coff_section_data
#undef pei_section_data
#undef coff_backend_info
#undef bfd_coff_swap_aux_in
#undef bfd_coff_swap_sym_in
#undef bfd_coff_swap_scnhdr_out
#undef bfd_coff_swap_filehdr_out
#undef bfd_coff_swap_aouthdr_out
#undef bfd_coff_filhsz
#undef bfd_coff_aoutsz
#undef bfd_coff_scnhsz
#undef bfd_coff_symesz
#undef bfd_coff_auxesz
#undef bfd_coff_relsz
#undef bfd_coff_linesz
#undef bfd_coff_filnmlen
#undef bfd_coff_long_section_names
#undef bfd_coff_swap_filehdr_in
#undef bfd_coff_swap_aouthdr_in
#undef bfd_coff_swap_scnhdr_in
#undef bfd_coff_bad_format_hook
#undef bfd_coff_set_arch_mach_hook
#undef bfd_coff_mkobject_hook
#undef bfd_coff_styp_to_sec_flags_hook
#undef bfd_coff_set_alignment_hook
#undef bfd_coff_slurp_symbol_table
#undef bfd_coff_symname_in_debug
#undef bfd_coff_force_symnames_in_strings
#undef bfd_coff_debug_string_prefix_length
#undef bfd_coff_print_aux
#undef bfd_coff_reloc16_extra_cases
#undef bfd_coff_reloc16_estimate
#undef bfd_coff_classify_symbol
#undef bfd_coff_compute_section_file_positions
#undef bfd_coff_start_final_link
#undef bfd_coff_relocate_section
#undef bfd_coff_rtype_to_howto
#undef bfd_coff_adjust_symndx
#undef bfd_coff_link_add_one_symbol
#undef bfd_coff_link_output_has_begun
#undef bfd_coff_final_link_postscript
#undef bfd_coff_have_print_pdata
#undef bfd_coff_print_pdata
#undef bfd_pei_p

/* Now re-define accessor macros as inline functions or keep as macros for use
 * in bad.c */
/* We need these for bad.c, so we'll define them after the function declarations
 */

/* BFD core functions */
extern bfd_size_type bfd_bread(void *, bfd_size_type, bfd *);
extern bfd_size_type bfd_bwrite(const void *, bfd_size_type, bfd *);
extern int bfd_seek(bfd *, file_ptr, int);
extern file_ptr bfd_tell(bfd *);
extern void *bfd_alloc(bfd *abfd, bfd_size_type wanted);
extern void *bfd_zalloc(bfd *abfd, bfd_size_type wanted);
extern void bfd_release(bfd *, void *);
extern void bfd_set_error(bfd_error_type error_tag);

extern bfd_boolean bfd_close(bfd *abfd);
extern bfd_boolean bfd_close_all_done(bfd *);
extern bfd_boolean bfd_set_format(bfd *, enum bfd_format);
extern bfd_boolean bfd_set_arch_mach(bfd *, enum bfd_architecture,
                                     unsigned long);
extern bfd_boolean bfd_set_start_address(bfd *, bfd_vma);
extern bfd_boolean bfd_set_file_flags(bfd *, flagword);

extern asection *bfd_get_section_by_name(bfd *, const char *);
extern int bfd_get_section_alignment(bfd *, asection *);

extern enum bfd_format bfd_get_format(bfd *);
extern enum bfd_architecture bfd_get_arch(bfd *);
extern unsigned long bfd_get_mach(bfd *);
extern flagword bfd_get_file_flags(bfd *);
extern flagword bfd_applicable_file_flags(bfd *);
extern unsigned int bfd_get_symcount(bfd *);
extern asymbol **bfd_get_outsymbols(bfd *);
extern bfd_vma bfd_get_start_address(bfd *);
extern const bfd_arch_info_type *bfd_get_arch_info(bfd *);
extern const char *bfd_get_target(bfd *);
extern const char *bfd_get_filename(bfd *);
extern const char *bfd_get_archive_filename(const bfd *);
extern enum bfd_flavour bfd_get_flavour(bfd *);
extern bfd_boolean bfd_little_endian(bfd *);
extern bfd_boolean bfd_big_endian(bfd *);

extern void bfd_nonfatal_message(const char *, const bfd *, const asection *,
                                 const char *, ...);
extern void non_fatal(const char *, ...);
extern void _bfd_error_handler(const char *, ...);
extern void fatal(const char *, ...);

extern const char *_bfd_coff_read_string_table(bfd *);
extern bfd_boolean _bfd_coff_get_external_symbols(bfd *);
extern bfd_boolean _bfd_coff_free_symbols(bfd *);

extern long coff_get_symtab_upper_bound(bfd *);
extern long coff_canonicalize_symtab(bfd *, asymbol **);
extern int coff_count_linenumbers(bfd *);
extern bfd_boolean coff_renumber_symbols(bfd *, int *);
extern void coff_mangle_symbols(bfd *);
extern bfd_boolean coff_write_symbols(bfd *);
extern bfd_boolean coff_write_linenumbers(bfd *);
extern bfd_boolean coff_write_relocs(bfd *, int);
extern bfd_boolean bfd_coff_slurp_symbol_table(bfd *);
extern bfd_boolean coff_slurp_line_table(bfd *, asection *);
extern asection *coff_section_from_bfd_index(bfd *, int);

extern combined_entry_type *coff_get_normalized_symtab(bfd *);

extern enum coff_symbol_classification
coff_classify_symbol(bfd *, struct internal_syment *);

extern char *copy_name(bfd *, const char *, size_t);
extern void *build_debug_section(bfd *, asection **);

extern long sec_to_styp_flags(const char *, flagword);

extern bfd_boolean coff_apply_checksum(bfd *);
extern void coff_set_flags(bfd *, unsigned int *, unsigned short *);
extern void coff_swap_aouthdr_out(bfd *, struct internal_aouthdr *, void *);
extern coff_symbol_type *coff_symbol_from(asymbol *);
extern bfd_boolean coff_compute_section_file_positions(bfd *);
extern bfd_boolean bfd_link_relocatable(void *);
extern int coff_swap_scnhdr_out(bfd *, struct internal_scnhdr *, void *);
extern void *bfd_malloc(bfd_size_type);
extern const char *bfd_printable_arch_mach(enum bfd_architecture,
                                           unsigned long);
extern long bfd_get_symtab_upper_bound(bfd *);
extern long coff_get_symtab_upper_bound(bfd *);
extern void *xmalloc(size_t);
extern void sprintf_vma(char *, bfd_vma);
extern long bfd_canonicalize_symtab(bfd *, asymbol **);
extern void bfd_map_over_sections(bfd *, void (*)(bfd *, asection *, void *),
                                  void *);

/* section_add struct (from objcopy.c) */
struct section_add {
  struct section_add *next;
  const char *name;
  const char *filename;
  size_t size;
  bfd_byte *contents;
  asection *section;
};

extern struct section_add *add_sections;
extern struct section_add *update_sections;
extern struct section_add *dump_sections;

/* section_list struct (from objcopy.c) */
struct section_list {
  struct section_list *next;
  const char *pattern;
  bfd_boolean used;
  unsigned int context;
  bfd_vma vma_val;
  bfd_vma lma_val;
  flagword flags;
};

/* Section context flags */
#define SECTION_CONTEXT_REMOVE (1 << 0)
#define SECTION_CONTEXT_COPY (1 << 1)
#define SECTION_CONTEXT_SET_VMA (1 << 2)
#define SECTION_CONTEXT_ALTER_VMA (1 << 3)
#define SECTION_CONTEXT_SET_LMA (1 << 4)
#define SECTION_CONTEXT_ALTER_LMA (1 << 5)
#define SECTION_CONTEXT_SET_FLAGS (1 << 6)
#define SECTION_CONTEXT_REMOVE_RELOCS (1 << 7)

struct section_list;
extern struct section_list *change_sections;
extern void *find_section_list(const char *, bfd_boolean, unsigned int);
extern void setup_bfd_headers(bfd *, bfd *);
extern void setup_section(bfd *, asection *, void *);
extern asection *bfd_make_section_with_flags(bfd *, const char *, flagword);
extern bfd_boolean bfd_set_section_size(bfd *, asection *, bfd_size_type);
extern bfd_boolean bfd_set_section_vma(bfd *, asection *, bfd_vma);
extern bfd_boolean bfd_set_section_alignment(bfd *, asection *, unsigned int);
extern unsigned int bfd_section_alignment(bfd *, asection *);
extern bfd_boolean bfd_set_section_lma(bfd *, asection *, bfd_vma);
extern bfd_boolean bfd_copy_private_bfd_data(bfd *, bfd *);
extern bfd_boolean bfd_set_file_flags(bfd *, flagword);

/* More missing macros and declarations */
#define GNU_BUILD_ATTRS_SECTION_NAME ".note.gnu.build-id"
#define FOPEN_WB "wb"

extern bfd_boolean is_merged_note_section(bfd *, asection *);
extern bfd_size_type merge_gnu_build_notes(bfd *, asection *, bfd_size_type,
                                           bfd_byte *);
extern bfd_size_type bfd_get_section_size(asection *);
extern bfd_boolean bfd_get_full_section_contents(bfd *, asection *,
                                                 bfd_byte **);
extern flagword bfd_get_section_flags(bfd *, asection *);
extern bfd_boolean bfd_malloc_and_get_section(bfd *, asection *, bfd_byte **);
extern asection *bfd_create_gnu_debuglink_section(bfd *, const char *);
extern bfd_boolean bfd_fill_in_gnu_debuglink_section(bfd *, asection *,
                                                     const char *);
extern unsigned int bfd_count_sections(bfd *);
extern void get_sections(bfd *, asection *, void *);

/* More missing declarations */
extern int compare_section_lma(const void *, const void *);
extern bfd_vma bfd_section_lma(bfd *, asection *);
extern bfd_size_type bfd_section_size(bfd *, asection *);
extern void *read_debugging_info(bfd *, asymbol **, long, bfd_boolean);
extern unsigned long htab_elements(htab_t);

/* More missing variables */
extern enum locals_action discard_locals;
enum locals_action { LOCALS_UNDEF, LOCALS_START_L, LOCALS_ALL };
extern struct section_rename *section_rename_list;
struct section_rename {
  const char *old_name;
  const char *new_name;
  flagword flags;
  struct section_rename *next;
};
extern int add_symbols;

/* More missing declarations */
extern void mark_symbols_used_in_relocations(bfd *, asection *, void *);
extern long filter_symbols(bfd *, bfd *, asymbol **, asymbol **, long);
extern bfd_boolean write_debugging_info(bfd *, void *, long *, asymbol ***);
extern void copy_relocations_in_section(bfd *, asection *, void *);
extern void copy_section(bfd *, asection *, void *);
extern bfd_boolean bfd_set_section_contents(bfd *, asection *, const void *,
                                            file_ptr, bfd_size_type);
extern bfd_boolean is_strip;
extern bfd_boolean bfd_alt_mach_code(bfd *, unsigned long);
/* ELF header struct */
typedef struct {
  unsigned short e_type;
  unsigned short e_machine;
  unsigned int e_version;
} Elf_Internal_Ehdr;

extern Elf_Internal_Ehdr *elf_elfheader(bfd *);

/* More missing declarations */
extern bfd *bfd_openr(const char *, const char *);
extern bfd *bfd_openw(const char *, const char *);
extern off_t get_file_size(const char *);
extern bfd_boolean bfd_check_format(bfd *, enum bfd_format);
extern bfd_boolean bfd_check_format_matches(bfd *, enum bfd_format, char ***);
extern void set_long_section_mode(bfd *, bfd *, int);
extern void copy_archive(bfd *, bfd *, const char *, bfd_boolean,
                         const bfd_arch_info_type *);
extern bfd_error_type bfd_get_error(void);
extern enum long_section_name_handling long_section_names;
enum long_section_name_handling { DISABLE, ENABLE, KEEP };
extern bfd_boolean force_output_target;
extern const bfd_arch_info_type *input_arch;
extern char *output_filename;
extern char *output_target;
extern char *input_filename;
extern char *input_target;
extern char **obj_matching;
extern char **core_matching;
extern void list_matching_formats(char **);

/* BFD flags */
#define BFD_COMPRESS 0x4000
#define BFD_DECOMPRESS 0x8000
#define BFD_COMPRESS_GABI 0x20000
#define BFD_CONVERT_ELF_COMMON 0x40000
#define BFD_USE_ELF_STT_COMMON 0x80000

/* ELF stt common enum */
enum bfd_link_elf_stt_common { unchanged, elf_stt_common, no_elf_stt_common };

/* Define to avoid the #include error in bad.c */
#define __A_MAGIC_SET__

/* PE defaults */
#define PE_DEF_FILE_ALIGNMENT 0x200
#define PE_DEF_SECTION_ALIGNMENT 0x1000

/* BFD write/read predicates */
#define bfd_write_p(abfd)                                                      \
  ((abfd)->direction == write_direction || (abfd)->direction == both_direction)
#define bfd_read_p(abfd)                                                       \
  ((abfd)->direction == read_direction || (abfd)->direction == both_direction)

/* BFD_SEND_FMT macro */
#define BFD_SEND_FMT(bfd, tag, args)                                           \
  ((bfd)->format == bfd_unknown                                                \
       ? FALSE                                                                 \
       : (*(bfd)->xvec->_bfd_write_contents[(int)(bfd)->format])args)

/* Make bfd_get_symcount and bfd_get_outsymbols assignable (they're lvalue
 * macros in real BFD) */
#define bfd_get_symcount(abfd) ((abfd)->symcount)
#define bfd_get_outsymbols(abfd) ((abfd)->outsymbols)

/* Enum values for do_debug_sections and strip_symbols */
enum debug_section_action {
  nothing = 0,
  compress = 1,
  compress_zlib,
  compress_gnu_zlib,
  compress_gabi_zlib,
  decompress
};

enum strip_action {
  STRIP_UNDEF,
  STRIP_NONE,
  STRIP_DEBUG,
  STRIP_UNNEEDED,
  STRIP_NONDEBUG,
  STRIP_DWO,
  STRIP_NONDWO,
  STRIP_ALL
};

/* Magic numbers for various targets */
#define ZMAGIC 0
#define OMAGIC 0
#define NMAGIC 0
#define PAGEMAGICBCS 0
#define APOLLO_COFF_VERSION_NUMBER 0
#define LYNXCOFFMAGIC 0
#define PAGEMAGICPEXECPAGED 0
#define PAGEMAGICPEXECSWAPPED 0
#define PAGEMAGICEXECSWAPPED 0
#define SH_PE_MAGIC 0
#define MIPS_PE_MAGIC 0
#define PE32PMAGIC 0
#define I960ROMAGIC 0

/* TI target IDs */
#define TI_TARGET_ID 0
#define TIC80_TARGET_ID 0
#define TICOFF_AOUT_MAGIC 0
#define TIC80_ARCH_MAGIC 0

/* ========================================================================
 * Variable declarations (from binutils/objcopy.c)
 * ======================================================================== */

extern int status;
extern bfd_boolean verbose;
extern bfd_boolean extract_symbol;
extern bfd_boolean set_start_set;
extern bfd_vma set_start;
extern bfd_vma change_start;
extern enum debug_section_action do_debug_sections;
extern bfd_boolean do_elf_stt_common;
extern enum strip_action strip_symbols;
extern flagword bfd_flags_to_set;
extern flagword bfd_flags_to_clear;
extern bfd_vma pe_file_alignment;
extern bfd_vma pe_heap_commit;
extern bfd_vma pe_heap_reserve;
extern bfd_vma pe_image_base;
extern bfd_vma pe_section_alignment;
extern bfd_vma pe_stack_commit;
extern bfd_vma pe_stack_reserve;
extern short pe_subsystem;
extern short pe_major_subsystem_version;
extern short pe_minor_subsystem_version;

extern const char *gnu_debuglink_filename;
extern bfd_boolean convert_debugging;
extern bfd_boolean change_leading_char;
extern bfd_boolean remove_leading_char;
extern bfd_boolean wildcard;
extern bfd_boolean localize_hidden;
extern bfd_boolean weaken;
extern bfd_boolean keep_file_symbols;
extern char *prefix_symbols_string;
extern char *prefix_sections_string;
extern char *prefix_alloc_sections_string;
extern int reverse_bytes;
extern int copy_byte;
extern int interleave;
extern int copy_width;
extern bfd_boolean preserve_dates;
extern int deterministic;
extern bfd_boolean merge_notes;
extern bfd_byte *merged_notes;
extern bfd_size_type merged_size;
extern bfd_boolean sections_removed;
extern bfd_boolean sections_copied;
extern bfd_vma change_section_address;
extern bfd_boolean gap_fill_set;
extern bfd_byte gap_fill;
extern bfd_boolean pad_to_set;
extern bfd_vma pad_to;
extern unsigned long use_alt_mach_code;
extern htab_t strip_specific_htab;
extern htab_t strip_unneeded_htab;
extern htab_t keep_specific_htab;
extern htab_t localize_specific_htab;
extern htab_t globalize_specific_htab;
extern htab_t keepglobal_specific_htab;
extern htab_t weaken_specific_htab;
extern htab_t redefine_specific_htab;
extern htab_t redefine_specific_reverse_htab;

extern asymbol **isympp;
extern asymbol **osympp;

/* ========================================================================
 * Re-define accessor macros for use in bad.c
 * ======================================================================== */

#define coff_data(bfd) ((coff_data_type *)((bfd)->tdata.coff_obj_data))
#define obj_pe(bfd) (coff_data(bfd)->pe)
#define obj_symbols(bfd) (coff_data(bfd)->symbols)
#define obj_sym_filepos(bfd) (coff_data(bfd)->sym_filepos)
#define obj_relocbase(bfd) (coff_data(bfd)->relocbase)
#define obj_raw_syments(bfd) (coff_data(bfd)->raw_syments)
#define obj_raw_syment_count(bfd) (coff_data(bfd)->raw_syment_count)
#define obj_convert(bfd) (coff_data(bfd)->conversion_table)
#define obj_coff_external_syms(bfd) (coff_data(bfd)->external_syms)
#define obj_coff_strings(bfd) (coff_data(bfd)->strings)
#define obj_coff_strings_len(bfd) (coff_data(bfd)->strings_len)
#define obj_coff_keep_strings(bfd) (coff_data(bfd)->keep_strings)

#define pe_data(bfd) ((pe_data_type *)((bfd)->tdata.pe_obj_data))
#define xcoff_data(abfd) ((struct xcoff_tdata *)((abfd)->tdata.xcoff_obj_data))

#define coff_section_data(abfd, sec)                                           \
  ((struct coff_section_tdata *)(sec)->used_by_bfd)

#define pei_section_data(abfd, sec)                                            \
  ((struct pei_section_tdata *)coff_section_data((abfd), (sec))->tdata)

#define coff_backend_info(abfd)                                                \
  ((bfd_coff_backend_data *)(abfd)->xvec->backend_data)

#define bfd_coff_swap_aux_in(a, e, t, c, ind, num, i)                          \
  ((coff_backend_info(a)->_bfd_coff_swap_aux_in)(a, e, t, c, ind, num, i))
#define bfd_coff_swap_sym_in(a, e, i)                                          \
  ((coff_backend_info(a)->_bfd_coff_swap_sym_in)(a, e, i))
#define bfd_coff_swap_scnhdr_out(abfd, i, o)                                   \
  ((coff_backend_info(abfd)->_bfd_coff_swap_scnhdr_out)(abfd, i, o))
#define bfd_coff_swap_filehdr_out(abfd, i, o)                                  \
  ((coff_backend_info(abfd)->_bfd_coff_swap_filehdr_out)(abfd, i, o))
#define bfd_coff_swap_aouthdr_out(abfd, i, o)                                  \
  ((coff_backend_info(abfd)->_bfd_coff_swap_aouthdr_out)(abfd, i, o))
#define bfd_coff_filhsz(abfd) (coff_backend_info(abfd)->_bfd_filhsz)
#define bfd_coff_aoutsz(abfd) (coff_backend_info(abfd)->_bfd_aoutsz)
#define bfd_coff_scnhsz(abfd) (coff_backend_info(abfd)->_bfd_scnhsz)
#define bfd_coff_symesz(abfd) (coff_backend_info(abfd)->_bfd_symesz)
#define bfd_coff_auxesz(abfd) (coff_backend_info(abfd)->_bfd_auxesz)
#define bfd_coff_relsz(abfd) (coff_backend_info(abfd)->_bfd_relsz)
#define bfd_coff_linesz(abfd) (coff_backend_info(abfd)->_bfd_linesz)
#define bfd_coff_filnmlen(abfd) (coff_backend_info(abfd)->_bfd_filnmlen)
#define bfd_coff_long_section_names(abfd)                                      \
  (coff_backend_info(abfd)->_bfd_coff_long_section_names)
#define bfd_coff_swap_filehdr_in(abfd, i, o)                                   \
  ((coff_backend_info(abfd)->_bfd_coff_swap_filehdr_in)(abfd, i, o))
#define bfd_coff_swap_aouthdr_in(abfd, i, o)                                   \
  ((coff_backend_info(abfd)->_bfd_coff_swap_aouthdr_in)(abfd, i, o))
#define bfd_coff_swap_scnhdr_in(abfd, i, o)                                    \
  ((coff_backend_info(abfd)->_bfd_coff_swap_scnhdr_in)(abfd, i, o))
#define bfd_coff_bad_format_hook(abfd, filehdr)                                \
  ((coff_backend_info(abfd)->_bfd_coff_bad_format_hook)(abfd, filehdr))
#define bfd_coff_set_arch_mach_hook(abfd, filehdr)                             \
  ((coff_backend_info(abfd)->_bfd_coff_set_arch_mach_hook)(abfd, filehdr))
#define bfd_coff_mkobject_hook(abfd, filehdr, aouthdr)                         \
  ((coff_backend_info(abfd)->_bfd_coff_mkobject_hook)(abfd, filehdr, aouthdr))
#define bfd_coff_styp_to_sec_flags_hook(abfd, scnhdr, name, section,           \
                                        flags_ptr)                             \
  ((coff_backend_info(abfd)->_bfd_styp_to_sec_flags_hook)(abfd, scnhdr, name,  \
                                                          section, flags_ptr))
#define bfd_coff_set_alignment_hook(abfd, sec, scnhdr)                         \
  ((coff_backend_info(abfd)->_bfd_set_alignment_hook)(abfd, sec, scnhdr))
#define bfd_coff_slurp_symbol_table(abfd)                                      \
  ((coff_backend_info(abfd)->_bfd_coff_slurp_symbol_table)(abfd))
#define bfd_coff_symname_in_debug(abfd, sym)                                   \
  ((coff_backend_info(abfd)->_bfd_coff_symname_in_debug)(abfd, sym))
#define bfd_coff_force_symnames_in_strings(abfd)                               \
  (coff_backend_info(abfd)->_bfd_coff_force_symnames_in_strings)
#define bfd_coff_debug_string_prefix_length(abfd)                              \
  (coff_backend_info(abfd)->_bfd_coff_debug_string_prefix_length)
#define bfd_coff_print_aux(abfd, file, base, symbol, aux, indaux)              \
  ((coff_backend_info(abfd)->_bfd_coff_print_aux)(abfd, file, base, symbol,    \
                                                  aux, indaux))
#define bfd_coff_reloc16_extra_cases(abfd, link_info, link_order, reloc, data, \
                                     src_ptr, dst_ptr)                         \
  ((coff_backend_info(abfd)->_bfd_coff_reloc16_extra_cases)(                   \
      abfd, link_info, link_order, reloc, data, src_ptr, dst_ptr))
#define bfd_coff_reloc16_estimate(abfd, section, reloc, shrink, link_info)     \
  ((coff_backend_info(abfd)->_bfd_coff_reloc16_estimate)(abfd, section, reloc, \
                                                         shrink, link_info))
#define bfd_coff_classify_symbol(abfd, sym)                                    \
  ((coff_backend_info(abfd)->_bfd_coff_classify_symbol)(abfd, sym))
#define bfd_coff_compute_section_file_positions(abfd)                          \
  ((coff_backend_info(abfd)->_bfd_coff_compute_section_file_positions)(abfd))
#define bfd_coff_start_final_link(obfd, info)                                  \
  ((coff_backend_info(obfd)->_bfd_coff_start_final_link)(obfd, info))
#define bfd_coff_relocate_section(obfd, info, ibfd, o, con, rel, isyms, secs)  \
  ((coff_backend_info(ibfd)->_bfd_coff_relocate_section)(                      \
      obfd, info, ibfd, o, con, rel, isyms, secs))
#define bfd_coff_rtype_to_howto(abfd, sec, rel, h, sym, addendp)               \
  ((coff_backend_info(abfd)->_bfd_coff_rtype_to_howto)(abfd, sec, rel, h, sym, \
                                                       addendp))
#define bfd_coff_adjust_symndx(obfd, info, ibfd, sec, rel, adjustedp)          \
  ((coff_backend_info(abfd)->_bfd_coff_adjust_symndx)(obfd, info, ibfd, sec,   \
                                                      rel, adjustedp))
#define bfd_coff_link_add_one_symbol(info, abfd, name, flags, section, value,  \
                                     string, cp, coll, hashp)                  \
  ((coff_backend_info(abfd)->_bfd_coff_link_add_one_symbol)(                   \
      info, abfd, name, flags, section, value, string, cp, coll, hashp))
#define bfd_coff_link_output_has_begun(a, p)                                   \
  ((coff_backend_info(a)->_bfd_coff_link_output_has_begun)(a, p))
#define bfd_coff_final_link_postscript(a, p)                                   \
  ((coff_backend_info(a)->_bfd_coff_final_link_postscript)(a, p))
#define bfd_coff_have_print_pdata(a)                                           \
  (coff_backend_info(a)->_bfd_coff_print_pdata)
#define bfd_coff_print_pdata(a, p)                                             \
  ((coff_backend_info(a)->_bfd_coff_print_pdata)(a, p))

#define bfd_pei_p(abfd) (CONST_STRNEQ((abfd)->xvec->name, "pei-"))

void coff_mangle_symbols(bfd *bfd_ptr) {
  unsigned int symbol_count = bfd_get_symcount(bfd_ptr);
  asymbol **symbol_ptr_ptr = bfd_ptr->outsymbols;
  unsigned int symbol_index;

  for (symbol_index = 0; symbol_index < symbol_count; symbol_index++) {
    coff_symbol_type *coff_symbol_ptr;

    coff_symbol_ptr = coff_symbol_from(symbol_ptr_ptr[symbol_index]);
    if (coff_symbol_ptr && coff_symbol_ptr->native) {
      int i;
      combined_entry_type *s = coff_symbol_ptr->native;

      BFD_ASSERT(s->is_sym);
      if (s->fix_value) {
        /* FIXME: We should use a union here.  */
        s->u.syment.n_value =
            (bfd_hostptr_t)((combined_entry_type *)((bfd_hostptr_t)
                                                        s->u.syment.n_value))
                ->offset;
        s->fix_value = 0;
      }
      if (s->fix_line) {
        /* The value is the offset into the line number entries
           for the symbol's section.  On output, the symbol's
           section should be N_DEBUG.  */
        s->u.syment.n_value =
            (coff_symbol_ptr->symbol.section->output_section->line_filepos +
             s->u.syment.n_value * bfd_coff_linesz(bfd_ptr));
        coff_symbol_ptr->symbol.section =
            coff_section_from_bfd_index(bfd_ptr, N_DEBUG);
        BFD_ASSERT(coff_symbol_ptr->symbol.flags & BSF_DEBUGGING);
      }
      for (i = 0; i < s->u.syment.n_numaux; i++) {
        combined_entry_type *a = s + i + 1;

        BFD_ASSERT(!a->is_sym);
        if (a->fix_tag) {
          a->u.auxent.x_sym.x_tagndx.l = a->u.auxent.x_sym.x_tagndx.p->offset;
          a->fix_tag = 0;
        }
        if (a->fix_end) {
          a->u.auxent.x_sym.x_fcnary.x_fcn.x_endndx.l =
              a->u.auxent.x_sym.x_fcnary.x_fcn.x_endndx.p->offset;
          a->fix_end = 0;
        }
        if (a->fix_scnlen) {
          a->u.auxent.x_csect.x_scnlen.l =
              a->u.auxent.x_csect.x_scnlen.p->offset;
          a->fix_scnlen = 0;
        }
      }
    }
  }
}

static bfd_boolean coff_write_object_contents(bfd *abfd) {
  asection *current;
  bfd_boolean hasrelocs = FALSE;
  bfd_boolean haslinno = FALSE;
#ifdef COFF_IMAGE_WITH_PE
  bfd_boolean hasdebug = FALSE;
#endif
  file_ptr scn_base;
  file_ptr reloc_base;
  file_ptr lineno_base;
  file_ptr sym_base;
  unsigned long reloc_size = 0, reloc_count = 0;
  unsigned long lnno_size = 0;
  bfd_boolean long_section_names;
  asection *text_sec = NULL;
  asection *data_sec = NULL;
  asection *bss_sec = NULL;
  struct internal_filehdr internal_f;
  struct internal_aouthdr internal_a;
#ifdef COFF_LONG_SECTION_NAMES
  size_t string_size = STRING_SIZE_SIZE;
#endif

  bfd_set_error(bfd_error_system_call);

  /* Make a pass through the symbol table to count line number entries and
     put them into the correct asections.  */
  lnno_size = coff_count_linenumbers(abfd) * bfd_coff_linesz(abfd);

  if (!abfd->output_has_begun) {
    if (!coff_compute_section_file_positions(abfd))
      return FALSE;
  }

  reloc_base = obj_relocbase(abfd);

  /* Work out the size of the reloc and linno areas.  */

  for (current = abfd->sections; current != NULL; current = current->next) {
#ifdef COFF_WITH_PE
    /* We store the actual reloc count in the first reloc's addr.  */
    if (obj_pe(abfd) && current->reloc_count >= 0xffff)
      reloc_count++;
#endif
    reloc_count += current->reloc_count;
  }

  reloc_size = reloc_count * bfd_coff_relsz(abfd);

  lineno_base = reloc_base + reloc_size;
  sym_base = lineno_base + lnno_size;

  /* Indicate in each section->line_filepos its actual file address.  */
  for (current = abfd->sections; current != NULL; current = current->next) {
    if (current->lineno_count) {
      current->line_filepos = lineno_base;
      current->moving_line_filepos = lineno_base;
      lineno_base += current->lineno_count * bfd_coff_linesz(abfd);
    } else
      current->line_filepos = 0;

    if (current->reloc_count) {
      current->rel_filepos = reloc_base;
      reloc_base += current->reloc_count * bfd_coff_relsz(abfd);
#ifdef COFF_WITH_PE
      /* Extra reloc to hold real count.  */
      if (obj_pe(abfd) && current->reloc_count >= 0xffff)
        reloc_base += bfd_coff_relsz(abfd);
#endif
    } else
      current->rel_filepos = 0;
  }

  /* Write section headers to the file.  */
  internal_f.f_nscns = 0;

  if ((abfd->flags & EXEC_P) != 0)
    scn_base = bfd_coff_filhsz(abfd) + bfd_coff_aoutsz(abfd);
  else {
    scn_base = bfd_coff_filhsz(abfd);
#ifdef RS6000COFF_C
#ifndef XCOFF64
    if (xcoff_data(abfd)->full_aouthdr)
      scn_base += bfd_coff_aoutsz(abfd);
    else
      scn_base += SMALL_AOUTSZ;
#endif
#endif
  }

  if (bfd_seek(abfd, scn_base, SEEK_SET) != 0)
    return FALSE;

  long_section_names = FALSE;
  for (current = abfd->sections; current != NULL; current = current->next) {
    struct internal_scnhdr section;
#ifdef COFF_IMAGE_WITH_PE
    bfd_boolean is_reloc_section = FALSE;

    if (strcmp(current->name, DOT_RELOC) == 0) {
      is_reloc_section = TRUE;
      hasrelocs = TRUE;
      pe_data(abfd)->has_reloc_section = 1;
    }
#endif

    internal_f.f_nscns++;

    strncpy(section.s_name, current->name, SCNNMLEN);

#ifdef COFF_LONG_SECTION_NAMES
    /* Handle long section names as in PE.  This must be compatible
       with the code in coff_write_symbols and _bfd_coff_final_link.  */
    if (bfd_coff_long_section_names(abfd)) {
      size_t len;

      len = strlen(current->name);
      if (len > SCNNMLEN) {
        /* The s_name field is defined to be NUL-padded but need not be
           NUL-terminated.  We use a temporary buffer so that we can still
           sprintf all eight chars without splatting a terminating NUL
           over the first byte of the following member (s_paddr).  */
        /* PR 21096: The +20 is to stop a bogus warning from gcc7 about
           a possible buffer overflow.  */
        char s_name_buf[SCNNMLEN + 1 + 20];

        /* An inherent limitation of the /nnnnnnn notation used to indicate
           the offset of the long name in the string table is that we
           cannot address entries beyone the ten million byte boundary.  */
        if (string_size >= 10000000) {
          bfd_set_error(bfd_error_file_too_big);
          _bfd_error_handler
              /* xgettext:c-format */
              (_("%B: section %A: string table overflow at offset %ld"), abfd,
               current, (unsigned long)string_size);
          return FALSE;
        }

        /* We do not need to use snprintf here as we have already verfied
           that string_size is not too big, plus we have an overlarge
           buffer, just in case.  */
        sprintf(s_name_buf, "/%lu", (unsigned long)string_size);
        /* Then strncpy takes care of any padding for us.  */
        strncpy(section.s_name, s_name_buf, SCNNMLEN);
        string_size += len + 1;
        long_section_names = TRUE;
      }
    }
#endif

#ifdef _LIB
    /* Always set s_vaddr of .lib to 0.  This is right for SVR3.2
       Ian Taylor <ian@cygnus.com>.  */
    if (strcmp(current->name, _LIB) == 0)
      section.s_vaddr = 0;
    else
#endif
      section.s_vaddr = current->vma;
    section.s_paddr = current->lma;
    section.s_size = current->size;
#ifdef coff_get_section_load_page
    section.s_page = coff_get_section_load_page(current);
#else
    section.s_page = 0;
#endif

#ifdef COFF_WITH_PE
    section.s_paddr = 0;
#endif
#ifdef COFF_IMAGE_WITH_PE
    /* Reminder: s_paddr holds the virtual size of the section.  */
    if (coff_section_data(abfd, current) != NULL &&
        pei_section_data(abfd, current) != NULL)
      section.s_paddr = pei_section_data(abfd, current)->virt_size;
    else
      section.s_paddr = 0;
#endif

    /* If this section has no size or is unloadable then the scnptr
       will be 0 too.  */
    if (current->size == 0 ||
        (current->flags & (SEC_LOAD | SEC_HAS_CONTENTS)) == 0)
      section.s_scnptr = 0;
    else
      section.s_scnptr = current->filepos;

    section.s_relptr = current->rel_filepos;
    section.s_lnnoptr = current->line_filepos;
    section.s_nreloc = current->reloc_count;
    section.s_nlnno = current->lineno_count;
#ifndef COFF_IMAGE_WITH_PE
    /* In PEI, relocs come in the .reloc section.  */
    if (current->reloc_count != 0)
      hasrelocs = TRUE;
#endif
    if (current->lineno_count != 0)
      haslinno = TRUE;
#ifdef COFF_IMAGE_WITH_PE
    if ((current->flags & SEC_DEBUGGING) != 0 && !is_reloc_section)
      hasdebug = TRUE;
#endif

#ifdef RS6000COFF_C
#ifndef XCOFF64
    /* Indicate the use of an XCOFF overflow section header.  */
    if (current->reloc_count >= 0xffff || current->lineno_count >= 0xffff) {
      section.s_nreloc = 0xffff;
      section.s_nlnno = 0xffff;
    }
#endif
#endif

    section.s_flags = sec_to_styp_flags(current->name, current->flags);

    if (!strcmp(current->name, _TEXT))
      text_sec = current;
    else if (!strcmp(current->name, _DATA))
      data_sec = current;
    else if (!strcmp(current->name, _BSS))
      bss_sec = current;

#ifdef I960
    section.s_align =
        (current->alignment_power ? 1 << current->alignment_power : 0);
#endif
#ifdef COFF_ENCODE_ALIGNMENT
    COFF_ENCODE_ALIGNMENT(section, current->alignment_power);
    if ((unsigned int)COFF_DECODE_ALIGNMENT(section.s_flags) !=
        current->alignment_power) {
      bfd_boolean warn = coff_data(abfd)->link_info &&
                         !bfd_link_relocatable(coff_data(abfd)->link_info);

      _bfd_error_handler
          /* xgettext:c-format */
          (_("%B:%s section %s: alignment 2**%u not representable"), abfd,
           warn ? " warning:" : "", current->name, current->alignment_power);
      if (!warn) {
        bfd_set_error(bfd_error_nonrepresentable_section);
        return FALSE;
      }
    }
#endif

#ifdef COFF_IMAGE_WITH_PE
    /* Suppress output of the sections if they are null.  ld
       includes the bss and data sections even if there is no size
       assigned to them.  NT loader doesn't like it if these section
       headers are included if the sections themselves are not
       needed.  See also coff_compute_section_file_positions.  */
    if (section.s_size == 0)
      internal_f.f_nscns--;
    else
#endif
    {
      SCNHDR buff;
      bfd_size_type amt = bfd_coff_scnhsz(abfd);

      if (coff_swap_scnhdr_out(abfd, &section, &buff) == 0 ||
          bfd_bwrite(&buff, amt, abfd) != amt)
        return FALSE;
    }

#ifdef COFF_WITH_PE
    /* PE stores COMDAT section information in the symbol table.  If
       this section is supposed to have some COMDAT info, track down
       the symbol in the symbol table and modify it.  */
    if ((current->flags & SEC_LINK_ONCE) != 0) {
      unsigned int i, count;
      asymbol **psym;
      coff_symbol_type *csym = NULL;
      asymbol **psymsec;

      psymsec = NULL;
      count = bfd_get_symcount(abfd);
      for (i = 0, psym = abfd->outsymbols; i < count; i++, psym++) {
        if ((*psym)->section != current)
          continue;

        /* Remember the location of the first symbol in this
           section.  */
        if (psymsec == NULL)
          psymsec = psym;

        /* See if this is the section symbol.  */
        if (strcmp((*psym)->name, current->name) == 0) {
          csym = coff_symbol_from(*psym);
          if (csym == NULL || csym->native == NULL || !csym->native->is_sym ||
              csym->native->u.syment.n_numaux < 1 ||
              csym->native->u.syment.n_sclass != C_STAT ||
              csym->native->u.syment.n_type != T_NULL)
            continue;

          /* Here *PSYM is the section symbol for CURRENT.  */

          break;
        }
      }

      /* Did we find it?
         Note that we might not if we're converting the file from
         some other object file format.  */
      if (i < count) {
        combined_entry_type *aux;

        /* We don't touch the x_checksum field.  The
           x_associated field is not currently supported.  */

        aux = csym->native + 1;
        BFD_ASSERT(!aux->is_sym);
        switch (current->flags & SEC_LINK_DUPLICATES) {
        case SEC_LINK_DUPLICATES_DISCARD:
          aux->u.auxent.x_scn.x_comdat = IMAGE_COMDAT_SELECT_ANY;
          break;

        case SEC_LINK_DUPLICATES_ONE_ONLY:
          aux->u.auxent.x_scn.x_comdat = IMAGE_COMDAT_SELECT_NODUPLICATES;
          break;

        case SEC_LINK_DUPLICATES_SAME_SIZE:
          aux->u.auxent.x_scn.x_comdat = IMAGE_COMDAT_SELECT_SAME_SIZE;
          break;

        case SEC_LINK_DUPLICATES_SAME_CONTENTS:
          aux->u.auxent.x_scn.x_comdat = IMAGE_COMDAT_SELECT_EXACT_MATCH;
          break;
        }

        /* The COMDAT symbol must be the first symbol from this
           section in the symbol table.  In order to make this
           work, we move the COMDAT symbol before the first
           symbol we found in the search above.  It's OK to
           rearrange the symbol table at this point, because
           coff_renumber_symbols is going to rearrange it
           further and fix up all the aux entries.  */
        if (psym != psymsec) {
          asymbol *hold;
          asymbol **pcopy;

          hold = *psym;
          for (pcopy = psym; pcopy > psymsec; pcopy--)
            pcopy[0] = pcopy[-1];
          *psymsec = hold;
        }
      }
    }
#endif /* COFF_WITH_PE */
  }

#ifdef RS6000COFF_C
#ifndef XCOFF64
  /* XCOFF handles overflows in the reloc and line number count fields
     by creating a new section header to hold the correct values.  */
  for (current = abfd->sections; current != NULL; current = current->next) {
    if (current->reloc_count >= 0xffff || current->lineno_count >= 0xffff) {
      struct internal_scnhdr scnhdr;
      SCNHDR buff;
      bfd_size_type amt;

      internal_f.f_nscns++;
      memcpy(scnhdr.s_name, ".ovrflo", 8);
      scnhdr.s_paddr = current->reloc_count;
      scnhdr.s_vaddr = current->lineno_count;
      scnhdr.s_size = 0;
      scnhdr.s_scnptr = 0;
      scnhdr.s_relptr = current->rel_filepos;
      scnhdr.s_lnnoptr = current->line_filepos;
      scnhdr.s_nreloc = current->target_index;
      scnhdr.s_nlnno = current->target_index;
      scnhdr.s_flags = STYP_OVRFLO;
      amt = bfd_coff_scnhsz(abfd);
      if (coff_swap_scnhdr_out(abfd, &scnhdr, &buff) == 0 ||
          bfd_bwrite(&buff, amt, abfd) != amt)
        return FALSE;
    }
  }
#endif
#endif

  /* OK, now set up the filehdr...  */

  /* Don't include the internal abs section in the section count */

  /* We will NOT put a fucking timestamp in the header here. Every time you
     put it back, I will come in and take it out again.  I'm sorry.  This
     field does not belong here.  We fill it with a 0 so it compares the
     same but is not a reasonable time. -- gnu@cygnus.com  */
  internal_f.f_timdat = 0;
  internal_f.f_flags = 0;

  if (abfd->flags & EXEC_P)
    internal_f.f_opthdr = bfd_coff_aoutsz(abfd);
  else {
    internal_f.f_opthdr = 0;
#ifdef RS6000COFF_C
#ifndef XCOFF64
    if (xcoff_data(abfd)->full_aouthdr)
      internal_f.f_opthdr = bfd_coff_aoutsz(abfd);
    else
      internal_f.f_opthdr = SMALL_AOUTSZ;
#endif
#endif
  }

  if (!hasrelocs)
    internal_f.f_flags |= F_RELFLG;
  if (!haslinno)
    internal_f.f_flags |= F_LNNO;
  if (abfd->flags & EXEC_P)
    internal_f.f_flags |= F_EXEC;
#ifdef COFF_IMAGE_WITH_PE
  if (!hasdebug)
    internal_f.f_flags |= IMAGE_FILE_DEBUG_STRIPPED;
  if (pe_data(abfd)->real_flags & IMAGE_FILE_LARGE_ADDRESS_AWARE)
    internal_f.f_flags |= IMAGE_FILE_LARGE_ADDRESS_AWARE;
#endif

#ifndef COFF_WITH_pex64
#ifdef COFF_WITH_PE
  internal_f.f_flags |= IMAGE_FILE_32BIT_MACHINE;
#else
  if (bfd_little_endian(abfd))
    internal_f.f_flags |= F_AR32WR;
  else
    internal_f.f_flags |= F_AR32W;
#endif
#endif

#ifdef TI_TARGET_ID
  /* Target id is used in TI COFF v1 and later; COFF0 won't use this field,
     but it doesn't hurt to set it internally.  */
  internal_f.f_target_id = TI_TARGET_ID;
#endif
#ifdef TIC80_TARGET_ID
  internal_f.f_target_id = TIC80_TARGET_ID;
#endif

  /* FIXME, should do something about the other byte orders and
     architectures.  */

#ifdef RS6000COFF_C
  if ((abfd->flags & DYNAMIC) != 0)
    internal_f.f_flags |= F_SHROBJ;
  if (bfd_get_section_by_name(abfd, _LOADER) != NULL)
    internal_f.f_flags |= F_DYNLOAD;
#endif

  memset(&internal_a, 0, sizeof internal_a);

  /* Set up architecture-dependent stuff.  */
  {
    unsigned int magic = 0;
    unsigned short flags = 0;

    coff_set_flags(abfd, &magic, &flags);
    internal_f.f_magic = magic;
    internal_f.f_flags |= flags;
    /* ...and the "opt"hdr...  */

#ifdef TICOFF_AOUT_MAGIC
    internal_a.magic = TICOFF_AOUT_MAGIC;
#define __A_MAGIC_SET__
#endif
#ifdef TIC80COFF
    internal_a.magic = TIC80_ARCH_MAGIC;
#define __A_MAGIC_SET__
#endif /* TIC80 */
#ifdef I860
    /* FIXME: What are the a.out magic numbers for the i860?  */
    internal_a.magic = 0;
#define __A_MAGIC_SET__
#endif /* I860 */
#ifdef I960
    internal_a.magic = (magic == I960ROMAGIC ? NMAGIC : OMAGIC);
#define __A_MAGIC_SET__
#endif /* I960 */
#if M88
#define __A_MAGIC_SET__
    internal_a.magic = PAGEMAGICBCS;
#endif /* M88 */

#if APOLLO_M68
#define __A_MAGIC_SET__
    internal_a.magic = APOLLO_COFF_VERSION_NUMBER;
#endif

#if defined(M68) || defined(WE32K) || defined(M68K)
#define __A_MAGIC_SET__
#if defined(LYNXOS)
    internal_a.magic = LYNXCOFFMAGIC;
#else
#if defined(TARG_AUX)
    internal_a.magic = (abfd->flags & D_PAGED   ? PAGEMAGICPEXECPAGED
                        : abfd->flags & WP_TEXT ? PAGEMAGICPEXECSWAPPED
                                                : PAGEMAGICEXECSWAPPED);
#else
#if defined(PAGEMAGICPEXECPAGED)
    internal_a.magic = PAGEMAGICPEXECPAGED;
#endif
#endif /* TARG_AUX */
#endif /* LYNXOS */
#endif /* M68 || WE32K || M68K */

#if defined(ARM)
#define __A_MAGIC_SET__
    internal_a.magic = ZMAGIC;
#endif

#if defined(PPC_PE)
#define __A_MAGIC_SET__
    internal_a.magic = IMAGE_NT_OPTIONAL_HDR_MAGIC;
#endif

#if defined MCORE_PE
#define __A_MAGIC_SET__
    internal_a.magic = IMAGE_NT_OPTIONAL_HDR_MAGIC;
#endif

#if defined(I386)
#define __A_MAGIC_SET__
#if defined LYNXOS
    internal_a.magic = LYNXCOFFMAGIC;
#elif defined AMD64
    internal_a.magic = IMAGE_NT_OPTIONAL_HDR64_MAGIC;
#else
    internal_a.magic = ZMAGIC;
#endif
#endif /* I386 */

#if defined(IA64)
#define __A_MAGIC_SET__
    internal_a.magic = PE32PMAGIC;
#endif /* IA64 */

#if defined(SPARC)
#define __A_MAGIC_SET__
#if defined(LYNXOS)
    internal_a.magic = LYNXCOFFMAGIC;
#endif /* LYNXOS */
#endif /* SPARC */

#ifdef RS6000COFF_C
#define __A_MAGIC_SET__
    internal_a.magic = (abfd->flags & D_PAGED)   ? RS6K_AOUTHDR_ZMAGIC
                       : (abfd->flags & WP_TEXT) ? RS6K_AOUTHDR_NMAGIC
                                                 : RS6K_AOUTHDR_OMAGIC;
#endif

#if defined(SH) && defined(COFF_WITH_PE)
#define __A_MAGIC_SET__
    internal_a.magic = SH_PE_MAGIC;
#endif

#if defined(MIPS) && defined(COFF_WITH_PE)
#define __A_MAGIC_SET__
    internal_a.magic = MIPS_PE_MAGIC;
#endif

#ifndef __A_MAGIC_SET__
#include "Your aouthdr magic number is not being set!"
#else
#undef __A_MAGIC_SET__
#endif
  }

  /* FIXME: Does anybody ever set this to another value?  */
  internal_a.vstamp = 0;

  /* Now should write relocs, strings, syms.  */
  obj_sym_filepos(abfd) = sym_base;

  if (bfd_get_symcount(abfd) != 0) {
    int firstundef;

    if (!coff_renumber_symbols(abfd, &firstundef))
      return FALSE;
    coff_mangle_symbols(abfd);
    if (!coff_write_symbols(abfd))
      return FALSE;
    if (!coff_write_linenumbers(abfd))
      return FALSE;
    if (!coff_write_relocs(abfd, firstundef))
      return FALSE;
  }
#ifdef COFF_LONG_SECTION_NAMES
  else if (long_section_names && !obj_coff_strings_written(abfd)) {
    /* If we have long section names we have to write out the string
       table even if there are no symbols.  */
    if (!coff_write_symbols(abfd))
      return FALSE;
  }
#endif
#ifdef COFF_IMAGE_WITH_PE
#ifdef PPC_PE
  else if ((abfd->flags & EXEC_P) != 0) {
    bfd_byte b;

    /* PowerPC PE appears to require that all executable files be
       rounded up to the page size.  */
    b = 0;
    if (bfd_seek(abfd, (file_ptr)BFD_ALIGN(sym_base, COFF_PAGE_SIZE) - 1,
                 SEEK_SET) != 0 ||
        bfd_bwrite(&b, (bfd_size_type)1, abfd) != 1)
      return FALSE;
  }
#endif
#endif

  /* If bfd_get_symcount (abfd) != 0, then we are not using the COFF
     backend linker, and obj_raw_syment_count is not valid until after
     coff_write_symbols is called.  */
  if (obj_raw_syment_count(abfd) != 0) {
    internal_f.f_symptr = sym_base;
#ifdef RS6000COFF_C
    /* AIX appears to require that F_RELFLG not be set if there are
       local symbols but no relocations.  */
    internal_f.f_flags &= ~F_RELFLG;
#endif
  } else {
    if (long_section_names)
      internal_f.f_symptr = sym_base;
    else
      internal_f.f_symptr = 0;
    internal_f.f_flags |= F_LSYMS;
  }

  if (text_sec) {
    internal_a.tsize = text_sec->size;
    internal_a.text_start = internal_a.tsize ? text_sec->vma : 0;
  }
  if (data_sec) {
    internal_a.dsize = data_sec->size;
    internal_a.data_start = internal_a.dsize ? data_sec->vma : 0;
  }
  if (bss_sec) {
    internal_a.bsize = bss_sec->size;
    if (internal_a.bsize && bss_sec->vma < internal_a.data_start)
      internal_a.data_start = bss_sec->vma;
  }

  internal_a.entry = bfd_get_start_address(abfd);
  internal_f.f_nsyms = obj_raw_syment_count(abfd);

#ifdef RS6000COFF_C
  if (xcoff_data(abfd)->full_aouthdr) {
    bfd_vma toc;
    asection *loader_sec;

    internal_a.vstamp = 1;

    internal_a.o_snentry = xcoff_data(abfd)->snentry;
    if (internal_a.o_snentry == 0)
      internal_a.entry = (bfd_vma)-1;

    if (text_sec != NULL) {
      internal_a.o_sntext = text_sec->target_index;
      internal_a.o_algntext = bfd_get_section_alignment(abfd, text_sec);
    } else {
      internal_a.o_sntext = 0;
      internal_a.o_algntext = 0;
    }
    if (data_sec != NULL) {
      internal_a.o_sndata = data_sec->target_index;
      internal_a.o_algndata = bfd_get_section_alignment(abfd, data_sec);
    } else {
      internal_a.o_sndata = 0;
      internal_a.o_algndata = 0;
    }
    loader_sec = bfd_get_section_by_name(abfd, ".loader");
    if (loader_sec != NULL)
      internal_a.o_snloader = loader_sec->target_index;
    else
      internal_a.o_snloader = 0;
    if (bss_sec != NULL)
      internal_a.o_snbss = bss_sec->target_index;
    else
      internal_a.o_snbss = 0;

    toc = xcoff_data(abfd)->toc;
    internal_a.o_toc = toc;
    internal_a.o_sntoc = xcoff_data(abfd)->sntoc;

    internal_a.o_modtype = xcoff_data(abfd)->modtype;
    if (xcoff_data(abfd)->cputype != -1)
      internal_a.o_cputype = xcoff_data(abfd)->cputype;
    else {
      switch (bfd_get_arch(abfd)) {
      case bfd_arch_rs6000:
        internal_a.o_cputype = 4;
        break;
      case bfd_arch_powerpc:
        if (bfd_get_mach(abfd) == bfd_mach_ppc)
          internal_a.o_cputype = 3;
        else
          internal_a.o_cputype = 1;
        break;
      default:
        abort();
      }
    }
    internal_a.o_maxstack = xcoff_data(abfd)->maxstack;
    internal_a.o_maxdata = xcoff_data(abfd)->maxdata;
  }
#endif

#ifdef COFF_WITH_PE
  {
    /* After object contents are finalized so we can compute a reasonable hash,
       but before header is written so we can update it to point to debug
       directory.  */
    struct pe_tdata *pe = pe_data(abfd);

    if (pe->build_id.after_write_object_contents != NULL)
      (*pe->build_id.after_write_object_contents)(abfd);
  }
#endif

  /* Now write header.  */
  if (bfd_seek(abfd, (file_ptr)0, SEEK_SET) != 0)
    return FALSE;

  {
    char *buff;
    bfd_size_type amount = bfd_coff_filhsz(abfd);

    buff = (char *)bfd_malloc(amount);
    if (buff == NULL)
      return FALSE;

    bfd_coff_swap_filehdr_out(abfd, &internal_f, buff);
    amount = bfd_bwrite(buff, amount, abfd);

    free(buff);

    if (amount != bfd_coff_filhsz(abfd))
      return FALSE;
  }

  if (abfd->flags & EXEC_P) {
    /* Note that peicode.h fills in a PEAOUTHDR, not an AOUTHDR.
       include/coff/pe.h sets AOUTSZ == sizeof (PEAOUTHDR)).  */
    char *buff;
    bfd_size_type amount = bfd_coff_aoutsz(abfd);

    buff = (char *)bfd_malloc(amount);
    if (buff == NULL)
      return FALSE;

    coff_swap_aouthdr_out(abfd, &internal_a, buff);
    amount = bfd_bwrite(buff, amount, abfd);

    free(buff);

    if (amount != bfd_coff_aoutsz(abfd))
      return FALSE;

#ifdef COFF_IMAGE_WITH_PE
    if (!coff_apply_checksum(abfd))
      return FALSE;
#endif
  }
#ifdef RS6000COFF_C
  else {
    AOUTHDR buff;
    size_t size;

    /* XCOFF seems to always write at least a small a.out header.  */
    coff_swap_aouthdr_out(abfd, &internal_a, &buff);
    if (xcoff_data(abfd)->full_aouthdr)
      size = bfd_coff_aoutsz(abfd);
    else
      size = SMALL_AOUTSZ;
    if (bfd_bwrite(&buff, (bfd_size_type)size, abfd) != size)
      return FALSE;
  }
#endif

  return TRUE;
}

bfd_boolean bfd_close(bfd *abfd) {
  if (bfd_write_p(abfd)) {
    if (!BFD_SEND_FMT(abfd, _bfd_write_contents, (abfd)))
      return FALSE;
  }

  return bfd_close_all_done(abfd);
}

static void coff_pointerize_aux(bfd *abfd, combined_entry_type *table_base,
                                combined_entry_type *symbol,
                                unsigned int indaux,
                                combined_entry_type *auxent) {
  unsigned int type = symbol->u.syment.n_type;
  unsigned int n_sclass = symbol->u.syment.n_sclass;

  BFD_ASSERT(symbol->is_sym);
  if (coff_backend_info(abfd)->_bfd_coff_pointerize_aux_hook) {
    if ((*coff_backend_info(abfd)->_bfd_coff_pointerize_aux_hook)(
            abfd, table_base, symbol, indaux, auxent))
      return;
  }

  /* Don't bother if this is a file or a section.  */
  if (n_sclass == C_STAT && type == T_NULL)
    return;
  if (n_sclass == C_FILE)
    return;

  BFD_ASSERT(!auxent->is_sym);
  /* Otherwise patch up.  */
#define N_TMASK coff_data(abfd)->local_n_tmask
#define N_BTSHFT coff_data(abfd)->local_n_btshft

  if ((ISFCN(type) || ISTAG(n_sclass) || n_sclass == C_BLOCK ||
       n_sclass == C_FCN) &&
      auxent->u.auxent.x_sym.x_fcnary.x_fcn.x_endndx.l > 0) {
    auxent->u.auxent.x_sym.x_fcnary.x_fcn.x_endndx.p =
        table_base + auxent->u.auxent.x_sym.x_fcnary.x_fcn.x_endndx.l;
    auxent->fix_end = 1;
  }
  /* A negative tagndx is meaningless, but the SCO 3.2v4 cc can
     generate one, so we must be careful to ignore it.  */
  if (auxent->u.auxent.x_sym.x_tagndx.l > 0) {
    auxent->u.auxent.x_sym.x_tagndx.p =
        table_base + auxent->u.auxent.x_sym.x_tagndx.l;
    auxent->fix_tag = 1;
  }
}

combined_entry_type *coff_get_normalized_symtab(bfd *abfd) {
  combined_entry_type *internal;
  combined_entry_type *internal_ptr;
  combined_entry_type *symbol_ptr;
  combined_entry_type *internal_end;
  size_t symesz;
  char *raw_src;
  char *raw_end;
  const char *string_table = NULL;
  asection *debug_sec = NULL;
  char *debug_sec_data = NULL;
  bfd_size_type size;

  if (obj_raw_syments(abfd) != NULL)
    return obj_raw_syments(abfd);

  if (!_bfd_coff_get_external_symbols(abfd))
    return NULL;

  size = obj_raw_syment_count(abfd) * sizeof(combined_entry_type);
  /* Check for integer overflow.  */
  if (size < obj_raw_syment_count(abfd))
    return NULL;
  internal = (combined_entry_type *)bfd_zalloc(abfd, size);
  if (internal == NULL && size != 0)
    return NULL;
  internal_end = internal + obj_raw_syment_count(abfd);

  raw_src = (char *)obj_coff_external_syms(abfd);

  /* Mark the end of the symbols.  */
  symesz = bfd_coff_symesz(abfd);
  raw_end = (char *)raw_src + obj_raw_syment_count(abfd) * symesz;

  /* FIXME SOMEDAY.  A string table size of zero is very weird, but
     probably possible.  If one shows up, it will probably kill us.  */

  /* Swap all the raw entries.  */
  for (internal_ptr = internal; raw_src < raw_end;
       raw_src += symesz, internal_ptr++) {
    unsigned int i;

    bfd_coff_swap_sym_in(abfd, (void *)raw_src,
                         (void *)&internal_ptr->u.syment);
    symbol_ptr = internal_ptr;
    internal_ptr->is_sym = TRUE;

    /* PR 17512: file: 1353-1166-0.004.  */
    if (symbol_ptr->u.syment.n_sclass == C_FILE &&
        symbol_ptr->u.syment.n_numaux > 0 &&
        raw_src + symesz + symbol_ptr->u.syment.n_numaux * symesz > raw_end) {
      bfd_release(abfd, internal);
      return NULL;
    }

    for (i = 0; i < symbol_ptr->u.syment.n_numaux; i++) {
      internal_ptr++;
      /* PR 17512: Prevent buffer overrun.  */
      if (internal_ptr >= internal_end) {
        bfd_release(abfd, internal);
        return NULL;
      }

      raw_src += symesz;
      bfd_coff_swap_aux_in(abfd, (void *)raw_src, symbol_ptr->u.syment.n_type,
                           symbol_ptr->u.syment.n_sclass, (int)i,
                           symbol_ptr->u.syment.n_numaux,
                           &(internal_ptr->u.auxent));

      internal_ptr->is_sym = FALSE;
      coff_pointerize_aux(abfd, internal, symbol_ptr, i, internal_ptr);
    }
  }

  /* Free the raw symbols, but not the strings (if we have them).  */
  obj_coff_keep_strings(abfd) = TRUE;
  if (!_bfd_coff_free_symbols(abfd))
    return NULL;

  for (internal_ptr = internal; internal_ptr < internal_end; internal_ptr++) {
    BFD_ASSERT(internal_ptr->is_sym);

    if (internal_ptr->u.syment.n_sclass == C_FILE &&
        internal_ptr->u.syment.n_numaux > 0) {
      combined_entry_type *aux = internal_ptr + 1;

      /* Make a file symbol point to the name in the auxent, since
         the text ".file" is redundant.  */
      BFD_ASSERT(!aux->is_sym);

      if (aux->u.auxent.x_file.x_n.x_zeroes == 0) {
        /* The filename is a long one, point into the string table.  */
        if (string_table == NULL) {
          string_table = _bfd_coff_read_string_table(abfd);
          if (string_table == NULL)
            return NULL;
        }

        if ((bfd_size_type)(aux->u.auxent.x_file.x_n.x_offset) >=
            obj_coff_strings_len(abfd))
          internal_ptr->u.syment._n._n_n._n_offset =
              (bfd_hostptr_t)_("<corrupt>");
        else
          internal_ptr->u.syment._n._n_n._n_offset =
              (bfd_hostptr_t)(string_table +
                              (aux->u.auxent.x_file.x_n.x_offset));
      } else {
        /* Ordinary short filename, put into memory anyway.  The
           Microsoft PE tools sometimes store a filename in
           multiple AUX entries.  */
        if (internal_ptr->u.syment.n_numaux > 1 && coff_data(abfd)->pe)
          internal_ptr->u.syment._n._n_n._n_offset = (bfd_hostptr_t)copy_name(
              abfd, aux->u.auxent.x_file.x_fname,
              internal_ptr->u.syment.n_numaux * symesz);
        else
          internal_ptr->u.syment._n._n_n._n_offset =
              ((bfd_hostptr_t)copy_name(abfd, aux->u.auxent.x_file.x_fname,
                                        (size_t)bfd_coff_filnmlen(abfd)));
      }
    } else {
      if (internal_ptr->u.syment._n._n_n._n_zeroes != 0) {
        /* This is a "short" name.  Make it long.  */
        size_t i;
        char *newstring;

        /* Find the length of this string without walking into memory
           that isn't ours.  */
        for (i = 0; i < 8; ++i)
          if (internal_ptr->u.syment._n._n_name[i] == '\0')
            break;

        newstring = (char *)bfd_zalloc(abfd, (bfd_size_type)(i + 1));
        if (newstring == NULL)
          return NULL;
        strncpy(newstring, internal_ptr->u.syment._n._n_name, i);
        internal_ptr->u.syment._n._n_n._n_offset = (bfd_hostptr_t)newstring;
        internal_ptr->u.syment._n._n_n._n_zeroes = 0;
      } else if (internal_ptr->u.syment._n._n_n._n_offset == 0)
        internal_ptr->u.syment._n._n_n._n_offset = (bfd_hostptr_t) "";
      else if (!bfd_coff_symname_in_debug(abfd, &internal_ptr->u.syment)) {
        /* Long name already.  Point symbol at the string in the
           table.  */
        if (string_table == NULL) {
          string_table = _bfd_coff_read_string_table(abfd);
          if (string_table == NULL)
            return NULL;
        }
        if (internal_ptr->u.syment._n._n_n._n_offset >=
                obj_coff_strings_len(abfd) ||
            string_table + internal_ptr->u.syment._n._n_n._n_offset <
                string_table)
          internal_ptr->u.syment._n._n_n._n_offset =
              (bfd_hostptr_t)_("<corrupt>");
        else
          internal_ptr->u.syment._n._n_n._n_offset =
              ((bfd_hostptr_t)(string_table +
                               internal_ptr->u.syment._n._n_n._n_offset));
      } else {
        /* Long name in debug section.  Very similar.  */
        if (debug_sec_data == NULL)
          debug_sec_data = build_debug_section(abfd, &debug_sec);
        if (debug_sec_data != NULL) {
          BFD_ASSERT(debug_sec != NULL);
          /* PR binutils/17512: Catch out of range offsets into the debug data.
           */
          if (internal_ptr->u.syment._n._n_n._n_offset > debug_sec->size ||
              debug_sec_data + internal_ptr->u.syment._n._n_n._n_offset <
                  debug_sec_data)
            internal_ptr->u.syment._n._n_n._n_offset =
                (bfd_hostptr_t)_("<corrupt>");
          else
            internal_ptr->u.syment._n._n_n._n_offset =
                (bfd_hostptr_t)(debug_sec_data +
                                internal_ptr->u.syment._n._n_n._n_offset);
        } else
          internal_ptr->u.syment._n._n_n._n_offset = (bfd_hostptr_t) "";
      }
    }
    internal_ptr += internal_ptr->u.syment.n_numaux;
  }

  obj_raw_syments(abfd) = internal;
  BFD_ASSERT(obj_raw_syment_count(abfd) ==
             (unsigned int)(internal_ptr - internal));

  return internal;
}

static bfd_boolean coff_slurp_symbol_table(bfd *abfd) {
  combined_entry_type *native_symbols;
  coff_symbol_type *cached_area;
  unsigned int *table_ptr;
  bfd_size_type amt;
  unsigned int number_of_symbols = 0;
  bfd_boolean ret = TRUE;

  if (obj_symbols(abfd))
    return TRUE;

  /* Read in the symbol table.  */
  if ((native_symbols = coff_get_normalized_symtab(abfd)) == NULL)
    return FALSE;

  /* Allocate enough room for all the symbols in cached form.  */
  amt = obj_raw_syment_count(abfd);
  amt *= sizeof(coff_symbol_type);
  cached_area = (coff_symbol_type *)bfd_alloc(abfd, amt);
  if (cached_area == NULL)
    return FALSE;

  amt = obj_raw_syment_count(abfd);
  amt *= sizeof(unsigned int);
  table_ptr = (unsigned int *)bfd_zalloc(abfd, amt);

  if (table_ptr == NULL)
    return FALSE;
  else {
    coff_symbol_type *dst = cached_area;
    unsigned int last_native_index = obj_raw_syment_count(abfd);
    unsigned int this_index = 0;

    while (this_index < last_native_index) {
      combined_entry_type *src = native_symbols + this_index;
      table_ptr[this_index] = number_of_symbols;

      dst->symbol.the_bfd = abfd;
      BFD_ASSERT(src->is_sym);
      dst->symbol.name = (char *)(src->u.syment._n._n_n._n_offset);
      /* We use the native name field to point to the cached field.  */
      src->u.syment._n._n_n._n_zeroes = (bfd_hostptr_t)dst;
      dst->symbol.section =
          coff_section_from_bfd_index(abfd, src->u.syment.n_scnum);
      dst->symbol.flags = 0;
      /* PR 17512: file: 079-7098-0.001:0.1.  */
      dst->symbol.value = 0;
      dst->done_lineno = FALSE;

      switch (src->u.syment.n_sclass) {
#ifdef I960
      case C_LEAFEXT:
        /* Fall through to next case.  */
#endif

      case C_EXT:
      case C_WEAKEXT:
#if defined ARM
      case C_THUMBEXT:
      case C_THUMBEXTFUNC:
#endif
#ifdef RS6000COFF_C
      case C_HIDEXT:
#if !defined _AIX52 && !defined AIX_WEAK_SUPPORT
      case C_AIX_WEAKEXT:
#endif
#endif
#ifdef C_SYSTEM
      case C_SYSTEM: /* System Wide variable.  */
#endif
#ifdef COFF_WITH_PE
      /* In PE, 0x68 (104) denotes a section symbol.  */
      case C_SECTION:
      /* In PE, 0x69 (105) denotes a weak external symbol.  */
      case C_NT_WEAK:
#endif
        switch (coff_classify_symbol(abfd, &src->u.syment)) {
        case COFF_SYMBOL_GLOBAL:
          dst->symbol.flags = BSF_EXPORT | BSF_GLOBAL;
#if defined COFF_WITH_PE
          /* PE sets the symbol to a value relative to the
             start of the section.  */
          dst->symbol.value = src->u.syment.n_value;
#else
          dst->symbol.value =
              (src->u.syment.n_value - dst->symbol.section->vma);
#endif
          if (ISFCN((src->u.syment.n_type)))
            /* A function ext does not go at the end of a
               file.  */
            dst->symbol.flags |= BSF_NOT_AT_END | BSF_FUNCTION;
          break;

        case COFF_SYMBOL_COMMON:
          dst->symbol.section = bfd_com_section_ptr;
          dst->symbol.value = src->u.syment.n_value;
          break;

        case COFF_SYMBOL_UNDEFINED:
          dst->symbol.section = bfd_und_section_ptr;
          dst->symbol.value = 0;
          break;

        case COFF_SYMBOL_PE_SECTION:
          dst->symbol.flags |= BSF_EXPORT | BSF_SECTION_SYM;
          dst->symbol.value = 0;
          break;

        case COFF_SYMBOL_LOCAL:
          dst->symbol.flags = BSF_LOCAL;
#if defined COFF_WITH_PE
          /* PE sets the symbol to a value relative to the
             start of the section.  */
          dst->symbol.value = src->u.syment.n_value;
#else
          dst->symbol.value =
              (src->u.syment.n_value - dst->symbol.section->vma);
#endif
          if (ISFCN((src->u.syment.n_type)))
            dst->symbol.flags |= BSF_NOT_AT_END | BSF_FUNCTION;
          break;
        }

#ifdef RS6000COFF_C
        /* A symbol with a csect entry should not go at the end.  */
        if (src->u.syment.n_numaux > 0)
          dst->symbol.flags |= BSF_NOT_AT_END;
#endif

#ifdef COFF_WITH_PE
        if (src->u.syment.n_sclass == C_NT_WEAK)
          dst->symbol.flags |= BSF_WEAK;

        if (src->u.syment.n_sclass == C_SECTION && src->u.syment.n_scnum > 0)
          dst->symbol.flags = BSF_LOCAL;
#endif
        if (src->u.syment.n_sclass == C_WEAKEXT
#ifdef RS6000COFF_C
            || src->u.syment.n_sclass == C_AIX_WEAKEXT
#endif
        )
          dst->symbol.flags |= BSF_WEAK;

        break;

      case C_STAT: /* Static.  */
#ifdef I960
      case C_LEAFSTAT: /* Static leaf procedure.  */
#endif
#if defined ARM
      case C_THUMBSTAT:     /* Thumb static.  */
      case C_THUMBLABEL:    /* Thumb label.  */
      case C_THUMBSTATFUNC: /* Thumb static function.  */
#endif
#ifdef RS6000COFF_C
      case C_DWARF: /* A label in a dwarf section.  */
      case C_INFO:  /* A label in a comment section.  */
#endif
      case C_LABEL: /* Label.  */
        if (src->u.syment.n_scnum == N_DEBUG)
          dst->symbol.flags = BSF_DEBUGGING;
        else
          dst->symbol.flags = BSF_LOCAL;

        /* Base the value as an index from the base of the
           section, if there is one.  */
        if (dst->symbol.section) {
#if defined COFF_WITH_PE
          /* PE sets the symbol to a value relative to the
             start of the section.  */
          dst->symbol.value = src->u.syment.n_value;
#else
          dst->symbol.value =
              (src->u.syment.n_value - dst->symbol.section->vma);
#endif
        } else
          dst->symbol.value = src->u.syment.n_value;
        break;

      case C_MOS:     /* Member of structure.  */
      case C_EOS:     /* End of structure.  */
      case C_REGPARM: /* Register parameter.  */
      case C_REG:     /* register variable.  */
                      /* C_AUTOARG conflicts with TI COFF C_UEXT.  */
#if !defined(TIC80COFF) && !defined(TICOFF)
#ifdef C_AUTOARG
      case C_AUTOARG: /* 960-specific storage class.  */
#endif
#endif
      case C_TPDEF: /* Type definition.  */
      case C_ARG:
      case C_AUTO:  /* Automatic variable.  */
      case C_FIELD: /* Bit field.  */
      case C_ENTAG: /* Enumeration tag.  */
      case C_MOE:   /* Member of enumeration.  */
      case C_MOU:   /* Member of union.  */
      case C_UNTAG: /* Union tag.  */
        dst->symbol.flags = BSF_DEBUGGING;
        dst->symbol.value = (src->u.syment.n_value);
        break;

      case C_FILE:   /* File name.  */
      case C_STRTAG: /* Structure tag.  */
#ifdef RS6000COFF_C
      case C_GSYM:
      case C_LSYM:
      case C_PSYM:
      case C_RSYM:
      case C_RPSYM:
      case C_STSYM:
      case C_TCSYM:
      case C_BCOMM:
      case C_ECOML:
      case C_ECOMM:
      case C_DECL:
      case C_ENTRY:
      case C_FUN:
      case C_ESTAT:
#endif
        dst->symbol.flags = BSF_DEBUGGING;
        dst->symbol.value = (src->u.syment.n_value);
        break;

#ifdef RS6000COFF_C
      case C_BINCL: /* Beginning of include file.  */
      case C_EINCL: /* Ending of include file.  */
        /* The value is actually a pointer into the line numbers
           of the file.  We locate the line number entry, and
           set the section to the section which contains it, and
           the value to the index in that section.  */
        {
          asection *sec;

          dst->symbol.flags = BSF_DEBUGGING;
          for (sec = abfd->sections; sec != NULL; sec = sec->next)
            if (sec->line_filepos <= (file_ptr)src->u.syment.n_value &&
                ((file_ptr)(sec->line_filepos +
                            sec->lineno_count * bfd_coff_linesz(abfd)) >
                 (file_ptr)src->u.syment.n_value))
              break;
          if (sec == NULL)
            dst->symbol.value = 0;
          else {
            dst->symbol.section = sec;
            dst->symbol.value = ((src->u.syment.n_value - sec->line_filepos) /
                                 bfd_coff_linesz(abfd));
            src->fix_line = 1;
          }
        }
        break;

      case C_BSTAT:
        dst->symbol.flags = BSF_DEBUGGING;

        /* The value is actually a symbol index.  Save a pointer
           to the symbol instead of the index.  FIXME: This
           should use a union.  */
        src->u.syment.n_value =
            (long)(intptr_t)(native_symbols + src->u.syment.n_value);
        dst->symbol.value = src->u.syment.n_value;
        src->fix_value = 1;
        break;
#endif

      case C_BLOCK: /* ".bb" or ".eb".  */
      case C_FCN:   /* ".bf" or ".ef" (or PE ".lf").  */
      case C_EFCN:  /* Physical end of function.  */
#if defined COFF_WITH_PE
        /* PE sets the symbol to a value relative to the start
           of the section.  */
        dst->symbol.value = src->u.syment.n_value;
        if (strcmp(dst->symbol.name, ".bf") != 0) {
          /* PE uses funny values for .ef and .lf; don't
             relocate them.  */
          dst->symbol.flags = BSF_DEBUGGING;
        } else
          dst->symbol.flags = BSF_DEBUGGING | BSF_DEBUGGING_RELOC;
#else
        /* Base the value as an index from the base of the
           section.  */
        dst->symbol.flags = BSF_LOCAL;
        dst->symbol.value = (src->u.syment.n_value - dst->symbol.section->vma);
#endif
        break;

      case C_STATLAB: /* Static load time label.  */
        dst->symbol.value = src->u.syment.n_value;
        dst->symbol.flags = BSF_GLOBAL;
        break;

      case C_NULL:
        /* PE DLLs sometimes have zeroed out symbols for some
           reason.  Just ignore them without a warning.  */
        if (src->u.syment.n_type == 0 && src->u.syment.n_value == 0 &&
            src->u.syment.n_scnum == 0)
          break;
#ifdef RS6000COFF_C
        /* XCOFF specific: deleted entry.  */
        if (src->u.syment.n_value == C_NULL_VALUE)
          break;
#endif
        /* Fall through.  */
      case C_EXTDEF:  /* External definition.  */
      case C_ULABEL:  /* Undefined label.  */
      case C_USTATIC: /* Undefined static.  */
#ifndef COFF_WITH_PE
      /* C_LINE in regular coff is 0x68.  NT has taken over this storage
         class to represent a section symbol.  */
      case C_LINE: /* line # reformatted as symbol table entry.  */
        /* NT uses 0x67 for a weak symbol, not C_ALIAS.  */
      case C_ALIAS: /* Duplicate tag.  */
#endif
        /* New storage classes for TI COFF.  */
#if defined(TIC80COFF) || defined(TICOFF)
      case C_UEXT: /* Tentative external definition.  */
#endif
      case C_EXTLAB: /* External load time label.  */
      default:
        _bfd_error_handler
            /* xgettext:c-format */
            (_("%B: Unrecognized storage class %d for %s symbol `%s'"), abfd,
             src->u.syment.n_sclass, dst->symbol.section->name,
             dst->symbol.name);
        ret = FALSE;
        /* Fall through.  */
      case C_HIDDEN: /* Ext symbol in dmert public lib.  */
        /* PR 20722: These symbols can also be generated by
           building DLLs with --gc-sections enabled.  */
        dst->symbol.flags = BSF_DEBUGGING;
        dst->symbol.value = (src->u.syment.n_value);
        break;
      }

      dst->native = src;
      dst->symbol.udata.i = 0;
      dst->lineno = NULL;

      this_index += (src->u.syment.n_numaux) + 1;
      dst++;
      number_of_symbols++;
    }
  }

  obj_symbols(abfd) = cached_area;
  obj_raw_syments(abfd) = native_symbols;

  bfd_get_symcount(abfd) = number_of_symbols;
  obj_convert(abfd) = table_ptr;
  /* Slurp the line tables for each section too.  */
  {
    asection *p;

    p = abfd->sections;
    while (p) {
      if (!coff_slurp_line_table(abfd, p))
        return FALSE;
      p = p->next;
    }
  }

  return ret;
}

long coff_get_symtab_upper_bound(bfd *abfd) {
  if (!coff_slurp_symbol_table(abfd))
    return -1;

  return (bfd_get_symcount(abfd) + 1) * (sizeof(coff_symbol_type *));
}

long coff_canonicalize_symtab(bfd *abfd, asymbol **alocation) {
  unsigned int counter;
  coff_symbol_type *symbase;
  coff_symbol_type **location = (coff_symbol_type **)alocation;

  if (!bfd_coff_slurp_symbol_table(abfd))
    return -1;

  symbase = obj_symbols(abfd);
  counter = bfd_get_symcount(abfd);
  while (counter-- > 0)
    *location++ = symbase++;

  *location = NULL;

  return bfd_get_symcount(abfd);
}

bfd_boolean bfd_set_symtab(bfd *abfd, asymbol **location,
                           unsigned int symcount) {
  if (abfd->format != bfd_object || bfd_read_p(abfd)) {
    bfd_set_error(bfd_error_invalid_operation);
    return FALSE;
  }

  bfd_get_outsymbols(abfd) = location;
  bfd_get_symcount(abfd) = symcount;
  return TRUE;
}

static bfd_boolean copy_object(bfd *ibfd, bfd *obfd,
                               const bfd_arch_info_type *input_arch) {
  bfd_vma start;
  long symcount;
  asection **osections = NULL;
  asection *osec;
  asection *gnu_debuglink_section = NULL;
  bfd_size_type *gaps = NULL;
  bfd_size_type max_gap = 0;
  long symsize;
  void *dhandle;
  enum bfd_architecture iarch;
  unsigned int imach;
  unsigned int c, i;

  if (ibfd->xvec->byteorder != obfd->xvec->byteorder &&
      ibfd->xvec->byteorder != BFD_ENDIAN_UNKNOWN &&
      obfd->xvec->byteorder != BFD_ENDIAN_UNKNOWN) {
    /* PR 17636: Call non-fatal so that we return to our parent who
       may need to tidy temporary files.  */
    non_fatal(_("Unable to change endianness of input file(s)"));
    return FALSE;
  }

  if (!bfd_set_format(obfd, bfd_get_format(ibfd))) {
    bfd_nonfatal_message(NULL, obfd, NULL, NULL);
    return FALSE;
  }

  if (ibfd->sections == NULL) {
    non_fatal(_("error: the input file '%s' has no sections"),
              bfd_get_archive_filename(ibfd));
    return FALSE;
  }

  if (ibfd->xvec->flavour != bfd_target_elf_flavour) {
    if ((do_debug_sections & compress) != 0 && do_debug_sections != compress) {
      non_fatal(_("--compress-debug-sections=[zlib|zlib-gnu|zlib-gabi] is "
                  "unsupported on `%s'"),
                bfd_get_archive_filename(ibfd));
      return FALSE;
    }

    if (do_elf_stt_common) {
      non_fatal(_("--elf-stt-common=[yes|no] is unsupported on `%s'"),
                bfd_get_archive_filename(ibfd));
      return FALSE;
    }
  }

  if (verbose)
    printf(_("copy from `%s' [%s] to `%s' [%s]\n"),
           bfd_get_archive_filename(ibfd), bfd_get_target(ibfd),
           bfd_get_filename(obfd), bfd_get_target(obfd));

  if (extract_symbol)
    start = 0;
  else {
    if (set_start_set)
      start = set_start;
    else
      start = bfd_get_start_address(ibfd);
    start += change_start;
  }

  /* Neither the start address nor the flags
     need to be set for a core file.  */
  if (bfd_get_format(obfd) != bfd_core) {
    flagword flags;

    flags = bfd_get_file_flags(ibfd);
    flags |= bfd_flags_to_set;
    flags &= ~bfd_flags_to_clear;
    flags &= bfd_applicable_file_flags(obfd);

    if (strip_symbols == STRIP_ALL)
      flags &= ~HAS_RELOC;

    if (!bfd_set_start_address(obfd, start) ||
        !bfd_set_file_flags(obfd, flags)) {
      bfd_nonfatal_message(NULL, ibfd, NULL, NULL);
      return FALSE;
    }
  }

  /* Copy architecture of input file to output file.  */
  iarch = bfd_get_arch(ibfd);
  imach = bfd_get_mach(ibfd);
  if (input_arch) {
    if (bfd_get_arch_info(ibfd) == NULL ||
        bfd_get_arch_info(ibfd)->arch == bfd_arch_unknown) {
      iarch = input_arch->arch;
      imach = input_arch->mach;
    } else
      non_fatal(_("Input file `%s' ignores binary architecture parameter."),
                bfd_get_archive_filename(ibfd));
  }
  if (!bfd_set_arch_mach(obfd, iarch, imach) &&
      (ibfd->target_defaulted || bfd_get_arch(ibfd) != bfd_get_arch(obfd))) {
    if (bfd_get_arch(ibfd) == bfd_arch_unknown)
      non_fatal(_("Unable to recognise the format of the input file `%s'"),
                bfd_get_archive_filename(ibfd));
    else
      non_fatal(
          _("Output file cannot represent architecture `%s'"),
          bfd_printable_arch_mach(bfd_get_arch(ibfd), bfd_get_mach(ibfd)));
    return FALSE;
  }

  if (!bfd_set_format(obfd, bfd_get_format(ibfd))) {
    bfd_nonfatal_message(NULL, ibfd, NULL, NULL);
    return FALSE;
  }

  if (bfd_get_flavour(obfd) == bfd_target_coff_flavour && bfd_pei_p(obfd)) {
    /* Set up PE parameters.  */
    pe_data_type *pe = pe_data(obfd);

    /* Copy PE parameters before changing them.  */
    if (ibfd->xvec->flavour == bfd_target_coff_flavour && bfd_pei_p(ibfd))
      pe->pe_opthdr = pe_data(ibfd)->pe_opthdr;

    if (pe_file_alignment != (bfd_vma)-1)
      pe->pe_opthdr.FileAlignment = pe_file_alignment;
    else
      pe_file_alignment = PE_DEF_FILE_ALIGNMENT;

    if (pe_heap_commit != (bfd_vma)-1)
      pe->pe_opthdr.SizeOfHeapCommit = pe_heap_commit;

    if (pe_heap_reserve != (bfd_vma)-1)
      pe->pe_opthdr.SizeOfHeapCommit = pe_heap_reserve;

    if (pe_image_base != (bfd_vma)-1)
      pe->pe_opthdr.ImageBase = pe_image_base;

    if (pe_section_alignment != (bfd_vma)-1)
      pe->pe_opthdr.SectionAlignment = pe_section_alignment;
    else
      pe_section_alignment = PE_DEF_SECTION_ALIGNMENT;

    if (pe_stack_commit != (bfd_vma)-1)
      pe->pe_opthdr.SizeOfStackCommit = pe_stack_commit;

    if (pe_stack_reserve != (bfd_vma)-1)
      pe->pe_opthdr.SizeOfStackCommit = pe_stack_reserve;

    if (pe_subsystem != -1)
      pe->pe_opthdr.Subsystem = pe_subsystem;

    if (pe_major_subsystem_version != -1)
      pe->pe_opthdr.MajorSubsystemVersion = pe_major_subsystem_version;

    if (pe_minor_subsystem_version != -1)
      pe->pe_opthdr.MinorSubsystemVersion = pe_minor_subsystem_version;

    if (pe_file_alignment > pe_section_alignment) {
      char file_alignment[20], section_alignment[20];

      sprintf_vma(file_alignment, pe_file_alignment);
      sprintf_vma(section_alignment, pe_section_alignment);
      non_fatal(_("warning: file alignment (0x%s) > section alignment (0x%s)"),

                file_alignment, section_alignment);
    }
  }

  if (isympp)
    free(isympp);

  if (osympp != isympp)
    free(osympp);

  isympp = NULL;
  osympp = NULL;

  symsize = bfd_get_symtab_upper_bound(ibfd);
  if (symsize < 0) {
    bfd_nonfatal_message(NULL, ibfd, NULL, NULL);
    return FALSE;
  }

  osympp = isympp = (asymbol **)xmalloc(symsize);
  symcount = bfd_canonicalize_symtab(ibfd, isympp);
  if (symcount < 0) {
    bfd_nonfatal_message(NULL, ibfd, NULL, NULL);
    return FALSE;
  }
  /* PR 17512: file:  d6323821
     If the symbol table could not be loaded do not pretend that we have
     any symbols.  This trips us up later on when we load the relocs.  */
  if (symcount == 0) {
    free(isympp);
    osympp = isympp = NULL;
  }

  /* BFD mandates that all output sections be created and sizes set before
     any output is done.  Thus, we traverse all sections multiple times.  */
  bfd_map_over_sections(ibfd, setup_section, obfd);

  if (!extract_symbol)
    setup_bfd_headers(ibfd, obfd);

  if (add_sections != NULL) {
    struct section_add *padd;
    struct section_list *pset;

    for (padd = add_sections; padd != NULL; padd = padd->next) {
      flagword flags;

      pset = find_section_list(padd->name, FALSE, SECTION_CONTEXT_SET_FLAGS);
      if (pset != NULL)
        flags = pset->flags | SEC_HAS_CONTENTS;
      else
        flags = SEC_HAS_CONTENTS | SEC_READONLY | SEC_DATA;

      /* bfd_make_section_with_flags() does not return very helpful
         error codes, so check for the most likely user error first.  */
      if (bfd_get_section_by_name(obfd, padd->name)) {
        bfd_nonfatal_message(NULL, obfd, NULL, _("can't add section '%s'"),
                             padd->name);
        return FALSE;
      } else {
        /* We use LINKER_CREATED here so that the backend hooks
           will create any special section type information,
           instead of presuming we know what we're doing merely
           because we set the flags.  */
        padd->section = bfd_make_section_with_flags(obfd, padd->name,
                                                    flags | SEC_LINKER_CREATED);
        if (padd->section == NULL) {
          bfd_nonfatal_message(NULL, obfd, NULL, _("can't create section `%s'"),
                               padd->name);
          return FALSE;
        }
      }

      if (!bfd_set_section_size(obfd, padd->section, padd->size)) {
        bfd_nonfatal_message(NULL, obfd, padd->section, NULL);
        return FALSE;
      }

      pset = find_section_list(padd->name, FALSE,
                               SECTION_CONTEXT_SET_VMA |
                                   SECTION_CONTEXT_ALTER_VMA);
      if (pset != NULL &&
          !bfd_set_section_vma(obfd, padd->section, pset->vma_val)) {
        bfd_nonfatal_message(NULL, obfd, padd->section, NULL);
        return FALSE;
      }

      pset = find_section_list(padd->name, FALSE,
                               SECTION_CONTEXT_SET_LMA |
                                   SECTION_CONTEXT_ALTER_LMA);
      if (pset != NULL) {
        padd->section->lma = pset->lma_val;

        if (!bfd_set_section_alignment(
                obfd, padd->section,
                bfd_section_alignment(obfd, padd->section))) {
          bfd_nonfatal_message(NULL, obfd, padd->section, NULL);
          return FALSE;
        }
      }
    }
  }

  if (update_sections != NULL) {
    struct section_add *pupdate;

    for (pupdate = update_sections; pupdate != NULL; pupdate = pupdate->next) {
      pupdate->section = bfd_get_section_by_name(ibfd, pupdate->name);
      if (pupdate->section == NULL) {
        non_fatal(_("error: %s not found, can't be updated"), pupdate->name);
        return FALSE;
      }

      osec = pupdate->section->output_section;
      if (!bfd_set_section_size(obfd, osec, pupdate->size)) {
        bfd_nonfatal_message(NULL, obfd, osec, NULL);
        return FALSE;
      }
    }
  }

  if (merge_notes) {
    /* This palaver is necessary because we must set the output
       section size first, before its contents are ready.  */
    osec = bfd_get_section_by_name(ibfd, GNU_BUILD_ATTRS_SECTION_NAME);
    if (osec && is_merged_note_section(ibfd, osec)) {
      bfd_size_type size;

      size = bfd_get_section_size(osec);
      if (size == 0) {
        bfd_nonfatal_message(NULL, ibfd, osec,
                             _("warning: note section is empty"));
        merge_notes = FALSE;
      } else if (!bfd_get_full_section_contents(ibfd, osec, &merged_notes)) {
        bfd_nonfatal_message(NULL, ibfd, osec,
                             _("warning: could not load note section"));
        free(merged_notes);
        merged_notes = NULL;
        merge_notes = FALSE;
      } else {
        merged_size = merge_gnu_build_notes(ibfd, osec, size, merged_notes);
        if (merged_size == size) {
          /* Merging achieves nothing.  */
          free(merged_notes);
          merged_notes = NULL;
          merge_notes = FALSE;
          merged_size = 0;
        } else {
          if (osec->output_section == NULL ||
              !bfd_set_section_size(obfd, osec->output_section, merged_size)) {
            bfd_nonfatal_message(NULL, obfd, osec,
                                 _("warning: failed to set merged notes size"));
            free(merged_notes);
            merged_notes = NULL;
            merge_notes = FALSE;
            merged_size = 0;
          }
        }
      }
    }
  }

  if (dump_sections != NULL) {
    struct section_add *pdump;

    for (pdump = dump_sections; pdump != NULL; pdump = pdump->next) {
      osec = bfd_get_section_by_name(ibfd, pdump->name);
      if (osec == NULL) {
        bfd_nonfatal_message(NULL, ibfd, NULL,
                             _("can't dump section '%s' - it does not exist"),
                             pdump->name);
        continue;
      }

      if ((bfd_get_section_flags(ibfd, osec) & SEC_HAS_CONTENTS) == 0) {
        bfd_nonfatal_message(NULL, ibfd, osec,
                             _("can't dump section - it has no contents"));
        continue;
      }

      bfd_size_type size = bfd_get_section_size(osec);
      if (size == 0) {
        bfd_nonfatal_message(NULL, ibfd, osec,
                             _("can't dump section - it is empty"));
        continue;
      }

      FILE *f;
      f = fopen(pdump->filename, FOPEN_WB);
      if (f == NULL) {
        bfd_nonfatal_message(pdump->filename, NULL, NULL,
                             _("could not open section dump file"));
        continue;
      }

      bfd_byte *contents;
      if (bfd_malloc_and_get_section(ibfd, osec, &contents)) {
        if (fwrite(contents, 1, size, f) != size) {
          non_fatal(_("error writing section contents to %s (error: %s)"),
                    pdump->filename, strerror(errno));
          free(contents);
          return FALSE;
        }
      } else
        bfd_nonfatal_message(NULL, ibfd, osec,
                             _("could not retrieve section contents"));

      fclose(f);
      free(contents);
    }
  }

  if (gnu_debuglink_filename != NULL) {
    /* PR 15125: Give a helpful warning message if
       the debuglink section already exists, and
       allow the rest of the copy to complete.  */
    if (bfd_get_section_by_name(obfd, ".gnu_debuglink")) {
      non_fatal(_("%s: debuglink section already exists"),
                bfd_get_filename(obfd));
      gnu_debuglink_filename = NULL;
    } else {
      gnu_debuglink_section =
          bfd_create_gnu_debuglink_section(obfd, gnu_debuglink_filename);

      if (gnu_debuglink_section == NULL) {
        bfd_nonfatal_message(NULL, obfd, NULL,
                             _("cannot create debug link section `%s'"),
                             gnu_debuglink_filename);
        return FALSE;
      }

      /* Special processing for PE format files.  We
         have no way to distinguish PE from COFF here.  */
      if (bfd_get_flavour(obfd) == bfd_target_coff_flavour) {
        bfd_vma debuglink_vma;
        asection *highest_section;

        /* The PE spec requires that all sections be adjacent and sorted
           in ascending order of VMA.  It also specifies that debug
           sections should be last.  This is despite the fact that debug
           sections are not loaded into memory and so in theory have no
           use for a VMA.

           This means that the debuglink section must be given a non-zero
           VMA which makes it contiguous with other debug sections.  So
           walk the current section list, find the section with the
           highest VMA and start the debuglink section after that one.  */
        for (osec = obfd->sections, highest_section = NULL; osec != NULL;
             osec = osec->next)
          if (osec->vma > 0 &&
              (highest_section == NULL || osec->vma > highest_section->vma))
            highest_section = osec;

        if (highest_section)
          debuglink_vma =
              BFD_ALIGN(highest_section->vma + highest_section->size,
                        /* FIXME: We ought to be using
                           COFF_PAGE_SIZE here or maybe
                           bfd_get_section_alignment() (if it
                           was set) but since this is for PE
                           and we know the required alignment
                           it is easier just to hard code it.  */
                        0x1000);
        else
          /* Umm, not sure what to do in this case.  */
          debuglink_vma = 0x1000;

        bfd_set_section_vma(obfd, gnu_debuglink_section, debuglink_vma);
      }
    }
  }

  c = bfd_count_sections(obfd);
  if (c != 0 && (gap_fill_set || pad_to_set)) {
    asection **set;

    /* We must fill in gaps between the sections and/or we must pad
       the last section to a specified address.  We do this by
       grabbing a list of the sections, sorting them by VMA, and
       increasing the section sizes as required to fill the gaps.
       We write out the gap contents below.  */

    osections = (asection **)xmalloc(c * sizeof(asection *));
    set = osections;
    bfd_map_over_sections(obfd, get_sections, &set);

    qsort(osections, c, sizeof(asection *), compare_section_lma);

    gaps = (bfd_size_type *)xmalloc(c * sizeof(bfd_size_type));
    memset(gaps, 0, c * sizeof(bfd_size_type));

    if (gap_fill_set) {
      for (i = 0; i < c - 1; i++) {
        flagword flags;
        bfd_size_type size;
        bfd_vma gap_start, gap_stop;

        flags = bfd_get_section_flags(obfd, osections[i]);
        if ((flags & SEC_HAS_CONTENTS) == 0 || (flags & SEC_LOAD) == 0)
          continue;

        size = bfd_section_size(obfd, osections[i]);
        gap_start = bfd_section_lma(obfd, osections[i]) + size;
        gap_stop = bfd_section_lma(obfd, osections[i + 1]);
        if (gap_start < gap_stop) {
          if (!bfd_set_section_size(obfd, osections[i],
                                    size + (gap_stop - gap_start))) {
            bfd_nonfatal_message(NULL, obfd, osections[i],
                                 _("Can't fill gap after section"));
            status = 1;
            break;
          }
          gaps[i] = gap_stop - gap_start;
          if (max_gap < gap_stop - gap_start)
            max_gap = gap_stop - gap_start;
        }
      }
    }

    if (pad_to_set) {
      bfd_vma lma;
      bfd_size_type size;

      lma = bfd_section_lma(obfd, osections[c - 1]);
      size = bfd_section_size(obfd, osections[c - 1]);
      if (lma + size < pad_to) {
        if (!bfd_set_section_size(obfd, osections[c - 1], pad_to - lma)) {
          bfd_nonfatal_message(NULL, obfd, osections[c - 1],
                               _("can't add padding"));
          status = 1;
        } else {
          gaps[c - 1] = pad_to - (lma + size);
          if (max_gap < pad_to - (lma + size))
            max_gap = pad_to - (lma + size);
        }
      }
    }
  }

  /* Symbol filtering must happen after the output sections
     have been created, but before their contents are set.  */
  dhandle = NULL;
  if (convert_debugging)
    dhandle = read_debugging_info(ibfd, isympp, symcount, FALSE);

  if (strip_symbols == STRIP_DEBUG || strip_symbols == STRIP_ALL ||
      strip_symbols == STRIP_UNNEEDED || strip_symbols == STRIP_NONDEBUG ||
      strip_symbols == STRIP_DWO || strip_symbols == STRIP_NONDWO ||
      discard_locals != LOCALS_UNDEF || localize_hidden ||
      htab_elements(strip_specific_htab) != 0 ||
      htab_elements(keep_specific_htab) != 0 ||
      htab_elements(localize_specific_htab) != 0 ||
      htab_elements(globalize_specific_htab) != 0 ||
      htab_elements(keepglobal_specific_htab) != 0 ||
      htab_elements(weaken_specific_htab) != 0 ||
      htab_elements(redefine_specific_htab) != 0 || prefix_symbols_string ||
      sections_removed || sections_copied || convert_debugging ||
      change_leading_char || remove_leading_char || section_rename_list ||
      weaken || add_symbols) {
    /* Mark symbols used in output relocations so that they
       are kept, even if they are local labels or static symbols.

       Note we iterate over the input sections examining their
       relocations since the relocations for the output sections
       haven't been set yet.  mark_symbols_used_in_relocations will
       ignore input sections which have no corresponding output
       section.  */
    if (strip_symbols != STRIP_ALL)
      bfd_map_over_sections(ibfd, mark_symbols_used_in_relocations, isympp);
    osympp =
        (asymbol **)xmalloc((symcount + add_symbols + 1) * sizeof(asymbol *));
    symcount = filter_symbols(ibfd, obfd, osympp, isympp, symcount);
  }

  if (convert_debugging && dhandle != NULL) {
    if (!write_debugging_info(obfd, dhandle, &symcount, &osympp)) {
      status = 1;
      return FALSE;
    }
  }

  bfd_set_symtab(obfd, osympp, symcount);

  /* This has to happen before section positions are set.  */
  bfd_map_over_sections(ibfd, copy_relocations_in_section, obfd);

  /* This has to happen after the symbol table has been set.  */
  bfd_map_over_sections(ibfd, copy_section, obfd);

  if (add_sections != NULL) {
    struct section_add *padd;

    for (padd = add_sections; padd != NULL; padd = padd->next) {
      if (!bfd_set_section_contents(obfd, padd->section, padd->contents, 0,
                                    padd->size)) {
        bfd_nonfatal_message(NULL, obfd, padd->section, NULL);
        return FALSE;
      }
    }
  }

  if (update_sections != NULL) {
    struct section_add *pupdate;

    for (pupdate = update_sections; pupdate != NULL; pupdate = pupdate->next) {
      osec = pupdate->section->output_section;
      if (!bfd_set_section_contents(obfd, osec, pupdate->contents, 0,
                                    pupdate->size)) {
        bfd_nonfatal_message(NULL, obfd, osec, NULL);
        return FALSE;
      }
    }
  }

  if (merge_notes) {
    osec = bfd_get_section_by_name(obfd, GNU_BUILD_ATTRS_SECTION_NAME);
    if (osec && is_merged_note_section(obfd, osec)) {
      if (!bfd_set_section_contents(obfd, osec, merged_notes, 0, merged_size)) {
        bfd_nonfatal_message(
            NULL, obfd, osec,
            _("error: failed to copy merged notes into output"));
        return FALSE;
      }
    } else if (!is_strip)
      bfd_nonfatal_message(NULL, obfd, osec,
                           _("could not find any mergeable note sections"));
    free(merged_notes);
    merged_notes = NULL;
    merge_notes = FALSE;
  }

  if (gnu_debuglink_filename != NULL) {
    if (!bfd_fill_in_gnu_debuglink_section(obfd, gnu_debuglink_section,
                                           gnu_debuglink_filename)) {
      bfd_nonfatal_message(NULL, obfd, NULL,
                           _("cannot fill debug link section `%s'"),
                           gnu_debuglink_filename);
      return FALSE;
    }
  }

  if (gap_fill_set || pad_to_set) {
    bfd_byte *buf;

    /* Fill in the gaps.  */
    if (max_gap > 8192)
      max_gap = 8192;
    buf = (bfd_byte *)xmalloc(max_gap);
    memset(buf, gap_fill, max_gap);

    c = bfd_count_sections(obfd);
    for (i = 0; i < c; i++) {
      if (gaps[i] != 0) {
        bfd_size_type left;
        file_ptr off;

        left = gaps[i];
        off = bfd_section_size(obfd, osections[i]) - left;

        while (left > 0) {
          bfd_size_type now;

          if (left > 8192)
            now = 8192;
          else
            now = left;

          if (!bfd_set_section_contents(obfd, osections[i], buf, off, now)) {
            bfd_nonfatal_message(NULL, obfd, osections[i], NULL);
            return FALSE;
          }

          left -= now;
          off += now;
        }
      }
    }
  }

  /* Allow the BFD backend to copy any private data it understands
     from the input BFD to the output BFD.  This is done last to
     permit the routine to look at the filtered symbol table, which is
     important for the ECOFF code at least.  */
  if (!bfd_copy_private_bfd_data(ibfd, obfd)) {
    bfd_nonfatal_message(NULL, obfd, NULL, _("error copying private BFD data"));
    return FALSE;
  }

  /* Switch to the alternate machine code.  We have to do this at the
     very end, because we only initialize the header when we create
     the first section.  */
  if (use_alt_mach_code != 0) {
    if (!bfd_alt_mach_code(obfd, use_alt_mach_code)) {
      non_fatal(_("this target does not support %lu alternative machine codes"),
                use_alt_mach_code);
      if (bfd_get_flavour(obfd) == bfd_target_elf_flavour) {
        non_fatal(
            _("treating that number as an absolute e_machine value instead"));
        elf_elfheader(obfd)->e_machine = use_alt_mach_code;
      } else
        non_fatal(_("ignoring the alternative value"));
    }
  }

  return TRUE;
}

static void copy_file(const char *input_filename, const char *output_filename,
                      const char *input_target, const char *output_target,
                      const bfd_arch_info_type *input_arch) {
  bfd *ibfd;
  char **obj_matching;
  char **core_matching;
  off_t size = get_file_size(input_filename);

  if (size < 1) {
    if (size == 0)
      non_fatal(_("error: the input file '%s' is empty"), input_filename);
    status = 1;
    return;
  }

  /* To allow us to do "strip *" without dying on the first
     non-object file, failures are nonfatal.  */
  ibfd = bfd_openr(input_filename, input_target);
  if (ibfd == NULL) {
    bfd_nonfatal_message(input_filename, NULL, NULL, NULL);
    status = 1;
    return;
  }

  switch (do_debug_sections) {
  case compress:
  case compress_zlib:
  case compress_gnu_zlib:
  case compress_gabi_zlib:
    ibfd->flags |= BFD_COMPRESS;
    /* Don't check if input is ELF here since this information is
       only available after bfd_check_format_matches is called.  */
    if (do_debug_sections != compress_gnu_zlib)
      ibfd->flags |= BFD_COMPRESS_GABI;
    break;
  case decompress:
    ibfd->flags |= BFD_DECOMPRESS;
    break;
  default:
    break;
  }

  switch (do_elf_stt_common) {
  case elf_stt_common:
    ibfd->flags |= BFD_CONVERT_ELF_COMMON | BFD_USE_ELF_STT_COMMON;
    break;
    break;
  case no_elf_stt_common:
    ibfd->flags |= BFD_CONVERT_ELF_COMMON;
    break;
  default:
    break;
  }

  if (bfd_check_format(ibfd, bfd_archive)) {
    bfd_boolean force_output_target;
    bfd *obfd;

    /* bfd_get_target does not return the correct value until
       bfd_check_format succeeds.  */
    if (output_target == NULL) {
      output_target = bfd_get_target(ibfd);
      force_output_target = FALSE;
    } else
      force_output_target = TRUE;

    obfd = bfd_openw(output_filename, output_target);
    if (obfd == NULL) {
      bfd_nonfatal_message(output_filename, NULL, NULL, NULL);
      status = 1;
      return;
    }
    /* This is a no-op on non-Coff targets.  */
    set_long_section_mode(obfd, ibfd, long_section_names);

    copy_archive(ibfd, obfd, output_target, force_output_target, input_arch);
  } else if (bfd_check_format_matches(ibfd, bfd_object, &obj_matching)) {
    bfd *obfd;
  do_copy:

    /* bfd_get_target does not return the correct value until
       bfd_check_format succeeds.  */
    if (output_target == NULL)
      output_target = bfd_get_target(ibfd);

    obfd = bfd_openw(output_filename, output_target);
    if (obfd == NULL) {
      bfd_nonfatal_message(output_filename, NULL, NULL, NULL);
      status = 1;
      return;
    }
    /* This is a no-op on non-Coff targets.  */
    set_long_section_mode(obfd, ibfd, long_section_names);

    if (!copy_object(ibfd, obfd, input_arch))
      status = 1;

    /* PR 17512: file: 0f15796a.
       If the file could not be copied it may not be in a writeable
       state.  So use bfd_close_all_done to avoid the possibility of
       writing uninitialised data into the file.  */
    if (!(status ? bfd_close_all_done(obfd) : bfd_close(obfd))) {
      status = 1;
      bfd_nonfatal_message(output_filename, NULL, NULL, NULL);
      return;
    }

    if (!bfd_close(ibfd)) {
      status = 1;
      bfd_nonfatal_message(input_filename, NULL, NULL, NULL);
      return;
    }
  } else {
    bfd_error_type obj_error = bfd_get_error();
    bfd_error_type core_error;

    if (bfd_check_format_matches(ibfd, bfd_core, &core_matching)) {
      /* This probably can't happen..  */
      if (obj_error == bfd_error_file_ambiguously_recognized)
        free(obj_matching);
      goto do_copy;
    }

    core_error = bfd_get_error();
    /* Report the object error in preference to the core error.  */
    if (obj_error != core_error)
      bfd_set_error(obj_error);

    bfd_nonfatal_message(input_filename, NULL, NULL, NULL);

    if (obj_error == bfd_error_file_ambiguously_recognized) {
      list_matching_formats(obj_matching);
      free(obj_matching);
    }
    if (core_error == bfd_error_file_ambiguously_recognized) {
      list_matching_formats(core_matching);
      free(core_matching);
    }

    status = 1;
  }
}
