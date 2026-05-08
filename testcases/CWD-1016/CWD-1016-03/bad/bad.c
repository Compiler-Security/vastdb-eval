#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FFMAX(a,b) ((a) > (b) ? (a) : (b))
#define FFMIN(a,b) ((a) > (b) ? (b) : (a))
#define FFDIFFSIGN(x,y) (((x)>(y)) - ((x)<(y)))

int av_builtin_constant_p(unsigned v);

#define AV_CEIL_RSHIFT(a,b) (!av_builtin_constant_p(b) ? -((-(a)) >> (b)) \
                                                       : ((a) + (1<<(b)) - 1) >> (b))

#define FF_CEIL_RSHIFT AV_CEIL_RSHIFT

#define SQR(x) ((x) * (x))

#define AVERROR_BUG     (-523873695)
#define AVERROR(e)      (-(e))
#define ENOMEM          12
#define EINVAL          22

#define AV_HAVE_BIGENDIAN 0

typedef struct AVRational {
    int num;
    int den;
} AVRational;

typedef float FFTSample;

typedef struct FFTComplex {
    FFTSample re, im;
} FFTComplex;

typedef struct FFTContext FFTContext;
typedef struct DCTContext DCTContext;

enum DCTTransformType {
    DCT_II = 0,
    DCT_III,
    DCT_I,
    DST_I,
};

DCTContext *av_dct_init(int nbits, enum DCTTransformType type);
void av_dct_calc(DCTContext *s, FFTSample *data);
void av_dct_end(DCTContext *s);

enum AVPixelFormat {
    AV_PIX_FMT_NONE = -1,
    AV_PIX_FMT_GRAY8,
    AV_PIX_FMT_GRAY9,
    AV_PIX_FMT_GRAY10,
    AV_PIX_FMT_GRAY12,
    AV_PIX_FMT_GRAY16,
    AV_PIX_FMT_YUV410P,
    AV_PIX_FMT_YUV411P,
    AV_PIX_FMT_YUV420P,
    AV_PIX_FMT_YUV422P,
    AV_PIX_FMT_YUV440P,
    AV_PIX_FMT_YUV444P,
    AV_PIX_FMT_YUVJ420P,
    AV_PIX_FMT_YUVJ422P,
    AV_PIX_FMT_YUVJ440P,
    AV_PIX_FMT_YUVJ444P,
    AV_PIX_FMT_YUVJ411P,
    AV_PIX_FMT_YUV420P9,
    AV_PIX_FMT_YUV422P9,
    AV_PIX_FMT_YUV444P9,
    AV_PIX_FMT_YUV420P10,
    AV_PIX_FMT_YUV422P10,
    AV_PIX_FMT_YUV444P10,
    AV_PIX_FMT_YUV440P10,
    AV_PIX_FMT_YUV444P12,
    AV_PIX_FMT_YUV422P12,
    AV_PIX_FMT_YUV420P12,
    AV_PIX_FMT_YUV440P12,
    AV_PIX_FMT_YUV444P14,
    AV_PIX_FMT_YUV422P14,
    AV_PIX_FMT_YUV420P14,
    AV_PIX_FMT_YUV420P16,
    AV_PIX_FMT_YUV422P16,
    AV_PIX_FMT_YUV444P16,
    AV_PIX_FMT_GBRP,
    AV_PIX_FMT_GBRP9,
    AV_PIX_FMT_GBRP10,
    AV_PIX_FMT_GBRP12,
    AV_PIX_FMT_GBRP14,
    AV_PIX_FMT_GBRP16,
    AV_PIX_FMT_NB
};

enum AVPictureType {
    AV_PICTURE_TYPE_NONE = 0,
    AV_PICTURE_TYPE_I,
    AV_PICTURE_TYPE_P,
    AV_PICTURE_TYPE_B,
    AV_PICTURE_TYPE_S,
    AV_PICTURE_TYPE_SI,
    AV_PICTURE_TYPE_SP,
    AV_PICTURE_TYPE_BI,
};

