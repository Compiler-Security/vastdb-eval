#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>

/* ============================================================
 * Type definitions extracted from include/dwg.h
 * ============================================================ */

#ifndef EXPORT
#  define EXPORT __attribute__((visibility("default")))
#endif

typedef unsigned char BITCODE_RC;
typedef unsigned char BITCODE_B;
typedef unsigned char BITCODE_BB;
typedef uint16_t BITCODE_BS;
typedef int16_t BITCODE_BSd;
typedef uint16_t BITCODE_RS;
typedef uint32_t BITCODE_BL;
typedef int32_t BITCODE_BLd;
typedef uint32_t BITCODE_RL;
typedef int32_t BITCODE_RLd;
typedef uint64_t BITCODE_RLL;
typedef uint64_t BITCODE_BLL;
typedef double BITCODE_BD;
typedef double BITCODE_RD;
typedef unsigned char* BITCODE_TF;
typedef char* BITCODE_TV;
#define BITCODE_T BITCODE_TV
typedef BITCODE_RS* BITCODE_TU;
typedef int32_t BITCODE_UMC;

typedef enum DWG_VERSION_TYPE
{
  R_INVALID,
  R_1_1, R_1_2, R_1_3, R_1_4, R_1_402b,
  R_2_0, R_2_1, R_2_21, R_2_22, R_2_4, R_2_5, R_2_6,
  R_9, R_9c1, R_10, R_10c1, R_10c2,
  R_11, R_12, R_12c1,
  R_13, R_13c3,
  R_14,
  R_2000, R_2004, R_2007, R_2010, R_2013, R_2018, R_2021,
  R_AFTER
} Dwg_Version_Type;

typedef enum DWG_OBJECT_SUPERTYPE
{
  DWG_SUPERTYPE_ENTITY, DWG_SUPERTYPE_OBJECT
} Dwg_Object_Supertype;

typedef enum DWG_ERROR
{
  DWG_NOERR = 0,
  DWG_ERR_WRONGCRC         = 1,
  DWG_ERR_NOTYETSUPPORTED  = 1 << 1,
  DWG_ERR_UNHANDLEDCLASS   = 1 << 2,
  DWG_ERR_INVALIDTYPE      = 1 << 3,
  DWG_ERR_INVALIDHANDLE    = 1 << 4,
  DWG_ERR_INVALIDEED       = 1 << 5,
  DWG_ERR_VALUEOUTOFBOUNDS = 1 << 6,
  DWG_ERR_CLASSESNOTFOUND  = 1 << 7,
  DWG_ERR_SECTIONNOTFOUND  = 1 << 8,
  DWG_ERR_PAGENOTFOUND     = 1 << 9,
  DWG_ERR_INTERNALERROR    = 1 << 10,
  DWG_ERR_INVALIDDWG       = 1 << 11,
  DWG_ERR_IOERROR          = 1 << 12,
  DWG_ERR_OUTOFMEM         = 1 << 13,
} Dwg_Error;

typedef struct _dwg_handle
{
  BITCODE_RC code;
  BITCODE_RC size;
  unsigned long value;
  BITCODE_B is_global;
} Dwg_Handle;

typedef struct _dwg_object_ref
{
  struct _dwg_object* obj;
  Dwg_Handle handleref;
  unsigned long absolute_ref;
} Dwg_Object_Ref;

typedef Dwg_Object_Ref* BITCODE_H;

typedef struct _dwg_color
{
  BITCODE_BSd index;
  BITCODE_BS flag;
  BITCODE_BS raw;
  BITCODE_BL rgb;
  unsigned method;
  BITCODE_T  name;
  BITCODE_T  book_name;
  BITCODE_H  handle;
  BITCODE_BB alpha_type;
  BITCODE_RC alpha;
} Dwg_Color;

typedef Dwg_Color BITCODE_CMC;

typedef struct _dwg_time_bll
{
  BITCODE_BL days;
  BITCODE_BL ms;
  BITCODE_BD value;
} Dwg_Bitcode_TimeBLL;

typedef Dwg_Bitcode_TimeBLL BITCODE_TIMEBLL;
typedef Dwg_Bitcode_TimeBLL BITCODE_TIMERLL;

typedef struct _dwg_bitcode_2rd
{
  BITCODE_RD x;
  BITCODE_RD y;
} Dwg_Bitcode_2RD;

typedef struct _dwg_bitcode_2bd
{
  BITCODE_BD x;
  BITCODE_BD y;
} Dwg_Bitcode_2BD;

typedef struct _dwg_bitcode_3rd
{
  BITCODE_RD x;
  BITCODE_RD y;
  BITCODE_RD z;
} Dwg_Bitcode_3RD;

typedef struct _dwg_bitcode_3bd
{
  BITCODE_BD x;
  BITCODE_BD y;
  BITCODE_BD z;
} Dwg_Bitcode_3BD;

typedef Dwg_Bitcode_2RD  BITCODE_2RD;
typedef Dwg_Bitcode_2BD  BITCODE_2BD;
typedef Dwg_Bitcode_3RD  BITCODE_3RD;
typedef Dwg_Bitcode_3BD  BITCODE_3BD;

