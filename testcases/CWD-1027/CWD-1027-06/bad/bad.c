#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GList  GList_Glib
#define GMenuItem GMenuItem_GIO
#define GTimer GTimer_GTK

#undef GList
#undef GMenuItem
#undef GTimer

#define MONO_UI_FAMILIES "monospace"

typedef uint32_t unichar_t;

typedef uint32_t Color;

#define COLOR_UNKNOWN		((Color) 0xffffffff)
#define COLOR_TRANSPARENT	((Color) 0xffffffff)
#define COLOR_DEFAULT		((Color) 0xfffffffe)
#define COLOR_WARNING		((Color) 0xfffffffd)

typedef struct grect {
    int32_t x,y,width,height;
} GRect;

#define GRECT_EMPTY { 0, 0, 0, 0 }

typedef struct gpoint {
    int16_t x,y;
} GPoint;

struct _GImage;
typedef struct gimage {
    short list_len;
    union {
	struct _GImage *image;
	struct _GImage **images;
    } u;
    void *userdata;
} GImage;

struct font_instance;
typedef struct font_instance FontInstance, GFont;

struct gwindow;
typedef struct gwindow *GWindow;
typedef struct gdisplay GDisplay;
typedef struct gtimer GTimer;

enum cursor_types { ct_default, ct_pointer, ct_backpointer, ct_hand,
    ct_question, ct_cross, ct_4way, ct_text, ct_watch, ct_draganddrop,
    ct_invisible,
    ct_user, ct_user2 };

enum event_type { et_noevent = -1, et_char, et_charup,
    et_mousemove, et_mousedown, et_mouseup,
    et_crossing,
    et_focus,
    et_expose, et_visibility, et_resize, et_timer,
    et_close, et_create,
    et_map, et_destroy,
    et_selclear,
    et_drag, et_dragout, et_drop,
    et_lastnativeevent=et_drop,
    et_controlevent, et_user };

enum window_attr_mask { wam_events=0x2, wam_bordwidth=0x4,
    wam_bordcol=0x8, wam_backcol=0x10, wam_cursor=0x20, wam_wtitle=0x40,
    wam_ititle=0x80, wam_icon=0x100, wam_nodecor=0x200,
    wam_positioned=0x400, wam_centered=0x800, wam_undercursor=0x1000,
    wam_noresize=0x2000, wam_restrict=0x4000, wam_redirect=0x8000,
    wam_isdlg=0x10000, wam_notrestricted=0x20000,
    wam_transient=0x40000,
    wam_utf8_wtitle=0x80000, wam_utf8_ititle=0x100000,
    wam_nocairo=0x200000, wam_verytransient=0x400000, wam_palette=0x800000 };

typedef struct gwindow_attrs {
    enum window_attr_mask mask;
    uint32_t event_masks;
    int16_t border_width;
    Color border_color;
    Color background_color;
    enum cursor_types cursor;
    const unichar_t *window_title;
    const unichar_t *icon_title;
    struct gwindow *icon;
    unsigned int nodecoration: 1;
    unsigned int positioned: 1;
    unsigned int centered: 2;
    unsigned int undercursor: 1;
    unsigned int noresize: 1;
    unsigned int restrict_input_to_me: 1;
    unsigned int redirect_chars_to_me: 1;
    unsigned int is_dlg: 1;
    unsigned int not_restricted: 1;
    GWindow redirect_from;
    GWindow transient;
    const char *utf8_window_title;
    const char *utf8_icon_title;
} GWindowAttrs;

typedef struct gtextinfo {
    unichar_t *text;
    GImage *image;
    Color fg;
    Color bg;
    void *userdata;
    GFont *font;
    unsigned int disabled: 1;
    unsigned int image_precedes: 1;
    unsigned int checkable: 1;
    unsigned int checked: 1;
    unsigned int selected: 1;
    unsigned int line: 1;
    unsigned int text_is_1byte: 1;
    unsigned int text_in_resource: 1;
    unsigned int changed: 1;
    unichar_t mnemonic;
    char* text_untranslated;
} GTextInfo;

enum box_flags {
    box_foreground_border_inner = 1,
    box_foreground_border_outer = 2,
    box_active_border_inner = 4,
    box_foreground_shadow_outer = 8,
    box_do_depressed_background = 0x10,
    box_draw_default = 0x20,
    box_generate_colors = 0x40,
    box_gradient_bg = 0x80,
    box_flag_mask = 0xFF
};