enum AVColorPrimaries {
    AVCOL_PRI_RESERVED0   = 0,
    AVCOL_PRI_BT709       = 1,
    AVCOL_PRI_UNSPECIFIED = 2,
    AVCOL_PRI_RESERVED    = 3,
    AVCOL_PRI_BT470M      = 4,
    AVCOL_PRI_BT470BG     = 5,
    AVCOL_PRI_SMPTE170M   = 6,
    AVCOL_PRI_SMPTE240M   = 7,
    AVCOL_PRI_FILM        = 8,
    AVCOL_PRI_BT2020      = 9,
    AVCOL_PRI_NB,
};

enum AVColorTransferCharacteristic {
    AVCOL_TRC_RESERVED0    = 0,
    AVCOL_TRC_BT709        = 1,
    AVCOL_TRC_UNSPECIFIED  = 2,
    AVCOL_TRC_RESERVED     = 3,
    AVCOL_TRC_GAMMA22      = 4,
    AVCOL_TRC_GAMMA28      = 5,
    AVCOL_TRC_SMPTE170M    = 6,
    AVCOL_TRC_SMPTE240M    = 7,
    AVCOL_TRC_LINEAR       = 8,
    AVCOL_TRC_LOG          = 9,
    AVCOL_TRC_LOG_SQRT     = 10,
    AVCOL_TRC_IEC61966_2_4 = 11,
    AVCOL_TRC_NB,
};

enum AVColorSpace {
    AVCOL_SPC_RGB         = 0,
    AVCOL_SPC_BT709       = 1,
    AVCOL_SPC_UNSPECIFIED = 2,
    AVCOL_SPC_RESERVED    = 3,
    AVCOL_SPC_FCC         = 4,
    AVCOL_SPC_BT470BG     = 5,
    AVCOL_SPC_SMPTE170M   = 6,
    AVCOL_SPC_SMPTE240M   = 7,
    AVCOL_SPC_YCGCO       = 8,
    AVCOL_SPC_YCOCG       = 9,
    AVCOL_SPC_BT2020_NCL  = 10,
    AVCOL_SPC_BT2020_CL   = 11,
    AVCOL_SPC_NB,
};

enum AVChromaLocation {
    AVCHROMA_LOC_UNSPECIFIED = 0,
    AVCHROMA_LOC_LEFT,
    AVCHROMA_LOC_CENTER,
    AVCHROMA_LOC_TOPLEFT,
    AVCHROMA_LOC_TOP,
    AVCHROMA_LOC_BOTTOMLEFT,
    AVCHROMA_LOC_BOTTOM,
    AVCHROMA_LOC_NB,
};

typedef struct AVComponentDescriptor {
    int plane;
    int step;
    int offset;
    int shift;
    int depth;
} AVComponentDescriptor;

typedef struct AVPixFmtDescriptor {
    const char *name;
    uint8_t nb_components;
    uint8_t log2_chroma_w;
    uint8_t log2_chroma_h;
    uint64_t flags;
    AVComponentDescriptor comp[4];
    const char *alias;
} AVPixFmtDescriptor;

const AVPixFmtDescriptor *av_pix_fmt_desc_get(enum AVPixelFormat pix_fmt);
int av_pix_fmt_count_planes(enum AVPixelFormat pix_fmt);
int av_image_copy_plane(uint8_t *dst, int dst_linesize,
                        const uint8_t *src, int src_linesize,
                        int width, int height);

typedef struct AVBufferRef AVBufferRef;
typedef struct AVFrameSideData AVFrameSideData;

typedef struct AVFrame {
    uint8_t *data[8];
    int linesize[8];
    uint8_t **extended_data;
    int width, height;
    int nb_samples;
    int format;
    int64_t pts;
    int64_t pkt_dts;
    int64_t pkt_pos;
    int64_t pkt_duration;
    int64_t best_effort_timestamp;
    int key_frame;
    enum AVPictureType pict_type;
    AVRational sample_aspect_ratio;
    int64_t coded_picture_number;
    int64_t display_picture_number;
    int quality;
    int interlaced_frame;
    int top_field_first;
    char **metadata;
    int repeat_pict;
    int64_t reordered_opaque;
    float pkt_pos_float;
    void *opaque;
    AVBufferRef *hw_frames_ctx;
    AVBufferRef **extended_buf;
    int nb_extended_buf;
    AVFrameSideData **side_data;
    int nb_side_data;
    int flags;
    int color_range;
    enum AVColorPrimaries color_primaries;
    enum AVColorTransferCharacteristic color_trc;
    enum AVColorSpace colorspace;
    int chroma_location;
    int64_t time_base;
} AVFrame;