typedef struct _dwg_binary_chunk
{
  short size;
  int  codepage;
  union {
    char *data;
    void *wdata;
  } u;
} Dwg_Binary_Chunk;

typedef struct _dwg_resbuf
{
  short type;
  union
  {
    double  pt[3];
    char    i8;
    short   i16;
    int     i32;
    BITCODE_BLL i64;
    double  dbl;
    unsigned char hdl[8];
    Dwg_Handle h;
    struct _dwg_binary_chunk str;
  } value;
  struct _dwg_resbuf *nextrb;
} Dwg_Resbuf;

/* DWG_OBJECT_TYPE enum - must come before Dwg_Object */
typedef enum DWG_OBJECT_TYPE
{
  DWG_TYPE_UNUSED = 0x00,
  DWG_TYPE_TEXT = 0x01,
  DWG_TYPE_ATTRIB = 0x02,
  DWG_TYPE_ATTDEF = 0x03,
  DWG_TYPE_BLOCK = 0x04,
  DWG_TYPE_ENDBLK = 0x05,
  DWG_TYPE_SEQEND = 0x06,
  DWG_TYPE_INSERT = 0x07,
  DWG_TYPE_MINSERT = 0x08,
  DWG_TYPE_VERTEX_2D = 0x0a,
  DWG_TYPE_VERTEX_3D = 0x0b,
  DWG_TYPE_VERTEX_MESH = 0x0c,
  DWG_TYPE_VERTEX_PFACE = 0x0d,
  DWG_TYPE_VERTEX_PFACE_FACE = 0x0e,
  DWG_TYPE_POLYLINE_2D = 0x0f,
  DWG_TYPE_POLYLINE_3D = 0x10,
  DWG_TYPE_ARC = 0x11,
  DWG_TYPE_CIRCLE = 0x12,
  DWG_TYPE_LINE = 0x13,
  DWG_TYPE_POINT = 0x1b,
  DWG_TYPE__3DFACE = 0x1c,
  DWG_TYPE_POLYLINE_PFACE = 0x1d,
  DWG_TYPE_POLYLINE_MESH = 0x1e,
  DWG_TYPE_SOLID = 0x1f,
  DWG_TYPE_TRACE = 0x20,
  DWG_TYPE_SHAPE = 0x21,
  DWG_TYPE_VIEWPORT = 0x22,
  DWG_TYPE_ELLIPSE = 0x23,
  DWG_TYPE_SPLINE = 0x24,
  DWG_TYPE_3DSOLID = 0x25,
  DWG_TYPE_REGION = 0x26,
  DWG_TYPE_3DFACE = 0x27,
  DWG_TYPE_BODY = 0x28,
  DWG_TYPE_RAY = 0x29,
  DWG_TYPE_XLINE = 0x2a,
  DWG_TYPE_DICTIONARY = 0x2b,
  DWG_TYPE_OLEFRAME = 0x2c,
  DWG_TYPE_MTEXT = 0x2d,
  DWG_TYPE_LEADER = 0x2e,
  DWG_TYPE_TOLERANCE = 0x2f,
  DWG_TYPE_MLINE = 0x30,
  DWG_TYPE_BLOCK_CONTROL = 0x31,
  DWG_TYPE_BLOCK_HEADER = 0x32,
  DWG_TYPE_LAYER_CONTROL = 0x33,
  DWG_TYPE_LAYER = 0x34,
  DWG_TYPE_STYLE_CONTROL = 0x35,
  DWG_TYPE_STYLE = 0x36,
  DWG_TYPE_LTYPE_CONTROL = 0x37,
  DWG_TYPE_LTYPE = 0x38,
  DWG_TYPE_VIEW_CONTROL = 0x39,
  DWG_TYPE_VIEW = 0x3a,
  DWG_TYPE_UCS_CONTROL = 0x3b,
  DWG_TYPE_UCS = 0x3c,
  DWG_TYPE_VPORT_CONTROL = 0x3d,
  DWG_TYPE_VPORT = 0x3e,
  DWG_TYPE_APPID_CONTROL = 0x3f,
  DWG_TYPE_APPID = 0x40,
  DWG_TYPE_DIMSTYLE_CONTROL = 0x41,
  DWG_TYPE_DIMSTYLE = 0x42,
  DWG_TYPE_VX_CONTROL = 0x43,
  DWG_TYPE_VX_TABLE_RECORD = 0x44,
  DWG_TYPE_GROUP = 0x45,
  DWG_TYPE_MLEADERSTYLE = 0x46,
  DWG_TYPE_PLOTSETTINGS = 0x49,
  DWG_TYPE_LAYOUT = 0x4a,
  DWG_TYPE_VISUALSTYLE = 0x56,
  DWG_TYPE_MATERIAL = 0x58,
  DWG_TYPE_SCALE = 0x5b,
  DWG_TYPE_HELIX = 0x5d,
  DWG_TYPE_WIPEOUT = 0x5e,
  DWG_TYPE_UNDERLAY = 0x60,
  DWG_TYPE_UNDERLAYDEFINITION = 0x61,
  DWG_TYPE_FREED       = 0xfffd,
  DWG_TYPE_UNKNOWN_ENT = 0xfffe,
  DWG_TYPE_UNKNOWN_OBJ = 0xffff,
} Dwg_Object_Type;

