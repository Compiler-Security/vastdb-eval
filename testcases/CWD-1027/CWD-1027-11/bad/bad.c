#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>

typedef struct z_stream_s {
    const unsigned char *next_in;
    unsigned int avail_in;
    unsigned long total_in;
    unsigned char *next_out;
    unsigned int avail_out;
    unsigned long total_out;
    char *msg;
    struct internal_state *state;
    void *zalloc;
    void *zfree;
    void *opaque;
    int data_type;
    unsigned long adler;
    unsigned long reserved;
} z_stream;

#define PNG_CONST const
#define PNG_RESTRICT restrict
#define PNG_EMPTY /* empty */
#define PNG_FUNCTION(type, name, args, attributes) attributes type name args
#define PNG_ALLOCATED __attribute__((__malloc__))
#define PNGAPI
#define PNGCAPI
#define PNG_IMPEXP
#define PNG_EXPORT_TYPE(type) PNG_IMPEXP type
#define PNG_LINKAGE_API
#define PNG_EXPORTA(ordinal, type, name, args, attributes) \
    PNG_FUNCTION(PNG_EXPORT_TYPE(type), (PNGAPI name), args, PNG_LINKAGE_API attributes)
#define PNG_EXPORT(ordinal, type, name, args) \
    PNG_EXPORTA(ordinal, type, name, args, PNG_EMPTY)
#define PNG_CALLBACK(type, name, args) type (PNGAPI name) args
#define PNG_USE_RESULT __attribute__((__warn_unused_result__))
#define PNG_NORETURN __attribute__((__noreturn__))
#define PNG_DEPRECATED __attribute__((__deprecated__))
#define PNG_PRIVATE __attribute__((__deprecated__))
#define PNG_SIZE_MAX ((size_t)(-1))
#define png_constcast(type, ptr) ((type)(ptr))
#define png_voidcast(type, ptr) ((type)(ptr))
#define PNG_UNUSED(param) (void)(param)

typedef unsigned char png_byte;
typedef png_byte * png_bytep;
typedef const png_byte * png_const_bytep;
typedef unsigned short png_uint_16;
typedef short png_int_16;
typedef unsigned int png_uint_32;
typedef int png_int_32;
typedef size_t png_size_t;
typedef ptrdiff_t png_ptrdiff_t;
typedef png_uint_32 png_alloc_size_t;
typedef png_int_32 png_fixed_point;
typedef void * png_voidp;
typedef const void * png_const_voidp;
typedef png_uint_32 * png_uint_32p;
typedef const png_uint_32 * png_const_uint_32p;
typedef char * png_charp;
typedef const char * png_const_charp;
typedef png_fixed_point * png_fixed_point_p;
typedef const png_fixed_point * png_const_fixed_point_p;
typedef size_t * png_size_tp;
typedef const size_t * png_const_size_tp;
typedef png_byte ** png_bytepp;
typedef png_uint_32 ** png_uint_32pp;
typedef png_int_32 ** png_int_32pp;
typedef png_uint_16 ** png_uint_16pp;
typedef png_int_16 ** png_int_16pp;
typedef const char ** png_const_charpp;
typedef char ** png_charpp;
typedef char *** png_charppp;
typedef unsigned char * png_bytep;
typedef const unsigned char * png_const_bytep;
typedef void *voidpf;
typedef void *voidp;

struct png_struct_def;
struct png_info_def;
struct png_row_info_struct;

typedef struct png_struct_def * png_structp;
typedef const struct png_struct_def * png_const_structp;
typedef struct png_struct_def * PNG_RESTRICT png_structrp;
typedef const struct png_struct_def * PNG_RESTRICT png_const_structrp;
typedef struct png_struct_def png_struct;
typedef struct png_info_def * png_infop;
typedef const struct png_info_def * png_const_infop;
typedef struct png_row_info_struct * png_row_infop;

struct png_unknown_chunk {
    png_byte name[5];
    png_byte *data;
    size_t size;
    png_byte location;
};
typedef struct png_unknown_chunk png_unknown_chunk;
typedef png_unknown_chunk *png_unknown_chunkp;

typedef struct png_color_8_struct {
    png_byte red;
    png_byte green;
    png_byte blue;
    png_byte gray;
} png_color_8, *png_colorp;
typedef struct png_color_16_struct {
    png_byte index;
    png_uint_16 red;
    png_uint_16 green;
    png_uint_16 blue;
    png_uint_16 gray;
} png_color_16, *png_color_16p;