typedef struct gbox {
    unsigned char border_type;
    unsigned char border_shape;
    unsigned char border_width;
    unsigned char padding;
    unsigned char rr_radius;
    unsigned char flags;
    Color border_brightest;
    Color border_brighter;
    Color border_darkest;
    Color border_darker;
    Color main_background;
    Color main_foreground;
    Color disabled_background;
    Color disabled_foreground;
    Color active_border;
    Color depressed_background;
    Color gradient_bg_end;
    Color border_inner;
    Color border_outer;
} GBox;

#define GBOX_EMPTY { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 }

enum gg_flags { gg_visible=1, gg_enabled=2, gg_pos_in_pixels=4,
    gg_sb_vert=8, gg_line_vert=gg_sb_vert,
    gg_but_default=0x10, gg_but_cancel=0x20,
    gg_cb_on=0x40, gg_rad_startnew=0x80,
    gg_rad_continueold=0x100,
    gg_list_alphabetic=0x100, gg_list_multiplesel=0x200,
    gg_list_exactlyone=0x400, gg_list_internal=0x800,
    gg_group_prevlabel=0x1000, gg_group_end=0x2000,
    gg_textarea_wrap=0x4000,
    gg_tabset_scroll=0x8000, gg_tabset_filllines=0x10000, gg_tabset_fill1line = 0x20000,
    gg_tabset_nowindow=gg_textarea_wrap,
    gg_rowcol_alphabetic=gg_list_alphabetic,
    gg_rowcol_vrules=0x40000, gg_rowcol_hrules=0x800000,
    gg_rowcol_displayonly=0x1000000,
    gg_dontcopybox=0x10000000,
    gg_pos_use0=0x20000000, gg_pos_under=0x40000000,
    gg_pos_newline = (int) 0x80000000,
    gg_file_pulldown=gg_sb_vert, gg_file_multiple = gg_list_multiplesel,
    gg_text_xim = gg_tabset_scroll,
    gg_tabset_vert = gg_sb_vert,
    gg_s1_vert = gg_sb_vert,
    gg_s1_flowalign = gg_list_alphabetic,
    gg_s1_right = gg_list_multiplesel,
    gg_s1_bottom = gg_s1_right,
    gg_s1_center = gg_list_internal,
    gg_s1_expand = gg_group_end,
    gg_flow_vert = gg_s1_vert,
    gg_flow_right = gg_s1_right,
    gg_flow_bottom = gg_flow_right,
    gg_flow_center = gg_s1_center,
    gg_flow_expand = gg_s1_expand,
    gg_flow_obottom = gg_list_exactlyone,
    gg_flow_oright = gg_flow_obottom,
    gg_flow_ocenter = gg_group_prevlabel,
    gg_flow_lright = gg_rowcol_hrules,
    gg_flow_lbottom = gg_dontcopybox,
    gg_flow_lhcenter = gg_rowcol_displayonly,
    gg_flow_lvcenter = gg_pos_use0,
    gg_flow_noalignlabel = gg_tabset_scroll
};

typedef struct ggadget GGadget;
typedef struct ggadget *GGadgetSet;

enum et_subtype { et_buttonpress, et_buttonactivate, et_radiochanged,
    et_listselected, et_listdoubleclick,
    et_scrollbarchange,
    et_textchanged, et_textfocuschanged,
    et_save, et_lastsubtype };

struct gevent;
typedef int (*GDrawEH)(GWindow,struct gevent *);

typedef struct ggadgetdata {
    GRect pos;
    GBox *box;
    unichar_t mnemonic;
    unichar_t shortcut;
    uint8_t short_mask;
    uint8_t cols;
    short cid;
    GTextInfo *label;
    union {
	GTextInfo *list;
	struct ggadgetcreatedata **boxelements;
	GDrawEH drawable_e_h;
	Color col;
    } u;
    enum gg_flags flags;
    const char *popup_msg;
} GGadgetData;

typedef struct ggadgetcreatedata {
    GGadget *(*creator)(struct gwindow *base, GGadgetData *gd,void *data);
    GGadgetData gd;
    void *data;
    GGadget *ret;
} GGadgetCreateData;

#define GCD_Glue	((GGadgetCreateData *) -1)
#define GCD_ColSpan	((GGadgetCreateData *) -2)
#define GCD_RowSpan	((GGadgetCreateData *) -3)
#define GCD_HPad10	((GGadgetCreateData *) -4)

#define GLINE_MAX	20