/* Dwg_Eed_Data forward */
typedef struct _dwg_entity_eed_data Dwg_Eed_Data;

typedef struct _dwg_entity_eed
{
  BITCODE_BS size;
  Dwg_Handle handle;
  Dwg_Eed_Data *data;
  BITCODE_TF raw;
} Dwg_Eed;

/* Dwg_Object_Object forward */
typedef struct _dwg_object_object Dwg_Object_Object;

/* Dwg_Struct forward - needed by Dwg_Object_Entity */
typedef struct _dwg_struct Dwg_Struct;

typedef struct _dwg_object_entity
{
  BITCODE_BL objid;
  union
  {
    void *UNUSED;
    void *VIEWPORT;
  } tio;
  Dwg_Struct *dwg;
  BITCODE_BL num_eed;
  Dwg_Eed *eed;
  BITCODE_B   preview_exists;
  BITCODE_B   preview_is_proxy;
  BITCODE_BLL preview_size;
  BITCODE_TF preview;
  BITCODE_BB entmode;
  BITCODE_BL num_reactors;
  BITCODE_B is_xdic_missing;
  BITCODE_B isbylayerlt;
  BITCODE_B nolinks;
  BITCODE_B has_ds_data;
  BITCODE_CMC color;
  BITCODE_BD ltype_scale;
  BITCODE_BB ltype_flags;
  BITCODE_BB plotstyle_flags;
  BITCODE_BB material_flags;
  BITCODE_RC shadow_flags;
  BITCODE_B has_full_visualstyle;
  BITCODE_B has_face_visualstyle;
  BITCODE_B has_edge_visualstyle;
  BITCODE_BS invisible;
  BITCODE_RC linewt;
  BITCODE_RC flag_r11;
  BITCODE_RS kind_r11;
  BITCODE_RS opts_r11;
  BITCODE_RC extra_r11;
  BITCODE_RS layer_r11;
  BITCODE_RC color_r11;
  BITCODE_RS ltype_r11;
  BITCODE_RD elevation_r11;
  BITCODE_RD thickness_r11;
  BITCODE_RS paper_r11;
  BITCODE_BL __iterator;
  BITCODE_H ownerhandle;
  BITCODE_H* reactors;
  BITCODE_H xdicobjhandle;
  BITCODE_H prev_entity;
  BITCODE_H next_entity;
  BITCODE_H layer;
  BITCODE_H ltype;
  BITCODE_H material;
  BITCODE_H shadow;
  BITCODE_H plotstyle;
  BITCODE_H full_visualstyle;
  BITCODE_H face_visualstyle;
  BITCODE_H edge_visualstyle;
} Dwg_Object_Entity;

typedef struct _dwg_class
{
  BITCODE_BS number;
  BITCODE_BS proxyflag;
  char *appname;
  char *cppname;
  char *dxfname;
  BITCODE_TU dxfname_u;
  BITCODE_B  is_zombie;
  BITCODE_BS item_class_id;
  BITCODE_BL num_instances;
  BITCODE_BL dwg_version;
  BITCODE_BL maint_version;
  BITCODE_BL unknown_1;
  BITCODE_BL unknown_2;
} Dwg_Class;

typedef struct _dwg_object
{
  BITCODE_RL size;
  unsigned long address;
  unsigned int type;
  BITCODE_RL index;
  enum DWG_OBJECT_TYPE fixedtype;
  char *name;
  char *dxfname;
  Dwg_Object_Supertype supertype;
  union
  {
    Dwg_Object_Entity *entity;
    Dwg_Object_Object *object;
  } tio;
  Dwg_Handle handle;
  Dwg_Struct *parent;
  Dwg_Class *klass;
  BITCODE_RL bitsize;
  unsigned long bitsize_pos;
  unsigned long hdlpos;
  BITCODE_B  was_bitsize_set;
  BITCODE_B  has_strings;
  BITCODE_RL stringstream_size;
  BITCODE_UMC handlestream_size;
  unsigned long common_size;
  BITCODE_RL num_unknown_bits;
  BITCODE_TF unknown_bits;
} Dwg_Object;

typedef struct _dwg_chain
{
  unsigned char *chain;
  long unsigned int size;
  long unsigned int byte;
  unsigned char bit;
} Dwg_Chain;

/* Dwg_Header_Variables minimal stub */
typedef struct _dwg_header_variables {
  BITCODE_RL size;
  BITCODE_RL bitsize;
} Dwg_Header_Variables;

/* Dwg_Struct */
struct _dwg_struct
{
  unsigned int num_objects;
  Dwg_Object *object;
  unsigned int opts;
  Dwg_Header_Variables header_vars;
  Dwg_Version_Type version;
};

typedef struct _dwg_data
{
  Dwg_Struct parent;
  unsigned int opts;
  Dwg_Header_Variables header_vars;
  Dwg_Chain thumbnail;
  BITCODE_BL num_classes;
  Dwg_Class * dwg_class;
  BITCODE_BL num_objects;
  Dwg_Object * object;
  BITCODE_BL num_entities;
  BITCODE_BL num_object_refs;
  Dwg_Object_Ref **object_ref;
  void *object_map;
  int dirty_refs;
  Dwg_Version_Type version;
  Dwg_Version_Type from_version;
} Dwg_Data;