typedef void (*png_error_ptr) (png_structp png_ptr, png_const_charp msg);
typedef void (*png_rw_ptr) (png_structp png_ptr, png_bytep data, png_size_t size);
typedef void (*png_flush_ptr) (png_structp png_ptr);
typedef void (*png_read_status_ptr) (png_structp png_ptr, png_uint_32 row);
typedef void (*png_write_status_ptr) (png_structp png_ptr, png_uint_32 row);
typedef void (*png_user_transform_ptr) (png_structp png_ptr, png_row_infop row_info, png_bytep row);
typedef void (*png_progressive_info_ptr) (png_structp png_ptr, png_infop info_ptr);
typedef void (*png_progressive_row_ptr) (png_structp png_ptr, png_bytep row, png_uint_32 row_num, int pass);
typedef void (*png_progressive_end_ptr) (png_structp png_ptr);
typedef void* (*png_malloc_ptr) (png_const_voidp ptr, png_alloc_size_t size);
typedef void (*png_free_ptr) (png_const_voidp ptr, png_voidp data);
typedef void (*png_longjmp_ptr) (jmp_buf env, int val);

#define PNG_LIBPNG_VER_STRING "1.6.47.git"
#define PNG_INFO_IDAT 0x0010
#define PNG_INFO_VALID 0x0020
#define PNG_FLAG_BENIGN_ERRORS_WARN 0x0008
#define PNG_FLAG_APP_WARNINGS_WARN 0x0010
#define PNG_SEQUENTIAL_READ_SUPPORTED
#define PNG_IDAT_READ_SIZE 4096
#define PNG_USER_WIDTH_MAX 0x7fffffff
#define PNG_USER_HEIGHT_MAX 0x7fffffff
#define PNG_USER_CHUNK_CACHE_MAX 0
#define PNG_READ_SUPPORTED
#define PNG_WRITE_SUPPORTED
#define PNG_SETJMP_SUPPORTED
#define PNG_USER_MEM_SUPPORTED
#define PNG_USER_LIMITS_SUPPORTED
#define PNG_BENIGN_READ_ERRORS_SUPPORTED
#define PNG_RELEASE_BUILD 1
#define PNG_IS_READ_STRUCT 0x01

typedef enum
{
   VERBOSE,        /* switches on all messages */
   INFORMATION,
   WARNINGS,       /* switches on warnings */
   LIBPNG_WARNING,
   APP_WARNING,
   ERRORS,         /* just errors */
   APP_FAIL,       /* continuable error - no need to longjmp */
   LIBPNG_ERROR,   /* this and higher cause a longjmp */
   LIBPNG_BUG,     /* erroneous behavior in libpng */
   APP_ERROR,      /* such as out-of-memory in a callback */
   QUIET,          /* no normal messages */
   USER_ERROR,     /* such as file-not-found */
   INTERNAL_ERROR
} error_level;
#define LEVEL_MASK      0xf   /* where the level is in 'options' */

#define EXHAUSTIVE      0x010 /* Test all combinations of active options */
#define STRICT          0x020 /* Fail on warnings as well as errors */
#define LOG             0x040 /* Log pass/fail to stdout */
#define CONTINUE        0x080 /* Continue on APP_FAIL errors */
#define SKIP_BUGS       0x100 /* Skip over known bugs */
#define LOG_SKIPPED     0x200 /* Log skipped bugs */
#define FIND_BAD_COMBOS 0x400 /* Attempt to deduce bad combos */
#define LIST_COMBOS     0x800 /* List combos by name */

/* Result masks apply to the result bits in the 'results' field below; these
 * bits are simple 1U<<error_level.  A pass requires either nothing worse than
 * warnings (--relaxes) or nothing worse than information (--strict)
 */
#define RESULT_STRICT(r)   (((r) & ~((1U<<WARNINGS)-1)) == 0)
#define RESULT_RELAXED(r)  (((r) & ~((1U<<ERRORS)-1)) == 0)

#define TRANSFORM_R 0x01
#define TTABLE_SIZE 64
#define VERBOSE 0x01
#define WARNINGS 0x02
#define ERRORS 0x03
#define QUIET 0x00
#define RELAXED 0x20000

