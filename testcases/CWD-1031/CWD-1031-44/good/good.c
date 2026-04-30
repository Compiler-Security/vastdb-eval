#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <getopt.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

#ifndef M_PI
#  define M_PI 3.14159265358979323846264338327950288
#endif
#ifndef M_PI_2
#  define M_PI_2 1.57079632679489661923132169163975144
#endif

#define BITCODE_DOUBLE double

typedef unsigned char BITCODE_RC;
#ifdef _WIN32
# define FORMAT_RC "0x%2x"
#else
# define FORMAT_RC "0x%hhx"
#endif
#define FORMAT_RCd "%d"
#define FORMAT_RCu "%u"
typedef signed char BITCODE_RCd;
typedef unsigned char BITCODE_RCu;
typedef unsigned char BITCODE_B;
#define FORMAT_B "%d"
typedef unsigned char BITCODE_BB;
#define FORMAT_BB "%u"
typedef unsigned char BITCODE_3B;
#define FORMAT_3B "%u"
typedef uint16_t BITCODE_BS;
typedef int16_t BITCODE_BSd;
typedef uint16_t BITCODE_RS;
typedef uint16_t BITCODE_RSx;
typedef uint32_t BITCODE_BL;
typedef uint32_t BITCODE_BLx;
typedef int32_t BITCODE_BLd;
typedef uint32_t BITCODE_RL;
typedef uint32_t BITCODE_RLx;
typedef int32_t BITCODE_RLd;
#define FORMAT_BS "%" PRIu16
#define FORMAT_BSd "%" PRId16
#define FORMAT_RS "%" PRIu16
#define FORMAT_RSx "0x%" PRIx16
#define FORMAT_BL "%" PRIu32
#define FORMAT_RL "%" PRIu32
#define FORMAT_BLd "%" PRId32
#define FORMAT_RLd "%" PRId32
#define FORMAT_RLx "0x%" PRIx32
#define FORMAT_BLX "%" PRIX32
#define FORMAT_BLx "0x%" PRIx32
typedef long BITCODE_MC;
#define FORMAT_MC  "%ld"
typedef unsigned long BITCODE_UMC;
#define FORMAT_UMC "%lu"
typedef BITCODE_BL BITCODE_MS;
#define FORMAT_MS FORMAT_BL
typedef BITCODE_DOUBLE BITCODE_RD;
#define FORMAT_RD "%f"
typedef uint64_t BITCODE_RLL;
typedef uint64_t BITCODE_BLL;
#define FORMAT_RLL "0x%" PRIx64
#define FORMAT_BLL "%" PRIu64
typedef BITCODE_RS dwg_wchar_t;
#define DWGCHAR dwg_wchar_t
typedef char* BITCODE_TF;
#define FORMAT_TF "\"%s\""
typedef char* BITCODE_TV;
#define FORMAT_TV "\"%s\""
typedef BITCODE_DOUBLE BITCODE_BT;
#define FORMAT_BT "%f"
typedef BITCODE_DOUBLE BITCODE_DD;
#define FORMAT_DD "%f"
typedef BITCODE_DOUBLE BITCODE_BD;
#define FORMAT_BD "%f"
typedef BITCODE_RC BITCODE_4BITS;
#define FORMAT_4BITS "%1x"
#define BITCODE_T  BITCODE_TV
typedef BITCODE_RS* BITCODE_TU;
#define FORMAT_TU "\"%hn\""

typedef struct _dwg_time_bll
{
  BITCODE_BL days;
  BITCODE_BL ms;
  BITCODE_BD value;
} Dwg_Bitcode_TimeBLL;

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

typedef Dwg_Bitcode_TimeBLL BITCODE_TIMEBLL;
typedef Dwg_Bitcode_TimeBLL BITCODE_TIMERLL;
typedef Dwg_Bitcode_2RD  BITCODE_2RD;
typedef Dwg_Bitcode_2BD  BITCODE_2BD;
typedef Dwg_Bitcode_2BD  BITCODE_2DPOINT;
typedef Dwg_Bitcode_2BD  BITCODE_2BD_1;
typedef Dwg_Bitcode_3RD  BITCODE_3RD;
typedef Dwg_Bitcode_3BD  BITCODE_3BD;
typedef Dwg_Bitcode_3BD  BITCODE_3DPOINT;
typedef Dwg_Bitcode_3BD  BITCODE_3BD_1;
typedef Dwg_Bitcode_3BD  BITCODE_BE;
#define BITCODE_3DVECTOR BITCODE_3BD_1

typedef enum DWG_VERSION_TYPE
{
  R_INVALID,
  R_1_1, R_1_2, R_1_4, R_2_0, R_2_1, R_2_5, R_2_6, R_9, R_10, R_11,
  R_13, R_14, R_2000, R_2004, R_2007, R_2010, R_2013, R_2018,
  R_AFTER
} Dwg_Version_Type;
#define DWG_VERSIONS (int)(R_AFTER+1)

typedef enum DWG_OBJECT_SUPERTYPE
{
  DWG_SUPERTYPE_ENTITY, DWG_SUPERTYPE_OBJECT
} Dwg_Object_Supertype;

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
  DWG_TYPE_DIMENSION_ORDINATE = 0x14,
  DWG_TYPE_DIMENSION_LINEAR = 0x15,
  DWG_TYPE_DIMENSION_ALIGNED = 0x16,
  DWG_TYPE_DIMENSION_ANG3PT = 0x17,
  DWG_TYPE_DIMENSION_ANG2LN = 0x18,
  DWG_TYPE_DIMENSION_RADIUS = 0x19,
  DWG_TYPE_DIMENSION_DIAMETER = 0x1A,
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
  DWG_TYPE_REGION = 0x25,
  DWG_TYPE__3DSOLID = 0x26,
  DWG_TYPE_BODY = 0x27,
  DWG_TYPE_RAY = 0x28,
  DWG_TYPE_XLINE = 0x29,
  DWG_TYPE_DICTIONARY = 0x2a,
  DWG_TYPE_OLEFRAME = 0x2b,
  DWG_TYPE_MTEXT = 0x2c,
  DWG_TYPE_LEADER = 0x2d,
  DWG_TYPE_TOLERANCE = 0x2e,
  DWG_TYPE_MLINE = 0x2f,
  DWG_TYPE_BLOCK_CONTROL = 0x30,
  DWG_TYPE_BLOCK_HEADER = 0x31,
  DWG_TYPE_LAYER_CONTROL = 0x32,
  DWG_TYPE_LAYER = 0x33,
  DWG_TYPE_STYLE_CONTROL = 0x34,
  DWG_TYPE_STYLE = 0x35,
  DWG_TYPE_LTYPE_CONTROL = 0x38,
  DWG_TYPE_LTYPE = 0x39,
  DWG_TYPE_VIEW_CONTROL = 0x3c,
  DWG_TYPE_VIEW = 0x3d,
  DWG_TYPE_UCS_CONTROL = 0x3e,
  DWG_TYPE_UCS = 0x3f,
  DWG_TYPE_VPORT_CONTROL = 0x40,
  DWG_TYPE_VPORT = 0x41,
  DWG_TYPE_APPID_CONTROL = 0x42,
  DWG_TYPE_APPID = 0x43,
  DWG_TYPE_DIMSTYLE_CONTROL = 0x44,
  DWG_TYPE_DIMSTYLE = 0x45,
  DWG_TYPE_VPORT_ENTITY_CONTROL = 0x46,
  DWG_TYPE_VPORT_ENTITY_HEADER = 0x47,
  DWG_TYPE_GROUP = 0x48,
  DWG_TYPE_MLINESTYLE = 0x49,
  DWG_TYPE_OLE2FRAME = 0x4a,
  DWG_TYPE_DUMMY = 0x4b,
  DWG_TYPE_LONG_TRANSACTION = 0x4c,
  DWG_TYPE_LWPOLYLINE = 0x4d,
  DWG_TYPE_HATCH = 0x4e,
  DWG_TYPE_XRECORD = 0x4f,
  DWG_TYPE_PLACEHOLDER = 0x50,
  DWG_TYPE_VBA_PROJECT = 0x51,
  DWG_TYPE_LAYOUT = 0x52,
  DWG_TYPE_PROXY_ENTITY = 0x1f2,
  DWG_TYPE_PROXY_OBJECT = 0x1f3,
  DWG_TYPE_ACSH_HISTORY_CLASS = 0x1ff + 1,
  DWG_TYPE_ACSH_SWEEP_CLASS,
  DWG_TYPE_ANNOTSCALEOBJECTCONTEXTDATA,
  DWG_TYPE_ARC_DIMENSION,
  DWG_TYPE_ASSOC2DCONSTRAINTGROUP,
  DWG_TYPE_ASSOCACTION,
  DWG_TYPE_ASSOCALIGNEDDIMACTIONBODY,
  DWG_TYPE_ASSOCDEPENDENCY,
  DWG_TYPE_ASSOCGEOMDEPENDENCY,
  DWG_TYPE_ASSOCNETWORK,
  DWG_TYPE_ASSOCOSNAPPOINTREFACTIONPARAM,
  DWG_TYPE_ASSOCPERSSUBENTMANAGER,
  DWG_TYPE_ASSOCPLANESURFACEACTIONBODY,
  DWG_TYPE_ASSOCVERTEXACTIONPARAM,
  DWG_TYPE_CAMERA,
  DWG_TYPE_CELLSTYLEMAP,
  DWG_TYPE_DATATABLE,
  DWG_TYPE_DBCOLOR,
  DWG_TYPE_DETAILVIEWSTYLE,
  DWG_TYPE_DICTIONARYVAR,
  DWG_TYPE_DICTIONARYWDFLT,
  DWG_TYPE_DIMASSOC,
  DWG_TYPE_DOCUMENTOPTIONS,
  DWG_TYPE_DYNAMICBLOCKPURGEPREVENTER,
  DWG_TYPE_EVALUATION_GRAPH,
  DWG_TYPE_FIELD,
  DWG_TYPE_FIELDLIST,
  DWG_TYPE_GEODATA,
  DWG_TYPE_GEOMAPIMAGE,
  DWG_TYPE_GEOPOSITIONMARKER,
  DWG_TYPE_HELIX,
  DWG_TYPE_IDBUFFER,
  DWG_TYPE_IMAGE,
  DWG_TYPE_IMAGEDEF,
  DWG_TYPE_IMAGEDEF_REACTOR,
  DWG_TYPE_LAYER_FILTER,
  DWG_TYPE_LAYER_INDEX,
  DWG_TYPE_LAYOUTPRINTCONFIG,
  DWG_TYPE_LIGHT,
  DWG_TYPE_LIGHTLIST,
  DWG_TYPE_MATERIAL,
  DWG_TYPE_MESH,
  DWG_TYPE_MLEADERSTYLE,
  DWG_TYPE_MULTILEADER,
  DWG_TYPE_NAVISWORKSMODELDEF,
  DWG_TYPE_NPOCOLLECTION,
  DWG_TYPE_OBJECTCONTEXTDATA,
  DWG_TYPE_OBJECT_PTR,
  DWG_TYPE_PERSSUBENTMANAGER,
  DWG_TYPE_PLOTSETTINGS,
  DWG_TYPE_POINTCLOUD,
  DWG_TYPE_RASTERVARIABLES,
  DWG_TYPE_RENDERENVIRONMENT,
  DWG_TYPE_RENDERGLOBAL,
  DWG_TYPE_MENTALRAYRENDERSETTINGS,
  DWG_TYPE_RAPIDRTRENDERENVIRONMENT,
  DWG_TYPE_RAPIDRTRENDERSETTINGS,
  DWG_TYPE_RTEXT,
  DWG_TYPE_SCALE,
  DWG_TYPE_SECTIONVIEWSTYLE,
  DWG_TYPE_SORTENTSTABLE,
  DWG_TYPE_SPATIAL_FILTER,
  DWG_TYPE_SPATIAL_INDEX,
  DWG_TYPE_SUN,
  DWG_TYPE_SUNSTUDY,
  DWG_TYPE_PLANESURFACE,
  DWG_TYPE_EXTRUDEDSURFACE,
  DWG_TYPE_LOFTEDSURFACE,
  DWG_TYPE_REVOLVEDSURFACE,
  DWG_TYPE_SWEPTSURFACE,
  DWG_TYPE_TABLE,
  DWG_TYPE_TABLECONTENT,
  DWG_TYPE_TABLEGEOMETRY,
  DWG_TYPE_TABLESTYLE,
  DWG_TYPE_UNDERLAY,
  DWG_TYPE_UNDERLAYDEFINITION,
  DWG_TYPE_VISUALSTYLE,
  DWG_TYPE_WIPEOUT,
  DWG_TYPE_WIPEOUTVARIABLES,
  DWG_TYPE_XREFPANELOBJECT,
  DWG_TYPE_FREED       = 0xfffd,
  DWG_TYPE_UNKNOWN_ENT = 0xfffe,
  DWG_TYPE_UNKNOWN_OBJ = 0xffff,
} Dwg_Object_Type;

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
#define DWG_ERR_CRITICAL DWG_ERR_CLASSESNOTFOUND

typedef struct _dwg_handle
{
  BITCODE_RC code;
  BITCODE_RC size;
  unsigned long value;
} Dwg_Handle;

#define FORMAT_H "%u.%u.%lX"
#define ARGS_H(hdl) (hdl).code, (hdl).size, (hdl).value
#define FORMAT_REF "(%u.%u.%lX) abs:%lX"
#define ARGS_REF(ref) (ref)->handleref.code, (ref)->handleref.size, \
    (ref)->handleref.value, (ref)->absolute_ref

typedef struct _dwg_object_entity Dwg_Object_Entity;
typedef struct _dwg_object_object Dwg_Object_Object;
typedef struct _dwg_object Dwg_Object;
typedef struct _dwg_struct Dwg_Data;
typedef struct _dwg_class Dwg_Class;
typedef struct _dwg_section Dwg_Section;
typedef struct _dwg_object_ref Dwg_Object_Ref;
typedef struct Dwg_R2004_Header Dwg_R2004_Header;
typedef struct Dwg_AuxHeader Dwg_AuxHeader;
typedef struct Dwg_SummaryInfo Dwg_SummaryInfo;
typedef Dwg_Object_Ref* BITCODE_H;

typedef struct _dwg_color
{
  BITCODE_BSd index;
  BITCODE_BS flag;
  BITCODE_BL rgb;
  BITCODE_H  handle;
  BITCODE_T  name;
  BITCODE_T  book_name;
  BITCODE_BB alpha_type;
  BITCODE_RC alpha;
} Dwg_Color;

typedef Dwg_Color BITCODE_CMC;
typedef Dwg_Color BITCODE_ENC;

struct _dwg_binary_chunk
{
  short size;
  int  codepage;
  union {
    char *data;
    DWGCHAR *wdata;
  } u;
};

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
  struct _dwg_resbuf *next;
} Dwg_Resbuf;