int av_frame_copy_props(AVFrame *dst, const AVFrame *src);

enum AVMediaType {
    AVMEDIA_TYPE_UNKNOWN = -1,
    AVMEDIA_TYPE_VIDEO,
    AVMEDIA_TYPE_AUDIO,
    AVMEDIA_TYPE_DATA,
    AVMEDIA_TYPE_SUBTITLE,
    AVMEDIA_TYPE_ATTACHMENT,
    AVMEDIA_TYPE_NB
};

typedef struct AVFilterPad {
    const char *name;
    enum AVMediaType type;
    int (*config_props)(struct AVFilterLink *link);
} AVFilterPad;

typedef struct AVFilterContext AVFilterContext;
typedef struct AVFilterLink AVFilterLink;
typedef struct AVFilter AVFilter;
typedef struct AVFilterInternal AVFilterInternal;
typedef struct AVClass AVClass;

struct AVFilterContext {
    const AVClass *av_class;
    const AVFilter *filter;
    void *priv;
    AVFilterLink **inputs;
    unsigned nb_inputs;
    AVFilterLink **outputs;
    unsigned nb_outputs;
    AVFilterInternal *internal;
};

struct AVFilterLink {
    AVFilterContext *src;
    AVFilterContext *dst;
    AVFilterPad *srcpad;
    AVFilterPad *dstpad;
    enum AVMediaType type;
    int w;
    int h;
    AVRational sample_aspect_ratio;
    uint64_t channel_layout;
    int sample_rate;
    int format;
    AVRational time_base;
    int frame_rate;
    AVBufferRef *bufs;
    AVFrame *cur_buf;
    unsigned cur_buf_offset;
    int frames;
    int frame_count;
    int closed;
    int init_done;
    int eof;
    int channels;
    int partial_frame;
    int request_count;
    int request_sample_count;
    int min_samples;
    int max_samples;
    int curve;
    int disabled;
    int needs_reinit;
    int frame_blocked;
    int fifo_depth;
    int buffer_pool_size;
    int ready;
};

struct AVFilter {
    const char *name;
    const char *description;
    const void *options;
    const AVFilterPad *inputs;
    const AVFilterPad *outputs;
    const AVClass *priv_class;
    int flags;
    size_t priv_size;
    int (*init)(AVFilterContext *ctx);
    void (*uninit)(AVFilterContext *ctx);
    int (*query_formats)(AVFilterContext *ctx);
};

#define AVFILTER_FLAG_SUPPORT_TIMELINE_INTERNAL (1 << 0)
#define AVFILTER_FLAG_DYNAMIC_INPUTS            (1 << 1)
#define AVFILTER_FLAG_SLICE_THREADS             (1 << 2)

AVFrame *ff_get_video_buffer(AVFilterLink *link, int w, int h);
int ff_set_common_formats(AVFilterContext *ctx, void *fmts);
void *ff_make_format_list(const enum AVPixelFormat *pix_fmts);
int ff_filter_get_nb_threads(AVFilterContext *ctx);
int ff_insert_inpad(AVFilterContext *ctx, unsigned index, AVFilterPad *pad);
void ff_filter_frame(AVFilterLink *link, AVFrame *frame);

struct AVFilterInternal {
    int (*execute)(AVFilterContext *ctx, int (*func)(AVFilterContext *ctx, void *arg, int jobnr, int nb_jobs),
                   void *arg, void *ret, int nb_jobs);
};

#define EXT_STOP 0
#define EXT_INFINITY 1

typedef struct FFFrameSyncIn {
    int64_t pts;
    int64_t time_base;
    int sync;
    int before;
    int after;
} FFFrameSyncIn;

typedef struct FFFrameSync {
    AVFilterContext *parent;
    void *opaque;
    FFFrameSyncIn *in;
    int nb_in;
    AVRational time_base;
    int64_t pts;
    int (*on_event)(struct FFFrameSync *fs);
} FFFrameSync;

int av_log2(unsigned v);

#define av_assert0(cond) ((void)0)
#define av_assert1(cond) ((void)0)
#define av_assert2(cond) ((void)0)

void *av_calloc(size_t nmemb, size_t size);
void av_freep(void *p);