enum ghvbox_expand { gb_expandglue=-4, gb_expandgluesame=-3, gb_samesize=-2,
    gb_expandall=-1 };

typedef struct gresfont {
    GFont *fi;
    char *rstr;
    uint8_t can_free_name;
} GResFont;

#define GRESFONT_INIT(defstr) { NULL, (defstr), false }

struct dlg_info {
    int done;
    int ret;
    int multi;
    int exposed;
    int size_diff;
    int bcnt;
};

extern GBox _GGadget_defaultbutton_box;
extern GResFont _gtextfield_font;
extern GDisplay *screen_display;

char *vsmprintf(const char *fmt, va_list args);
unichar_t *utf82u_copy(const char *utf8buf);
void GGadgetInit(void);
void GProgressPauseTimer(void);
void GProgressResumeTimer(void);
void GTextFieldInit(void);
int FindLineBreaks(const unichar_t *question, GTextInfo linebreaks[GLINE_MAX+1]);
int d_e_h(GWindow gw, struct gevent *event);
int w_e_h(GWindow gw, struct gevent *event);

GWindow GDrawCreateTopWindow(GDisplay *gdisp, GRect *pos, int (*eh)(GWindow,struct gevent *), void *user_data, GWindowAttrs *wattrs);
void GDrawSetDefaultFont(GWindow gw);
void GDrawDefaultFontMetrics(GWindow gw,int *as, int *ds, int *ld);
int GDrawGetTextWidth(GWindow gw, const unichar_t *text, int cnt);
int GDrawGetText8Width(GWindow gw, const char *text, int cnt);
int GDrawPointsToPixels(GWindow gw,int points);
void GDrawSetFont(GWindow gw, GFont *fi);
void GDrawSetVisible(GWindow w, int visible);

GGadget *GLabelCreate(struct gwindow *base, GGadgetData *gd,void *data);
GGadget *GButtonCreate(struct gwindow *base, GGadgetData *gd,void *data);
GGadget *GTextFieldCreate(struct gwindow *base, GGadgetData *gd,void *data);
GGadget *GPasswordCreate(struct gwindow *base, GGadgetData *gd,void *data);
GGadget *GHBoxCreate(struct gwindow *base, GGadgetData *gd,void *data);
GGadget *GHVGroupCreate(struct gwindow *base, GGadgetData *gd,void *data);
void GGadgetsCreate(GWindow base, GGadgetCreateData *gcd);
void GHVBoxSetExpandableCol(GGadget *g,int col);
void GHVBoxFitWindow(GGadget *g);
int g_vsnprintf(char *str, size_t size, const char *format, va_list ap);

char *vsmprintf(const char *fmt, va_list args) {
    va_list args2;
    char* ret;
    int len;

    va_copy(args2, args);
    len = g_vsnprintf(NULL, 0, fmt, args2);
    va_end(args2);

    if (len < 0) {
        return NULL;
    }

    ret = malloc(len + 1);
    if (ret == NULL) {
        perror("smprintf");
        abort();
        return NULL;
    }

    len = g_vsnprintf(ret, len + 1, fmt, args);

    if (len < 0) {
        free(ret);
        return NULL;
    }

    return ret;
}