typedef struct _dwg_header_variables {
  BITCODE_RL size;
  BITCODE_RL bitsize_hi;
  BITCODE_RL bitsize;
  BITCODE_RC ACADMAINTVER;
  BITCODE_BLL REQUIREDVERSIONS;
  BITCODE_TV DWGCODEPAGE;
  BITCODE_BD unknown_0;
  BITCODE_BD unknown_1;
  BITCODE_BD unknown_2;
  BITCODE_BD unknown_3;
  BITCODE_TV unknown_text1;
  BITCODE_TV unknown_text2;
  BITCODE_TV unknown_text3;
  BITCODE_TV unknown_text4;
  BITCODE_BL unknown_8;
  BITCODE_BL unknown_9;
  BITCODE_BS unknown_10;
  BITCODE_H VPORT_ENTITY_HEADER;
  BITCODE_B DIMASO;
  BITCODE_B DIMSHO;
  BITCODE_B DIMSAV;
  BITCODE_B PLINEGEN;
  BITCODE_B ORTHOMODE;
  BITCODE_B REGENMODE;
  BITCODE_B FILLMODE;
  BITCODE_B QTEXTMODE;
  BITCODE_B PSLTSCALE;
  BITCODE_B LIMCHECK;
  BITCODE_B BLIPMODE;
  BITCODE_B unknown_11;
  BITCODE_B USRTIMER;
  BITCODE_B SKPOLY;
  BITCODE_B ANGDIR;
  BITCODE_B SPLFRAME;
  BITCODE_B ATTREQ;
  BITCODE_B ATTDIA;
  BITCODE_B MIRRTEXT;
  BITCODE_B WORLDVIEW;
  BITCODE_B WIREFRAME;
  BITCODE_B TILEMODE;
  BITCODE_B PLIMCHECK;
  BITCODE_B VISRETAIN;
  BITCODE_B DELOBJ;
  BITCODE_B DISPSILH;
  BITCODE_B PELLIPSE;
  BITCODE_BS SAVEIMAGES;
  BITCODE_BS PROXYGRAPHICS;
  BITCODE_BS MEASUREMENT;
  BITCODE_BS DRAGMODE;
  BITCODE_BS TREEDEPTH;
  BITCODE_BS LUNITS;
  BITCODE_BS LUPREC;
  BITCODE_BS AUNITS;
  BITCODE_BS AUPREC;
  BITCODE_BS OSMODE;
  BITCODE_BS ATTMODE;
  BITCODE_BS COORDS;
  BITCODE_BS PDMODE;
  BITCODE_BS PICKSTYLE;
  BITCODE_BL unknown_12;
  BITCODE_BL unknown_13;
  BITCODE_BL unknown_14;
  BITCODE_BS USERI1;
  BITCODE_BS USERI2;
  BITCODE_BS USERI3;
  BITCODE_BS USERI4;
  BITCODE_BS USERI5;
  BITCODE_BS SPLINESEGS;
  BITCODE_BS SURFU;
  BITCODE_BS SURFV;
  BITCODE_BS SURFTYPE;
  BITCODE_BS SURFTAB1;
  BITCODE_BS SURFTAB2;
  BITCODE_BS SPLINETYPE;
  BITCODE_BS SHADEDGE;
  BITCODE_BS SHADEDIF;
  BITCODE_BS UNITMODE;
  BITCODE_BS MAXACTVP;
  BITCODE_BS ISOLINES;
  BITCODE_BS CMLJUST;
  BITCODE_BS TEXTQLTY;
  BITCODE_BL unknown_14b;
  BITCODE_BD LTSCALE;
  BITCODE_BD TEXTSIZE;
  BITCODE_BD TRACEWID;
  BITCODE_BD SKETCHINC;
  BITCODE_BD FILLETRAD;
  BITCODE_BD THICKNESS;
  BITCODE_BD ANGBASE;
  BITCODE_BD PDSIZE;
  BITCODE_BD PLINEWID;
  BITCODE_BD USERR1;
  BITCODE_BD USERR2;
  BITCODE_BD USERR3;
  BITCODE_BD USERR4;
  BITCODE_BD USERR5;
  BITCODE_BD CHAMFERA;
  BITCODE_BD CHAMFERB;
  BITCODE_BD CHAMFERC;
  BITCODE_BD CHAMFERD;
  BITCODE_BD FACETRES;
  BITCODE_BD CMLSCALE;
  BITCODE_BD CELTSCALE;
  BITCODE_TV MENU;
  BITCODE_TIMEBLL TDCREATE;
  BITCODE_TIMEBLL TDUPDATE;
  BITCODE_TIMEBLL TDUCREATE;
  BITCODE_TIMEBLL TDUUPDATE;
  BITCODE_BL unknown_15;
  BITCODE_BL unknown_16;
  BITCODE_BL unknown_17;
  BITCODE_TIMEBLL TDINDWG;
  BITCODE_TIMEBLL TDUSRTIMER;
  BITCODE_CMC CECOLOR;
  BITCODE_BS HANDLING;
  BITCODE_H HANDSEED;
  BITCODE_H CLAYER;
  BITCODE_H TEXTSTYLE;
  BITCODE_H CELTYPE;
  BITCODE_H CMATERIAL;
  BITCODE_H DIMSTYLE;
  BITCODE_H CMLSTYLE;
  BITCODE_BD PSVPSCALE;
  BITCODE_3BD PINSBASE;
  BITCODE_3BD PEXTMIN;
  BITCODE_3BD PEXTMAX;
  BITCODE_2RD PLIMMIN;
  BITCODE_2RD PLIMMAX;
  BITCODE_BD  PELEVATION;
  BITCODE_3BD PUCSORG;
  BITCODE_3BD PUCSXDIR;
  BITCODE_3BD PUCSYDIR;
  BITCODE_H PUCSNAME;
  BITCODE_H PUCSBASE;
  BITCODE_H PUCSORTHOREF;
  BITCODE_BS PUCSORTHOVIEW;
  BITCODE_3BD PUCSORGTOP;
  BITCODE_3BD PUCSORGBOTTOM;
  BITCODE_3BD PUCSORGLEFT;
  BITCODE_3BD PUCSORGRIGHT;
  BITCODE_3BD PUCSORGFRONT;
  BITCODE_3BD PUCSORGBACK;
  BITCODE_3BD INSBASE;
  BITCODE_3BD EXTMIN;
  BITCODE_3BD EXTMAX;
  BITCODE_2RD LIMMIN;
  BITCODE_2RD LIMMAX;
  BITCODE_2RD VIEWCTR;
  BITCODE_BD ELEVATION;
  BITCODE_RD VIEWSIZE;
  BITCODE_RS SNAPMODE;
  BITCODE_2RD SNAPUNIT;
  BITCODE_2RD SNAPBASE;
  BITCODE_RD SNAPANG;
  BITCODE_RS SNAPSTYL;
  BITCODE_RS SNAPISOPAIR;
  BITCODE_RS GRIDMODE;
  BITCODE_2RD GRIDUNIT;
  BITCODE_3BD UCSORG;
  BITCODE_3BD UCSXDIR;
  BITCODE_3BD UCSYDIR;
  BITCODE_H UCSNAME;
  BITCODE_H UCSBASE;
  BITCODE_BS UCSORTHOVIEW;
  BITCODE_H UCSORTHOREF;
  BITCODE_3BD UCSORGTOP;
  BITCODE_3BD UCSORGBOTTOM;
  BITCODE_3BD UCSORGLEFT;
  BITCODE_3BD UCSORGRIGHT;
  BITCODE_3BD UCSORGFRONT;
  BITCODE_3BD UCSORGBACK;
  BITCODE_TV DIMPOST;
  BITCODE_TV DIMAPOST;
  BITCODE_B DIMTOL;
  BITCODE_B DIMLIM;
  BITCODE_B DIMTIH;
  BITCODE_B DIMTOH;
  BITCODE_B DIMSE1;
  BITCODE_B DIMSE2;
  BITCODE_B DIMALT;
  BITCODE_B DIMTOFL;
  BITCODE_B DIMSAH;
  BITCODE_B DIMTIX;
  BITCODE_B DIMSOXD;
  BITCODE_BS DIMALTD;
  BITCODE_BS DIMZIN;
  BITCODE_B DIMSD1;
  BITCODE_B DIMSD2;
  BITCODE_BS DIMTOLJ;
  BITCODE_BS DIMJUST;
  BITCODE_BS DIMFIT;
  BITCODE_B DIMUPT;
  BITCODE_BS DIMTZIN;
  BITCODE_BS DIMMALTZ;
  BITCODE_BS DIMMALTTZ;
  BITCODE_BS DIMTAD;
  BITCODE_BS DIMUNIT;
  BITCODE_BS DIMAUNIT;
  BITCODE_BS DIMDEC;
  BITCODE_BS DIMTDEC;
  BITCODE_BS DIMALTU;
  BITCODE_BS DIMALTTD;
  BITCODE_H DIMTXSTY;
  BITCODE_BD DIMSCALE;
  BITCODE_BD DIMASZ;
  BITCODE_BD DIMEXO;
  BITCODE_BD DIMDLI;
  BITCODE_BD DIMEXE;
  BITCODE_BD DIMRND;
  BITCODE_BD DIMDLE;
  BITCODE_BD DIMTP;
  BITCODE_BD DIMTM;
  BITCODE_BD DIMFXL;
  BITCODE_BD DIMJOGANG;
  BITCODE_BS DIMTFILL;
  BITCODE_CMC DIMTFILLCLR;
  BITCODE_BS DIMAZIN;
  BITCODE_BS DIMARCSYM;
  BITCODE_BD DIMTXT;
  BITCODE_BD DIMCEN;
  BITCODE_BD DIMTSZ;
  BITCODE_BD DIMALTF;
  BITCODE_BD DIMLFAC;
  BITCODE_BD DIMTVP;
  BITCODE_BD DIMTFAC;
  BITCODE_BD DIMGAP;
  BITCODE_T DIMPOST_T;
  BITCODE_T DIMAPOST_T;
  BITCODE_T DIMBLK_T;
  BITCODE_T DIMBLK1_T;
  BITCODE_T DIMBLK2_T;
  BITCODE_BD DIMALTRND;
  BITCODE_RS DIMCLRD_C;
  BITCODE_RS DIMCLRE_C;
  BITCODE_RS DIMCLRT_C;
  BITCODE_CMC DIMCLRD;
  BITCODE_CMC DIMCLRE;
  BITCODE_CMC DIMCLRT;
  BITCODE_BS DIMADEC;
  BITCODE_BS DIMFRAC;
  BITCODE_BS DIMLUNIT;
  BITCODE_BS DIMDSEP;
  BITCODE_BS DIMTMOVE;
  BITCODE_BS DIMALTZ;
  BITCODE_BS DIMALTTZ;
  BITCODE_BS DIMATFIT;
  BITCODE_B  DIMFXLON;
  BITCODE_B  DIMTXTDIRECTION;
  BITCODE_BD DIMALTMZF;
  BITCODE_T  DIMALTMZS;
  BITCODE_BD DIMMZF;
  BITCODE_T  DIMMZS;
  BITCODE_H DIMLDRBLK;
  BITCODE_H DIMBLK;
  BITCODE_H DIMBLK1;
  BITCODE_H DIMBLK2;
  BITCODE_H DIMLTYPE;
  BITCODE_H DIMLTEX1;
  BITCODE_H DIMLTEX2;
  BITCODE_BSd DIMLWD;
  BITCODE_BSd DIMLWE;
  BITCODE_H BLOCK_CONTROL_OBJECT;
  BITCODE_H LAYER_CONTROL_OBJECT;
  BITCODE_H STYLE_CONTROL_OBJECT;
  BITCODE_H LTYPE_CONTROL_OBJECT;
  BITCODE_H VIEW_CONTROL_OBJECT;
  BITCODE_H UCS_CONTROL_OBJECT;
  BITCODE_H VPORT_CONTROL_OBJECT;
  BITCODE_H APPID_CONTROL_OBJECT;
  BITCODE_H DIMSTYLE_CONTROL_OBJECT;
  BITCODE_H VPORT_ENTITY_CONTROL_OBJECT;
  BITCODE_H DICTIONARY_ACAD_GROUP;
  BITCODE_H DICTIONARY_ACAD_MLINESTYLE;
  BITCODE_H DICTIONARY_NAMED_OBJECT;
  BITCODE_BS TSTACKALIGN;
  BITCODE_BS TSTACKSIZE;
  BITCODE_TV HYPERLINKBASE;
  BITCODE_TV STYLESHEET;
  BITCODE_H DICTIONARY_LAYOUT;
  BITCODE_H DICTIONARY_PLOTSETTINGS;
  BITCODE_H DICTIONARY_PLOTSTYLENAME;
  BITCODE_H DICTIONARY_MATERIAL;
  BITCODE_H DICTIONARY_COLOR;
  BITCODE_H DICTIONARY_VISUALSTYLE;
  BITCODE_H DICTIONARY_LIGHTLIST;
  BITCODE_H unknown_20;
  BITCODE_BL FLAGS;
  BITCODE_BSd CELWEIGHT;
  BITCODE_B  ENDCAPS;
  BITCODE_B  JOINSTYLE;
  BITCODE_B  LWDISPLAY;
  BITCODE_B  XEDIT;
  BITCODE_B  EXTNAMES;
  BITCODE_B  PSTYLEMODE;
  BITCODE_B  OLESTARTUP;
  BITCODE_BS INSUNITS;
  BITCODE_BS CEPSNTYPE;
  BITCODE_H CPSNID;
  BITCODE_TV FINGERPRINTGUID;
  BITCODE_TV VERSIONGUID;
  BITCODE_RC SORTENTS;
  BITCODE_RC INDEXCTL;
  BITCODE_RC HIDETEXT;
  BITCODE_RC XCLIPFRAME;
  BITCODE_RC DIMASSOC;
  BITCODE_RC HALOGAP;
  BITCODE_BS OBSCOLOR;
  BITCODE_BS INTERSECTIONCOLOR;
  BITCODE_RC OBSLTYPE;
  BITCODE_RC INTERSECTIONDISPLAY;
  BITCODE_TV PROJECTNAME;
  BITCODE_H BLOCK_RECORD_PSPACE;
  BITCODE_H BLOCK_RECORD_MSPACE;
  BITCODE_H LTYPE_BYLAYER;
  BITCODE_H LTYPE_BYBLOCK;
  BITCODE_H LTYPE_CONTINUOUS;
  BITCODE_B CAMERADISPLAY;
  BITCODE_BL unknown_21;
  BITCODE_BL unknown_22;
  BITCODE_BD unknown_23;
  BITCODE_BD STEPSPERSEC;
  BITCODE_BD STEPSIZE;
  BITCODE_BD _3DDWFPREC;
  BITCODE_BD LENSLENGTH;
  BITCODE_BD CAMERAHEIGHT;
  BITCODE_RC SOLIDHIST;
  BITCODE_RC SHOWHIST;
  BITCODE_BD PSOLWIDTH;
  BITCODE_BD PSOLHEIGHT;
  BITCODE_BD LOFTANG1;
  BITCODE_BD LOFTANG2;
  BITCODE_BD LOFTMAG1;
  BITCODE_BD LOFTMAG2;
  BITCODE_BS LOFTPARAM;
  BITCODE_RC LOFTNORMALS;
  BITCODE_BD LATITUDE;
  BITCODE_BD LONGITUDE;
  BITCODE_BD NORTHDIRECTION;
  BITCODE_BL TIMEZONE;
  BITCODE_RC LIGHTGLYPHDISPLAY;
  BITCODE_RC TILEMODELIGHTSYNCH;
  BITCODE_RC DWFFRAME;
  BITCODE_RC DGNFRAME;
  BITCODE_B REALWORLDSCALE;
  BITCODE_CMC INTERFERECOLOR;
  BITCODE_H INTERFEREOBJVS;
  BITCODE_H INTERFEREVPVS;
  BITCODE_H DRAGVS;
  BITCODE_RC CSHADOW;
  BITCODE_BD SHADOWPLANELOCATION;
  BITCODE_BS unknown_54;
  BITCODE_BS unknown_55;
  BITCODE_BS unknown_56;
  BITCODE_BS unknown_57;
} Dwg_Header_Variables;

typedef int Dwg_Entity_UNUSED;

typedef struct _dwg_entity_TEXT
{
  struct _dwg_object_entity *parent;
  BITCODE_RC dataflags;
  BITCODE_RD elevation;
  BITCODE_2DPOINT insertion_pt;
  BITCODE_2DPOINT alignment_pt;
  BITCODE_BE extrusion;
  BITCODE_RD thickness;
  BITCODE_RD oblique_ang;
  BITCODE_RD rotation;
  BITCODE_RD height;
  BITCODE_RD width_factor;
  BITCODE_TV text_value;
  BITCODE_BS generation;
  BITCODE_BS horiz_alignment;
  BITCODE_BS vert_alignment;
  BITCODE_H style;
} Dwg_Entity_TEXT;

#define COMMON_ENTITY_POLYLINE \
  struct _dwg_object_entity *parent; \
  BITCODE_B has_vertex; \
  BITCODE_BL num_owned; \
  BITCODE_H first_vertex; \
  BITCODE_H last_vertex; \
  BITCODE_H* vertex; \
  BITCODE_H seqend

typedef struct _dwg_entity_INSERT
{
  struct _dwg_object_entity *parent;
  BITCODE_3DPOINT   ins_pt;
  BITCODE_BB        scale_flag;
  BITCODE_3DPOINT   scale;
  BITCODE_BD        rotation;
  BITCODE_BE        extrusion;
  BITCODE_B         has_attribs;
  BITCODE_BL        num_owned;
  BITCODE_H   block_header;
  BITCODE_H   first_attrib;
  BITCODE_H   last_attrib;
  BITCODE_H*  attrib_handles;
  BITCODE_H   seqend;
} Dwg_Entity_INSERT;

typedef struct _dwg_entity_LINE
{
  struct _dwg_object_entity *parent;
  BITCODE_RC z_is_zero;
  BITCODE_3BD start;
  BITCODE_3BD end;
  BITCODE_BT thickness;
  BITCODE_BE extrusion;
} Dwg_Entity_LINE;

typedef struct _dwg_entity_CIRCLE
{
  struct _dwg_object_entity *parent;
  BITCODE_3BD center;
  BITCODE_BD radius;
  BITCODE_BT thickness;
  BITCODE_BE extrusion;
} Dwg_Entity_CIRCLE;

typedef struct _dwg_entity_ARC
{
  struct _dwg_object_entity *parent;
  BITCODE_3BD center;
  BITCODE_BD radius;
  BITCODE_BT thickness;
  BITCODE_BE extrusion;
  BITCODE_BD start_angle;
  BITCODE_BD end_angle;
} Dwg_Entity_ARC;

typedef struct _dwg_entity_POLYLINE_2D
{
  COMMON_ENTITY_POLYLINE;
  BITCODE_BS flag;
  BITCODE_BS curve_type;
  BITCODE_BD start_width;
  BITCODE_BD end_width;
  BITCODE_BT thickness;
  BITCODE_BD elevation;
  BITCODE_BE extrusion;
} Dwg_Entity_POLYLINE_2D;

typedef struct _dwg_entity_LWPOLYLINE
{
  struct _dwg_object_entity *parent;
  BITCODE_BS flag;
  BITCODE_BD const_width;
  BITCODE_BD elevation;
  BITCODE_BD thickness;
  BITCODE_BE extrusion;
  BITCODE_BL num_points;
  BITCODE_2RD* points;
  BITCODE_BL num_bulges;
  BITCODE_BD* bulges;
  BITCODE_BL num_widths;
  struct { BITCODE_BD start; BITCODE_BD end; }* widths;
} Dwg_Entity_LWPOLYLINE;

#define COMMON_TABLE_CONTROL_FIELDS \
  struct _dwg_object_object *parent; \
  BITCODE_BS num_entries; \
  BITCODE_H* entries; \
  BITCODE_BL objid