PNG_FUNCTION(void, png_warning, (png_const_structrp png_ptr, png_const_charp msg), PNG_EMPTY);
PNG_FUNCTION(void, png_error, (png_const_structrp png_ptr, png_const_charp msg), PNG_NORETURN);
PNG_FUNCTION(void, png_set_mem_fn, (png_structrp png_ptr, png_voidp mem_ptr, png_malloc_ptr malloc_fn, png_free_ptr free_fn), PNG_EMPTY);
PNG_FUNCTION(void, png_set_error_fn, (png_structrp png_ptr, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn), PNG_EMPTY);
PNG_FUNCTION(int, png_user_version_check, (png_structrp png_ptr, png_const_charp user_png_ver), PNG_EMPTY);
PNG_FUNCTION(png_voidp, png_malloc_warn, (png_const_structrp png_ptr, png_alloc_size_t size), PNG_ALLOCATED);
PNG_FUNCTION(void, png_zalloc, (voidpf opaque, unsigned int items, unsigned int size), PNG_EMPTY);
PNG_FUNCTION(void, png_zfree, (voidpf opaque, voidp ptr), PNG_EMPTY);
PNG_FUNCTION(png_infop, png_create_info_struct, (png_const_structrp png_ptr), PNG_ALLOCATED);
PNG_FUNCTION(void, png_set_read_fn, (png_structrp png_ptr, png_voidp io_ptr, png_rw_ptr read_data_fn), PNG_EMPTY);
PNG_FUNCTION(void, png_read_png, (png_structrp png_ptr, png_infop info_ptr, int transforms, png_voidp params), PNG_EMPTY);
PNG_FUNCTION(png_bytep *, png_get_rows, (png_const_structrp png_ptr, png_const_infop info_ptr), PNG_EMPTY);
PNG_FUNCTION(png_uint_32, png_get_rowbytes, (png_const_structrp png_ptr, png_const_infop info_ptr), PNG_EMPTY);
PNG_FUNCTION(png_uint_32, png_get_valid, (png_const_structrp png_ptr, png_const_infop info_ptr, png_uint_32 flag), PNG_EMPTY);
PNG_FUNCTION(int, png_get_IHDR, (png_const_structrp png_ptr, png_const_infop info_ptr, png_uint_32 *width, png_uint_32 *height, int *bit_depth, int *color_type, int *interlace_method, int *compression_method, int *filter_method), PNG_EMPTY);
PNG_FUNCTION(void, png_set_user_limits, (png_structrp png_ptr, png_uint_32 user_width_max, png_uint_32 user_height_max), PNG_EMPTY);
PNG_FUNCTION(png_byte, png_get_bit_depth, (png_const_structrp png_ptr, png_const_infop info_ptr), PNG_EMPTY);
PNG_FUNCTION(png_byte, png_get_color_type, (png_const_structrp png_ptr, png_const_infop info_ptr), PNG_EMPTY);
PNG_FUNCTION(png_byte, png_get_interlace_type, (png_const_structrp png_ptr, png_const_infop info_ptr), PNG_EMPTY);

struct buffer {
    char dummy; /* minimal definition to avoid incomplete type */
};
struct display {
    jmp_buf *error_return;
    int options;
    int results;
    const char *filename;
    const char *operation;
    int transforms;
    png_structp read_pp;
    png_infop read_ip;
    png_structp original_pp;
    png_infop original_ip;
    png_bytep *original_rows;
    png_uint_32 original_rowbytes;
    png_uint_32 chunks;
    int active_transforms;
    int ignored_transforms;
    int color_type;
    int bit_depth;
    int interlace_method;
    int compression_method;
    int filter_method;
    png_uint_32 width;
    png_uint_32 height;
    struct buffer original_file;
    struct buffer written_file;
};
typedef struct display display;
typedef struct buffer buffer;
struct transform_info_struct {
    const char *name;
    int transform;
    png_uint_32 valid_chunks;
    int color_mask_required;
    int color_mask_absent;
    int bit_depths;
    int when;
};
extern struct transform_info_struct transform_info[];
extern int read_transforms;
extern int write_transforms;
extern int rw_transforms;
void display_clean_read(struct display *dp);
void display_error(png_structp pp, const char *msg);
void display_warning(png_structp pp, const char *msg);
void display_log(struct display *dp, int level, const char *fmt, ...);
void display_init(struct display *dp);
void display_cache_file(struct display *dp, const char *filename);
void display_clean(struct display *dp);
void display_destroy(struct display *dp);
void buffer_start_read(struct buffer *bp);
int compare_read(struct display *dp, int transforms);
void write_png(struct display *dp, png_infop ip, int transforms);
unsigned int first_transform(unsigned int active);
int is_combo(unsigned int transform);
int skip_transform(struct display *dp, unsigned int transform);
void validate_T(void);
void read_function(png_structp pp, png_bytep data, png_size_t size);