enum AVLogLevel {
    AV_LOG_QUIET = -8,
    AV_LOG_PANIC = 0,
    AV_LOG_FATAL = 8,
    AV_LOG_ERROR = 16,
    AV_LOG_WARNING = 24,
    AV_LOG_INFO = 32,
    AV_LOG_VERBOSE = 40,
    AV_LOG_DEBUG = 48,
};

void av_log(void *avcl, int level, const char *fmt, ...);

#define AV_LOG_WARNING 24
#define AV_LOG_ERROR   16

enum AVOptionType {
    AV_OPT_TYPE_FLAGS,
    AV_OPT_TYPE_INT,
    AV_OPT_TYPE_INT64,
    AV_OPT_TYPE_FLOAT,
    AV_OPT_TYPE_DOUBLE,
    AV_OPT_TYPE_RATIONAL,
    AV_OPT_TYPE_STRING,
    AV_OPT_TYPE_PIXEL_FMT,
    AV_OPT_TYPE_SAMPLE_FMT,
    AV_OPT_TYPE_VIDEO_RATE,
    AV_OPT_TYPE_DURATION,
    AV_OPT_TYPE_COLOR,
    AV_OPT_TYPE_CHANNEL_LAYOUT,
    AV_OPT_TYPE_BOOL,
};

#define AV_OPT_FLAG_FILTERING_PARAM (1 << 1)
#define AV_OPT_FLAG_VIDEO_PARAM     (1 << 7)

typedef struct AVOption {
    const char *name;
    const char *help;
    enum AVOptionType type;
    int offset;
    union {
        int64_t i64;
        double dbl;
    } default_val;
    double min;
    double max;
    int flags;
    const char *unit;
} AVOption;

#define AVFILTER_DEFINE_CLASS(filter) \
    static const AVClass filter##_class = { \
        .class_name = #filter, \
        .item_name  = NULL, \
        .option     = NULL, \
        .version    = 0, \
    }

#define MAX_NB_THREADS 32

enum FilterModes {
    BASIC,
    FINAL,
    NB_MODES,
};

typedef struct PosCode {
    int x, y;
} PosCode;

typedef struct PosPairCode {
    double score;
    int x, y;
} PosPairCode;

typedef struct SliceContext {
    DCTContext *gdctf, *gdcti;
    DCTContext *dctf, *dcti;
    FFTSample *bufferh;
    FFTSample *bufferv;
    FFTSample *bufferz;
    FFTSample *buffer;
    FFTSample *rbufferh;
    FFTSample *rbufferv;
    FFTSample *rbufferz;
    FFTSample *rbuffer;
    float *num, *den;
    PosPairCode match_blocks[256];
    int nb_match_blocks;
    PosCode *search_positions;
} SliceContext;

typedef struct BM3DContext {
    const AVClass *class;

    float sigma;
    int block_size;
    int block_step;
    int group_size;
    int bm_range;
    int bm_step;
    float th_mse;
    float hard_threshold;
    int mode;
    int ref;
    int planes;

    int depth;
    int max;
    int nb_planes;
    int planewidth[4];
    int planeheight[4];
    int group_bits;
    int pgroup_size;

    SliceContext slices[MAX_NB_THREADS];

    FFFrameSync fs;
    int nb_threads;

    void (*get_block_row)(const uint8_t *srcp, int src_linesize,
                          int y, int x, int block_size, float *dst);
    double (*do_block_ssd)(struct BM3DContext *s, PosCode *pos,
                           const uint8_t *src, int src_stride,
                           int r_y, int r_x);
    void (*do_output)(struct BM3DContext *s, uint8_t *dst, int dst_linesize,
                      int plane, int nb_jobs);
    void (*block_filtering)(struct BM3DContext *s,
                            const uint8_t *src, int src_linesize,
                            const uint8_t *ref, int ref_linesize,
                            int y, int x, int plane, int jobnr);
} BM3DContext;

typedef struct ThreadData {
    const uint8_t *src;
    int src_linesize;
    const uint8_t *ref;
    int ref_linesize;
    int plane;
} ThreadData;

static void block_matching(BM3DContext *s, const uint8_t *ref, int ref_linesize,
                           int j, int i, int plane, int jobnr);

static void do_output(BM3DContext *s, uint8_t *dst, int dst_linesize,
                      int plane, int nb_jobs);