#define COMMON_TABLE_FIELDS() \
  struct _dwg_object_object *parent; \
  BITCODE_RC flag; \
  BITCODE_TV name; \
  BITCODE_RS used; \
  BITCODE_B xrefref; \
  BITCODE_BS xrefindex_plus1; \
  BITCODE_B xrefdep

typedef struct _dwg_object_BLOCK_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
  BITCODE_H model_space;
  BITCODE_H paper_space;
} Dwg_Object_BLOCK_CONTROL;

typedef struct _dwg_object_BLOCK_HEADER
{
  COMMON_TABLE_FIELDS();
  BITCODE_BL __iterator;
  BITCODE_RC flag2;
  BITCODE_RS flag3;
  BITCODE_B anonymous;
  BITCODE_B hasattrs;
  BITCODE_B blkisxref;
  BITCODE_B xrefoverlaid;
  BITCODE_B loaded_bit;
  BITCODE_BL num_owned;
  BITCODE_3DPOINT base_pt;
  BITCODE_TV xref_pname;
  BITCODE_RL num_inserts;
  BITCODE_TV description;
  BITCODE_BL preview_size;
  char     * preview;
  BITCODE_BS insert_units;
  BITCODE_B explodable;
  BITCODE_RC block_scaling;
  BITCODE_H null_handle;
  BITCODE_H block_entity;
  BITCODE_H first_entity;
  BITCODE_H last_entity;
  BITCODE_H* entities;
  BITCODE_H endblk_entity;
  BITCODE_H* inserts;
  BITCODE_H layout;
} Dwg_Object_BLOCK_HEADER;

typedef struct _dwg_object_STYLE_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
} Dwg_Object_STYLE_CONTROL;

typedef struct _dwg_object_STYLE
{
  COMMON_TABLE_FIELDS();
  BITCODE_B vertical;
  BITCODE_B shape_file;
  BITCODE_BD fixed_height;
  BITCODE_BD width_factor;
  BITCODE_BD oblique_ang;
  BITCODE_RC generation;
  BITCODE_BD last_height;
  BITCODE_TV font_name;
  BITCODE_TV bigfont_name;
  BITCODE_H extref;
} Dwg_Object_STYLE;

typedef struct _dwg_object_LAYER_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
} Dwg_Object_LAYER_CONTROL;

typedef struct _dwg_object_LTYPE_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
  BITCODE_H bylayer;
  BITCODE_H byblock;
} Dwg_Object_LTYPE_CONTROL;

typedef struct _dwg_object_VIEW_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
} Dwg_Object_VIEW_CONTROL;

typedef struct _dwg_object_UCS_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
} Dwg_Object_UCS_CONTROL;

typedef struct _dwg_object_VPORT_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
} Dwg_Object_VPORT_CONTROL;

typedef struct _dwg_object_APPID_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
} Dwg_Object_APPID_CONTROL;

typedef struct _dwg_object_DIMSTYLE_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
  BITCODE_RC num_morehandles;
  BITCODE_H* morehandles;
} Dwg_Object_DIMSTYLE_CONTROL;

typedef struct _dwg_object_VPORT_ENTITY_CONTROL
{
  COMMON_TABLE_CONTROL_FIELDS;
} Dwg_Object_VPORT_ENTITY_CONTROL;

typedef struct _dwg_object_ref
{
  struct _dwg_object* obj;
  Dwg_Handle handleref;
  unsigned long absolute_ref;
} Dwg_Object_Ref;

#pragma pack(1)
typedef struct _dwg_entity_eed_data
{
  BITCODE_RC code;
  union eed_data_t {
    struct { BITCODE_RC length; BITCODE_RS codepage; char string[1]; } eed_0;
    struct { BITCODE_RS length; DWGCHAR string[1]; } eed_0_r2007;
    struct { char invalid[1]; } eed_1;
    struct { BITCODE_RC byte; } eed_2;
    struct { BITCODE_RL layer; } eed_3;
    struct { BITCODE_RC length; char data[1]; } eed_4;
    struct { BITCODE_RLL entity; } eed_5;
    struct { BITCODE_3RD point; } eed_10;
    struct { BITCODE_RD real; } eed_40;
    struct { BITCODE_RS rs; } eed_70;
    struct { BITCODE_RL rl; } eed_71;
  } u;
} Dwg_Eed_Data;
#pragma pack()

typedef struct _dwg_entity_eed
{
  BITCODE_BS size;
  Dwg_Handle handle;
  Dwg_Eed_Data *data;
  char* raw;
} Dwg_Eed;

typedef struct _dwg_object_entity
{
  BITCODE_BL objid;
  union
  {
    Dwg_Entity_UNUSED *UNUSED;
    Dwg_Entity_TEXT *TEXT;
    struct _dwg_entity_ATTRIB *ATTRIB;
    struct _dwg_entity_ATTDEF *ATTDEF;
    struct _dwg_entity_BLOCK *BLOCK;
    struct _dwg_entity_ENDBLK *ENDBLK;
    struct _dwg_entity_SEQEND *SEQEND;
    struct _dwg_entity_INSERT *INSERT;
    struct _dwg_entity_MINSERT *MINSERT;
    struct _dwg_entity_VERTEX_2D *VERTEX_2D;
    struct _dwg_entity_VERTEX_3D *VERTEX_3D;
    struct _dwg_entity_VERTEX_MESH *VERTEX_MESH;
    struct _dwg_entity_VERTEX_PFACE *VERTEX_PFACE;
    struct _dwg_entity_VERTEX_PFACE_FACE *VERTEX_PFACE_FACE;
    struct _dwg_entity_POLYLINE_2D *POLYLINE_2D;
    struct _dwg_entity_POLYLINE_3D *POLYLINE_3D;
    struct _dwg_entity_ARC *ARC;
    struct _dwg_entity_CIRCLE *CIRCLE;
    struct _dwg_entity_LINE *LINE;
    struct _dwg_entity_DIMENSION_ORDINATE *DIMENSION_ORDINATE;
    struct _dwg_entity_DIMENSION_LINEAR *DIMENSION_LINEAR;
    struct _dwg_entity_DIMENSION_ALIGNED *DIMENSION_ALIGNED;
    struct _dwg_entity_DIMENSION_ANG3PT *DIMENSION_ANG3PT;
    struct _dwg_entity_DIMENSION_ANG2LN *DIMENSION_ANG2LN;
    struct _dwg_entity_DIMENSION_RADIUS *DIMENSION_RADIUS;
    struct _dwg_entity_DIMENSION_DIAMETER *DIMENSION_DIAMETER;
    struct _dwg_entity_POINT *POINT;
    struct _dwg_entity__3DFACE *_3DFACE;
    struct _dwg_entity_POLYLINE_PFACE *POLYLINE_PFACE;
    struct _dwg_entity_POLYLINE_MESH *POLYLINE_MESH;
    struct _dwg_entity_SOLID *SOLID;
    struct _dwg_entity_TRACE *TRACE;
    struct _dwg_entity_SHAPE *SHAPE;
    struct _dwg_entity_VIEWPORT *VIEWPORT;
    struct _dwg_entity_ELLIPSE *ELLIPSE;
    struct _dwg_entity_SPLINE *SPLINE;
    struct _dwg_entity_3DSOLID *_3DSOLID;
    struct _dwg_entity_REGION *REGION;
    struct _dwg_entity_BODY *BODY;
    struct _dwg_entity_RAY *RAY;
    struct _dwg_entity_XLINE *XLINE;
    struct _dwg_entity_OLEFRAME *OLEFRAME;
    struct _dwg_entity_MTEXT *MTEXT;
    struct _dwg_entity_LEADER *LEADER;
    struct _dwg_entity_TOLERANCE *TOLERANCE;
    struct _dwg_entity_MLINE *MLINE;
    struct _dwg_entity_OLE2FRAME *OLE2FRAME;
    struct _dwg_entity_HATCH *HATCH;
    struct _dwg_entity_LWPOLYLINE *LWPOLYLINE;
    struct _dwg_entity_PROXY_ENTITY *PROXY_ENTITY;
    struct _dwg_entity_UNKNOWN_ENT *UNKNOWN_ENT;
  } tio;
  struct _dwg_struct *dwg;
  BITCODE_BL num_eed;
  Dwg_Eed *eed;
  BITCODE_B   preview_exists;
  BITCODE_BLL preview_size;
  char       *preview;
  BITCODE_BB entmode;
  BITCODE_BL num_reactors;
  BITCODE_B xdic_missing_flag;
  BITCODE_B isbylayerlt;
  BITCODE_B nolinks;
  BITCODE_B has_ds_binary_data;
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

typedef struct _dwg_object_object
{
  BITCODE_BL objid;
  union
  {
    Dwg_Object_BLOCK_CONTROL *BLOCK_CONTROL;
    Dwg_Object_BLOCK_HEADER *BLOCK_HEADER;
    Dwg_Object_LAYER_CONTROL *LAYER_CONTROL;
    struct _dwg_object_LAYER *LAYER;
    Dwg_Object_STYLE_CONTROL *STYLE_CONTROL;
    Dwg_Object_STYLE *STYLE;
    Dwg_Object_LTYPE_CONTROL *LTYPE_CONTROL;
    struct _dwg_object_LTYPE *LTYPE;
    Dwg_Object_VIEW_CONTROL *VIEW_CONTROL;
    struct _dwg_object_VIEW *VIEW;
    Dwg_Object_UCS_CONTROL *UCS_CONTROL;
    struct _dwg_object_UCS *UCS;
    Dwg_Object_VPORT_CONTROL *VPORT_CONTROL;
    struct _dwg_object_VPORT *VPORT;
    Dwg_Object_APPID_CONTROL *APPID_CONTROL;
    struct _dwg_object_APPID *APPID;
    Dwg_Object_DIMSTYLE_CONTROL *DIMSTYLE_CONTROL;
    struct _dwg_object_DIMSTYLE *DIMSTYLE;
    Dwg_Object_VPORT_ENTITY_CONTROL *VPORT_ENTITY_CONTROL;
    struct _dwg_object_VPORT_ENTITY_HEADER *VPORT_ENTITY_HEADER;
    struct _dwg_object_GROUP *GROUP;
    struct _dwg_object_MLINESTYLE *MLINESTYLE;
    struct _dwg_object_OLE2FRAME *OLE2FRAME;
    struct _dwg_object_DUMMY *DUMMY;
    struct _dwg_object_LONG_TRANSACTION *LONG_TRANSACTION;
    struct _dwg_object_HATCH *HATCH;
    struct _dwg_object_XRECORD *XRECORD;
    struct _dwg_object_PLACEHOLDER *PLACEHOLDER;
    struct _dwg_object_OLEFRAME *OLEFRAME;
    struct _dwg_object_LAYOUT *LAYOUT;
    struct _dwg_object_PROXY_OBJECT *PROXY_OBJECT;
    struct _dwg_object_UNKNOWN_OBJ *UNKNOWN_OBJ;
  } tio;
  struct _dwg_struct *dwg;
  BITCODE_BL num_eed;
  Dwg_Eed *eed;
  BITCODE_H ownerhandle;
  BITCODE_BL num_reactors;
  BITCODE_H* reactors;
  BITCODE_H xdicobjhandle;
  BITCODE_B xdic_missing_flag;
  BITCODE_B has_ds_binary_data;
  Dwg_Handle *handleref;
} Dwg_Object_Object;

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
  struct _dwg_struct *parent;
  BITCODE_RL bitsize;
  unsigned long bitsize_pos;
  unsigned long hdlpos;
  BITCODE_B  has_strings;
  BITCODE_RL stringstream_size;
  BITCODE_UMC handlestream_size;
  unsigned long common_size;
  unsigned long num_unknown_bits;
  char *unknown_bits;
} Dwg_Object;

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

typedef struct _dwg_chain
{
  unsigned char *chain;
  long unsigned int size;
  long unsigned int byte;
  unsigned char bit;
} Dwg_Chain;

typedef enum DWG_SECTION_TYPE
{
  SECTION_HEADER = 1,
  SECTION_AUXHEADER = 2,
  SECTION_CLASSES = 3,
  SECTION_HANDLES = 4,
  SECTION_TEMPLATE = 5,
  SECTION_OBJFREESPACE = 6,
  SECTION_OBJECTS = 7,
  SECTION_REVHISTORY = 8,
  SECTION_SUMMARYINFO = 9,
  SECTION_PREVIEW = 10,
  SECTION_APPINFO = 11,
  SECTION_APPINFOHISTORY = 12,
  SECTION_FILEDEPLIST = 13,
  SECTION_SECURITY,
  SECTION_VBAPROJECT,
  SECTION_SIGNATURE,
  SECTION_PROTOTYPE,
  SECTION_UNKNOWN,
} Dwg_Section_Type;

typedef enum DWG_SECTION_TYPE_R13
{
  SECTION_HEADER_R13 = 0,
  SECTION_CLASSES_R13 = 1,
  SECTION_HANDLES_R13 = 2,
  SECTION_2NDHEADER_R13 = 3,
  SECTION_MEASUREMENT_R13 = 4,
  SECTION_AUXHEADER_R2000 = 5,
} Dwg_Section_Type_R13;

typedef struct _dwg_section
{
  int32_t    number;
  BITCODE_RL size;
  uint64_t   address;
  BITCODE_RL parent;
  BITCODE_RL left;
  BITCODE_RL right;
  BITCODE_RL x00;
  Dwg_Section_Type type;
  char name[64];
  BITCODE_RL section_type;
  BITCODE_RL decomp_data_size;
  BITCODE_RL comp_data_size;
  BITCODE_RL compression_type;
  BITCODE_RL checksum;
} Dwg_Section;

typedef struct
{
  BITCODE_RL num_desc;
  BITCODE_RL compressed;
  BITCODE_RL max_size;
  BITCODE_RL encrypted;
  BITCODE_RL num_desc2;
} Dwg_Section_InfoHdr;

typedef struct
{
  int64_t    size;
  BITCODE_RL num_sections;
  BITCODE_RL max_decomp_size;
  BITCODE_RL unknown;
  BITCODE_RL compressed;
  BITCODE_RL type;
  BITCODE_RL encrypted;
  char name[64];
  Dwg_Section **sections;
} Dwg_Section_Info;

typedef struct _dwg_SummaryInfo_Property
{
  BITCODE_T key;
  BITCODE_T value;
} Dwg_SummaryInfo_Property;

typedef struct _dwg_second_header {
  BITCODE_RL size;
  BITCODE_BL address;
  BITCODE_RC version[12];
  BITCODE_B null_b[4];
  BITCODE_RC unknown_10;
  BITCODE_RC unknown_rc4[4];
  BITCODE_RC num_sections;
  struct { BITCODE_RC nr; BITCODE_BL address; BITCODE_BL size; } section[6];
  BITCODE_BS num_handlers;
  struct { BITCODE_RC size; BITCODE_RC nr; BITCODE_RC* data; } handlers[16];
  BITCODE_RL junk_r14_1;
  BITCODE_RL junk_r14_2;
} Dwg_Second_Header;

