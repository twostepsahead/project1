/*
 * Copyright 2014 Garrett D'Amore <garrett@damore.org>
 *
 * This file is made available under the terms of the Less License.
 */

#include <regex.h>

struct mlist;
struct loption;

void *ecalloc(int, unsigned int);
char *easprintf(const char *, ...);
char *estrdup(const char *);
char *skipsp(char *);
int sprefix(char *, char *, int);
void quit(int);
void raw_mode(int);
char *special_key_str(int);
void get_term(void);
void init(void);
void deinit(void);
void home(void);
void add_line(void);
void lower_left(void);
void line_left(void);
void goto_line(int);
void vbell(void);
void ring_bell(void);
void do_clear(void);
void clear_eol(void);
void clear_bot(void);
void at_enter(int);
void at_exit(void);
void at_switch(int);
int is_at_equiv(int, int);
int apply_at_specials(int);
void putbs(void);
void match_brac(int, int, int, int);
int ch_get(void);
void ch_ungetchar(int);
void end_logfile(void);
void sync_logfile(void);
int ch_seek(off_t);
int ch_end_seek(void);
int ch_beg_seek(void);
off_t ch_length(void);
off_t ch_tell(void);
int ch_forw_get(void);
int ch_back_get(void);
void ch_setbufspace(int);
void ch_flush(void);
int seekable(int);
void ch_set_eof(void);
void ch_init(int, int);
void ch_close(void);
int ch_getflags(void);
void init_charset(void);
int binary_char(LWCHAR);
int control_char(LWCHAR);
char *prchar(LWCHAR);
char *prutfchar(LWCHAR);
int utf_len(char);
int is_utf8_well_formed(const char *);
LWCHAR get_wchar(const char *);
void put_wchar(char **, LWCHAR);
LWCHAR step_char(char **, int, char *);
int is_composing_char(LWCHAR);
int is_ubin_char(LWCHAR);
int is_wide_char(LWCHAR);
int is_combining_char(LWCHAR, LWCHAR);
void cmd_reset(void);
void clear_cmd(void);
void cmd_putstr(char *);
int len_cmdbuf(void);
void set_mlist(void *, int);
void cmd_addhist(struct mlist *, const char *);
void cmd_accept(void);
int cmd_char(int);
off_t cmd_int(long *);
char *get_cmdbuf(void);
char *cmd_lastpattern(void);
void init_cmdhist(void);
void save_cmdhist(void);
int in_mca(void);
void dispversion(void);
int getcc(void);
void ungetcc(int);
void ungetsc(char *);
void commands(void);
int cvt_length(int);
int *cvt_alloc_chpos(int);
void cvt_text(char *, char *, int *, int *, int);
void init_cmds(void);
void add_fcmd_table(char *, int);
void add_ecmd_table(char *, int);
int fcmd_decode(const char *, char **);
int ecmd_decode(const char *, char **);
char *lgetenv(char *);
int lesskey(char *, int);
void add_hometable(char *, char *, int);
int editchar(int, int);
void init_textlist(struct textlist *, char *);
char *forw_textlist(struct textlist *, char *);
char *back_textlist(struct textlist *, char *);
int edit(char *);
int edit_ifile(IFILE);
int edit_list(char *);
int edit_first(void);
int edit_last(void);
int edit_next(int);
int edit_prev(int);
int edit_index(int);
IFILE save_curr_ifile(void);
void unsave_ifile(IFILE);
void reedit_ifile(IFILE);
void reopen_curr_ifile(void);
int edit_stdin(void);
void cat_file(void);
void use_logfile(char *);
char *shell_unquote(char *);
char *get_meta_escape(void);
char *shell_quote(const char *);
char *homefile(char *);
char *fexpand(char *);
char *fcomplete(char *);
int bin_file(int f);
char *lglob(char *);
char *open_altfile(char *, int *, void **);
void close_altfile(char *, char *, void *);
int is_dir(char *);
char *bad_file(char *);
off_t filesize(int);
char *last_component(char *);
int eof_displayed(void);
int entire_file_displayed(void);
void squish_check(void);
void forw(int, off_t, int, int, int);
void back(int, off_t, int, int);
void forward(int, int, int);
void backward(int, int, int);
int get_back_scroll(void);
void del_ifile(IFILE);
IFILE next_ifile(IFILE);
IFILE prev_ifile(IFILE);
IFILE getoff_ifile(IFILE);
int nifile(void);
IFILE get_ifile(char *, IFILE);
char *get_filename(IFILE);
int get_index(IFILE);
void store_pos(IFILE, struct scrpos *);
void get_pos(IFILE, struct scrpos *);
int opened(IFILE);
void hold_ifile(IFILE, int);
int held_ifile(IFILE);
void set_open(IFILE);
void *get_filestate(IFILE);
void set_filestate(IFILE, void *);
off_t forw_line(off_t);
off_t back_line(off_t);
void set_attnpos(off_t);
void jump_forw(void);
void jump_back(off_t);
void repaint(void);
void jump_percent(int, long);
void jump_line_loc(off_t, int);
void jump_loc(off_t, int);
void init_line(void);
int is_ascii_char(LWCHAR);
void prewind(void);
void plinenum(off_t);
void pshift_all(void);
int is_ansi_end(LWCHAR);
int is_ansi_middle(LWCHAR);
int pappend(char, off_t);
int pflushmbc(void);
void pdone(int, int);
void set_status_col(char);
int gline(int, int *);
void null_line(void);
off_t forw_raw_line(off_t, char **, int *);
off_t back_raw_line(off_t, char **, int *);
void clr_linenum(void);
void add_lnum(off_t, off_t);
off_t find_linenum(off_t);
off_t find_pos(off_t);
off_t currline(int);
void lsystem(const char *, const char *);
int pipe_mark(int, char *);
void init_mark(void);
int badmark(int);
void setmark(int);
void lastmark(void);
void gomark(int);
off_t markpos(int);
void unmark(IFILE);
void opt_o(int, char *);
void opt__O(int, char *);
void opt_j(int, char *);
void calc_jump_sline(void);
void opt_shift(int, char *);
void calc_shift_count(void);
void opt_k(int, char *);
void opt_t(int, char *);
void opt__T(int, char *);
void opt_p(int, char *);
void opt__P(int, char *);
void opt_b(int, char *);
void opt_i(int, char *);
void opt__V(int, char *);
void opt_x(int, char *);
void opt_quote(int, char *);
void opt_query(int, char *);
int get_swindow(void);
char *propt(int);
void scan_option(char *, int);
void toggle_option(struct loption *, int, char *, int);
int opt_has_param(struct loption *);
char *opt_prompt(struct loption *);
int isoptpending(void);
void nopendopt(void);
int getnum(char **, char *, int *);
long getfraction(char **, char *, int *);
int get_quit_at_eof(void);
void init_option(void);
struct loption *findopt(int);
struct loption *findopt_name(char **, char **, int *);
int iread(int, unsigned char *, unsigned int);
char *errno_message(char *);
int percentage(off_t, off_t);
off_t percent_pos(off_t, int, long);
void put_line(void);
void flush(int);
int putchr(int);
void putstr(const char *);
void get_return(void);
void error(const char *, PARG *);
void ierror(const char *, PARG *);
int query(const char *, PARG *);
int compile_pattern(char *, int, regex_t **);
void uncompile_pattern(regex_t **);
int match_pattern(void *, char *, char *, int, char **, char **,
    int, int);
off_t position(int);
void add_forw_pos(off_t);
void add_back_pos(off_t);
void pos_clear(void);
void pos_init(void);
int onscreen(off_t);
int empty_screen(void);
int empty_lines(int, int);
void get_scrpos(struct scrpos *);
int adjsline(int);
void init_prompt(void);
char *pr_expand(const char *, int);
char *eq_message(void);
char *prompt_string(void);
char *wait_message(void);
void init_search(void);
void repaint_hilite(int);
void clear_attn(void);
void undo_search(void);
void clr_hilite(void);
int is_filtered(off_t);
int is_hilited(off_t, off_t, int, int *);
void chg_caseless(void);
void chg_hilite(void);
int search(int, char *, int);
void prep_hilite(off_t, off_t, int);
void set_filter_pattern(char *, int);
int is_filtering(void);
void sigwinch(int);
void init_signals(int);
void psignals(void);
void cleantags(void);
void findtag(char *);
off_t tagsearch(void);
char *nexttag(int);
char *prevtag(int);
int ntags(void);
int curr_tag(void);
int edit_tagfile(void);
void open_getchr(void);
int getchr(void);
void *lsignal(int, void (*)(int));
char *helpfile(void);