struct png_struct_def {
#ifdef PNG_SETJMP_SUPPORTED
    jmp_buf jmp_buf_local;
    png_longjmp_ptr longjmp_fn;
    jmp_buf *jmp_buf_ptr;
    size_t jmp_buf_size;
#endif
    png_error_ptr error_fn;
    png_error_ptr warning_fn;
    png_voidp error_ptr;
    png_rw_ptr write_data_fn;
    png_rw_ptr read_data_fn;
    png_voidp io_ptr;
    png_uint_32 mode;
    png_uint_32 flags;
    png_uint_32 transformations;
    png_uint_32 zowner;
    z_stream zstream;
    png_uint_32 chunks;
    png_uint_32 width;
    png_uint_32 height;
    png_uint_32 num_rows;
    png_uint_32 usr_width;
    size_t rowbytes;
    png_uint_32 iwidth;
    png_uint_32 row_number;
    png_uint_32 chunk_name;
    png_bytep prev_row;
    png_bytep row_buf;
    png_colorp palette;
    png_uint_16 num_palette;
    png_uint_16 num_trans;
    png_byte compression;
    png_byte filter;
    png_byte interlaced;
    png_byte pass;
    png_byte do_filter;
    png_byte color_type;
    png_byte bit_depth;
    png_byte usr_bit_depth;
    png_byte pixel_depth;
    png_byte channels;
    png_byte sig_bytes;
    png_byte maximum_pixel_depth;
    png_byte transformed_pixel_depth;
    png_bytep big_row_buf;
#ifdef PNG_USER_MEM_SUPPORTED
    png_voidp mem_ptr;
    png_malloc_ptr malloc_fn;
    png_free_ptr free_fn;
#endif
#ifdef PNG_USER_LIMITS_SUPPORTED
    png_uint_32 user_width_max;
    png_uint_32 user_height_max;
    png_uint_32 user_chunk_cache_max;
    png_alloc_size_t user_chunk_malloc_max;
#endif
#ifdef PNG_SEQUENTIAL_READ_SUPPORTED
    unsigned int IDAT_read_size;
#endif
};

struct png_info_def {
    png_uint_32 width;
    png_uint_32 height;
    png_uint_32 valid;
    size_t rowbytes;
    png_colorp palette;
    png_uint_16 num_palette;
    png_uint_16 num_trans;
    png_byte bit_depth;
    png_byte color_type;
    png_byte compression_type;
    png_byte filter_type;
    png_byte interlace_type;
    png_byte channels;
    png_byte pixel_depth;
    png_byte spare_byte;
    png_byte signature[8];
#ifdef PNG_INFO_IMAGE_SUPPORTED
    png_bytepp row_pointers;
#endif
};

PNG_FUNCTION(png_voidp /* PRIVATE */,
png_malloc_base,(png_const_structrp png_ptr, png_alloc_size_t size),
    PNG_EMPTY)
{
   /* Moved to png_malloc_base from png_malloc_default in 1.6.0; the DOS
    * allocators have also been removed in 1.6.0, so any 16-bit system now has
    * to implement a user memory handler.  This checks to be sure it isn't
    * called with big numbers.
    */
#  ifdef PNG_MAX_MALLOC_64K
      /* This is support for legacy systems which had segmented addressing
       * limiting the maximum allocation size to 65536.  It takes precedence
       * over PNG_SIZE_MAX which is set to 65535 on true 16-bit systems.
       *
       * TODO: libpng-1.8: finally remove both cases.
       */
      if (size > 65536U) return NULL;
#  endif

   /* This is checked too because the system malloc call below takes a (size_t).
    */
   if (size > PNG_SIZE_MAX) return NULL;

#  ifdef PNG_USER_MEM_SUPPORTED
      if (png_ptr != NULL && png_ptr->malloc_fn != NULL)
         return (png_voidp)png_ptr->malloc_fn(png_constcast(png_structrp,png_ptr), size);
#  else
      PNG_UNUSED(png_ptr)
#  endif

   /* Use the system malloc */
   return malloc((size_t)/*SAFE*/size); /* checked for truncation above */
}

PNG_FUNCTION(png_voidp,PNGAPI
png_malloc_warn,(png_const_structrp png_ptr, png_alloc_size_t size),
    PNG_ALLOCATED)
{
   if (png_ptr != NULL)
   {
      png_voidp ret = png_malloc_base(png_ptr, size);

      if (ret != NULL)
         return ret;

      png_warning(png_ptr, "Out of memory");
   }

   return NULL;
}