typedef struct Dwg_AuxHeader
{
  BITCODE_RC   aux_intro[3];
  BITCODE_RS   dwg_version;
  BITCODE_RS   maint_version;
  BITCODE_RL   num_saves;
  BITCODE_RL   minus_1;
  BITCODE_RS   num_saves_1;
  BITCODE_RS   num_saves_2;
  BITCODE_RL   zero;
  BITCODE_RS   dwg_version_1;
  BITCODE_RS   maint_version_1;
  BITCODE_RL   num_writes;
  BITCODE_RL   minus_1_2;
  BITCODE_RS   num_writes_1;
  BITCODE_RS   num_writes_2;
  BITCODE_RL   zero_2;
  BITCODE_RS   dwg_version_2;
  BITCODE_RS   maint_version_2;
  BITCODE_RL   num_writes_3;
  BITCODE_RL   minus_1_3;
  BITCODE_RS   num_writes_3_1;
  BITCODE_RS   num_writes_3_2;
  BITCODE_RL   zero_3;
  BITCODE_RS   dwg_version_3;
  BITCODE_RS   maint_version_3;
  BITCODE_RL   unknown_r13_1;
  BITCODE_RL   unknown_r13_2;
  BITCODE_RL   unknown_r13_3;
  BITCODE_RC   unknown_3[32];
  BITCODE_RL   unknown_4;
  BITCODE_RL   unknown_5;
  BITCODE_RL   unknown_6;
  BITCODE_RL   unknown_7;
  BITCODE_RL   unknown_8;
  BITCODE_RL   unknown_9;
  BITCODE_RL   unknown_10;
  BITCODE_RL   unknown_11;
  BITCODE_RL   unknown_12;
  BITCODE_RL   unknown_13;
  BITCODE_RL   unknown_14;
  BITCODE_RL   unknown_15;
  BITCODE_RL   unknown_16;
  BITCODE_RL   unknown_17;
  BITCODE_RL   unknown_18;
  BITCODE_RL   unknown_19;
  BITCODE_RL   unknown_20;
  BITCODE_RL   unknown_21;
  BITCODE_RL   unknown_22;
  BITCODE_RL   unknown_23;
  BITCODE_RL   unknown_24;
  BITCODE_RL   unknown_25;
  BITCODE_RL   unknown_26;
  BITCODE_RL   unknown_27;
  BITCODE_RL   unknown_28;
  BITCODE_RL   unknown_29;
  BITCODE_RL   unknown_30;
  BITCODE_RL   unknown_31;
  BITCODE_RL   unknown_32;
  BITCODE_RL   unknown_33;
  BITCODE_RL   unknown_34;
  BITCODE_RL   unknown_35;
  BITCODE_RL   unknown_36;
  BITCODE_RL   unknown_37;
  BITCODE_RL   unknown_38;
  BITCODE_RL   unknown_39;
  BITCODE_RL   unknown_40;
  BITCODE_RL   unknown_41;
  BITCODE_RL   unknown_42;
  BITCODE_RL   unknown_43;
  BITCODE_RL   unknown_44;
  BITCODE_RL   unknown_45;
  BITCODE_RL   unknown_46;
  BITCODE_RL   unknown_47;
  BITCODE_RL   unknown_48;
  BITCODE_RL   unknown_49;
  BITCODE_RL   unknown_50;
  BITCODE_RL   unknown_51;
  BITCODE_RL   unknown_52;
  BITCODE_RL   unknown_53;
  BITCODE_RL   unknown_54;
  BITCODE_RL   unknown_55;
  BITCODE_RL   unknown_56;
  BITCODE_RL   unknown_57;
  BITCODE_RL   unknown_58;
  BITCODE_RL   unknown_59;
  BITCODE_RL   unknown_60;
  BITCODE_RL   unknown_61;
  BITCODE_RL   unknown_62;
  BITCODE_RL   unknown_63;
  BITCODE_RL   unknown_64;
  BITCODE_RL   unknown_65;
  BITCODE_RL   unknown_66;
  BITCODE_RL   unknown_67;
  BITCODE_RL   unknown_68;
  BITCODE_RL   unknown_69;
  BITCODE_RL   unknown_70;
  BITCODE_RL   unknown_71;
  BITCODE_RL   unknown_72;
  BITCODE_RL   unknown_73;
  BITCODE_RL   unknown_74;
  BITCODE_RL   unknown_75;
  BITCODE_RL   unknown_76;
  BITCODE_RL   unknown_77;
  BITCODE_RL   unknown_78;
  BITCODE_RL   unknown_79;
  BITCODE_RL   unknown_80;
  BITCODE_RL   unknown_81;
  BITCODE_RL   unknown_82;
  BITCODE_RL   unknown_83;
  BITCODE_RL   unknown_84;
  BITCODE_RL   unknown_85;
  BITCODE_RL   unknown_86;
  BITCODE_RL   unknown_87;
  BITCODE_RL   unknown_88;
  BITCODE_RL   unknown_89;
  BITCODE_RL   unknown_90;
  BITCODE_RL   unknown_91;
  BITCODE_RL   unknown_92;
  BITCODE_RL   unknown_93;
  BITCODE_RL   unknown_94;
  BITCODE_RL   unknown_95;
  BITCODE_RL   unknown_96;
  BITCODE_RL   unknown_97;
  BITCODE_RL   unknown_98;
  BITCODE_RL   unknown_99;
  BITCODE_RL   unknown_100;
  BITCODE_RL   unknown_101;
  BITCODE_RL   unknown_102;
  BITCODE_RL   unknown_103;
  BITCODE_RL   unknown_104;
  BITCODE_RL   unknown_105;
  BITCODE_RL   unknown_106;
  BITCODE_RL   unknown_107;
  BITCODE_RL   unknown_108;
  BITCODE_RL   unknown_109;
  BITCODE_RL   unknown_110;
  BITCODE_RL   unknown_111;
  BITCODE_RL   unknown_112;
  BITCODE_RL   unknown_113;
  BITCODE_RL   unknown_114;
  BITCODE_RL   unknown_115;
  BITCODE_RL   unknown_116;
  BITCODE_RL   unknown_117;
  BITCODE_RL   unknown_118;
  BITCODE_RL   unknown_119;
  BITCODE_RL   unknown_120;
  BITCODE_RL   unknown_121;
  BITCODE_RL   unknown_122;
} Dwg_AuxHeader;

typedef struct Dwg_R2004_Header
{
  BITCODE_RC file_ID_string[12];
  BITCODE_RLx header_address;
  BITCODE_RL header_size;
  BITCODE_RL x04;
  BITCODE_RLd root_tree_node_gap;
  BITCODE_RLd lowermost_left_tree_node_gap;
  BITCODE_RLd lowermost_right_tree_node_gap;
  BITCODE_RL unknown_long;
  BITCODE_RL last_section_id;
  BITCODE_RLL last_section_address;
  BITCODE_RLL second_header_address;
  BITCODE_RL num_gaps;
  BITCODE_RL num_sections;
  BITCODE_RL x20;
  BITCODE_RL x80;
  BITCODE_RL x40;
  BITCODE_RL section_map_id;
  BITCODE_RLL section_map_address;
  BITCODE_RL section_info_id;
  BITCODE_RL section_array_size;
  BITCODE_RL gap_array_size;
  BITCODE_RLx crc32;
  BITCODE_RC padding[12];
  BITCODE_RL section_type;
  BITCODE_RL decomp_data_size;
  BITCODE_RL comp_data_size;
  BITCODE_RL compression_type;
  BITCODE_RLx checksum;
} Dwg_R2004_Header;

typedef struct Dwg_SummaryInfo
{
  BITCODE_RC intro[7];
  BITCODE_BL num_properties;
  Dwg_SummaryInfo_Property *properties;
} Dwg_SummaryInfo;

struct _dwg_struct
{
  struct Dwg_Header
  {
    Dwg_Version_Type version;
    Dwg_Version_Type from_version;
    BITCODE_RC   zero_5[5];
    BITCODE_RC   is_maint;
    BITCODE_RC   zero_one_or_three;
    BITCODE_RS   unknown_s[3];
    BITCODE_RL   thumbnail_address;
    BITCODE_RC   dwg_version;
    BITCODE_RC   maint_version;
    BITCODE_RS   codepage;
    BITCODE_RC   unknown_0;
    BITCODE_RC   app_dwg_version;
    BITCODE_RC   app_maint_version;
    BITCODE_RL   security_type;
    BITCODE_RL   rl_1c_address;
    BITCODE_RL   summaryinfo_address;
    BITCODE_RL   vbaproj_address;
    BITCODE_RL   rl_28_80;
    BITCODE_RL   num_sections;
    Dwg_Section* section;
    Dwg_Section_InfoHdr section_infohdr;
    Dwg_Section_Info* section_info;
  } header;

  Dwg_R2004_Header r2004_header;
  Dwg_AuxHeader auxheader;
  Dwg_SummaryInfo summaryinfo;
  Dwg_Chain thumbnail;
  Dwg_Header_Variables header_vars;
  BITCODE_BS num_classes;
  Dwg_Class * dwg_class;
  BITCODE_BL num_objects;
  Dwg_Object * object;
  BITCODE_BL num_entities;
  BITCODE_BL num_object_refs;
  Dwg_Object_Ref **object_ref;
  struct _inthash *object_map;
  int dirty_refs;
  Dwg_Object *mspace_block;
  Dwg_Object *pspace_block;
  Dwg_Object_BLOCK_CONTROL      block_control;
  Dwg_Object_LAYER_CONTROL      layer_control;
  Dwg_Object_STYLE_CONTROL      style_control;
  Dwg_Object_LTYPE_CONTROL      ltype_control;
  Dwg_Object_VIEW_CONTROL       view_control;
  Dwg_Object_UCS_CONTROL        ucs_control;
  Dwg_Object_VPORT_CONTROL      vport_control;
  Dwg_Object_APPID_CONTROL      appid_control;
  Dwg_Object_DIMSTYLE_CONTROL   dimstyle_control;
  Dwg_Object_VPORT_ENTITY_CONTROL  vport_entity_control;
  struct _dwg_second_header second_header;
  unsigned int layout_type;
  unsigned int opts;
};

typedef struct _bit_chain
{
  unsigned char *chain;
  long unsigned int size;
  long unsigned int byte;
  unsigned char bit;
  FILE *fh;
  Dwg_Version_Type version;
  Dwg_Version_Type from_version;
  unsigned char opts;
} Bit_Chain;

struct _hashbucket
{
  uint32_t key;
  uint32_t value;
};
typedef struct _inthash
{
  struct _hashbucket *array;
  uint32_t size;
  uint32_t elems;
} dwg_inthash;

#define DWG_LOGLEVEL_NONE 0
#define DWG_LOGLEVEL_ERROR 1
#define DWG_LOGLEVEL_INFO 2
#define DWG_LOGLEVEL_TRACE 3
#define DWG_LOGLEVEL_HANDLE 4
#define DWG_LOGLEVEL_INSANE 5
#define DWG_LOGLEVEL_ALL 9
#ifndef DWG_LOGLEVEL
#  define DWG_LOGLEVEL DWG_LOGLEVEL_ERROR
#endif
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

#define VERSION(v)                                                            \
  cur_ver = v;                                                                \
  if (dat->version == v)
#define VERSIONS(v1, v2)                                                      \
  cur_ver = v1;                                                               \
  if (dat->version >= v1 && dat->version <= v2)
#define SINCE(v)                                                              \
  cur_ver = v;                                                                \
  if (dat->version >= v)
#define UNTIL(v)                                                              \
  cur_ver = v;                                                                \
  if (dat->version <= v)
#define RESET_VER cur_ver = dat->version;

#define DEBUG_POS
#define DEBUG_HERE
#define DEBUG_POS_OBJ
#define DEBUG_HERE_OBJ

#define SAFENAME(name) (name) ? (name) : ""
#define SAFEDXFNAME (obj && obj->dxfname ? obj->dxfname : "")

#define strEQ(s1, s2) !strcmp ((s1), (s2))
#define strNE(s1, s2) strcmp ((s1), (s2))
#define strEQc(s1, s2) !strcmp ((s1), s2 "")

#define rad2deg(ang) (ang) * 90.0 / M_PI_2
#define deg2rad(ang) (ang) * M_PI_2 / 90.0

#define ATTRIBUTE_MALLOC
#define RETURNS_NONNULL

#ifndef EXPORT
#  if defined(_WIN32) && defined(ENABLE_SHARED)
#    ifdef DLL_EXPORT
#      define EXPORT __declspec(dllexport)
#    else
#      define EXPORT __declspec(dllimport)
#    endif
#  elif defined(__clang__) || defined(__clang) || \
         (defined( __GNUC__) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 303)
#    define EXPORT __attribute__ ((visibility ("default")))
#  else
#    define EXPORT
#  endif
#endif

#define DWG_OPTS_LOGLEVEL 0xf
#define DWG_OPTS_MINIMAL  0x10
#define DWG_OPTS_INDXF    0x20
#define DWG_OPTS_DXFB     0x40

#define COMMON_ENTITY_POLYLINE \
  struct _dwg_object_entity *parent; \
  BITCODE_B has_vertex; \
  BITCODE_BL num_owned; \
  BITCODE_H first_vertex; \
  BITCODE_H last_vertex; \
  BITCODE_H* vertex; \
  BITCODE_H seqend

#define DIMENSION_COMMON \
    struct _dwg_object_entity *parent; \
    BITCODE_RC class_version; \
    BITCODE_TV blockname; \
    BITCODE_BE extrusion; \
    BITCODE_3BD def_pt; \
    BITCODE_2RD text_midpt; \
    BITCODE_BD elevation; \
    BITCODE_RC flag; \
    BITCODE_RC flag1; \
    BITCODE_TV user_text; \
    BITCODE_BD text_rotation; \
    BITCODE_BD horiz_dir; \
    BITCODE_3BD ins_scale; \
    BITCODE_BD ins_rotation; \
    BITCODE_BS attachment; \
    BITCODE_BS lspace_style; \
    BITCODE_BD lspace_factor; \
    BITCODE_BD act_measurement; \
    BITCODE_B unknown; \
    BITCODE_B flip_arrow1; \
    BITCODE_B flip_arrow2; \
    BITCODE_2RD clone_ins_pt; \
    BITCODE_H dimstyle;       \
    BITCODE_H block;

#define _3DSOLID_FIELDS \
    BITCODE_BL modeler_format_version; \
    BITCODE_BL acis_data_size; \
    BITCODE_TF acis_data

#define TODO_ENCODER HANDLER (OUTPUT, "TODO: Encoder\n");
#define TODO_DECODER HANDLER (OUTPUT, "TODO: Decoder\n");

#define MIN(a,b) (((a)<(b))?(a):(b))

#define FIELD_VALUE(name) _obj->name
#define FIELDG(name, type, dxf)
#define FIELD_RL(name, dxf) FIELDG (name, RL, dxf);
#define FIELD_BLx(name, dxf) FIELDG (name, BLx, dxf);
#define FIELD_B(name, dxf) FIELDG (name, B, dxf);
#define FIELD_RC(name, dxf) FIELDG (name, RC, dxf);
#define FIELD_BS(name, dxf) FIELDG (name, BS, dxf);
#define FIELD_BL(name, dxf) FIELDG (name, BL, dxf);
#define FIELD_VECTOR(name, type, size, dxf)

typedef enum DWG_SENTINEL
{
  DWG_SENTINEL_HEADER_END,
  DWG_SENTINEL_THUMBNAIL_BEGIN,
  DWG_SENTINEL_THUMBNAIL_END,
  DWG_SENTINEL_VARIABLE_BEGIN,
  DWG_SENTINEL_VARIABLE_END,
  DWG_SENTINEL_CLASS_BEGIN,
  DWG_SENTINEL_CLASS_END,
  DWG_SENTINEL_SECOND_HEADER_BEGIN,
  DWG_SENTINEL_SECOND_HEADER_END
} Dwg_Sentinel;

extern const char version_codes[DWG_VERSIONS][7];
const char *dwg_version_type (const Dwg_Version_Type version);
EXPORT void dwg_errstrings (int error);
unsigned char *dwg_sentinel (Dwg_Sentinel sentinel);

void bit_advance_position (Bit_Chain *dat, long advance);
unsigned long bit_position (Bit_Chain *dat);
void bit_set_position (Bit_Chain *dat, unsigned long bitpos);
void bit_reset_chain (Bit_Chain *dat);
BITCODE_B bit_read_B (Bit_Chain *dat);
BITCODE_BB bit_read_BB (Bit_Chain *dat);
BITCODE_3B bit_read_3B (Bit_Chain *dat);
BITCODE_4BITS bit_read_4BITS (Bit_Chain *dat);
BITCODE_RC bit_read_RC (Bit_Chain *dat);
BITCODE_RS bit_read_RS (Bit_Chain *dat);
BITCODE_RS bit_read_RS_LE (Bit_Chain *dat);
BITCODE_RL bit_read_RL (Bit_Chain *dat);
BITCODE_RLL bit_read_RLL (Bit_Chain *dat);
BITCODE_RD bit_read_RD (Bit_Chain *dat);
BITCODE_BS bit_read_BS (Bit_Chain *dat);
BITCODE_BL bit_read_BL (Bit_Chain *dat);
BITCODE_BS bit_read_BOT (Bit_Chain *dat);
BITCODE_BLL bit_read_BLL (Bit_Chain *dat);
BITCODE_BD bit_read_BD (Bit_Chain *dat);
BITCODE_MC bit_read_MC (Bit_Chain *dat);
BITCODE_UMC bit_read_UMC (Bit_Chain *dat);
BITCODE_MS bit_read_MS (Bit_Chain *dat);
BITCODE_DD bit_read_DD (Bit_Chain *dat, double);
BITCODE_BT bit_read_BT (Bit_Chain *dat);
int bit_read_H (Bit_Chain *restrict dat, Dwg_Handle *restrict handle);
uint16_t bit_read_CRC (Bit_Chain *dat);
int bit_check_CRC (Bit_Chain *dat, long unsigned int start_address, const uint16_t seed);
uint16_t bit_calc_CRC (const uint16_t seed, unsigned char *adr, long len);
void bit_read_fixed (Bit_Chain *restrict dat, BITCODE_RC *restrict dest, unsigned int length);
BITCODE_TV bit_read_TV (Bit_Chain *restrict dat);
BITCODE_TU bit_read_TU (Bit_Chain *restrict dat);
BITCODE_TF bit_read_TF (Bit_Chain *dat, unsigned int length);
int bit_search_sentinel (Bit_Chain *dat, unsigned char sentinel[16]);
EXPORT char *bit_convert_TU (BITCODE_TU restrict wstr);
EXPORT BITCODE_TU bit_utf8_to_TU (char *restrict str);

int dat_read_file (Bit_Chain *restrict dat, FILE *restrict fp, const char *restrict filename);
int dat_read_stream (Bit_Chain *restrict dat, FILE *restrict fp);

int resolve_objectref_vector (Bit_Chain *restrict dat, Dwg_Data *restrict dwg);