static void do_output16(BM3DContext *s, uint8_t *dst, int dst_linesize,
                        int plane, int nb_jobs);

static double do_block_ssd(BM3DContext *s, PosCode *pos,
                           const uint8_t *src, int src_stride,
                           int r_y, int r_x);

static double do_block_ssd16(BM3DContext *s, PosCode *pos,
                             const uint8_t *src, int src_stride,
                             int r_y, int r_x);

static void get_block_row(const uint8_t *srcp, int src_linesize,
                          int y, int x, int block_size, float *dst)
{
    const uint8_t *src = srcp + y * src_linesize + x;
    int j;

    for (j = 0; j < block_size; j++) {
        dst[j] = src[j];
    }
}

static void get_block_row16(const uint8_t *srcp, int src_linesize,
                            int y, int x, int block_size, float *dst)
{
    const uint16_t *src = (uint16_t *)srcp + y * src_linesize / 2 + x;
    int j;

    for (j = 0; j < block_size; j++) {
        dst[j] = src[j];
    }
}

static void basic_block_filtering(BM3DContext *s, const uint8_t *src, int src_linesize,
                                  const uint8_t *ref, int ref_linesize,
                                  int y, int x, int plane, int jobnr)
{
    SliceContext *sc = &s->slices[jobnr];
    const int buffer_linesize = s->block_size * s->block_size;
    const int nb_match_blocks = sc->nb_match_blocks;
    const int block_size = s->block_size;
    const int width = s->planewidth[plane];
    const int pgroup_size = s->pgroup_size;
    const int group_size = s->group_size;
    float *buffer = sc->buffer;
    float *bufferh = sc->bufferh;
    float *bufferv = sc->bufferv;
    float *bufferz = sc->bufferz;
    float threshold[4];
    float den_weight, num_weight;
    int retained = 0;
    int i, j, k;

    for (k = 0; k < nb_match_blocks; k++) {
        const int y = sc->match_blocks[k].y;
        const int x = sc->match_blocks[k].x;

        for (i = 0; i < block_size; i++) {
            s->get_block_row(src, src_linesize, y + i, x, block_size, bufferh + block_size * i);
            av_dct_calc(sc->dctf, bufferh + block_size * i);
        }

        for (i = 0; i < block_size; i++) {
            for (j = 0; j < block_size; j++) {
                bufferv[i * block_size + j] = bufferh[j * block_size + i];
            }
            av_dct_calc(sc->dctf, bufferv + i * block_size);
        }

        for (i = 0; i < block_size; i++) {
            memcpy(buffer + k * buffer_linesize + i * block_size,
                   bufferv + i * block_size, block_size * 4);
        }
    }

    for (i = 0; i < block_size; i++) {
        for (j = 0; j < block_size; j++) {
            for (k = 0; k < nb_match_blocks; k++)
                bufferz[k] = buffer[buffer_linesize * k + i * block_size + j];
            if (group_size > 1)
                av_dct_calc(sc->gdctf, bufferz);
            bufferz += pgroup_size;
        }
    }

    threshold[0] = s->hard_threshold * s->sigma;
    threshold[1] = threshold[0] * sqrtf(2.f);
    threshold[2] = threshold[0] * 2.f;
    threshold[3] = threshold[0] * sqrtf(8.f);
    bufferz = sc->bufferz;

    for (i = 0; i < block_size; i++) {
        for (j = 0; j < block_size; j++) {
            for (k = 0; k < nb_match_blocks; k++) {
                const float thresh = threshold[(j == 0) + (i == 0) + (k == 0)];

                if (bufferz[k] > thresh || bufferz[k] < -thresh) {
                    retained++;
                } else {
                    bufferz[k] = 0;
                }
            }
            bufferz += pgroup_size;
        }
    }

    bufferz = sc->bufferz;
    buffer = sc->buffer;
    for (i = 0; i < block_size; i++) {
        for (j = 0; j < block_size; j++) {
            if (group_size > 1)
                av_dct_calc(sc->gdcti, bufferz);
            for (k = 0; k < nb_match_blocks; k++) {
                buffer[buffer_linesize * k + i * block_size + j] = bufferz[k];
            }
            bufferz += pgroup_size;
        }
    }

    den_weight = retained < 1 ? 1.f : 1.f / retained;
    num_weight = den_weight;

    buffer = sc->buffer;
    for (k = 0; k < nb_match_blocks; k++) {
        float *num = sc->num + y * width + x;
        float *den = sc->den + y * width + x;

        for (i = 0; i < block_size; i++) {
            memcpy(bufferv + i * block_size,
                   buffer + k * buffer_linesize + i * block_size,
                   block_size * 4);
        }

        for (i = 0; i < block_size; i++) {
            av_dct_calc(sc->dcti, bufferv + block_size * i);
            for (j = 0; j < block_size; j++) {
                bufferh[j * block_size + i] = bufferv[i * block_size + j];
            }
        }

        for (i = 0; i < block_size; i++) {
            av_dct_calc(sc->dcti, bufferh + block_size * i);
            for (j = 0; j < block_size; j++) {
                num[j] += bufferh[i * block_size + j] * num_weight;
                den[j] += den_weight;
            }
            num += width;
            den += width;
        }
    }
}