PNG_FUNCTION(png_structp /* PRIVATE */,
png_create_png_struct,(png_const_charp user_png_ver, png_voidp error_ptr,
    png_error_ptr error_fn, png_error_ptr warn_fn, png_voidp mem_ptr,
    png_malloc_ptr malloc_fn, png_free_ptr free_fn),PNG_ALLOCATED)
{
   png_struct create_struct;
#  ifdef PNG_SETJMP_SUPPORTED
      jmp_buf create_jmp_buf;
#  endif

   /* This temporary stack-allocated structure is used to provide a place to
    * build enough context to allow the user provided memory allocator (if any)
    * to be called.
    */
   memset(&create_struct, 0, (sizeof create_struct));

#  ifdef PNG_USER_LIMITS_SUPPORTED
      create_struct.user_width_max = PNG_USER_WIDTH_MAX;
      create_struct.user_height_max = PNG_USER_HEIGHT_MAX;

#     ifdef PNG_USER_CHUNK_CACHE_MAX
      create_struct.user_chunk_cache_max = PNG_USER_CHUNK_CACHE_MAX;
#     endif

#     if PNG_USER_CHUNK_MALLOC_MAX > 0 /* default to compile-time limit */
      create_struct.user_chunk_malloc_max = PNG_USER_CHUNK_MALLOC_MAX;

      /* No compile-time limit, so initialize to the system limit: */
#     elif defined PNG_MAX_MALLOC_64K /* legacy system limit */
      create_struct.user_chunk_malloc_max = 65536U;

#     else /* modern system limit SIZE_MAX (C99) */
      create_struct.user_chunk_malloc_max = PNG_SIZE_MAX;
#     endif
#  endif

   /* The following two API calls simply set fields in png_struct, so it is safe
    * to do them now even though error handling is not yet set up.
    */
#  ifdef PNG_USER_MEM_SUPPORTED
      png_set_mem_fn(&create_struct, mem_ptr, malloc_fn, free_fn);
#  else
      PNG_UNUSED(mem_ptr)
      PNG_UNUSED(malloc_fn)
      PNG_UNUSED(free_fn)
#  endif

   /* (*error_fn) can return control to the caller after the error_ptr is set,
    * this will result in a memory leak unless the error_fn does something
    * extremely sophisticated.  The design lacks merit but is implicit in the
    * API.
    */
   png_set_error_fn(&create_struct, error_ptr, error_fn, warn_fn);

#  ifdef PNG_SETJMP_SUPPORTED
      if (!setjmp(create_jmp_buf))
#  endif
      {
#  ifdef PNG_SETJMP_SUPPORTED
         /* Temporarily fake out the longjmp information until we have
          * successfully completed this function.  This only works if we have
          * setjmp() support compiled in, but it is safe - this stuff should
          * never happen.
          */
         create_struct.jmp_buf_ptr = &create_jmp_buf;
         create_struct.jmp_buf_size = 0; /*stack allocation*/
         create_struct.longjmp_fn = longjmp;
#  endif
         /* Call the general version checker (shared with read and write code):
          */
         if (png_user_version_check(&create_struct, user_png_ver) != 0)
         {
            png_structrp png_ptr = png_voidcast(png_structrp,
                png_malloc_warn(&create_struct, (sizeof *png_ptr)));

            if (png_ptr != NULL)
            {
               /* png_ptr->zstream holds a back-pointer to the png_struct, so
                * this can only be done now:
                */
               create_struct.zstream.zalloc = png_zalloc;
               create_struct.zstream.zfree = png_zfree;
               create_struct.zstream.opaque = png_ptr;

#              ifdef PNG_SETJMP_SUPPORTED
               /* Eliminate the local error handling: */
               create_struct.jmp_buf_ptr = NULL;
               create_struct.jmp_buf_size = 0;
               create_struct.longjmp_fn = 0;
#              endif

               *png_ptr = create_struct;

               /* This is the successful return point */
               return png_ptr;
            }
         }
      }

   /* A longjmp because of a bug in the application storage allocator or a
    * simple failure to allocate the png_struct.
    */
   return NULL;
}

PNG_FUNCTION(png_structp,PNGAPI
png_create_read_struct_2,(png_const_charp user_png_ver, png_voidp error_ptr,
    png_error_ptr error_fn, png_error_ptr warn_fn, png_voidp mem_ptr,
    png_malloc_ptr malloc_fn, png_free_ptr free_fn),PNG_ALLOCATED)
{
   png_structp png_ptr = png_create_png_struct(user_png_ver, error_ptr,
       error_fn, warn_fn, mem_ptr, malloc_fn, free_fn);

   if (png_ptr != NULL)
   {
      png_ptr->mode = PNG_IS_READ_STRUCT;

      /* Added in libpng-1.6.0; this can be used to detect a read structure if
       * required (it will be zero in a write structure.)
       */
#     ifdef PNG_SEQUENTIAL_READ_SUPPORTED
         png_ptr->IDAT_read_size = PNG_IDAT_READ_SIZE;
#     endif

#     ifdef PNG_BENIGN_READ_ERRORS_SUPPORTED
         png_ptr->flags |= PNG_FLAG_BENIGN_ERRORS_WARN;

         /* In stable builds only warn if an application error can be completely
          * handled.
          */
#        if PNG_RELEASE_BUILD
            png_ptr->flags |= PNG_FLAG_APP_WARNINGS_WARN;
#        endif
#     endif

      /* TODO: delay this, it can be done in png_init_io (if the app doesn't
       * do it itself) avoiding setting the default function if it is not
       * required.
       */
      png_set_read_fn(png_ptr, NULL, NULL);
   }

   return png_ptr;
}