int dwg_decode (Bit_Chain *restrict dat, Dwg_Data *restrict dwg);
int dwg_decode_header_variables (Bit_Chain *dat, Bit_Chain *hdl_dat, Bit_Chain *str_dat, Dwg_Data *restrict dwg);
int dwg_decode_add_object (Dwg_Data *restrict dwg, Bit_Chain *dat, Bit_Chain *hdl_dat, long unsigned int address);
int dwg_decode_unknown (Bit_Chain *restrict dat, Dwg_Object *restrict obj);
int dwg_decode_entity (Bit_Chain *dat, Bit_Chain *hdl_dat, Bit_Chain *str_dat, Dwg_Object_Entity *restrict ent);
int dwg_decode_ATTRIB (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_ATTDEF (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_BLOCK (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_ENDBLK (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_SEQEND (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_INSERT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_MINSERT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VERTEX_2D (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VERTEX_3D (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VERTEX_MESH (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VERTEX_PFACE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VERTEX_PFACE_FACE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_POLYLINE_2D (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_POLYLINE_3D (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_ARC (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_CIRCLE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_LINE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DIMENSION_ORDINATE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DIMENSION_LINEAR (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DIMENSION_ALIGNED (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DIMENSION_ANG3PT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DIMENSION_ANG2LN (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DIMENSION_RADIUS (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DIMENSION_DIAMETER (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_POINT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode__3DFACE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_POLYLINE_PFACE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_POLYLINE_MESH (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_SOLID (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_TRACE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_SHAPE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VIEWPORT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_ELLIPSE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_SPLINE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_REGION (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode__3DSOLID (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_BODY (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_RAY (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_XLINE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DICTIONARY (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_MTEXT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_LEADER (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_TOLERANCE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_MLINE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_BLOCK_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_BLOCK_HEADER (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_LAYER_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_LAYER (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_STYLE_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_STYLE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_LTYPE_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_LTYPE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VIEW_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VIEW (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_UCS_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_UCS (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VPORT_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VPORT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_APPID_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_APPID (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DIMSTYLE_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DIMSTYLE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VPORT_ENTITY_CONTROL (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_VPORT_ENTITY_HEADER (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_GROUP (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_MLINESTYLE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_OLE2FRAME (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_DUMMY (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_LONG_TRANSACTION (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_LWPOLYLINE (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_HATCH (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_XRECORD (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_PLACEHOLDER (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_OLEFRAME (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_LAYOUT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_PROXY_ENTITY (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_PROXY_OBJECT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_variable_type (Dwg_Data *dwg, Bit_Chain *dat, Bit_Chain *hdl_dat, Dwg_Object *obj);
int dwg_decode_UNKNOWN_ENT (Bit_Chain *dat, Dwg_Object *obj);
int dwg_decode_UNKNOWN_OBJ (Bit_Chain *dat, Dwg_Object *obj);
int dwg_validate_INSERT (Dwg_Object *restrict obj);
int dwg_validate_POLYLINE (Dwg_Object *restrict obj);
void check_POLYLINE_handles (Dwg_Object *obj);

dwg_inthash *hash_new (uint32_t size);
uint32_t hash_get (dwg_inthash *hash, uint32_t key);
void hash_set (dwg_inthash *hash, uint32_t key, uint32_t value);
void hash_free (dwg_inthash *hash);

EXPORT double dwg_model_x_min (const Dwg_Data *restrict);
EXPORT double dwg_model_y_min (const Dwg_Data *restrict);
EXPORT double dwg_model_x_max (const Dwg_Data *restrict);
EXPORT double dwg_model_y_max (const Dwg_Data *restrict);
EXPORT Dwg_Object_Ref * dwg_model_space_ref (Dwg_Data *restrict dwg);
EXPORT Dwg_Object_Ref * dwg_paper_space_ref (Dwg_Data *restrict dwg);
EXPORT Dwg_Object *dwg_ref_object_silent (const Dwg_Data *restrict dwg, Dwg_Object_Ref *restrict ref);
EXPORT Dwg_Object *dwg_resolve_handle (const Dwg_Data *restrict dwg, const unsigned long absref);
EXPORT Dwg_Object *dwg_next_object (const Dwg_Object *obj);
EXPORT Dwg_Object *get_first_owned_entity (const Dwg_Object *restrict hdr);
EXPORT Dwg_Object *get_next_owned_entity (const Dwg_Object *restrict hdr, const Dwg_Object *restrict current);
EXPORT int dwg_add_object (Dwg_Data *restrict dwg);
EXPORT int dwg_add_TEXT (Dwg_Object *obj);
EXPORT int dwg_class_is_entity (const Dwg_Class *restrict klass);
EXPORT unsigned char *dwg_bmp (const Dwg_Data *restrict, BITCODE_RL *restrict);
EXPORT void dwg_free (Dwg_Data *restrict dwg);
EXPORT int dwg_read_file (const char *restrict filename, Dwg_Data *restrict dwg);
EXPORT int dxf_cvt_lweight (const BITCODE_BSd value);

char * ATTRIBUTE_MALLOC htmlescape (const char *restrict src, const int codepage);
char * ATTRIBUTE_MALLOC htmlwescape (BITCODE_TU wsrc);

void normalize (BITCODE_3DPOINT *out, BITCODE_3DPOINT pt);
void cross (BITCODE_3DPOINT *out, BITCODE_3DPOINT pt1, BITCODE_3DPOINT pt2);
void transform_OCS_2d (BITCODE_2DPOINT *out, BITCODE_2DPOINT pt, BITCODE_BE ext);
void transform_OCS (BITCODE_3DPOINT *out, BITCODE_3DPOINT pt, BITCODE_BE ext);

static double transform_X (double x);
static double transform_Y (double y);
static void output_INSERT (Dwg_Object *obj);
static void output_LINE (Dwg_Object *obj);
static void output_CIRCLE (Dwg_Object *obj);
static void output_ARC (Dwg_Object *obj);
static void output_POLYLINE_2D (Dwg_Object *obj);
static void output_LWPOLYLINE (Dwg_Object *obj);
static void output_object (Dwg_Object *obj);
static void output_BLOCK_HEADER (Dwg_Object_Ref *ref);
static void output_SVG (Dwg_Data *dwg);
static int decode_R13_R2000 (Bit_Chain *restrict dat, Dwg_Data *restrict dwg);
static int usage (void);
static int opt_version (void);
static int help (void);

static int opts = 0;
Dwg_Data g_dwg;
double model_xmin, model_ymin;
double page_width, page_height, scale;
static int loglevel = 0;
static int cur_ver = 0;
static bool env_var_checked_p = false;

static double
transform_X (double x)
{
  return x - model_xmin;
}

static double
transform_Y (double y)
{
  return page_height - (y - model_ymin);
}
static void
output_TEXT (Dwg_Object *obj)
{
  Dwg_Data *dwg = obj->parent;
  Dwg_Entity_TEXT *text = obj->tio.entity->tio.TEXT;
  char *escaped;
  const char *fontfamily;
  BITCODE_H style_ref = text->style;
  Dwg_Object *o = style_ref ? dwg_ref_object_silent (dwg, style_ref) : NULL;
  Dwg_Object_STYLE *style = o ? o->tio.object->tio.STYLE : NULL;
  BITCODE_2DPOINT pt;

  if (!text->text_value){
    return;
  }

  if (dwg->header.version >= R_2007)
    escaped = htmlwescape ((BITCODE_TU)text->text_value);
  else
    escaped = htmlescape (text->text_value, (int)dwg->header.codepage);
  if (style && style->font_name && *style->font_name
#ifdef HAVE_STRCASESTR
      && strcasestr (style->font_name, ".ttf")
#else
      && (strstr (style->font_name, ".ttf")
          || strstr (style->font_name, ".TTF"))
#endif
  )
    {
#ifdef HAVE_STRCASESTR
      if (strcasestr (style->font_name, "Arial"))
#else
      if ((strstr (style->font_name, "arial"))
          || strstr (style->font_name, "Arial"))
#endif
        {
          fontfamily = "Arial";
        }
      else
        fontfamily = "Verdana";
    }
  else
    fontfamily = "Courier";

  transform_OCS_2d (&pt, text->insertion_pt, text->extrusion);
  printf ("\t<text id=\"dwg-object-%d\" x=\"%f\" y=\"%f\" "
          "font-family=\"%s\" font-size=\"%f\" fill=\"blue\">%s</text>\n",
          obj->index, transform_X (pt.x), transform_Y (pt.y), fontfamily,
          text->height /* fontsize */, escaped ? escaped : "");
  free (escaped);
}

static void
output_object (Dwg_Object *obj)
{
  if (!obj)
    {
      fprintf (stderr, "object is NULL\n");
      return;
    }

  switch (obj->type)
    {
    case DWG_TYPE_INSERT:
      output_INSERT (obj);
      break;
    case DWG_TYPE_LINE:
      output_LINE (obj);
      break;
    case DWG_TYPE_CIRCLE:
      output_CIRCLE (obj);
      break;
    case DWG_TYPE_TEXT:
      output_TEXT (obj);
      break;
    case DWG_TYPE_ARC:
      output_ARC (obj);
      break;
    case DWG_TYPE_POLYLINE_2D:
      output_POLYLINE_2D (obj);
      break;
    case DWG_TYPE_LWPOLYLINE:
      output_LWPOLYLINE (obj);
      break;
    default:
      // ignored
      break;
    }
}

EXPORT Dwg_Object *
get_first_owned_entity (const Dwg_Object *hdr)
{
  unsigned int version = hdr->parent->header.version;
  Dwg_Object_BLOCK_HEADER *_hdr = hdr->tio.object->tio.BLOCK_HEADER;
  if (hdr->type != DWG_TYPE_BLOCK_HEADER)
    {
      LOG_ERROR ("Invalid BLOCK_HEADER type %d", hdr->type);
      return NULL;
    }

  if (R_13 <= version && version <= R_2000)
    {
      return _hdr->first_entity ? _hdr->first_entity->obj : NULL;
    }
  else if (version >= R_2004)
    {
      _hdr->__iterator = 0;
      if (_hdr->entities && _hdr->num_owned && _hdr->entities[0])
        return _hdr->entities[0]->obj;
      else
        return NULL;
    }

  // TODO: preR13 block table
  LOG_ERROR ("Unsupported version: %d\n", version);
  return NULL;
}

EXPORT Dwg_Object *
dwg_next_object (const Dwg_Object *obj)
{
  Dwg_Data *dwg;
  if (!obj)
    return NULL;
  dwg = obj->parent;
  if ((obj->index + 1) > (dwg->num_objects - 1))
    return NULL;
  return &dwg->object[obj->index + 1];
}

static void
output_BLOCK_HEADER (Dwg_Object_Ref *ref)
{
  Dwg_Object *obj;
  Dwg_Object_BLOCK_HEADER *hdr;

  if (!ref) // silently ignore empty pspaces
    return;
  if (!ref->obj)
    return;
  obj = ref->obj;
  if (obj->type != DWG_TYPE_BLOCK_HEADER)
    {
      fprintf (stderr, "Argument not a BLOCK_HEADER reference\n");
      return;
    }
  /* TODO: Review.  (This check avoids a segfault, but it is
     still unclear whether or not the condition is valid.)  */
  if (!obj->tio.object)
    {
      fprintf (stderr, "Found null obj->tio.object\n");
      return;
    }

  hdr = obj->tio.object->tio.BLOCK_HEADER;
  if (hdr->name)
    {
      char *escaped;
      Dwg_Data *dwg = obj->parent;
      if (dwg->header.version >= R_2007)
        escaped = htmlwescape ((BITCODE_TU)hdr->name);
      else
        escaped = htmlescape (hdr->name, dwg->header.codepage);
      printf ("\t<g id=\"symbol-%lX\" >\n\t\t<!-- %s -->\n", ref->absolute_ref,
              escaped ? escaped : "");
      if (escaped)
        free (escaped);
    }

  obj = get_first_owned_entity (ref->obj);
  while (obj)
    {
      output_object (obj);
      obj = get_next_owned_entity (ref->obj, obj);
    }

  printf ("\t</g>\n");
}

EXPORT Dwg_Object_Ref *
dwg_model_space_ref (Dwg_Data *dwg)
{
  if (dwg->header_vars.BLOCK_RECORD_MSPACE
      && dwg->header_vars.BLOCK_RECORD_MSPACE->obj)
    return dwg->header_vars.BLOCK_RECORD_MSPACE;
  return dwg->block_control.model_space && dwg->block_control.model_space->obj
             ? dwg->block_control.model_space
             : NULL;
}

static void
output_SVG (Dwg_Data *dwg)
{
  BITCODE_BS i;
  Dwg_Object *obj;
  Dwg_Object_Ref *ref;
  Dwg_Object_BLOCK_CONTROL *block_control;
  double dx, dy;

  model_xmin = dwg_model_x_min (dwg);
  model_ymin = dwg_model_y_min (dwg);

  dx = (dwg_model_x_max (dwg) - dwg_model_x_min (dwg));
  dy = (dwg_model_y_max (dwg) - dwg_model_y_min (dwg));
  // double scale_x = dx / (dwg_page_x_max(dwg) - dwg_page_x_min(dwg));
  // double scale_y = dy / (dwg_page_y_max(dwg) - dwg_page_y_min(dwg));
  // scale = 25.4 / 72; // pt:mm
  page_width = dx;
  page_height = dy;
  // scale *= (scale_x > scale_y ? scale_x : scale_y);

  printf ("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n"
          "<svg\n"
          "   xmlns:svg=\"http://www.w3.org/2000/svg\"\n"
          "   xmlns=\"http://www.w3.org/2000/svg\"\n"
          "   xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
          "   version=\"1.1\"\n"
          "   width=\"%f\"\n"
          "   height=\"%f\"\n"
          ">\n",
          page_width, page_height);

  if ((ref = dwg_model_space_ref (dwg)))
    output_BLOCK_HEADER (ref);
  if ((ref = dwg_paper_space_ref (dwg)))
    output_BLOCK_HEADER (ref);
  printf ("\t<defs>\n");
  for (i = 0; i < dwg->block_control.num_entries; i++)
    {
      if ((ref = dwg->block_control.entries[i]))
        output_BLOCK_HEADER (ref);
    }
  printf ("\t</defs>\n");

  printf ("</svg>\n");
}

BITCODE_TV
bit_read_TV(Bit_Chain *restrict dat) {
  unsigned int i;
  unsigned int length;
  unsigned char *chain;

  length = bit_read_BS(dat);
  if (dat->byte + length >= dat->size) {
    loglevel = dat->opts & DWG_OPTS_LOGLEVEL;
    LOG_ERROR("%s buffer overflow at %lu, length %u", __FUNCTION__, dat->byte,
              length)
    return NULL;
  }
  chain = (unsigned char *)malloc(length + 1);
  for (i = 0; i < length; i++) {
    chain[i] = bit_read_RC(dat);
#if 0
      /* we support encode and codepages */
      if (!isprint (chain[i]))
        chain[i] = '~';
#endif
  }
  chain[i] = '\0';

  return (char *)chain;
}

static int
dwg_decode_TEXT_private (Bit_Chain *dat, Bit_Chain *str_dat, Dwg_Object *obj)
{
  int error;
  Dwg_Object_Entity *_ent = obj->tio.entity;
  Dwg_Entity_TEXT *_obj = obj->tio.entity->tio.TEXT;

  _ent->dwg = obj->parent;
  _ent->objid = obj->index;
  _obj->parent = _ent;

  error = dwg_decode_entity (dat, dat, str_dat, _ent);
  if (error >= DWG_ERR_CLASSESNOTFOUND)
    return error;

  if (dat->version >= R_2000 && dat->version < R_2007)
    _obj->text_value = bit_read_TV (dat);

  return error;
}

static int
dwg_decode_TEXT (Bit_Chain *dat, Dwg_Object *obj)
{
  int error = dwg_add_TEXT (obj);
  if (error)
    return error;
  cur_ver = R_2007;
  if (dat->version >= R_2007)
    {
      Bit_Chain obj_dat, str_dat, hdl_dat;
      obj_dat = *dat;
      hdl_dat = *dat;
      str_dat = *dat;
      error = dwg_decode_TEXT_private (&obj_dat, &str_dat, obj);
    }
  else
    {
      error = dwg_decode_TEXT_private (dat, dat, obj);
    }
  return error;
}

int
dwg_decode_add_object (Dwg_Data *restrict dwg, Bit_Chain *dat,
                       Bit_Chain *hdl_dat, long unsigned int address)
{
  long unsigned int objpos, restartpos;
  Bit_Chain abs_dat = { NULL };
  unsigned char previous_bit;
  Dwg_Object *restrict obj;
  BITCODE_BL num = dwg->num_objects;
  int error = 0;
  int realloced = 0;

  /* Keep the previous full chain  */
  abs_dat = *dat;

  /* Use the indicated address for the object
   */
  dat->byte = address;
  dat->bit = 0;

  // DEBUG_HERE;
  /*
   * Reserve memory space for objects. A realloc violates all internal
   * pointers.
   */
  realloced = dwg_add_object (dwg);
  if (realloced > 0)
    {
      *dat = abs_dat;
      return realloced; // i.e. DWG_ERR_OUTOFMEM
    }
  obj = &dwg->object[num];
  LOG_INFO ("==========================================\n"
            "Object number: %lu/%lX",
            (unsigned long)num, (unsigned long)num)

  obj->size = bit_read_MS (dat);
  LOG_INFO (", Size: %d [MS]", obj->size)
  SINCE (R_2010)
  {
    /* This is not counted in the object size */
    obj->handlestream_size = bit_read_UMC (dat);
    LOG_INFO (", Hdlsize: " FORMAT_UMC " [UMC] ", obj->handlestream_size);
    obj->bitsize = obj->size * 8 - obj->handlestream_size;
    // TODO boundscheck
  }

  objpos = bit_position (dat); // absolute
  obj->address = dat->byte;

  /* Until here dat is absolute. now restrict it */
  bit_reset_chain (dat);
  if (obj->size > dat->size)
    {
      LOG_ERROR ("\nInvalid object size. Would overflow");
      *dat = abs_dat;
      return DWG_ERR_VALUEOUTOFBOUNDS;
    }
  dat->size = obj->size;

  SINCE (R_2010) { obj->type = bit_read_BOT (dat); }
  else
  {
    obj->type = bit_read_BS (dat);
  }
  LOG_INFO (", Type: %d [%s]\n", obj->type,
            dat->version >= R_2010 ? "BOT" : "BS");
  restartpos = bit_position (dat); // relative

  /* Check the type of the object
   */
  switch (obj->type)
    {
    case DWG_TYPE_TEXT:
      error = dwg_decode_TEXT (dat, obj);
      break;
    case DWG_TYPE_ATTRIB:
      error = dwg_decode_ATTRIB (dat, obj);
      break;
    case DWG_TYPE_ATTDEF:
      error = dwg_decode_ATTDEF (dat, obj);
      break;
    case DWG_TYPE_BLOCK:
      error = dwg_decode_BLOCK (dat, obj);
      break;
    case DWG_TYPE_ENDBLK:
      error = dwg_decode_ENDBLK (dat, obj);
      break;
    case DWG_TYPE_SEQEND:
      error = dwg_decode_SEQEND (dat, obj);
      if (dat->version >= R_13 && obj->tio.entity->ownerhandle)
        {
          Dwg_Object *restrict owner = dwg_resolve_handle (
              dwg, obj->tio.entity->ownerhandle->absolute_ref);
          if (!owner)
            {
              LOG_WARN ("no SEQEND.ownerhandle")
            }
          else if (owner->fixedtype == DWG_TYPE_INSERT
                   || owner->fixedtype == DWG_TYPE_MINSERT)
            {
              /* SEQEND handle for the owner needed in validate_INSERT */
              hash_set (dwg->object_map, obj->handle.value, (uint32_t)num);
              (void)dwg_validate_INSERT (owner);
            }
          else if (owner->fixedtype == DWG_TYPE_POLYLINE_2D
                   || owner->fixedtype == DWG_TYPE_POLYLINE_3D
                   || owner->fixedtype == DWG_TYPE_POLYLINE_PFACE
                   || owner->fixedtype == DWG_TYPE_POLYLINE_MESH)
            {
              Dwg_Entity_POLYLINE_2D *restrict _obj
                  = owner->tio.entity->tio.POLYLINE_2D;
              if (!_obj->seqend)
                /* SEQEND handle for the owner needed in validate_POLYLINE */
                hash_set (dwg->object_map, obj->handle.value, (uint32_t)num);
              (void)dwg_validate_POLYLINE (owner);
            }
        }
      break;
    case DWG_TYPE_INSERT:
      error = dwg_decode_INSERT (dat, obj);
      break;
    case DWG_TYPE_MINSERT:
      error = dwg_decode_MINSERT (dat, obj);
      break;
    case DWG_TYPE_VERTEX_2D:
      error = dwg_decode_VERTEX_2D (dat, obj);
      break;
    case DWG_TYPE_VERTEX_3D:
      error = dwg_decode_VERTEX_3D (dat, obj);
      break;
    case DWG_TYPE_VERTEX_MESH:
      error = dwg_decode_VERTEX_MESH (dat, obj);
      break;
    case DWG_TYPE_VERTEX_PFACE:
      error = dwg_decode_VERTEX_PFACE (dat, obj);
      break;
    case DWG_TYPE_VERTEX_PFACE_FACE:
      error = dwg_decode_VERTEX_PFACE_FACE (dat, obj);
      break;
    case DWG_TYPE_POLYLINE_2D:
      error = dwg_decode_POLYLINE_2D (dat, obj);
      if (dat->version >= R_2010)
        check_POLYLINE_handles (obj);
      break;
    case DWG_TYPE_POLYLINE_3D:
      error = dwg_decode_POLYLINE_3D (dat, obj);
      if (dat->version >= R_2010)
        check_POLYLINE_handles (obj);
      break;
    case DWG_TYPE_ARC:
      error = dwg_decode_ARC (dat, obj);
      break;
    case DWG_TYPE_CIRCLE:
      error = dwg_decode_CIRCLE (dat, obj);
      break;
    case DWG_TYPE_LINE:
      error = dwg_decode_LINE (dat, obj);
      break;
    case DWG_TYPE_DIMENSION_ORDINATE:
      error = dwg_decode_DIMENSION_ORDINATE (dat, obj);
      break;
    case DWG_TYPE_DIMENSION_LINEAR:
      error = dwg_decode_DIMENSION_LINEAR (dat, obj);
      break;
    case DWG_TYPE_DIMENSION_ALIGNED:
      error = dwg_decode_DIMENSION_ALIGNED (dat, obj);
      break;
    case DWG_TYPE_DIMENSION_ANG3PT:
      error = dwg_decode_DIMENSION_ANG3PT (dat, obj);
      break;
    case DWG_TYPE_DIMENSION_ANG2LN:
      error = dwg_decode_DIMENSION_ANG2LN (dat, obj);
      break;
    case DWG_TYPE_DIMENSION_RADIUS:
      error = dwg_decode_DIMENSION_RADIUS (dat, obj);
      break;
    case DWG_TYPE_DIMENSION_DIAMETER:
      error = dwg_decode_DIMENSION_DIAMETER (dat, obj);
      break;
    case DWG_TYPE_POINT:
      error = dwg_decode_POINT (dat, obj);
      break;
    case DWG_TYPE__3DFACE:
      error = dwg_decode__3DFACE (dat, obj);
      break;
    case DWG_TYPE_POLYLINE_PFACE:
      error = dwg_decode_POLYLINE_PFACE (dat, obj);
      if (dat->version >= R_2010)
        check_POLYLINE_handles (obj);
      break;
    case DWG_TYPE_POLYLINE_MESH:
      error = dwg_decode_POLYLINE_MESH (dat, obj);
      if (dat->version >= R_2010)
        check_POLYLINE_handles (obj);
      break;
    case DWG_TYPE_SOLID:
      error = dwg_decode_SOLID (dat, obj);
      break;
    case DWG_TYPE_TRACE:
      error = dwg_decode_TRACE (dat, obj);
      break;
    case DWG_TYPE_SHAPE:
      error = dwg_decode_SHAPE (dat, obj);
      break;
    case DWG_TYPE_VIEWPORT:
      error = dwg_decode_VIEWPORT (dat, obj);
      break;
    case DWG_TYPE_ELLIPSE:
      error = dwg_decode_ELLIPSE (dat, obj);
      break;
    case DWG_TYPE_SPLINE:
      error = dwg_decode_SPLINE (dat, obj);
      break;
    case DWG_TYPE_REGION:
      error = dwg_decode_REGION (dat, obj);
      break;
    case DWG_TYPE__3DSOLID:
      error = dwg_decode__3DSOLID (dat, obj);
      break;
    case DWG_TYPE_BODY:
      error = dwg_decode_BODY (dat, obj);
      break;
    case DWG_TYPE_RAY:
      error = dwg_decode_RAY (dat, obj);
      break;
    case DWG_TYPE_XLINE:
      error = dwg_decode_XLINE (dat, obj);
      break;
    case DWG_TYPE_DICTIONARY:
      error = dwg_decode_DICTIONARY (dat, obj);
      break;
    case DWG_TYPE_MTEXT:
      error = dwg_decode_MTEXT (dat, obj);
      break;
    case DWG_TYPE_LEADER:
      error = dwg_decode_LEADER (dat, obj);
      break;
    case DWG_TYPE_TOLERANCE:
      error = dwg_decode_TOLERANCE (dat, obj);
      break;
    case DWG_TYPE_MLINE:
      error = dwg_decode_MLINE (dat, obj);
      break;
    case DWG_TYPE_BLOCK_CONTROL:
      error = dwg_decode_BLOCK_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.BLOCK_CONTROL)
        {
          obj->tio.object->tio.BLOCK_CONTROL->objid = num;
          if (!dwg->block_control.parent) // only once
            dwg->block_control = *obj->tio.object->tio.BLOCK_CONTROL;
          else
            LOG_WARN ("Second BLOCK_CONTROL object ignored");
        }
      break;
    case DWG_TYPE_BLOCK_HEADER:
      error = dwg_decode_BLOCK_HEADER (dat, obj);
      /*
       * We cannot cache dwg->*space_block here as dwg->objects might get
       * realloc'ed. See dwg_model_space_object() and dwg_paper_space_object()
       * instead.
       */
      break;
    case DWG_TYPE_LAYER_CONTROL:
      error = dwg_decode_LAYER_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.LAYER_CONTROL)
        {
          obj->tio.object->tio.LAYER_CONTROL->objid = num;
          dwg->layer_control = *obj->tio.object->tio.LAYER_CONTROL;
        }
      break;
    case DWG_TYPE_LAYER:
      error = dwg_decode_LAYER (dat, obj);
      break;
    case DWG_TYPE_STYLE_CONTROL:
      error = dwg_decode_STYLE_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.STYLE_CONTROL)
        {
          obj->tio.object->tio.STYLE_CONTROL->objid = num;
          dwg->style_control = *obj->tio.object->tio.STYLE_CONTROL;
        }
      break;
    case DWG_TYPE_STYLE:
      error = dwg_decode_STYLE (dat, obj);
      break;
    case DWG_TYPE_LTYPE_CONTROL:
      error = dwg_decode_LTYPE_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.LTYPE_CONTROL)
        {
          obj->tio.object->tio.LTYPE_CONTROL->objid = num;
          dwg->ltype_control = *obj->tio.object->tio.LTYPE_CONTROL;
        }
      break;
    case DWG_TYPE_LTYPE:
      error = dwg_decode_LTYPE (dat, obj);
      break;
    case DWG_TYPE_VIEW_CONTROL:
      error = dwg_decode_VIEW_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.VIEW_CONTROL)
        {
          obj->tio.object->tio.VIEW_CONTROL->objid = num;
          dwg->view_control = *obj->tio.object->tio.VIEW_CONTROL;
        }
      break;
    case DWG_TYPE_VIEW:
      error = dwg_decode_VIEW (dat, obj);
      break;
    case DWG_TYPE_UCS_CONTROL:
      error = dwg_decode_UCS_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.UCS_CONTROL)
        {
          obj->tio.object->tio.UCS_CONTROL->objid = num;
          dwg->ucs_control = *obj->tio.object->tio.UCS_CONTROL;
        }
      break;
    case DWG_TYPE_UCS:
      error = dwg_decode_UCS (dat, obj);
      break;
    case DWG_TYPE_VPORT_CONTROL:
      error = dwg_decode_VPORT_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.VPORT_CONTROL)
        {
          obj->tio.object->tio.VPORT_CONTROL->objid = num;
          dwg->vport_control = *obj->tio.object->tio.VPORT_CONTROL;
        }
      break;
    case DWG_TYPE_VPORT:
      error = dwg_decode_VPORT (dat, obj);
      break;
    case DWG_TYPE_APPID_CONTROL:
      error = dwg_decode_APPID_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.APPID_CONTROL)
        {
          obj->tio.object->tio.APPID_CONTROL->objid = num;
          dwg->appid_control = *obj->tio.object->tio.APPID_CONTROL;
        }
      break;
    case DWG_TYPE_APPID:
      error = dwg_decode_APPID (dat, obj);
      break;
    case DWG_TYPE_DIMSTYLE_CONTROL:
      error = dwg_decode_DIMSTYLE_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.DIMSTYLE_CONTROL)
        {
          obj->tio.object->tio.DIMSTYLE_CONTROL->objid = num;
          dwg->dimstyle_control = *obj->tio.object->tio.DIMSTYLE_CONTROL;
        }
      break;
    case DWG_TYPE_DIMSTYLE:
      error = dwg_decode_DIMSTYLE (dat, obj);
      break;
    case DWG_TYPE_VPORT_ENTITY_CONTROL:
      error = dwg_decode_VPORT_ENTITY_CONTROL (dat, obj);
      if (!error && obj->tio.object->tio.VPORT_ENTITY_CONTROL)
        {
          obj->tio.object->tio.VPORT_ENTITY_CONTROL->objid = num;
          dwg->vport_entity_control
              = *obj->tio.object->tio.VPORT_ENTITY_CONTROL;
        }
      break;
    case DWG_TYPE_VPORT_ENTITY_HEADER:
      error = dwg_decode_VPORT_ENTITY_HEADER (dat, obj);
      break;
    case DWG_TYPE_GROUP:
      error = dwg_decode_GROUP (dat, obj);
      break;
    case DWG_TYPE_MLINESTYLE:
      error = dwg_decode_MLINESTYLE (dat, obj);
      break;
    case DWG_TYPE_OLE2FRAME:
      error = dwg_decode_OLE2FRAME (dat, obj);
      break;
    case DWG_TYPE_DUMMY:
      error = dwg_decode_DUMMY (dat, obj);
      break;
    case DWG_TYPE_LONG_TRANSACTION:
      error = dwg_decode_LONG_TRANSACTION (dat, obj);
      break;
    case DWG_TYPE_LWPOLYLINE:
      error = dwg_decode_LWPOLYLINE (dat, obj);
      break;
    case DWG_TYPE_HATCH:
      error = dwg_decode_HATCH (dat, obj);
      break;
    case DWG_TYPE_XRECORD:
      error = dwg_decode_XRECORD (dat, obj);
      break;
    case DWG_TYPE_PLACEHOLDER:
      error = dwg_decode_PLACEHOLDER (dat, obj);
      break;
    case DWG_TYPE_OLEFRAME:
      error = dwg_decode_OLEFRAME (dat, obj);
      break;
    case DWG_TYPE_VBA_PROJECT:
      LOG_ERROR ("Unhandled Object VBA_PROJECT. Has its own section");
      // dwg_decode_VBA_PROJECT(dat, obj);
      error = DWG_ERR_UNHANDLEDCLASS;
      break;
    case DWG_TYPE_LAYOUT:
      error = dwg_decode_LAYOUT (dat, obj);
      break;
    case DWG_TYPE_PROXY_ENTITY:
      error = dwg_decode_PROXY_ENTITY (dat, obj);
      break;
    case DWG_TYPE_PROXY_OBJECT:
      error = dwg_decode_PROXY_OBJECT (dat, obj);
      break;
    default:
      if (obj->type == dwg->layout_type)
        error = dwg_decode_LAYOUT (dat, obj);
      /* > 500 */
      else if ((error = dwg_decode_variable_type (dwg, dat, hdl_dat, obj))
               & DWG_ERR_UNHANDLEDCLASS)
        {
          int is_entity = 0;
          int i = obj->type - 500;
          Dwg_Class *klass = NULL;

          /* restart and read into the UNKNOWN_OBJ object */
          /* the relative offset from type after common_entity_data */
          // obj->common_size = bit_position(dat) - restartpos;
          // LOG_HANDLE("common_size: %lu\n", obj->common_size); // needed for
          // unknown
          bit_set_position (dat, restartpos);

          if (i >= 0 && i < (int)dwg->num_classes)
            {
              klass = &dwg->dwg_class[i];
              is_entity = dwg_class_is_entity (klass);
            }
          else
            {
              if (i < 0)
                LOG_ERROR ("Invalid class index %d < 0", i)
              else
                LOG_ERROR ("Invalid class index %d >= %d", i,
                           (int)dwg->num_classes)
              obj->type = 0;
              *dat = abs_dat;
              return error | DWG_ERR_VALUEOUTOFBOUNDS;
            }

          // properly dwg_decode_object/_entity for eed, reactors, xdic
          if (is_entity)
            error |= dwg_decode_UNKNOWN_ENT (dat, obj);
          else
            error |= dwg_decode_UNKNOWN_OBJ (dat, obj);

          if (!dat)
            return error;
          if (error >= DWG_ERR_CRITICAL)
            *dat = abs_dat;
        }
    }

  if (obj->handle.value)
    { // empty only with UNKNOWN
      LOG_HANDLE (" object_map{%lX} = %lu\n", obj->handle.value,
                  (unsigned long)num);
      hash_set (dwg->object_map, obj->handle.value, (uint32_t)num);
    }

  if (dat->byte > 8 * dat->size)
    {
      LOG_ERROR ("Invalid object address (overflow): %lu > %lu", dat->byte,
                 8 * dat->size);
      *dat = abs_dat;
      return error | DWG_ERR_INVALIDDWG;
    }

  /* Restore the old absolute chain.
     CRC needs to be calculated from address, which is before our 0 position.
   */
  restartpos = bit_position (dat);
  *dat = abs_dat;
  bit_set_position (dat, objpos + restartpos);

  /* Now 1 padding bits until next byte, and then a RS CRC */
  if (dat->bit)
    {
      unsigned char r = 8 - dat->bit;
      LOG_HANDLE (" padding: %X/%X (%d bits)\n", dat->chain[dat->byte],
                  dat->chain[dat->byte] & ((1 << r) - 1), r);
      bit_advance_position (dat, r);
    }
  bit_set_position (dat, (obj->address + obj->size) * 8 - 2);
  if (!bit_check_CRC (dat, address, 0xC0C1))
    error |= DWG_ERR_WRONGCRC;

  /* Reset to previous addresses for return */
  *dat = abs_dat;

  return realloced ? -1 : error; // re-alloced or not
}

static int
decode_R13_R2000 (Bit_Chain *restrict dat, Dwg_Data *restrict dwg)
{
  Dwg_Object *obj = NULL;
  unsigned int section_size = 0;
  unsigned char sgdc[2];
  unsigned int crc, crc2;
  long unsigned int size;
  long unsigned int lasta;
  long unsigned int lastmap;
  long unsigned int startpos;
  long unsigned int object_begin;
  long unsigned int object_end;
  long unsigned int pvz;
  BITCODE_BL j, k;
  int error = 0;
  const char *section_names[]
      = { "AcDb:Header", "AcDb:Classes", "AcDb:Handles",
          "2NDHEADER",   "MEASUREMENT",  "AcDb:AuxHeader" };

  {
    int i;
    struct Dwg_Header *_obj = &dwg->header;
    Bit_Chain *hdl_dat = dat;
    dat->byte = 0x06;
    // clang-format off
    // header.spec included for code generation - skipped
    // clang-format on
  }

  /* Section Locator Records 0x15 */
  assert (dat->byte == 0x15);
  dwg->header.num_sections = bit_read_RL (dat);
  LOG_TRACE ("\nnum_sections: " FORMAT_RL " [RL]\n", dwg->header.num_sections)
  if (!dwg->header.num_sections) // ODA writes zeros.
    dwg->header.num_sections = 6;
  if (dwg->header.num_sections < 3)
    {
      LOG_ERROR ("Not enough sections: " FORMAT_RL, dwg->header.num_sections);
      return DWG_ERR_INVALIDDWG;
    }
  if (dwg->header.num_sections > 10)
    {
      LOG_ERROR ("Too many sections: " FORMAT_RL, dwg->header.num_sections);
      return DWG_ERR_INVALIDDWG;
    }

  // So far seen 3-6 sections. Most emit only 3-5 sections.
  dwg->header.section = (Dwg_Section *)calloc (
      1, sizeof (Dwg_Section) * dwg->header.num_sections);
  if (!dwg->header.section)
    {
      LOG_ERROR ("Out of memory");
      return DWG_ERR_OUTOFMEM;
    }
  /* section 0: header vars
   *         1: class section
   *         2: object map
   *         3: (R13 c3 and later): 2nd header (special table, no sentinels)
   *         4: optional: MEASUREMENT
   *         5: optional: AuxHeader (no sentinels)
   */
  for (j = 0; j < dwg->header.num_sections; j++)
    {
      dwg->header.section[j].number = bit_read_RC (dat);
      dwg->header.section[j].address = bit_read_RL (dat);
      dwg->header.section[j].size = bit_read_RL (dat);
      if (j < 6)
        strcpy (dwg->header.section[j].name, section_names[j]);
      LOG_TRACE ("section[%u].number:  %4d [RC] %s\n", j,
                 (int)dwg->header.section[j].number,
                 dwg->header.section[j].name)
      LOG_TRACE ("section[%u].address: %4u [RL]\n", j,
                 (unsigned)dwg->header.section[j].address)
      LOG_TRACE ("section[%u].size:    %4u [RL]\n", j,
                 (unsigned)dwg->header.section[j].size);
      if (dwg->header.section[j].address + dwg->header.section[j].size
          > dat->size)
        {
          LOG_ERROR ("section[%u] address or size overflow", j);
          return DWG_ERR_INVALIDDWG;
        }
    }

  // Check CRC up to now (note: ODA has a bug here)
  crc2 = bit_calc_CRC (0xC0C1, &dat->chain[0], dat->byte); // from 0 to now
  crc = bit_read_RS (dat);
  LOG_TRACE ("crc: %04X [RSx] from 0-%lu\n", crc, dat->byte - 2);
  if (crc != crc2)
    {
      LOG_ERROR ("Header CRC mismatch %04X <=> %04X", crc, crc2);
      error |= DWG_ERR_WRONGCRC;
    }

  if (bit_search_sentinel (dat, dwg_sentinel (DWG_SENTINEL_HEADER_END)))
    LOG_TRACE ("         HEADER (end):    %4u\n", (unsigned)dat->byte)

  /*-------------------------------------------------------------------------
   * Section 5 AuxHeader
   * R2000+, mostly redundant file header information
   */

  if (dwg->header.num_sections == 6 && dwg->header.version >= R_2000)
    {
      int i;
      struct Dwg_AuxHeader *_obj = &dwg->auxheader;
      Bit_Chain *hdl_dat = dat;
      BITCODE_BL end_address
          = dwg->header.section[SECTION_AUXHEADER_R2000].address
            + dwg->header.section[SECTION_AUXHEADER_R2000].size;

      obj = NULL;
      dat->byte = dwg->header.section[SECTION_AUXHEADER_R2000].address;
      LOG_TRACE ("\n"
                 "=======> AuxHeader:       %4u\n",
                 (unsigned)dat->byte)
      LOG_TRACE ("         AuxHeader (end): %4u\n", (unsigned)end_address)
      if (dat->size < end_address)
        {
          LOG_ERROR ("Invalid AuxHeader size: buffer overflow")
          error |= DWG_ERR_SECTIONNOTFOUND;
        }
      else
        {
          BITCODE_BL old_size = dat->size;
          dat->size = end_address;
          // clang-format off
          // auxheader.spec included for code generation - skipped
          // clang-format on
          dat->size = old_size;
        }
    }

  /*-------------------------------------------------------------------------
   * Picture (Pre-R13C3?)
   */

  if (bit_search_sentinel (dat, dwg_sentinel (DWG_SENTINEL_THUMBNAIL_BEGIN)))
    {
      unsigned long int start_address;

      dat->bit = 0;
      start_address = dat->byte;
      LOG_TRACE ("\n=======> Thumbnail:       %4u\n",
                 (unsigned int)start_address - 16);
      if (dwg->header.thumbnail_address
          && dwg->header.thumbnail_address != dat->byte - 16)
        LOG_WARN ("Illegal HEADER.thumbnail_address: %i != %lu",
                  dwg->header.thumbnail_address, dat->byte - 16)
      dwg->header.thumbnail_address = dat->byte - 16;
      if (bit_search_sentinel (dat, dwg_sentinel (DWG_SENTINEL_THUMBNAIL_END)))
        {
          BITCODE_RL bmpsize;
          LOG_TRACE ("         Thumbnail (end): %4u\n",
                     (unsigned int)dat->byte)
          dwg->thumbnail.size = (dat->byte - 16) - start_address;
          dwg->thumbnail.chain
              = (unsigned char *)calloc (dwg->thumbnail.size, 1);
          if (!dwg->thumbnail.chain)
            {
              LOG_ERROR ("Out of memory");
              return DWG_ERR_OUTOFMEM;
            }
          memcpy (dwg->thumbnail.chain, &dat->chain[start_address],
                  dwg->thumbnail.size);
          dat->byte += dwg->thumbnail.size;

          dwg_bmp (dwg, &bmpsize);
          if (bmpsize > dwg->thumbnail.size)
            LOG_ERROR ("BMP size overflow: %i > %lu\n", bmpsize,
                       dwg->thumbnail.size)
        }
    }

  /*-------------------------------------------------------------------------
   * Header Variables, section 0
   */

  LOG_INFO ("\n"
            "=======> Header Variables:         %4u\n",
            (unsigned int)dwg->header.section[SECTION_HEADER_R13].address)
  LOG_INFO ("         Header Variables   (end): %4u\n",
            (unsigned int)(dwg->header.section[SECTION_HEADER_R13].address
                           + dwg->header.section[SECTION_HEADER_R13].size))
  if (dwg->header.section[SECTION_HEADER_R13].address < 58
      || dwg->header.section[SECTION_HEADER_R13].address
                 + dwg->header.section[SECTION_HEADER_R13].size
             > dat->size)
    {
      LOG_ERROR ("Invalid Header section, skipped")
      error |= DWG_ERR_SECTIONNOTFOUND;
      goto classes_section;
    }
  // after sentinel
  dat->byte = pvz = dwg->header.section[SECTION_HEADER_R13].address + 16;
  // LOG_HANDLE ("@ 0x%lx.%lu\n", bit_position (dat)/8, bit_position (dat)%8);
  dwg->header_vars.size = bit_read_RL (dat);
  LOG_TRACE ("         Length: " FORMAT_RL " [RL]\n", dwg->header_vars.size)
  dat->bit = 0;

  dwg_decode_header_variables (dat, dat, dat, dwg);

  // LOG_HANDLE ("@ 0x%lx.%lu\n", bit_position (dat)/8, bit_position (dat)%8);
  // check slack
  if (dat->bit || dat->byte != pvz + dwg->header_vars.size + 4)
    {
      unsigned char r = 8 - dat->bit;
      LOG_HANDLE (" padding: %ld byte, %d bits\n",
                  pvz + dwg->header_vars.size + 4 - dat->byte, r);
    }
  // Check CRC, hardcoded to 2 before end sentinel
  LOG_HANDLE (" crc pos: %lu\n", pvz + dwg->header_vars.size + 4);
  bit_set_position (dat, (pvz + dwg->header_vars.size + 4) * 8);
  crc = bit_read_RS (dat);
  LOG_TRACE ("crc: %04X [RSx] from %lu-%lu=%ld\n", crc, pvz, dat->byte - 2,
             dat->byte - 2 - pvz);
  crc2 = 0;
  // LOG_HANDLE ("@ 0x%lx\n", bit_position (dat)/8);
  // LOG_HANDLE ("HEADER_R13.address of size 0x%lx\n", pvz);
  // LOG_HANDLE ("HEADER_R13.size %d\n",
  // dwg->header.section[SECTION_HEADER_R13].size);
  // typical sizes: 400-599
  if (dwg->header.section[SECTION_HEADER_R13].size > 34
      && dwg->header.section[SECTION_HEADER_R13].size < 0xfff
      && pvz < dat->byte
      && pvz + dwg->header.section[SECTION_HEADER_R13].size < dat->size)
    {
      // not dwg->header_vars.size, rather -4 (minus the section_size).
      // section_size + data, i.e. minus the 2x sentinel (32) + crc itself (2)
      // if we would include the crc we would always get 0000
      BITCODE_RL crc_size = dwg->header.section[SECTION_HEADER_R13].size - 34;
      LOG_HANDLE (" calc Header crc size: " FORMAT_RL "\n", crc_size);
      crc2 = bit_calc_CRC (0xC0C1, &dat->chain[pvz], crc_size);
    }
  if (crc != crc2)
    {
      LOG_WARN ("Header Section[%ld] CRC mismatch %04X <=> %04X",
                (long)dwg->header.section[SECTION_HEADER_R13].number, crc,
                crc2);
      error |= DWG_ERR_WRONGCRC;
    }

  /*-------------------------------------------------------------------------
   * Classes, section 1
   */
classes_section:
  LOG_INFO ("\n"
            "=======> CLASS (start): %4lu\n",
            (long)dwg->header.section[SECTION_CLASSES_R13].address)
  LOG_INFO ("         CLASS (end)  : %4lu\n",
            (long)(dwg->header.section[SECTION_CLASSES_R13].address
                   + dwg->header.section[SECTION_CLASSES_R13].size))
  dat->byte = dwg->header.section[SECTION_CLASSES_R13].address + 16;
  dat->bit = 0;

  size = bit_read_RL (dat);
  lasta = dat->byte + size;
  LOG_TRACE ("         Length: %lu [RL]\n", size);

  /* Read the classes
   */
  dwg->layout_type = 0;
  dwg->num_classes = 0;
  do
    {
      BITCODE_BS i;
      Dwg_Class *klass;

      i = dwg->num_classes;
      if (i == 0)
        dwg->dwg_class = malloc (sizeof (Dwg_Class));
      else
        dwg->dwg_class
            = realloc (dwg->dwg_class, (i + 1) * sizeof (Dwg_Class));
      if (!dwg->dwg_class)
        {
          LOG_ERROR ("Out of memory");
          return DWG_ERR_OUTOFMEM;
        }
      klass = &dwg->dwg_class[i];
      memset (klass, 0, sizeof (Dwg_Class));
      klass->number = bit_read_BS (dat);
      klass->proxyflag = bit_read_BS (dat);
      klass->appname = bit_read_TV (dat);
      klass->cppname = bit_read_TV (dat);
      klass->dxfname = bit_read_TV (dat);
      klass->is_zombie = bit_read_B (dat);
      // 1f2 for entities, 1f3 for objects
      klass->item_class_id = bit_read_BS (dat);
      LOG_TRACE ("Class %d 0x%x %s\n"
                 " %s \"%s\" %d 0x%x\n",
                 klass->number, klass->proxyflag, klass->dxfname,
                 klass->cppname, klass->appname, klass->is_zombie,
                 klass->item_class_id)

#if 0
      SINCE (R_2007) //? dead code it seems. see read_2004_section_classes()
      {
        klass->num_instances = bit_read_BL (dat);
        klass->dwg_version = bit_read_BL (dat);
        klass->maint_version = bit_read_BL (dat);
        klass->unknown_1 = bit_read_BL (dat);
        klass->unknown_2 = bit_read_BL (dat);
        LOG_TRACE (
            " num_instances: %d, dwg/maint version: %d/%d, unk: %d/%d\n",
            klass->num_instances, klass->dwg_version, klass->maint_version,
            klass->unknown_1, klass->unknown_2);
      }
#endif

      if (klass->dxfname && strEQc ((const char *)klass->dxfname, "LAYOUT"))
        dwg->layout_type = klass->number;

      dwg->num_classes++;
      if (dwg->num_classes > 500)
        {
          LOG_ERROR ("number of classes is greater than 500");
          break;
        }
    }
  while (dat->byte < (lasta - 1));

  // Check Section CRC
  dat->byte = dwg->header.section[SECTION_CLASSES_R13].address
              + dwg->header.section[SECTION_CLASSES_R13].size - 18;
  dat->bit = 0;
  pvz = dwg->header.section[SECTION_CLASSES_R13].address + 16;
  if (!bit_check_CRC (dat, pvz, 0xC0C1))
    error |= DWG_ERR_WRONGCRC;

  dat->byte += 16;         // sentinel
  pvz = bit_read_RL (dat); // Unknown bitlong inter class and object
  LOG_TRACE ("unknown: 0x%04lx [RL] @%lu\n", pvz, dat->byte - 4)
  LOG_INFO ("Number of classes read: %u\n", dwg->num_classes)

  /*-------------------------------------------------------------------------
   * Object-map, section 2
   */

  dat->byte = dwg->header.section[SECTION_HANDLES_R13].address;
  dat->bit = 0;

  lastmap = dat->byte + dwg->header.section[SECTION_HANDLES_R13].size; // 4
  dwg->num_objects = 0;
  object_begin = dat->size;
  object_end = 0;
  LOG_INFO ("\n"
            "=======> Handles (start) : %8u\n",
            (unsigned int)dwg->header.section[SECTION_HANDLES_R13].address)
  LOG_INFO ("         Handles (end)   : %8u\n",
            (unsigned int)(dwg->header.section[SECTION_HANDLES_R13].address
                           + dwg->header.section[SECTION_HANDLES_R13].size))
  LOG_INFO ("         Length: %u\n",
            (unsigned int)dwg->header.section[SECTION_HANDLES_R13].size)

  do
    {
      long unsigned int last_offset = 0;
      long unsigned int last_handle = 0;
      long unsigned int oldpos = 0;
      long unsigned int maxh
          = (unsigned long)dwg->header.section[SECTION_HANDLES_R13].size << 1;
      BITCODE_BL max_handles
          = maxh < INT32_MAX ? (BITCODE_BL)maxh
                             : dwg->header.section[SECTION_HANDLES_R13].size;
      int added;

      startpos = dat->byte;
      section_size = bit_read_RS_LE (dat);
      LOG_TRACE ("Handles page size: %u [RS_LE] @%lu\n", section_size,
                 startpos);
      if (section_size > 2040)
        {
          LOG_ERROR ("Object-map section size greater than 2040!")
          return DWG_ERR_VALUEOUTOFBOUNDS;
        }

      while (dat->byte - startpos < section_size)
        {
          BITCODE_UMC handleoff;
          BITCODE_MC offset;
          // BITCODE_BL last_handle = dwg->num_objects
          //   ? dwg->object[dwg->num_objects - 1].handle.value : 0;

          oldpos = dat->byte;
          // the offset from the previous handle. default: 1, unsigned
          handleoff = bit_read_UMC (dat);
          // the offset from the previous address. default: obj->size, signed
          offset = bit_read_MC (dat);
          if (!handleoff || handleoff > max_handles - last_handle)
            {
              BITCODE_MC prevsize
                  = dwg->num_objects
                        ? dwg->object[dwg->num_objects - 1].size + 4
                        : 0L;
              LOG_WARN ("Ignore invalid handleoff (@%lu)", oldpos);
              if (offset == 1
                  || (offset > 0 && offset < prevsize && prevsize > 0)
                  || (offset < 0 && labs ((long)offset) < prevsize
                      && prevsize > 0))
                {
                  if (offset != prevsize)
                    LOG_ERROR ("Invalid offset: %ld [MC]", offset);
                  offset = prevsize;
                  LOG_WARN ("Recover invalid offset to %ld", offset);
                }
            }
          last_offset += offset;
          LOG_TRACE ("\nNext object: %lu ", (unsigned long)dwg->num_objects)
          LOG_TRACE ("Handleoff: %lX [UMC] "
                     "Offset: " FORMAT_MC " [MC] @%lu\n",
                     handleoff, offset, last_offset)

          if (dat->byte == oldpos)
            break;

          if (object_end < last_offset)
            object_end = last_offset;
          if (object_begin > last_offset)
            object_begin = last_offset;

          added = dwg_decode_add_object (dwg, dat, dat, last_offset);
          if (added > 0)
            error |= added; // else not added (skipped) or -1 for re-allocated
          last_handle = dwg->object[dwg->num_objects - 1].handle.value;
          // LOG_HANDLE ("dat: @%lu.%u\n", dat->byte, dat->bit);
        }
      if (dat->byte == oldpos)
        break;

      // CRC on
      if (dat->bit > 0)
        {
          dat->byte += 1;
          dat->bit = 0;
        }

      crc = bit_read_RS_LE (dat);
      LOG_TRACE ("\nHandles page crc: %04X [RS_LE] (%lu-%lu = %u)\n", crc,
                 startpos, startpos + section_size, section_size);
      crc2 = bit_calc_CRC (0xC0C1, dat->chain + startpos, section_size);
      if (crc != crc2)
        {
          LOG_ERROR ("Handles Section[%ld] page CRC mismatch %04X <=> %04X",
                     (long)dwg->header.section[SECTION_HANDLES_R13].number,
                     crc, crc2);
          // fails with r14
          // if (dwg->header.version == R_2000)
          //  return DWG_ERR_WRONGCRC;
          if (dat->version != R_14)
            error |= DWG_ERR_WRONGCRC;
        }
      if (dat->byte >= lastmap)
        break;
    }
  while (section_size > 2);

  LOG_INFO ("Num objects: %lu\n", (unsigned long)dwg->num_objects)
  LOG_INFO ("\n"
            "=======> Last Object      : %8lu\n",
            (unsigned long)object_begin)
  dat->byte = object_end;
  object_begin = bit_read_MS (dat);
  LOG_TRACE ("last object size: %lu [MS] (@%lu)\n", object_begin, object_end)
  LOG_INFO ("         Last Object (end): %8lu\n",
            (unsigned long)(object_end + object_begin + 2))

  /*
   // TODO: if the previous Handleoff got corrupted somehow, read this handle
   map
   // and try again.

   dat->byte = dwg->header.section[SECTION_HANDLES_R13].address - 2;
   // Unknown bitdouble inter object data and object map
   antcrc = bit_read_CRC (dat);
   LOG_TRACE("Address: %08u / Content: 0x%04X", dat->byte - 2, antcrc)

   // check CRC-on
   antcrc = 0xC0C1;
   do
   {
     startpos = dat->byte;
     sgdc[0] = bit_read_RC (dat);
     sgdc[1] = bit_read_RC (dat);
     section_size = (sgdc[0] << 8) | sgdc[1];
     section_size -= 2;
     dat->byte += section_size;
     crc = bit_read_CRC (dat);
     dat->byte -= 2;
     bit_write_CRC (dat, startpos, antcrc);
     dat->byte -= 2;
     crc2 = bit_read_CRC (dat);
     if (loglevel) HANDLER (OUTPUT, "Read: %X\nCreated: %X\t SEMO: %X\n",
                            crc, crc2, antcrc);
     //antcrc = crc;
   } while (section_size > 0);
   */

  /*-------------------------------------------------------------------------
   * Second header, section 3. R13-R2000 only.
   * But partially also since r2004.
   */

  if (bit_search_sentinel (dat,
                           dwg_sentinel (DWG_SENTINEL_SECOND_HEADER_BEGIN)))
    {
      BITCODE_RL i;
      BITCODE_RC sig, sig2;
      BITCODE_BL vcount;
      long unsigned int pvzadr;
      struct _dwg_second_header *_obj = &dwg->second_header;
      obj = NULL;

      LOG_INFO ("\n"
                "=======> Second Header 3 (start): %8u\n",
                (unsigned int)dat->byte - 16)
      pvzadr = dat->byte;
      LOG_TRACE ("pvzadr: %lx\n", pvzadr)

      FIELD_RL (size, 0);
      FIELD_BLx (address, 0);

      if (dwg->header.num_sections <= SECTION_2NDHEADER_R13)
        {
          LOG_WARN (
              "Only %d num_sections, but 2ndheader found, extending to 4",
              dwg->header.num_sections);
          dwg->header.num_sections = SECTION_2NDHEADER_R13 + 1; /* 4 */
          dwg->header.section
              = realloc (dwg->header.section,
                         dwg->header.num_sections * sizeof (Dwg_Section));
          memset (&dwg->header.section[SECTION_2NDHEADER_R13], 0,
                  sizeof (Dwg_Section));
        }
      if (!dwg->header.section[SECTION_2NDHEADER_R13].address)
        {
          dwg->header.section[SECTION_2NDHEADER_R13].address
              = dwg->second_header.address;
          dwg->header.section[SECTION_2NDHEADER_R13].size
              = dwg->second_header.size;
        }

      // AC1012, AC1014 or AC1015. This is a char[11], zero padded.
      // with \n at 12.
      bit_read_fixed (dat, _obj->version, 12);
      LOG_TRACE ("version: %s [TFF 12]\n", _obj->version)
      for (i = 0; i < 4; i++)
        FIELD_B (null_b[i], 0);
      // DEBUG_HERE;
      // documented as 0x18,0x78,0x01,0x04 for R13, 0x18,0x78,0x01,0x05 for R14
      // r14:      7d f4 78 01
      // r2000:    14 64 78 01
      FIELD_RC (unknown_10, 0); // 0x10
      for (i = 0; i < 4; i++)
        FIELD_RC (unknown_rc4[i], 0);

      if (dat->version < R_2000 && FIELD_VALUE (unknown_10) == 0x18
          && FIELD_VALUE (unknown_rc4[0]) == 0x78)
        dat->byte -= 2;
      UNTIL (R_2000)
      {
        FIELD_RC (num_sections, 0); // r14: 5, r2000: 6
        for (i = 0; i < MIN (6, FIELD_VALUE (num_sections)); i++)
          {
            // address+sizes of sections 0-2 is correct, 3+4 is empty
            FIELD_RC (section[i].nr, 0);
            FIELD_BLx (section[i].address, 0);
            FIELD_BL (section[i].size, 0);
          }
        if (DWG_LOGLEVEL >= DWG_LOGLEVEL_HANDLE)
          {
            LOG_HANDLE ("1st header was:\n");
            for (i = 0; i < dwg->header.num_sections; i++)
              {
                LOG_HANDLE ("section[" FORMAT_RL "] " FORMAT_RLd " " FORMAT_RLL
                            " " FORMAT_RL " \n",
                            i, dwg->header.section[i].number,
                            dwg->header.section[i].address,
                            dwg->header.section[i].size);
              }
            LOG_HANDLE ("start 3: %lu\n", pvzadr - 16);
          }

        FIELD_BS (num_handlers, 0); // 14, resp. 16 in r14
        if (FIELD_VALUE (num_handlers) > 16)
          {
            LOG_ERROR ("Second header num_handlers > 16: %d\n",
                       FIELD_VALUE (num_handlers));
            FIELD_VALUE (num_handlers) = 14;
          }
        for (i = 0; i < FIELD_VALUE (num_handlers); i++)
          {
            FIELD_RC (handlers[i].size, 0);
            FIELD_RC (handlers[i].nr, 0);
            FIELD_VECTOR (handlers[i].data, RC, handlers[i].size, 0);
          }

        // TODO: CRC check
        crc = bit_read_CRC (dat);
        LOG_TRACE ("crc: %04X\n", crc);

        VERSION (R_14)
        {
          FIELD_RL (junk_r14_1, 0);
          FIELD_RL (junk_r14_2, 0);
        }
      }

      if (bit_search_sentinel (dat,
                               dwg_sentinel (DWG_SENTINEL_SECOND_HEADER_END)))
        LOG_INFO ("         Second Header 3 (end)  : %8u\n",
                  (unsigned int)dat->byte)
    }

  /*-------------------------------------------------------------------------
   * Section 4: MEASUREMENT
   */

  if (dwg->header.num_sections > 4)
    {
      LOG_INFO ("\n"
                "=======> MEASUREMENT 4 (start)  : %8u\n",
                (unsigned int)dwg->header.section[4].address)
      LOG_INFO ("         MEASUREMENT 4 (end)    : %8u\n",
                (unsigned int)(dwg->header.section[4].address
                               + dwg->header.section[4].size))
      dat->byte = dwg->header.section[4].address;
      dat->bit = 0;
      dwg->header_vars.MEASUREMENT = (BITCODE_BS)bit_read_RL (dat);
      LOG_TRACE ("MEASUREMENT: " FORMAT_BS " [RL] (0 English/1 Metric)\n",
                 dwg->header_vars.MEASUREMENT)

      // LOG_TRACE ("         Size bytes :\t%lu\n", dat->size)
    }

  // step II of handles parsing: resolve pointers from handle value
  // XXX: move this somewhere else
  LOG_INFO ("\nnum_objects: %lu\n", (unsigned long)dwg->num_objects)
  LOG_TRACE ("num_object_refs: %lu\n", (unsigned long)dwg->num_object_refs)
  LOG_TRACE ("Resolving pointers from ObjectRef vector:\n")
  error |= resolve_objectref_vector (dat, dwg);
  return error;
}

int
dwg_decode (Bit_Chain *restrict dat, Dwg_Data *restrict dwg)
{
  int i;
  char version[7];

  dwg->num_object_refs = 0;
  // dwg->num_layers = 0; // see now dwg->layer_control->num_entries
  dwg->num_entities = 0;
  dwg->num_objects = 0;
  dwg->num_classes = 0;
  dwg->thumbnail.size = 0;
  dwg->thumbnail.chain = NULL;
  dwg->header.num_sections = 0;
  dwg->header.section_infohdr.num_desc = 0;
  dwg->dwg_class = NULL;
  dwg->object_ref = NULL;
  dwg->object = NULL;
  dwg->object_map = hash_new (dat->size / 1000);
  if (!dwg->object_map)
    {
      // whatever, we are obviously on a tiny system
      dwg->object_map = hash_new (1024);
      if (!dwg->object_map)
        {
          LOG_ERROR ("Out of memory");
          return DWG_ERR_OUTOFMEM;
        }
    }
  dwg->dirty_refs = 1;

  // memset (&dwg->header, 0, sizeof (dwg->header)); // nope. needed for
  // version
  memset (&dwg->header_vars, 0, sizeof (dwg->header_vars));
  memset (&dwg->summaryinfo, 0, sizeof (dwg->summaryinfo));
  memset (&dwg->r2004_header.file_ID_string[0], 0, sizeof (dwg->r2004_header));
  memset (&dwg->auxheader.aux_intro[0], 0, sizeof (dwg->auxheader));
  memset (&dwg->second_header, 0, sizeof (dwg->second_header));

  if (dwg->opts)
    {
      loglevel = dwg->opts & DWG_OPTS_LOGLEVEL;
      dat->opts = dwg->opts;
    }

#ifdef USE_TRACING
  /* Before starting, set the logging level, but only do so once.  */
  if (!env_var_checked_p)
    {
      char *probe = getenv ("LIBREDWG_TRACE");
      if (probe)
        loglevel = atoi (probe);
      env_var_checked_p = true;
    }
#endif /* USE_TRACING */

  /* Version */
  dat->byte = 0;
  dat->bit = 0;
  if (!dat->chain || dat->size < 58)
    {
      LOG_ERROR ("dwg too small: %lu bytes", dat->size);
      return DWG_ERR_INVALIDDWG;
    }
  strncpy (version, (const char *)dat->chain, 6);
  version[6] = '\0';

  dwg->header.version = 0;
  for (i = 0; i < R_AFTER; i++)
    {
      if (strEQ (version, version_codes[(Dwg_Version_Type)i]))
        {
          dwg->header.version = (Dwg_Version_Type)i;
          break;
        }
    }
  if (!dwg->header.version)
    {
      if (strncmp (version, "AC", 2)) // let's ignore MC0.0 for now
        {
          LOG_ERROR ("Invalid DWG, magic: %s", version);
        }
      else
        {
          LOG_ERROR ("Invalid or unimplemented DWG version code %s", version);
        }
      return DWG_ERR_INVALIDDWG;
    }
  dat->version = dwg->header.version;
  dwg->header.from_version = dat->version;
  dat->from_version = dat->version;
  LOG_INFO ("This file's version code is: %s (%s)\n", version,
            dwg_version_type (dat->version))

#define WE_CAN                                                                \
  "This version of LibreDWG is only capable of decoding "                     \
  "version R13-R2018 (code: AC1012-AC1032) DWG files.\n"

  VERSIONS (R_13, R_2000) { return decode_R13_R2000 (dat, dwg); }

  // This line should not be reached
  LOG_ERROR ("LibreDWG does not support this version: %s.", version)
  return DWG_ERR_INVALIDDWG;
}

EXPORT int
dwg_read_file (const char *restrict filename, Dwg_Data *restrict dwg)
{
  FILE *fp;
  struct stat attrib;
  size_t size;
  Bit_Chain bit_chain = { 0 };
  int error;

  loglevel = dwg->opts & DWG_OPTS_LOGLEVEL;
  memset (dwg, 0, sizeof (Dwg_Data));
  dwg->opts = loglevel;

  if (strEQc (filename, "-"))
    {
      fp = stdin;
    }
  else
    {
      if (stat (filename, &attrib))
        {
          LOG_ERROR ("File not found: %s\n", filename);
          return DWG_ERR_IOERROR;
        }
      if (!(S_ISREG (attrib.st_mode)
#ifndef _WIN32
            || S_ISLNK (attrib.st_mode)
#endif
                ))
        {
          LOG_ERROR ("Error: %s\n", filename);
          return DWG_ERR_IOERROR;
        }
      fp = fopen (filename, "rb");
    }
  if (!fp)
    {
      LOG_ERROR ("Could not open file: %s\n", filename)
      return DWG_ERR_IOERROR;
    }

  /* Load whole file into memory, even if streamed (for now)
   */
  memset (&bit_chain, 0, sizeof (Bit_Chain));
  if (fp == stdin)
    {
      error = dat_read_stream (&bit_chain, fp);
      if (error >= DWG_ERR_CRITICAL)
        return error;
    }
  else
    {
      bit_chain.size = attrib.st_size;
      error = dat_read_file (&bit_chain, fp, filename);
      if (error >= DWG_ERR_CRITICAL)
        return error;
    }
  fclose (fp);

  /* Decode the dwg structure */
  error = dwg_decode (&bit_chain, dwg);
  if (error >= DWG_ERR_CRITICAL)
    {
      LOG_ERROR ("Failed to decode file: %s 0x%x\n", filename, error)
      free (bit_chain.chain);
      bit_chain.chain = NULL;
      bit_chain.size = 0;
      return error;
    }

  // TODO: does dwg hold any char* pointers to the bit_chain or are they all
  // copied?
  free (bit_chain.chain);
  bit_chain.chain = NULL;
  bit_chain.size = 0;

  return error;
}

int
main (int argc, char *argv[])
{
  int error;
  int i = 1;
  int c;
#ifdef HAVE_GETOPT_LONG
  int option_index = 0;
  static struct option long_options[]
      = { { "verbose", 1, &opts, 1 }, // optional
          { "help", 0, 0, 0 },
          { "version", 0, 0, 0 },
          { NULL, 0, NULL, 0 } };
#endif

  if (argc < 2)
    return usage ();

  while
#ifdef HAVE_GETOPT_LONG
      ((c = getopt_long (argc, argv, ":v::h", long_options, &option_index))
       != -1)
#else
      ((c = getopt (argc, argv, ":v::hi")) != -1)
#endif
    {
      if (c == -1)
        break;
      switch (c)
        {
        case ':': // missing arg
          if (optarg && !strcmp (optarg, "v"))
            {
              opts = 1;
              break;
            }
          fprintf (stderr, "%s: option '-%c' requires an argument\n", argv[0],
                   optopt);
          break;
#ifdef HAVE_GETOPT_LONG
        case 0:
          /* This option sets a flag */
          if (!strcmp (long_options[option_index].name, "verbose"))
            {
              if (opts < 0 || opts > 9)
                return usage ();
#  if defined(USE_TRACING) && defined(HAVE_SETENV)
              {
                char v[2];
                *v = opts + '0';
                *(v + 1) = 0;
                setenv ("LIBREDWG_TRACE", v, 1);
              }
#  endif
              break;
            }
          if (!strcmp (long_options[option_index].name, "version"))
            return opt_version ();
          if (!strcmp (long_options[option_index].name, "help"))
            return help ();
          break;
#else
        case 'i':
          return opt_version ();
#endif
        case 'v': // support -v3 and -v
          i = (optind > 0 && optind < argc) ? optind - 1 : 1;
          if (!memcmp (argv[i], "-v", 2))
            {
              opts = argv[i][2] ? argv[i][2] - '0' : 1;
            }
          if (opts < 0 || opts > 9)
            return usage ();
#if defined(USE_TRACING) && defined(HAVE_SETENV)
          {
            char v[2];
            *v = opts + '0';
            *(v + 1) = 0;
            setenv ("LIBREDWG_TRACE", v, 1);
          }
#endif
          break;
        case 'h':
          return help ();
        case '?':
          fprintf (stderr, "%s: invalid option '-%c' ignored\n", argv[0],
                   optopt);
          break;
        default:
          return usage ();
        }
    }
  i = optind;
  if (i >= argc)
    return usage ();

  memset (&g_dwg, 0, sizeof (Dwg_Data));
  g_dwg.opts = opts;
  error = dwg_read_file (argv[i], &g_dwg);
  if (error < DWG_ERR_CRITICAL)
    output_SVG (&g_dwg);

  dwg_free (&g_dwg);
  return error >= DWG_ERR_CRITICAL ? 1 : 0;
}
