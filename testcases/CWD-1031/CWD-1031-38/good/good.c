#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <sys/types.h>
#include <sys/uio.h>

#define MK_PATH_BASE 128

#define MK_FALSE 0
#define MK_TRUE  !MK_FALSE

#ifdef __GNUC__
  #define mk_unlikely(x) __builtin_expect((x),0)
  #define mk_likely(x) __builtin_expect((x),1)
#else
  #define mk_unlikely(x)      (x)
  #define mk_likely(x)        (x)
#endif

#define ALLOCSZ_ATTR(x,...)

struct mk_list
{
    struct mk_list *prev, *next;
};

#define mk_iovec iovec

struct mk_event;
struct mk_plugin_network;
struct mk_http_session;
struct mk_vhost;
struct mk_vhost_alias;

typedef struct
{
    char *data;
    unsigned long len;
} mk_ptr_t;

static inline ALLOCSZ_ATTR(1)
void *mk_mem_alloc(const size_t size)
{
    void *aux = malloc(size);

    if (mk_unlikely(!aux && size)) {
        perror("malloc");
        return NULL;
    }

    return aux;
}

static inline void mk_mem_free(void *ptr)
{
    free(ptr);
}

void mk_mem_pointers_init(void);

struct file_info
{
    size_t size;
    time_t last_modification;

    int flags_read_only;

    unsigned char exists;
    unsigned char is_file;
    unsigned char is_link;
    unsigned char is_directory;
    unsigned char exec_access;
    unsigned char read_access;
};

#define MK_IOV_CRLF "\r\n"
#define MK_IOV_CRLFCRLF "\r\n\r\n"
#define MK_IOV_LF "\n"
#define MK_IOV_LFLF "\n\n"
#define MK_IOV_LFLFLFLF "\n\n\n\n"
#define MK_IOV_SPACE " "
#define MK_IOV_SLASH "/"
#define MK_IOV_NONE ""
#define MK_IOV_EQUAL "="

struct mk_iov {
    int iov_idx;
    int buf_idx;
    int size;
    unsigned long total_len;
    struct mk_iovec *io;
    void **buf_to_free;
};

#define MK_STREAM_RAW       0
#define MK_STREAM_IOV       1
#define MK_STREAM_FILE      2
#define MK_STREAM_SOCKET    3

#define MK_CHANNEL_DISABLED 0
#define MK_CHANNEL_ENABLED  1

struct mk_channel {
    int type;
    int fd;
    int status;

    struct mk_event *event;
    struct mk_plugin_network *io;
    struct mk_list streams;
    void *thread;
};

struct mk_stream_input {
    int type;
    int fd;
    int dynamic;

    size_t bytes_total;
    off_t  bytes_offset;

    void *buffer;
    void *context;

    void (*cb_consumed)(struct mk_stream_input *, long);
    void (*cb_finished)(struct mk_stream_input *);

    struct mk_stream *stream;
    struct mk_list _head;
};

struct mk_stream {
    int preserve;
    int encoding;
    int dynamic;

    size_t bytes_total;
    off_t  bytes_offset;

    struct mk_channel *channel;

    void *context;

    void (*cb_finished) (struct mk_stream *);
    void (*cb_bytes_consumed) (struct mk_stream *, long);
    void (*cb_exception) (struct mk_stream *, int);

    struct mk_list inputs;

    struct mk_list _head;
};

#define MK_HEADER_IOV         32
#define MK_HEADER_ETAG_SIZE   32

struct response_headers
{
    int status;

    int connection;

    mk_ptr_t custom_status;

    long content_length;

    long real_length;

    int cgi;
    int pconnections_left;
    int breakline;

    int transfer_encoding;

    int upgrade;

    int ranges[2];

    time_t last_modified;
    mk_ptr_t allow_methods;
    mk_ptr_t content_type;
    mk_ptr_t content_encoding;
    char *location;

    int  etag_len;
    char etag_buf[MK_HEADER_ETAG_SIZE];

    struct mk_iov *_extra_rows;

    int sent;

    struct mk_iov headers_iov;
    struct mk_iovec __iov_io[MK_HEADER_IOV];
    void *__iov_buf[MK_HEADER_IOV];
};

struct mk_http_request
{
    int status;
    int protocol;

    int user_home;