PNG_FUNCTION(png_structp,PNGAPI
png_create_read_struct,(png_const_charp user_png_ver, png_voidp error_ptr,
    png_error_ptr error_fn, png_error_ptr warn_fn),PNG_ALLOCATED)
{
   return png_create_read_struct_2(user_png_ver, error_ptr, error_fn,
        warn_fn, NULL, NULL, NULL);
}

static void
read_png(struct display *dp, struct buffer *bp, const char *operation,
   int transforms)
{
   png_structp pp;
   png_infop   ip;

   /* This cleans out any previous read and sets operation and transforms to
    * empty.
    */
   display_clean_read(dp);

   if (operation != NULL) /* else this is a verify and do not overwrite info */
   {
      dp->operation = operation;
      dp->transforms = transforms;
   }

   dp->read_pp = pp = png_create_read_struct(PNG_LIBPNG_VER_STRING, dp,
      display_error, display_warning);
   if (pp == NULL)
      display_log(dp, LIBPNG_ERROR, "failed to create read struct");

   /* The png_read_png API requires us to make the info struct, but it does the
    * call to png_read_info.
    */
   dp->read_ip = ip = png_create_info_struct(pp);
   if (ip == NULL)
      display_log(dp, LIBPNG_ERROR, "failed to create info struct");

#  ifdef PNG_SET_USER_LIMITS_SUPPORTED
      /* Remove the user limits, if any */
      png_set_user_limits(pp, 0x7fffffff, 0x7fffffff);
#  endif

   /* Set the IO handling */
   buffer_start_read(bp);
   png_set_read_fn(pp, bp, read_function);

   png_read_png(pp, ip, transforms, NULL/*params*/);

#if 0 /* crazy debugging */
   {
      png_bytep pr = png_get_rows(pp, ip)[0];
      size_t rb = png_get_rowbytes(pp, ip);
      size_t cb;
      char c = ' ';

      fprintf(stderr, "%.4x %2d (%3lu bytes):", transforms, png_get_bit_depth(pp,ip), (unsigned long)rb);

      for (cb=0; cb<rb; ++cb)
         fputc(c, stderr), fprintf(stderr, "%.2x", pr[cb]), c='.';

      fputc('\n', stderr);
   }
#endif
}

static void
update_display(struct display *dp)
   /* called once after the first read to update all the info, original_pp and
    * original_ip must have been filled in.
    */
{
   png_structp pp;
   png_infop   ip;

   /* Now perform the initial read with a 0 transform. */
   read_png(dp, &dp->original_file, "original read", 0/*no transform*/);

   /* Move the result to the 'original' fields */
   dp->original_pp = pp = dp->read_pp, dp->read_pp = NULL;
   dp->original_ip = ip = dp->read_ip, dp->read_ip = NULL;

   dp->original_rowbytes = png_get_rowbytes(pp, ip);
   if (dp->original_rowbytes == 0)
      display_log(dp, LIBPNG_BUG, "png_get_rowbytes returned 0");

   dp->chunks = png_get_valid(pp, ip, 0xffffffff);
   if ((dp->chunks & PNG_INFO_IDAT) == 0) /* set by png_read_png */
      display_log(dp, LIBPNG_BUG, "png_read_png did not set IDAT flag");

   dp->original_rows = png_get_rows(pp, ip);
   if (dp->original_rows == NULL)
      display_log(dp, LIBPNG_BUG, "png_read_png did not create row buffers");

   if (!png_get_IHDR(pp, ip,
      &dp->width, &dp->height, &dp->bit_depth, &dp->color_type,
      &dp->interlace_method, &dp->compression_method, &dp->filter_method))
      display_log(dp, LIBPNG_BUG, "png_get_IHDR failed");

   /* 'active' transforms are discovered based on the original image format;
    * running one active transform can activate others.  At present the code
    * does not attempt to determine the closure.
    */
   {
      png_uint_32 chunks = dp->chunks;
      int active = 0, inactive = 0;
      int ct = dp->color_type;
      int bd = dp->bit_depth;
      unsigned int i;

      for (i=0; i<TTABLE_SIZE; ++i)
      {
         if (transform_info[i].name != NULL)
         {
            int transform = transform_info[i].transform;

            if ((transform_info[i].valid_chunks == 0 ||
                  (transform_info[i].valid_chunks & chunks) != 0) &&
               (transform_info[i].color_mask_required & ct) ==
                  transform_info[i].color_mask_required &&
               (transform_info[i].color_mask_absent & ct) == 0 &&
               (transform_info[i].bit_depths & bd) != 0 &&
               (transform_info[i].when & TRANSFORM_R) != 0)
               active |= transform;

            else if ((transform_info[i].when & TRANSFORM_R) != 0)
               inactive |= transform;
         }
      }

      /* Some transforms appear multiple times in the table; the 'active' status
       * is the logical OR of these and the inactive status must be adjusted to
       * take this into account.
       */
      inactive &= ~active;

      dp->active_transforms = active;
      dp->ignored_transforms = inactive; /* excluding write-only transforms */
   }
}