static GWindow DlgCreate8(const char *title,const char *question,va_list ap,
	const char **answers, int def, int cancel,
	struct dlg_info *d, int add_text, const char *defstr,
	int restrict_input, int center) {
    GTextInfo qlabels[GLINE_MAX+1], *blabels;
    GGadgetCreateData *gcd, *array[2*(GLINE_MAX+5)+1], boxes[5], **barray, *labarray[4];
    int lb, bcnt=0, l;
    GRect pos;
    GWindow gw;
    GWindowAttrs wattrs;
    int as, ds, ld, fh;
    int w, maxw, bw, bspace;
    int i, y;
    char *buf;
    unichar_t *ubuf;
    extern GBox _GGadget_defaultbutton_box;

    if ( d!=NULL )
	memset(d,0,sizeof(*d));
    buf = vsmprintf(question, ap);
    if ( screen_display==NULL ) {
	fprintf(stderr, "%s\n", buf );
	if ( d!=NULL ) d->done = true;
return( NULL );
    }
    ubuf = utf82u_copy(buf);
    free(buf);
    if( !ubuf ) {
	fprintf( stderr, "Failed to convert question string in DlgCreate8()\n" );
	return( NULL );
    }

    GGadgetInit();
    GProgressPauseTimer();
    memset(qlabels,'\0',sizeof(qlabels));
    lb = FindLineBreaks(ubuf,qlabels);
    for ( bcnt=0; answers[bcnt]!=NULL; ++bcnt);
    blabels = calloc(bcnt+1,sizeof(GTextInfo));
    barray = calloc(2*bcnt+3,sizeof(GGadgetCreateData *));
    for ( bcnt=0; answers[bcnt]!=NULL; ++bcnt) {
	blabels[bcnt].text = (unichar_t *) answers[bcnt];
	blabels[bcnt].text_is_1byte = true;
	blabels[bcnt].text_in_resource = true;	/* Look for mnemonics in the utf8 string (preceded by _) */
    }

    memset(&wattrs,0,sizeof(wattrs));
    /* If we have many questions in quick succession the dlg will jump around*/
    /*  as it tracks the cursor (which moves to the buttons). That's not good*/
    /*  So I don't do undercursor here */
    wattrs.mask = wam_events|wam_cursor|wam_utf8_wtitle|wam_isdlg;
    if ( restrict_input )
	wattrs.mask |= wam_restrict;
    else 
	wattrs.mask |= wam_notrestricted;
    if ( center )
	wattrs.mask |= wam_centered;
    else
	wattrs.mask |= wam_undercursor;
    wattrs.not_restricted = true;
    wattrs.restrict_input_to_me = 1;
    wattrs.is_dlg = true;
    wattrs.event_masks = ~(1<<et_charup);
    wattrs.undercursor = 1;
    wattrs.centered = 2;
    wattrs.cursor = ct_pointer;
    wattrs.utf8_window_title = (char *) title;
    pos.x = pos.y = 0;
    pos.width = 200; pos.height = 60;		/* We'll figure size later */
		/* but if we get the size too small, the cursor isn't in dlg */
    gw = GDrawCreateTopWindow(NULL,&pos,restrict_input?d_e_h:w_e_h,d,&wattrs);

    GGadgetInit();
    GDrawSetDefaultFont(gw);
    GDrawDefaultFontMetrics(gw,&as,&ds,&ld);
    fh = as+ds;
    maxw = 0;
    for ( i=0; i<lb; ++i ) {
	w = GDrawGetTextWidth(gw,qlabels[i].text,-1);
	if ( w>maxw ) maxw = w;
    }
    if ( add_text && defstr!=NULL ) {
	extern GResFont _gtextfield_font;

	GTextFieldInit();
	if ( _gtextfield_font.fi!=NULL ) {
	    GDrawSetFont(gw,_gtextfield_font.fi);
	    w = GDrawGetText8Width(gw,defstr,-1);
	    GDrawSetDefaultFont(gw);
	} else
	    w = 8*GDrawGetText8Width(gw,defstr,-1)/5;
	w += GDrawPointsToPixels(gw,40);
	if ( w >1000 ) w = 1000;
	if ( w>maxw ) maxw = w;
    }
    bw = 0;
    for ( i=0; i<bcnt; ++i ) {
	w = GDrawGetText8Width(gw,answers[i],-1);
	if ( w>bw ) bw = w;
    }
    bw += GDrawPointsToPixels(gw,20);
    bspace = GDrawPointsToPixels(gw,6);
    if ( (bw+bspace) * bcnt > maxw )
	maxw = (bw+bspace)*bcnt;
    if ( bcnt!=1 )
	bspace = (maxw-bcnt*bw)/(bcnt-1);
    maxw += GDrawPointsToPixels(gw,16);

    gcd = calloc(lb+bcnt+2,sizeof(GGadgetCreateData));
    memset(boxes,0,sizeof(boxes));
    l = 0;
    if ( lb==1 ) {
	gcd[0].gd.pos.width = GDrawGetTextWidth(gw,qlabels[0].text,-1);
	gcd[0].gd.pos.x = (maxw-gcd[0].gd.pos.width)/2;
	gcd[0].gd.pos.y = GDrawPointsToPixels(gw,6);
	gcd[0].gd.pos.height = fh;
	gcd[0].gd.flags = gg_visible | gg_enabled | gg_pos_in_pixels | gg_pos_use0;
	gcd[0].gd.label = &qlabels[0];
	gcd[0].creator = GLabelCreate;
	labarray[0] = GCD_Glue; labarray[1] = &gcd[0]; labarray[2] = GCD_Glue; labarray[3] = NULL;
	boxes[2].gd.flags = gg_visible|gg_enabled;
	boxes[2].gd.u.boxelements = labarray;
	boxes[2].creator = GHBoxCreate;
	array[0] = &boxes[2]; array[1] = NULL;
	l = 1;
    } else for ( i=0; i<lb; ++i ) {
	gcd[i].gd.pos.x = GDrawPointsToPixels(gw,8);
	gcd[i].gd.pos.y = GDrawPointsToPixels(gw,6)+i*fh;
	gcd[i].gd.pos.width = GDrawGetTextWidth(gw,qlabels[i].text,-1);
	gcd[i].gd.pos.height = fh;
	gcd[i].gd.flags = gg_visible | gg_enabled | gg_pos_in_pixels | gg_pos_use0;
	gcd[i].gd.label = &qlabels[i];
	gcd[i].creator = GLabelCreate;
	array[2*l] = &gcd[i]; array[2*l++ +1] = NULL;
    }
    y = GDrawPointsToPixels(gw,12)+lb*fh;
    if ( add_text ) {
	gcd[bcnt+lb].gd.pos.x = GDrawPointsToPixels(gw,8);
	gcd[bcnt+lb].gd.pos.y = y;
	gcd[bcnt+lb].gd.pos.width = maxw-2*GDrawPointsToPixels(gw,6);
	gcd[bcnt+lb].gd.flags = gg_visible | gg_enabled | gg_pos_in_pixels | gg_pos_use0 | gg_text_xim;
	gcd[bcnt+lb].gd.cid = bcnt;
	gcd[bcnt+lb].creator = GTextFieldCreate;
	if ( add_text==2 )
	    gcd[bcnt+lb].creator = GPasswordCreate;
	y += fh + GDrawPointsToPixels(gw,6) + GDrawPointsToPixels(gw,10);
	array[2*l] = &gcd[bcnt+lb]; array[2*l++ +1] = NULL;
    }
    y += GDrawPointsToPixels(gw,2);
    for ( i=0; i<bcnt; ++i ) {
	gcd[i+lb].gd.pos.x = GDrawPointsToPixels(gw,8) + i*(bw+bspace);
	gcd[i+lb].gd.pos.y = y;
	gcd[i+lb].gd.pos.width = bw;
	gcd[i+lb].gd.flags = gg_visible | gg_enabled | gg_pos_in_pixels | gg_pos_use0;
	if ( i==def ) {
	    gcd[i+lb].gd.flags |= gg_but_default;
	    if ( _GGadget_defaultbutton_box.flags&box_draw_default ) {
		gcd[i+lb].gd.pos.x -= GDrawPointsToPixels(gw,3);
		gcd[i+lb].gd.pos.y -= GDrawPointsToPixels(gw,3);
		gcd[i+lb].gd.pos.width += 2*GDrawPointsToPixels(gw,3);
	    }
	}
	if ( i==cancel )
	    gcd[i+lb].gd.flags |= gg_but_cancel;
	gcd[i+lb].gd.cid = i;
	gcd[i+lb].gd.label = &blabels[i];
	gcd[i+lb].creator = GButtonCreate;
	barray[2*i] = GCD_Glue; barray[2*i+1] = &gcd[i+lb];
    }
    barray[2*i] = GCD_Glue; barray[2*i+1] = NULL;
    boxes[3].gd.flags = gg_visible|gg_enabled;
    boxes[3].gd.u.boxelements = barray;
    boxes[3].creator = GHBoxCreate;
    array[2*l] = &boxes[3]; array[2*l++ +1] = NULL;
    array[2*l] = NULL;

    boxes[0].gd.pos.x = boxes[0].gd.pos.y = 2;
    boxes[0].gd.flags = gg_visible|gg_enabled;
    boxes[0].gd.u.boxelements = array;
    boxes[0].creator = GHVGroupCreate;

    GGadgetsCreate(gw,boxes);
    GHVBoxSetExpandableCol(boxes[3].ret,gb_expandgluesame);
    if ( boxes[2].ret!=NULL )
	GHVBoxSetExpandableCol(boxes[2].ret,gb_expandglue);
    GHVBoxFitWindow(boxes[0].ret);

    if ( d!=NULL ) {
	d->ret  = cancel;
	d->bcnt = bcnt;
    }
    GDrawSetVisible(gw,true);
    free(blabels);
    free(gcd);
    free(barray);
    for ( i=0; i<lb; ++i )
	free(qlabels[i].text);
    free(ubuf);
    GProgressResumeTimer();
return( gw );
}