static void final_block_filtering(BM3DContext *s, const uint8_t *src, int src_linesize,
                                  const uint8_t *ref, int ref_linesize,
                                  int y, int x, int plane, int jobnr)
{
    SliceContext *sc = &s->slices[jobnr];
    const int buffer_linesize = s->block_size * s->block_size;
    const int nb_match_blocks = sc->nb_match_blocks;
    const int block_size = s->block_size;
    const int width = s->planewidth[plane];
    const int pgroup_size = s->pgroup_size;
    const int group_size = s->group_size;
    const float sigma_sqr = s->sigma * s->sigma;
    float *buffer = sc->buffer;
    float *bufferh = sc->bufferh;
    float *bufferv = sc->bufferv;
    float *bufferz = sc->bufferz;
    float *rbuffer = sc->rbuffer;
    float *rbufferh = sc->rbufferh;
    float *rbufferv = sc->rbufferv;
    float *rbufferz = sc->rbufferz;
    float den_weight, num_weight;
    float l2_wiener = 0;
    int i, j, k;

    for (k = 0; k < nb_match_blocks; k++) {
        const int y = sc->match_blocks[k].y;
        const int x = sc->match_blocks[k].x;

        for (i = 0; i < block_size; i++) {
            s->get_block_row(src, src_linesize, y + i, x, block_size, bufferh + block_size * i);
            s->get_block_row(ref, ref_linesize, y + i, x, block_size, rbufferh + block_size * i);
            av_dct_calc(sc->dctf, bufferh + block_size * i);
            av_dct_calc(sc->dctf, rbufferh + block_size * i);
        }

        for (i = 0; i < block_size; i++) {
            for (j = 0; j < block_size; j++) {
                bufferv[i * block_size + j] = bufferh[j * block_size + i];
                rbufferv[i * block_size + j] = rbufferh[j * block_size + i];
            }
            av_dct_calc(sc->dctf, bufferv + i * block_size);
            av_dct_calc(sc->dctf, rbufferv + i * block_size);
        }

        for (i = 0; i < block_size; i++) {
            memcpy(buffer + k * buffer_linesize + i * block_size,
                   bufferv + i * block_size, block_size * 4);
            memcpy(rbuffer + k * buffer_linesize + i * block_size,
                   rbufferv + i * block_size, block_size * 4);
        }
    }

    for (i = 0; i < block_size; i++) {
        for (j = 0; j < block_size; j++) {
            for (k = 0; k < nb_match_blocks; k++) {
                bufferz[k] = buffer[buffer_linesize * k + i * block_size + j];
                rbufferz[k] = rbuffer[buffer_linesize * k + i * block_size + j];
            }
            if (group_size > 1) {
                av_dct_calc(sc->gdctf, bufferz);
                av_dct_calc(sc->gdctf, rbufferz);
            }
            bufferz += pgroup_size;
            rbufferz += pgroup_size;
        }
    }

    bufferz = sc->bufferz;
    rbufferz = sc->rbufferz;

    for (i = 0; i < block_size; i++) {
        for (j = 0; j < block_size; j++) {
            for (k = 0; k < nb_match_blocks; k++) {
                const float ref_sqr = rbufferz[k] * rbufferz[k];
                float wiener_coef = ref_sqr / (ref_sqr + sigma_sqr);

                if (isnan(wiener_coef))
                   wiener_coef = 1;
                bufferz[k] *= wiener_coef;
                l2_wiener += wiener_coef * wiener_coef;
            }
            bufferz += pgroup_size;
            rbufferz += pgroup_size;
        }
    }

    bufferz = sc->bufferz;
    buffer = sc->buffer;
    for (i = 0; i < block_size; i++) {
        for (j = 0; j < block_size; j++) {
            if (group_size > 1)
                av_dct_calc(sc->gdcti, bufferz);
            for (k = 0; k < nb_match_blocks; k++) {
                buffer[buffer_linesize * k + i * block_size + j] = bufferz[k];
            }
            bufferz += pgroup_size;
        }
    }

    l2_wiener = FFMAX(l2_wiener, 1e-15f);
    den_weight = 1.f / l2_wiener;
    num_weight = den_weight;

    for (k = 0; k < nb_match_blocks; k++) {
        float *num = sc->num + y * width + x;
        float *den = sc->den + y * width + x;

        for (i = 0; i < block_size; i++) {
            memcpy(bufferv + i * block_size,
                   buffer + k * buffer_linesize + i * block_size,
                   block_size * 4);
        }

        for (i = 0; i < block_size; i++) {
            av_dct_calc(sc->dcti, bufferv + block_size * i);
            for (j = 0; j < block_size; j++) {
                bufferh[j * block_size + i] = bufferv[i * block_size + j];
            }
        }

        for (i = 0; i < block_size; i++) {
            av_dct_calc(sc->dcti, bufferh + block_size * i);
            for (j = 0; j < block_size; j++) {
                num[j] += bufferh[i * block_size + j] * num_weight;
                den[j] += den_weight;
            }
            num += width;
            den += width;
        }
    }
}