    long port;

    uint64_t stream_size;

    struct mk_stream stream;
    struct mk_stream_input in_headers;
    struct mk_stream_input in_headers_extra;
    struct mk_stream_input in_file;
    struct mk_stream_input page_stream;

    int headers_len;

    int method;
    mk_ptr_t method_p;
    mk_ptr_t uri;
    mk_ptr_t uri_processed;

    mk_ptr_t protocol_p;

    mk_ptr_t body;

    int content_length;

    mk_ptr_t _content_length;
    mk_ptr_t content_type;
    mk_ptr_t connection;

    mk_ptr_t host;
    mk_ptr_t host_port;
    mk_ptr_t if_modified_since;
    mk_ptr_t last_modified_since;
    mk_ptr_t range;

    mk_ptr_t data;

    mk_ptr_t real_path;

    char real_path_static[MK_PATH_BASE];

    mk_ptr_t query_string;

    int stage30_blocked;

    void *stage30_handler;

    int file_fd;
    struct file_info file_info;

    int vhost_fdt_id;
    unsigned int vhost_fdt_hash;
    int vhost_fdt_enabled;

    struct mk_vhost   *host_conf;
    struct mk_vhost_alias *host_alias;

    void *handler_data;

    struct mk_http_session *session;

    void *thread;

    struct mk_list _head;

    struct response_headers headers;
};

int mk_string_char_search(const char *string, int c, int len);
char *mk_string_copy_substr(const char *string, int pos_init, int pos_end);

char *mk_string_copy_substr(const char *string, int pos_init, int pos_end)
{
    unsigned int size, bytes;
    char *buffer = 0;

    if (pos_init > pos_end) {
        return NULL;
    }

    size = (unsigned int) (pos_end - pos_init) + 1;
    if (size <= 2) {
        size = 4;
    }

    buffer = mk_mem_alloc(size);

    if (!buffer) {
        return NULL;
    }

    bytes = pos_end - pos_init;
    memcpy(buffer, string + pos_init, bytes);
    buffer[bytes] = '\0';

    return (char *) buffer;
}

static int mk_http_range_parse(struct mk_http_request *sr)
{
    int eq_pos, sep_pos, len;
    char *buffer = 0;
    struct response_headers *sh;

    if (!sr->range.data)
        return -1;

    if ((eq_pos = mk_string_char_search(sr->range.data, '=', sr->range.len)) < 0)
        return -1;

    if (strncasecmp(sr->range.data, "Bytes", eq_pos) != 0)
        return -1;

    if ((sep_pos = mk_string_char_search(sr->range.data, '-', sr->range.len)) < 0)
        return -1;

    len = sr->range.len;
    sh = &sr->headers;

    /* =-xxx */
    if (eq_pos + 1 == sep_pos) {
        sh->ranges[0] = -1;
        sh->ranges[1] = (unsigned long) atol(sr->range.data + sep_pos + 1);

        if (sh->ranges[1] <= 0) {
            return -1;
        }

        sh->content_length = sh->ranges[1];
        return 0;
    }

    /* =yyy-xxx */
    if ((eq_pos + 1 != sep_pos) && (len > sep_pos + 1)) {
        buffer = mk_string_copy_substr(sr->range.data, eq_pos + 1, sep_pos);
        if (!buffer) {
            return -1;
        }
        sh->ranges[0] = (unsigned long) atol(buffer);
        mk_mem_free(buffer);

        buffer = mk_string_copy_substr(sr->range.data, sep_pos + 1, len);
        sh->ranges[1] = (unsigned long) atol(buffer);
        mk_mem_free(buffer);

        if (sh->ranges[1] < 0 || (sh->ranges[0] > sh->ranges[1])) {
            return -1;
        }

        sh->content_length = abs(sh->ranges[1] - sh->ranges[0]) + 1;
        return 0;
    }
    /* =yyy- */
    if ((eq_pos + 1 != sep_pos) && (len == sep_pos + 1)) {
        buffer = mk_string_copy_substr(sr->range.data, eq_pos + 1, len);
        sr->headers.ranges[0] = (unsigned long) atol(buffer);
        mk_mem_free(buffer);

        sh->content_length = (sh->content_length - sh->ranges[0]);
        return 0;
    }

    return -1;
}