typedef struct _bit_chain
{
  unsigned char *chain;
  long unsigned int size;
  long unsigned int byte;
  unsigned char bit;
  unsigned char opts;
  Dwg_Version_Type version;
  Dwg_Version_Type from_version;
  FILE *fh;
} Bit_Chain;

typedef struct _dwg_entity_VIEWPORT
{
  struct _dwg_object_entity *parent;
  BITCODE_3BD center;
  BITCODE_BD width;
  BITCODE_BD height;
  BITCODE_RS on_off;
  BITCODE_RS id;
  BITCODE_3BD view_target;
  BITCODE_3BD VIEWDIR;
  BITCODE_BD twist_angle;
  BITCODE_BD VIEWSIZE;
  BITCODE_BD lens_length;
  BITCODE_BD front_clip_z;
  BITCODE_BD back_clip_z;
  BITCODE_BD SNAPANG;
  BITCODE_2RD VIEWCTR;
  BITCODE_2RD SNAPBASE;
  BITCODE_2RD SNAPUNIT;
  BITCODE_2RD GRIDUNIT;
  BITCODE_BS circle_zoom;
  BITCODE_BS grid_major;
  BITCODE_BL num_frozen_layers;
  BITCODE_BL status_flag;
  BITCODE_TV style_sheet;
  BITCODE_RC render_mode;
  BITCODE_B ucs_at_origin;
  BITCODE_B UCSVP;
  BITCODE_3BD ucsorg;
  BITCODE_3BD ucsxdir;
  BITCODE_3BD ucsydir;
  BITCODE_BD ucs_elevation;
  BITCODE_BS UCSORTHOVIEW;
  BITCODE_BS shadeplot_mode;
  BITCODE_B use_default_lights;
  BITCODE_RC default_lighting_type;
  BITCODE_BD brightness;
  BITCODE_BD contrast;
  BITCODE_CMC ambient_color;
  BITCODE_H vport_entity_header;
  BITCODE_H *frozen_layers;
  BITCODE_H clip_boundary;
  BITCODE_H named_ucs;
  BITCODE_H base_ucs;
  BITCODE_H background;
  BITCODE_H visualstyle;
  BITCODE_H shadeplot;
  BITCODE_H sun;
} Dwg_Entity_VIEWPORT;

/* ============================================================
 * Macros extracted from src/logging.h
 * ============================================================ */

#define DWG_LOGLEVEL_NONE 0
#define DWG_LOGLEVEL_ERROR 1
#define DWG_LOGLEVEL_INFO 2
#define DWG_LOGLEVEL_TRACE 3
#define DWG_LOGLEVEL_HANDLE 4
#define DWG_LOGLEVEL_INSANE 5
#define DWG_LOGLEVEL_ALL 9

static unsigned int loglevel;
#define DWG_LOGLEVEL loglevel

#define HANDLER fprintf
#define OUTPUT stderr