static int filter_slice(AVFilterContext *ctx, void *arg, int jobnr, int nb_jobs)
{
    BM3DContext *s = ctx->priv;
    SliceContext *sc = &s->slices[jobnr];
    const int block_step = s->block_step;
    ThreadData *td = arg;
    const uint8_t *src = td->src;
    const uint8_t *ref = td->ref;
    const int src_linesize = td->src_linesize;
    const int ref_linesize = td->ref_linesize;
    const int plane = td->plane;
    const int width = s->planewidth[plane];
    const int height = s->planeheight[plane];
    const int block_pos_bottom = height - s->block_size;
    const int block_pos_right  = width - s->block_size;
    const int slice_start = (((height + block_step - 1) / block_step) * jobnr / nb_jobs) * block_step;
    const int slice_end = (jobnr == nb_jobs - 1) ? block_pos_bottom + block_step :
                          (((height + block_step - 1) / block_step) * (jobnr + 1) / nb_jobs) * block_step;
    int i, j;

    memset(sc->num, 0, width * height * sizeof(FFTSample));
    memset(sc->den, 0, width * height * sizeof(FFTSample));

    for (j = slice_start; j < slice_end; j += block_step) {
        if (j > block_pos_bottom) {
            j = block_pos_bottom;
        }

        for (i = 0; i < block_pos_right + block_step; i += block_step) {
            if (i > block_pos_right) {
                i = block_pos_right;
            }

            block_matching(s, ref, ref_linesize, j, i, plane, jobnr);

            s->block_filtering(s, src, src_linesize,
                               ref, ref_linesize, j, i, plane, jobnr);
        }
    }

    return 0;
}

static int filter_frame(AVFilterContext *ctx, AVFrame **out, AVFrame *in, AVFrame *ref)
{
    BM3DContext *s = ctx->priv;
    AVFilterLink *outlink = ctx->outputs[0];
    int p;

    *out = ff_get_video_buffer(outlink, outlink->w, outlink->h);
    if (!*out)
        return AVERROR(ENOMEM);
    av_frame_copy_props(*out, in);

    for (p = 0; p < s->nb_planes; p++) {
        const int nb_jobs = FFMIN(s->nb_threads, s->planeheight[p] / s->block_step);
        ThreadData td;

        if (!((1 << p) & s->planes) || outlink->disabled) {
            av_image_copy_plane((*out)->data[p], (*out)->linesize[p],
                                in->data[p], in->linesize[p],
                                s->planewidth[p], s->planeheight[p]);
            continue;
        }

        td.src = in->data[p];
        td.src_linesize = in->linesize[p];
        td.ref = ref->data[p];
        td.ref_linesize = ref->linesize[p];
        td.plane = p;
        ctx->internal->execute(ctx, filter_slice, &td, NULL, nb_jobs);

        s->do_output(s, (*out)->data[p], (*out)->linesize[p], p, nb_jobs);
    }

    return 0;
}