static void
test_one_file(struct display *dp, const char *filename)
{
   /* First cache the file and update the display original file
    * information for the new file.
    */
   dp->operation = "cache file";
   dp->transforms = 0;
   display_cache_file(dp, filename);
   update_display(dp);

   /* First test: if there are options that should be ignored for this file
    * verify that they really are ignored.
    */
   if (dp->ignored_transforms != 0)
   {
      read_png(dp, &dp->original_file, "ignored transforms",
         dp->ignored_transforms);

      /* The result should be identical to the original_rows */
      if (!compare_read(dp, 0/*transforms applied*/))
         return; /* no point testing more */
   }

#ifdef PNG_WRITE_PNG_SUPPORTED
   /* Second test: write the original PNG data out to a new file (to test the
    * write side) then read the result back in and make sure that it hasn't
    * changed.
    */
   dp->operation = "write";
   write_png(dp, dp->original_ip, 0/*transforms*/);
   read_png(dp, &dp->written_file, NULL, 0/*transforms*/);
   if (!compare_read(dp, 0/*transforms applied*/))
      return;
#endif

   /* Third test: the active options.  Test each in turn, or, with the
    * EXHAUSTIVE option, test all possible combinations.
    */
   {
      /* Use unsigned int here because the code below to increment through all
       * the possibilities exhaustively has to use a compare and that must be
       * unsigned, because some transforms are negative on a 16-bit system.
       */
      unsigned int active = dp->active_transforms;
      int exhaustive = (dp->options & EXHAUSTIVE) != 0;
      unsigned int current = first_transform(active);
      unsigned int bad_transforms = 0;
      unsigned int bad_combo = ~0U;    /* bitwise AND of failing transforms */
      unsigned int bad_combo_list = 0; /* bitwise OR of failures */

      for (;;)
      {
         read_png(dp, &dp->original_file, "active transforms", current);

         /* If this involved any irreversible transformations then if we write
          * it out with just the reversible transformations and read it in again
          * with the same transforms we should get the same thing.  At present
          * this isn't done - it just seems like a waste of time and it would
          * require two sets of read png_struct/png_info.
          *
          * If there were no irreversible transformations then if we write it
          * out and read it back in again (without the reversible transforms)
          * we should get back to the place where we started.
          */
#ifdef PNG_WRITE_PNG_SUPPORTED
         if ((current & write_transforms) == current)
         {
            /* All transforms reversible: write the PNG with the transformations
             * reversed, then read it back in with no transformations.  The
             * result should be the same as the original apart from the loss of
             * low order bits because of the SHIFT/sBIT transform.
             */
            dp->operation = "reversible transforms";
            write_png(dp, dp->read_ip, current);

            /* And if this is read back in, because all the transformations were
             * reversible, the result should be the same.
             */
            read_png(dp, &dp->written_file, NULL, 0);
            if (!compare_read(dp, current/*for the SHIFT/sBIT transform*/))
            {
               /* This set of transforms failed.  If a single bit is set - if
                * there is just one transform - don't include this in further
                * 'exhaustive' tests.  Notice that each transform is tested on
                * its own before testing combos in the exhaustive case.
                */
               if (is_combo(current))
               {
                  bad_combo &= current;
                  bad_combo_list |= current;
               }

               else
                  bad_transforms |= current;
            }
         }
#endif

         /* Now move to the next transform */
         if (exhaustive) /* all combinations */
         {
            unsigned int next = current;

            do
            {
               if (next == read_transforms) /* Everything tested */
                  goto combo;

               ++next;
            }  /* skip known bad combos if the relevant option is set; skip
                * combos involving known bad single transforms in all cases.
                */
            while (  (next & read_transforms) <= current
                  || (next & active) == 0 /* skip cases that do nothing */
                  || (next & bad_transforms) != 0
                  || skip_transform(dp, next));

            assert((next & read_transforms) == next);
            current = next;
         }

         else /* one at a time */
         {
            active &= ~current;

            if (active == 0)
               goto combo;

            current = first_transform(active);
         }
      }

combo:
      if (dp->options & FIND_BAD_COMBOS)
      {
         /* bad_combos identifies the combos that occur in all failing cases;
          * bad_combo_list identifies transforms that do not prevent the
          * failure.
          */
         if (bad_combo != ~0U)
            printf("%s[0x%x]: PROBLEM: 0x%x[0x%x] ANTIDOTE: 0x%x\n",
               dp->filename, active, bad_combo, bad_combo_list,
               rw_transforms & ~bad_combo_list);

         else
            printf("%s: no %sbad combos found\n", dp->filename,
               (dp->options & SKIP_BUGS) ? "additional " : "");
      }
   }
}