#define LOG(level, args...)                                                   \
  {                                                                           \
    if (DWG_LOGLEVEL >= DWG_LOGLEVEL_##level)                                 \
      {                                                                       \
        HANDLER (OUTPUT, args);                                               \
      }                                                                       \
  }

#define LOG_ERROR(args...)                                                    \
  {                                                                           \
    if (DWG_LOGLEVEL >= DWG_LOGLEVEL_ERROR)                                   \
      {                                                                       \
        HANDLER (OUTPUT, "ERROR: ");                                          \
        LOG (ERROR, args)                                                     \
        HANDLER (OUTPUT, "\n");                                               \
      }                                                                       \
  }

#define LOG_WARN(args...)                                                     \
  {                                                                           \
    if (DWG_LOGLEVEL >= DWG_LOGLEVEL_ERROR)                                   \
      {                                                                       \
        HANDLER (OUTPUT, "Warning: ");                                        \
        LOG (ERROR, args)                                                     \
        HANDLER (OUTPUT, "\n");                                               \
      }                                                                       \
  }

#define LOG_INFO(args...) LOG (INFO, args)
#define LOG_TRACE(args...) LOG (TRACE, args)
#define LOG_HANDLE(args...) LOG (HANDLE, args)
#define LOG_INSANE(args...) LOG (INSANE, args)
#define LOG_ALL(args...) LOG (ALL, args)

/* ============================================================
 * Macros extracted from src/common.h
 * ============================================================ */

#define VERSIONS(v1, v2)                                                      \
  cur_ver = v1;                                                               \
  if (dat->version >= v1 && dat->version <= v2)
#define OTHER_VERSIONS else
#define PRE(v)                                                                \
  cur_ver = v;                                                                \
  if (dat->version < v)
#define SINCE(v)                                                              \
  cur_ver = v;                                                                \
  if (dat->version >= v)
#define PRIOR_VERSIONS else
#define RESET_VER cur_ver = dat->version;

/* ============================================================
 * Macros from include/dwg.h
 * ============================================================ */

#define FORMAT_RL "%" PRIu32
#define FORMAT_H "%u.%u.%lX"
#define ARGS_H(hdl) (hdl).code, (hdl).size, (hdl).value

#define DWG_OPTS_LOGLEVEL 0xf
#define DWG_OPTS_INDXF    0x40
#define DWG_OPTS_INJSON   0x80
#define DWG_OPTS_IN       (DWG_OPTS_INDXF | DWG_OPTS_INJSON)

#define IS_FROM_TU(dat) dat->from_version >= R_2007 && !(dat->opts & DWG_OPTS_IN)

/* ============================================================
 * Macros from src/dec_macros.h (simplified)
 * ============================================================ */

#define FIELD_VALUE(nam) _obj->nam

#define FIELDG(nam, type, dxf)                                                \
  {                                                                           \
    _obj->nam = bit_read_##type (dat);                                        \
  }

#define FIELD_RS(nam, dxf) FIELDG (nam, RS, dxf)
#define FIELD_RD(nam, dxf)                                                    \
  {                                                                           \
    FIELDG (nam, RD, dxf);                                                    \
  }

/* ============================================================
 * CHK_OVERFLOW_PLUS from src/bits.c
 * ============================================================ */

#define CHK_OVERFLOW_PLUS(plus, func, retval)                                 \
    if (dat->byte + (plus) > dat->size)                                       \
      {                                                                       \
        loglevel = dat->opts & DWG_OPTS_LOGLEVEL;                             \
        LOG_ERROR ("%s buffer overflow at %lu + %d >= %lu", func, dat->byte,  \
                   (int)(plus), dat->size)                                    \
        return retval;                                                        \
      }

/* ============================================================
 * External function declarations from src/bits.h
 * ============================================================ */

void bit_advance_position (Bit_Chain *dat, long advance);
unsigned long bit_position (Bit_Chain *dat);
void bit_set_position (Bit_Chain *dat, unsigned long bitpos);
void bit_reset_chain (Bit_Chain *dat);

BITCODE_B bit_read_B (Bit_Chain *dat);
BITCODE_BB bit_read_BB (Bit_Chain *dat);
BITCODE_RC bit_read_RC (Bit_Chain *dat);
BITCODE_RS bit_read_RS (Bit_Chain *dat);
BITCODE_RL bit_read_RL (Bit_Chain *dat);
BITCODE_BL bit_read_BL (Bit_Chain *dat);
BITCODE_BLL bit_read_BLL (Bit_Chain *dat);
BITCODE_BD bit_read_BD (Bit_Chain *dat);
BITCODE_RD bit_read_RD (Bit_Chain *dat);
BITCODE_BS bit_read_BS (Bit_Chain *dat);
BITCODE_TU bit_read_TU (Bit_Chain *restrict dat);
BITCODE_TV bit_read_TV (Bit_Chain *restrict dat);
BITCODE_T bit_read_T (Bit_Chain *restrict dat);
int bit_read_H (Bit_Chain *restrict dat, Dwg_Handle *restrict handle);
int bit_read_CMC (Bit_Chain *dat, Bit_Chain *str_dat, Dwg_Color *restrict color);

/* ============================================================
 * External function declarations from src/decode.h
 * ============================================================ */

int obj_handle_stream (Bit_Chain *restrict dat, Dwg_Object *restrict obj,
                        Bit_Chain *restrict hdl_dat);
int obj_string_stream (Bit_Chain *dat, Dwg_Object *restrict obj,
                        Bit_Chain *str_dat);
Dwg_Object_Ref *dwg_decode_handleref (Bit_Chain *restrict hdl_dat,
                                       Dwg_Object *restrict obj,
                                       Dwg_Data *restrict dwg);
Dwg_Object_Ref *dwg_decode_handleref_with_code (Bit_Chain *restrict hdl_dat,
                                                 Dwg_Object *restrict obj,
                                                 Dwg_Data *restrict dwg,
                                                 unsigned int code);

/* ============================================================
 * External function declarations from include/dwg.h
 * ============================================================ */

EXPORT BITCODE_BS dwg_find_color_index (BITCODE_BL rgb);

/* ============================================================
 * External function declarations (internal to decode.c)
 * ============================================================ */

int dwg_decode_eed (Bit_Chain *restrict dat,
                     Dwg_Object_Object *restrict obj);
int dwg_decode_common_entity_handle_data (Bit_Chain *dat,
                                           Bit_Chain *hdl_dat,
                                           Dwg_Object *restrict obj);
int obj_has_strings (unsigned int type);

#define CHK_OVERFLOW(func, retval)                                            \
  if (dat->byte >= dat->size)                                                 \
    {                                                                         \
      loglevel = dat->opts & DWG_OPTS_LOGLEVEL;                               \
      LOG_ERROR ("%s buffer overflow at %lu >= %lu", func, dat->byte,         \
                 dat->size)                                                   \
      return retval;                                                          \
    }

BITCODE_BB
bit_read_BB (Bit_Chain *dat)
{
  unsigned char result;
  unsigned char byte;

  CHK_OVERFLOW (__FUNCTION__, 0)
  byte = dat->chain[dat->byte];
  if (dat->bit < 7)
    result = (byte & (0xc0 >> dat->bit)) >> (6 - dat->bit);
  else
    {
      result = (byte & 0x01) << 1;
      if (dat->byte < dat->size - 1)
        {
          byte = dat->chain[dat->byte + 1];
          result |= (byte & 0x80) >> 7;
        }
    }

  bit_advance_position (dat, 2);
  return result;
}

BITCODE_BS
bit_read_BS (Bit_Chain *dat)
{
  const unsigned char two_bit_code = bit_read_BB (dat);
  if (two_bit_code == 0)
    {
      CHK_OVERFLOW (__FUNCTION__, 0)
      return bit_read_RS (dat);
    }
  else if (two_bit_code == 1)
    return (BITCODE_BS)bit_read_RC (dat) & 0xFF;
  else if (two_bit_code == 2)
    return 0;
  else /* if (two_bit_code == 3) */
    return 256;
}

BITCODE_TU
bit_read_TU (Bit_Chain *restrict dat)
{
  unsigned int i;
  unsigned int length;
  BITCODE_TU chain;

  CHK_OVERFLOW_PLUS (1, __FUNCTION__, NULL)
  length = bit_read_BS (dat);
  CHK_OVERFLOW_PLUS (length * 2, __FUNCTION__, NULL)
  chain = (BITCODE_TU)malloc ((length + 1) * 2);
  if (!chain)
    {
      loglevel = dat->opts & DWG_OPTS_LOGLEVEL;
      LOG_ERROR ("Out of memory")
      return NULL;
    }
  for (i = 0; i < length; i++)
    chain[i] = bit_read_RS (dat); // probably without byte swapping
  // normally not needed, as the DWG itself contains the ending 0 as last char
  // but we enforce writing it.
  chain[length] = 0;
  return chain;
}

BITCODE_T
bit_read_T (Bit_Chain *restrict dat)
{
  if (IS_FROM_TU (dat))
    return (BITCODE_T)bit_read_TU (dat);
  else
    return (BITCODE_T)bit_read_TV (dat);
}

int
bit_read_CMC (Bit_Chain *dat, Bit_Chain *str_dat, Dwg_Color *restrict color)
{
  memset (color, 0, sizeof (Dwg_Color));
  color->index = bit_read_BS (dat);
  if (dat->from_version >= R_2004) // truecolor
    {
      CHK_OVERFLOW_PLUS (1, __FUNCTION__, DWG_ERR_VALUEOUTOFBOUNDS)
      color->rgb = bit_read_BL (dat);
      CHK_OVERFLOW_PLUS (1, __FUNCTION__, DWG_ERR_VALUEOUTOFBOUNDS)
      color->method = color->rgb >> 0x18;
      color->flag = bit_read_RC (dat);
      CHK_OVERFLOW_PLUS (0, __FUNCTION__, DWG_ERR_VALUEOUTOFBOUNDS)
      if (color->flag < 4)
        {
          color->name
              = (color->flag & 1) ? (char *)bit_read_T (str_dat) : NULL;
          color->book_name
              = (color->flag & 2) ? (char *)bit_read_T (str_dat) : NULL;
        }
      else
        {
          LOG_ERROR ("Invalid CMC flag 0x%x ignored", color->flag);
          color->flag = 0;
        }
      if (color->method < 0xc0 || color->method > 0xc8)
        {
          LOG_ERROR ("Invalid CMC method 0x%x ignored", color->method);
          color->method = 0xc2;
          color->rgb = 0xc2000000 | (color->rgb & 0xffffff);
        }
      // fixup index by palette lookup
      color->index = dwg_find_color_index (color->rgb);
    }
  return 0;
}

unsigned long
bit_position (Bit_Chain *dat)
{
  return (dat->byte * 8) + (dat->bit & 7);
}

void
bit_advance_position (Bit_Chain *dat, long advance)
{
  const unsigned long pos = bit_position (dat);
  const unsigned long endpos = dat->size * 8;
  long bits = (long)dat->bit + advance;
  if (pos + advance > endpos)
    {
      loglevel = dat->opts & DWG_OPTS_LOGLEVEL;
      LOG_ERROR ("%s buffer overflow at pos %lu.%u, size %lu, advance by %ld",
                 __FUNCTION__, dat->byte, dat->bit, dat->size, advance);
    }
  else if ((long)pos + advance < 0)
    {
      loglevel = dat->opts & DWG_OPTS_LOGLEVEL;
      LOG_ERROR ("buffer underflow at pos %lu.%u, size %lu, advance by %ld",
                 dat->byte, dat->bit, dat->size, advance)
      dat->byte = 0;
      dat->bit = 0;
      return;
    }
  dat->byte += (bits >> 3);
  dat->bit = bits & 7;
}

int
obj_string_stream (Bit_Chain *restrict dat, Dwg_Object *restrict obj,
                   Bit_Chain *restrict str)
{
  BITCODE_RL start = obj->bitsize - 1; // in bits
  BITCODE_RL data_size = 0;            // in byte
  BITCODE_RL old_size;                 // in byte
  BITCODE_RL old_byte;
  assert (dat != str);
  old_size = str->size;
  old_byte = str->byte;

  str->chain += str->byte;
  // obj->strpos = str->byte * 8 + str->bit;

  str->byte = 0;
  str->bit = 0;
  str->size = (obj->bitsize / 8) + ((obj->bitsize % 8) ? 1 : 0);
  bit_advance_position (str, start - 8);

  if (str->byte >= old_size - old_byte)
    {
      LOG_ERROR ("obj_string_stream overflow, bitsize " FORMAT_RL " => " FORMAT_RL,
                 obj->bitsize, obj->size * 8);
      str->byte = old_byte;
      str->size = old_size;
      obj->has_strings = 0;
      obj->bitsize = obj->size * 8;
      return DWG_ERR_VALUEOUTOFBOUNDS;
    }
  LOG_HANDLE (" obj string stream +%u: @%lu.%u (%lu)", start, str->byte,
              str->bit & 7, bit_position (str));
  obj->has_strings = bit_read_B (str);
  LOG_TRACE (" has_strings: %d\n", (int)obj->has_strings);
  if (!obj->has_strings)
    {
      // FIXME wrong bit
      if (obj->fixedtype == DWG_TYPE_SCALE)
        obj->has_strings = 1;
      // str->size = 0;
      // bit_reset_chain (str);
      return 0;
    }

  bit_advance_position (str, -1); //-17
  str->byte -= 2;
  LOG_HANDLE (" @%lu.%u", str->byte, str->bit & 7);
  data_size = (BITCODE_RL)bit_read_RS (str);
  LOG_HANDLE (" data_size: %u/0x%x", data_size, data_size);

  if (data_size & 0x8000)
    {
      BITCODE_RS hi_size;
      bit_advance_position (str, -1); //-33
      str->byte -= 4;
      data_size &= 0x7FFF;
      hi_size = bit_read_RS (str);
      data_size |= (hi_size << 15);
      LOG_HANDLE (" data_size: %u/0x%x\n", data_size, data_size);
      // LOG_TRACE("  -33: @%lu\n", str->byte);
    }
  else
    LOG_HANDLE ("\n");
  str->byte -= 2;
  if (data_size > obj->bitsize)
    {
      LOG_WARN ("Invalid string stream data_size: @%lu.%u\n", str->byte,
                str->bit & 7);
      obj->has_strings = 0;
      bit_reset_chain (str);
      return DWG_ERR_NOTYETSUPPORTED; // a very low severity error
    }
  obj->stringstream_size = data_size;
  bit_advance_position (str, -(int)data_size);
  // bit_reset_chain (str);
  // LOG_TRACE(" %d: @%lu.%u (%lu)\n", -(int)data_size - 16, str->byte,
  // str->bit & 7,
  //          bit_position(str));
  // obj->strpos = obj->bitsize_pos + obj->bitsize - obj->stringstream_size;
  return 0;
}

static int
dwg_decode_entity (Bit_Chain *dat, Bit_Chain *hdl_dat, Bit_Chain *str_dat,
                   Dwg_Object_Entity *restrict ent)
{
  unsigned int i;
  int error = 0;
  Dwg_Data *dwg = (Dwg_Data *)ent->dwg;
  Dwg_Object *obj = &dwg->object[ent->objid];
  Dwg_Object_Entity *_obj = ent;
  Dwg_Class *klass = NULL;
  unsigned long objectpos = bit_position (dat);
  int has_wrong_bitsize = 0;
  Dwg_Version_Type cur_ver;

  // obj->dat_address = dat->byte; // the data stream offset
  obj->bitsize_pos = objectpos; // absolute. needed for encode
  PRE (R_13)
  {
    if (FIELD_VALUE (flag_r11) & 4 && FIELD_VALUE (kind_r11) > 2
        && FIELD_VALUE (kind_r11) != 22)
      FIELD_RD (elevation_r11, 30);
    if (FIELD_VALUE (flag_r11) & 8)
      FIELD_RD (thickness_r11, 39);
    if (FIELD_VALUE (flag_r11) & 0x20)
      {
        Dwg_Object_Ref *hdl
            = dwg_decode_handleref_with_code (dat, obj, dwg, 0);
        if (hdl)
          obj->handle = hdl->handleref;
      }
    if (FIELD_VALUE (extra_r11) & 4)
      FIELD_RS (paper_r11, 0);
  }

  VERSIONS (R_2000, R_2007)
  {
    obj->bitsize = bit_read_RL (dat); // until the handles
    LOG_TRACE ("bitsize: " FORMAT_RL " [RL] @%lu.%u\n", obj->bitsize,
               dat->byte-2, dat->bit);
    if (obj->bitsize > obj->size * 8)
      {
        LOG_ERROR ("Invalid bitsize " FORMAT_RL " > " FORMAT_RL, obj->bitsize,
                   obj->size * 8);
        obj->bitsize = obj->size * 8;
        has_wrong_bitsize = 1;
        error |= DWG_ERR_VALUEOUTOFBOUNDS;
      }
    else
      error |= obj_handle_stream (dat, obj, hdl_dat);
  }
  SINCE (R_2007)
  {
    SINCE (R_2010)
      LOG_HANDLE (" bitsize: " FORMAT_RL ",", obj->bitsize);
    // restrict the hdl_dat stream
    error |= obj_handle_stream (dat, obj, hdl_dat);
    // and set the string stream (restricted to size)
    // skip for all types without strings
    if (obj->type >= 500 || obj_has_strings (obj->type))
      error |= obj_string_stream (dat, obj, str_dat);
    else
      {
        str_dat->chain += str_dat->byte;
        str_dat->byte = 0;
        str_dat->bit = 0;
        bit_advance_position (str_dat, obj->bitsize - 1 - 8);
        str_dat->size = 0;
      }
  }

  error |= bit_read_H (dat, &(obj->handle));
  if (error & DWG_ERR_INVALIDHANDLE || !obj->handle.value || !obj->handle.size
      || obj->handle.code)
    {
      LOG_ERROR ("Invalid object handle " FORMAT_H " at pos @%lu.%u",
                 ARGS_H (obj->handle), dat->byte, dat->bit);
      // TODO reconstruct the handle and search in the bitsoup?
      if (has_wrong_bitsize)
        obj->bitsize = 0;
      ent->num_eed = 0;
      ent->preview_exists = 0;
      return error | DWG_ERR_INVALIDHANDLE;
    }
  LOG_TRACE ("handle: " FORMAT_H " [H 5]", ARGS_H (obj->handle))
  LOG_INSANE (" @%lu.%u", dat->byte, dat->bit)
  LOG_TRACE ("\n")
  PRE (R_13) { return DWG_ERR_NOTYETSUPPORTED; }

  if (has_wrong_bitsize)
    LOG_WARN ("Skip eed")
  else
    error |= dwg_decode_eed (dat, (Dwg_Object_Object *)ent);
  LOG_INSANE (" @%lu.%u\n", dat->byte, dat->bit)
  //LOG_TRACE ("\n")
  if (error & (DWG_ERR_INVALIDEED | DWG_ERR_VALUEOUTOFBOUNDS))
    return error;

  /* common_entity_data.spec inlined (simplified) */
  SINCE (R_13) {
    if (ent->preview_exists)
      {
        VERSIONS (R_13, R_2007)
          {
            ent->preview_size = bit_read_RL (dat);
          }
        SINCE (R_2010)
          {
            ent->preview_size = bit_read_BLL (dat);
          }
      }
  }

  VERSIONS (R_13, R_14)
    {
      obj->bitsize = bit_read_RL (dat);
    }

  ent->entmode = bit_read_BB (dat);
  ent->num_reactors = bit_read_BL (dat);

  VERSIONS (R_13, R_14)
    {
      ent->isbylayerlt = bit_read_B (dat);
    }
  SINCE (R_2004)
    {
      ent->is_xdic_missing = bit_read_B (dat);
    }
  PRE (R_2004)
    {
      ent->nolinks = bit_read_B (dat);
    }
  SINCE (R_2013)
    {
      ent->has_ds_data = bit_read_B (dat);
    }

  SINCE (R_2004)
    {
      BITCODE_BS flags;
      ent->color.raw = bit_read_RS (dat);
      ent->color.flag = ent->color.raw >> 8;
      ent->color.index = ent->color.raw & 0x1ff;
      ent->color.rgb = 0L;
      flags = ent->color.flag;
      if (flags & 0x20)
        {
          ent->color.alpha = bit_read_BL (dat);
          ent->color.alpha_type = ent->color.alpha >> 24;
          ent->color.alpha = ent->color.alpha & 0xFF;
        }
      if (flags & 0x40)
        {
          /* color.handle skipped */
        }
      else if (flags & 0x80)
        {
          ent->color.rgb = bit_read_BL (dat);
        }
    }

  ent->ltype_scale = bit_read_BD (dat);
  SINCE (R_2000)
    {
      ent->ltype_flags = bit_read_BB (dat);
      ent->plotstyle_flags = bit_read_BB (dat);
    }
  SINCE (R_2007)
    {
      ent->material_flags = bit_read_BB (dat);
      ent->shadow_flags = bit_read_RC (dat);
    }
  SINCE (R_2010)
    {
      ent->has_full_visualstyle = bit_read_B (dat);
      ent->has_face_visualstyle = bit_read_B (dat);
      ent->has_edge_visualstyle = bit_read_B (dat);
    }

  ent->invisible = bit_read_BS (dat);

  SINCE (R_2000) {
    ent->linewt = bit_read_RC (dat);
  }

  dwg_decode_common_entity_handle_data (dat, hdl_dat, obj);

  // elsewhere: object data, handles, padding bits, crc
  obj->common_size = bit_position (dat) - objectpos;
  LOG_HANDLE ("--common_size: %lu\n", obj->common_size); // needed for unknown

  return error;
}

static int
dwg_decode_VIEWPORT_private (Bit_Chain *dat, Bit_Chain *hdl_dat,
                             Bit_Chain *str_dat, Dwg_Object *obj)
{
  int error = 0;
  Dwg_Object_Entity *_ent = obj->tio.entity;
  Dwg_Entity_VIEWPORT *_obj = obj->tio.entity->tio.VIEWPORT;

  _ent->dwg = obj->parent;
  _ent->objid = obj->index;
  _obj->parent = _ent;

  error = dwg_decode_entity (dat, hdl_dat, str_dat, _ent);
  if (error >= DWG_ERR_CLASSESNOTFOUND)
    return error;

  if (obj->has_strings)
    error |= bit_read_CMC (dat, str_dat, &_obj->ambient_color);

  return error & ~DWG_ERR_UNHANDLEDCLASS;
}