static int config_input(AVFilterLink *inlink)
{
    const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(inlink->format);
    AVFilterContext *ctx = inlink->dst;
    BM3DContext *s = ctx->priv;
    int i, group_bits;

    s->nb_threads = FFMIN(ff_filter_get_nb_threads(ctx), MAX_NB_THREADS);
    s->nb_planes = av_pix_fmt_count_planes(inlink->format);
    s->depth = desc->comp[0].depth;
    s->max = (1 << s->depth) - 1;
    s->planeheight[1] = s->planeheight[2] = AV_CEIL_RSHIFT(inlink->h, desc->log2_chroma_h);
    s->planeheight[0] = s->planeheight[3] = inlink->h;
    s->planewidth[1]  = s->planewidth[2]  = AV_CEIL_RSHIFT(inlink->w, desc->log2_chroma_w);
    s->planewidth[0]  = s->planewidth[3]  = inlink->w;

    for (group_bits = 4; 1 << group_bits < s->group_size; group_bits++);
    s->group_bits = group_bits;
    s->pgroup_size = 1 << group_bits;

    for (i = 0; i < s->nb_threads; i++) {
        SliceContext *sc = &s->slices[i];

        sc->num = av_calloc(s->planewidth[0] * s->planeheight[0], sizeof(FFTSample));
        sc->den = av_calloc(s->planewidth[0] * s->planeheight[0], sizeof(FFTSample));
        if (!sc->num || !sc->den)
            return AVERROR(ENOMEM);

        sc->dctf = av_dct_init(av_log2(s->block_size), DCT_II);
        sc->dcti = av_dct_init(av_log2(s->block_size), DCT_III);
        if (!sc->dctf || !sc->dcti)
            return AVERROR(ENOMEM);

        if (s->group_bits > 1) {
            sc->gdctf = av_dct_init(s->group_bits, DCT_II);
            sc->gdcti = av_dct_init(s->group_bits, DCT_III);
            if (!sc->gdctf || !sc->gdcti)
                return AVERROR(ENOMEM);
        }

        sc->buffer = av_calloc(s->block_size * s->block_size * s->pgroup_size, sizeof(*sc->buffer));
        sc->bufferz = av_calloc(s->block_size * s->block_size * s->pgroup_size, sizeof(*sc->bufferz));
        sc->bufferh = av_calloc(s->block_size * s->block_size, sizeof(*sc->bufferh));
        sc->bufferv = av_calloc(s->block_size * s->block_size, sizeof(*sc->bufferv));
        if (!sc->bufferh || !sc->bufferv || !sc->buffer || !sc->bufferz)
            return AVERROR(ENOMEM);

        if (s->mode == FINAL) {
            sc->rbuffer = av_calloc(s->block_size * s->block_size * s->pgroup_size, sizeof(*sc->rbuffer));
            sc->rbufferz = av_calloc(s->block_size * s->block_size * s->pgroup_size, sizeof(*sc->rbufferz));
            sc->rbufferh = av_calloc(s->block_size * s->block_size, sizeof(*sc->rbufferh));
            sc->rbufferv = av_calloc(s->block_size * s->block_size, sizeof(*sc->rbufferv));
            if (!sc->rbufferh || !sc->rbufferv || !sc->rbuffer || !sc->rbufferz)
                return AVERROR(ENOMEM);
        }

        sc->search_positions = av_calloc(SQR(2 * s->bm_range / s->bm_step + 1), sizeof(*sc->search_positions));
        if (!sc->search_positions)
            return AVERROR(ENOMEM);
    }

    s->do_output = do_output;
    s->do_block_ssd = do_block_ssd;
    s->get_block_row = get_block_row;

    if (s->depth > 8) {
        s->do_output = do_output16;
        s->do_block_ssd = do_block_ssd16;
        s->get_block_row = get_block_row16;
    }

    return 0;
}