static int
do_test(struct display *dp, const char *file)
   /* Exists solely to isolate the setjmp clobbers */
{
   int ret = setjmp(dp->error_return);

   if (ret == 0)
   {
      test_one_file(dp, file);
      return 0;
   }

   else if (ret < ERRORS) /* shouldn't longjmp on warnings */
      display_log(dp, INTERNAL_ERROR, "unexpected return code %d", ret);

   return ret;
}

int
main(int argc, char **argv)
{
   /* For each file on the command line test it with a range of transforms */
   int option_end, ilog = 0;
   struct display d;

   validate_T();
   display_init(&d);

   for (option_end=1; option_end<argc; ++option_end)
   {
      const char *name = argv[option_end];

      if (strcmp(name, "--verbose") == 0)
         d.options = (d.options & ~LEVEL_MASK) | VERBOSE;

      else if (strcmp(name, "--warnings") == 0)
         d.options = (d.options & ~LEVEL_MASK) | WARNINGS;

      else if (strcmp(name, "--errors") == 0)
         d.options = (d.options & ~LEVEL_MASK) | ERRORS;

      else if (strcmp(name, "--quiet") == 0)
         d.options = (d.options & ~LEVEL_MASK) | QUIET;

      else if (strcmp(name, "--exhaustive") == 0)
         d.options |= EXHAUSTIVE;

      else if (strcmp(name, "--fast") == 0)
         d.options &= ~EXHAUSTIVE;

      else if (strcmp(name, "--strict") == 0)
         d.options |= STRICT;

      else if (strcmp(name, "--relaxed") == 0)
         d.options &= ~STRICT;

      else if (strcmp(name, "--log") == 0)
      {
         ilog = option_end; /* prevent display */
         d.options |= LOG;
      }

      else if (strcmp(name, "--nolog") == 0)
         d.options &= ~LOG;

      else if (strcmp(name, "--continue") == 0)
         d.options |= CONTINUE;

      else if (strcmp(name, "--stop") == 0)
         d.options &= ~CONTINUE;

      else if (strcmp(name, "--skip-bugs") == 0)
         d.options |= SKIP_BUGS;

      else if (strcmp(name, "--test-all") == 0)
         d.options &= ~SKIP_BUGS;

      else if (strcmp(name, "--log-skipped") == 0)
         d.options |= LOG_SKIPPED;

      else if (strcmp(name, "--nolog-skipped") == 0)
         d.options &= ~LOG_SKIPPED;

      else if (strcmp(name, "--find-bad-combos") == 0)
         d.options |= FIND_BAD_COMBOS;

      else if (strcmp(name, "--nofind-bad-combos") == 0)
         d.options &= ~FIND_BAD_COMBOS;

      else if (strcmp(name, "--list-combos") == 0)
         d.options |= LIST_COMBOS;

      else if (strcmp(name, "--nolist-combos") == 0)
         d.options &= ~LIST_COMBOS;

      else if (name[0] == '-' && name[1] == '-')
      {
         fprintf(stderr, "pngimage: %s: unknown option\n", name);
         return 99;
      }

      else
         break; /* Not an option */
   }

   {
      int i;
      int errors = 0;

      for (i=option_end; i<argc; ++i)
      {
         {
            int ret = do_test(&d, argv[i]);

            if (ret > QUIET) /* abort on user or internal error */
               return 99;
         }

         /* Here on any return, including failures, except user/internal issues
          */
         {
            int pass = (d.options & STRICT) ?
               RESULT_STRICT(d.results) : RESULT_RELAXED(d.results);

            if (!pass)
               ++errors;

            if (d.options & LOG)
            {
               int j;

               printf("%s: pngimage ", pass ? "PASS" : "FAIL");

               for (j=1; j<option_end; ++j)
                  if (j != ilog)
                     printf("%s ", argv[j]);

               printf("%s\n", d.filename);
            }
         }

         display_clean(&d);
      }

      /* Release allocated memory */
      display_destroy(&d);

      return errors != 0;
   }
}
