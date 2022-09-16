/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const int gappx              = 15;       /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Literation Mono Nerd Font:pixelsize=15:antialias=true:autohint=true" };
static const char dmenufont[]       = "Literation Mono Nerd Font:pixelsize=15:antialias=true:autohint=true";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char barfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*                    fg               bg               border   */
       [SchemeNorm]      = { normfgcolor,     normbgcolor,     normbordercolor },
       [SchemeBar]       = { barfgcolor,      normbgcolor,     normbordercolor },
       [SchemeStackNorm] = { normfgcolor,     normbgcolor,     selbordercolor },
       [SchemeSel]       = { selfgcolor,      selbgcolor,      selbordercolor  },
       [SchemeStackSel]  = { normbgcolor,     selbordercolor,  selbordercolor  },
};

/* tagging */
static const char *alttags[] = { "", "", "", "", "♫", "", "", "", "", "" };
static const char *tags[]    = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ NULL,       NULL,       NULL,       0,            False,       -1 },
};

/* layout(s) */
static const float mfact     = 0.60; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define AltMask Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/zsh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static const char *dmenucmd[]       = { "dmenu_run", "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]        = { "st", NULL };
static const char *explorercmd[]    = { "st", "-e", "lfub" , NULL };
static const char *kbdlightup[]     = { "brightnessctl", "-d", "asus::kbd_backlight", "set", "+1", NULL };
static const char *kbdlightdown[]   = { "brightnessctl", "-d", "asus::kbd_backlight", "set", "1-", NULL };
static const char *monlightup[]     = { "xbacklight", "-inc", "5", NULL };
static const char *monlightdown[]   = { "xbacklight", "-dec", "5", NULL };
static const char *musicplayer[]    = { "st", "-e", "ncmpcpp", NULL };
static const char *musicnext[]      = { "/bin/zsh", "-c", "MUSIC_COMMAND=next   /usr/local/bin/scripts/dwm/music", NULL };
static const char *musicprev[]      = { "/bin/zsh", "-c", "MUSIC_COMMAND=prev   /usr/local/bin/scripts/dwm/music", NULL };
static const char *musictoggle[]    = { "/bin/zsh", "-c", "MUSIC_COMMAND=toggle /usr/local/bin/scripts/dwm/music", NULL };
static const char *musicstop[]      = { "/bin/zsh", "-c", "MUSIC_COMMAND=stop   /usr/local/bin/scripts/dwm/music", NULL };
static const char *musicvolinc[]    = { "/bin/zsh", "-c", "MUSIC_COMMAND=volinc /usr/local/bin/scripts/dwm/music", NULL };
static const char *musicvoldec[]    = { "/bin/zsh", "-c", "MUSIC_COMMAND=voldec /usr/local/bin/scripts/dwm/music", NULL };
static const char *lowervolume[]    = { "/bin/zsh", "-c", "VOLUME_COMMAND=dec /usr/local/bin/scripts/dwm/volume" , NULL };
static const char *raisevolume[]    = { "/bin/zsh", "-c", "VOLUME_COMMAND=inc /usr/local/bin/scripts/dwm/volume" , NULL };
static const char *togglevolume[]   = { "/bin/zsh", "-c", "VOLUME_COMMAND=toggle /usr/local/bin/scripts/dwm/volume", NULL };
static const char *lockcmd[]        = { "/bin/zsh", "/usr/local/bin/scripts/dwm/lock", NULL };
static const char *runcmd[]         = { "/bin/zsh", "/usr/local/bin/scripts/dwm/run" , NULL };
static const char *powercmd[]       = { "/bin/zsh", "/usr/local/bin/scripts/dwm/shutdown_options" , NULL };
static const char *toggletouchpad[] = { "/bin/zsh", "/usr/local/bin/scripts/dwm/toggle_touchpad", NULL };
static const char *getlink[]        = { "/bin/zsh", "/usr/local/bin/scripts/dwm/getlink", NULL };
static const char *selinternet[]    = { "/bin/zsh", "/usr/local/bin/scripts/dwm/select_internet", NULL };
static const char *selpowerplan[]   = { "/bin/zsh", "/usr/local/bin/scripts/dwm/select_powerplan", NULL };
static const char *checkinternet[]  = { "/bin/zsh", "/usr/local/bin/scripts/dwm/check_internet", NULL };
static const char *automount[]      = { "/bin/zsh", "/usr/local/bin/scripts/dwm/automount", NULL };
static const char *autounmount[]    = { "/bin/zsh", "/usr/local/bin/scripts/dwm/autounmount", NULL };
static const char *scrot[]          = { "/bin/zsh", "/usr/local/bin/scripts/dwm/scrot", "full", "toclipboard", NULL };
static const char *scrotsel[]       = { "/bin/zsh", "/usr/local/bin/scripts/dwm/scrot", "sel",  "toclipboard", NULL };
static const char *scrotsave[]      = { "/bin/zsh", "/usr/local/bin/scripts/dwm/scrot", "full", NULL };
static const char *scrotselsave[]   = { "/bin/zsh", "/usr/local/bin/scripts/dwm/scrot", "sel",  NULL };
static const char *tweet[]          = { "/bin/zsh", "/usr/local/bin/scripts/dwm/tweet",  NULL };
static const char *themegen[]       = { "/bin/zsh", "/usr/local/bin/scripts/dwm/theme",  NULL };
static const char scratchpadname[]  = "scratchpad";
static const char *scratchpadcmd[]  = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,           spawn,          {.v = runcmd } },
	{ MODKEY|ShiftMask,             XK_r,           spawn,          {.v = powercmd } },
	{ MODKEY,                       XK_Return,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_Escape,      spawn,          {.v = lockcmd } },
	{ MODKEY,                       XK_e,           spawn,          {.v = explorercmd } },
	{ MODKEY,                       XK_g,           spawn,          {.v = toggletouchpad } },
	{ MODKEY,                       XK_p,           spawn,          {.v = musictoggle } },
	{ MODKEY,                       XK_u,           spawn,          {.v = musicvoldec } },
	{ MODKEY,                       XK_i,           spawn,          {.v = musicvolinc } },
	{ MODKEY,                       XK_n,           spawn,          {.v = getlink } },
	{ MODKEY|ControlMask,           XK_p,           spawn,          {.v = musicplayer } },
	{ MODKEY|ControlMask,           XK_t,           spawn,          {.v = tweet } },
	{ MODKEY|ControlMask,           XK_s,           spawn,          {.v = scrot } },
	{ MODKEY|AltMask,               XK_s,           spawn,          {.v = scrotsel } },
	{ MODKEY|ControlMask|ShiftMask, XK_s,           spawn,          {.v = scrotsave } },
	{ MODKEY|AltMask|ShiftMask,     XK_s,           spawn,          {.v = scrotselsave } },
	{ MODKEY|ShiftMask,             XK_p,           spawn,          {.v = musicplayer } },
	{ MODKEY|ShiftMask,             XK_o,           spawn,          {.v = musicnext } },
	{ MODKEY|ShiftMask,             XK_m,           spawn,          {.v = musicstop } },
	{ MODKEY|ShiftMask,             XK_y,           spawn,          {.v = musicprev } },
	{ MODKEY|ShiftMask,             XK_n,           spawn,          {.v = selinternet } },
	{ MODKEY|ShiftMask,             XK_x,           spawn,          {.v = selpowerplan } },
	{ MODKEY|ControlMask,           XK_m,           spawn,          {.v = automount } },
	{ MODKEY|ControlMask|ShiftMask, XK_m,           spawn,          {.v = autounmount } },
	{ MODKEY|ControlMask|ShiftMask, XK_n,           spawn,          {.v = checkinternet } },
	{ MODKEY,                       XK_w,           togglebar,      {0} },
 	{ MODKEY,                       XK_j,           focusstack,     {.i = +1 } },
 	{ MODKEY,                       XK_l,           focusstack,     {.i = +1 } },
 	{ MODKEY,                       XK_s,           focusstack,     {.i = +1 } },
 	{ MODKEY,                       XK_f,           focusstack,     {.i = +1 } },
 	{ MODKEY,                       XK_k,           focusstack,     {.i = -1 } },
 	{ MODKEY,                       XK_d,           focusstack,     {.i = -1 } },
 	{ MODKEY,                       XK_q,           focusstack,     {.i = -1 } },
 	{ MODKEY,                       XK_h,           focusstack,     {.i = -1 } },
 	{ MODKEY,                       XK_Left,        focusstack,     {.i = -1 } },
 	{ MODKEY,                       XK_Up,          focusstack,     {.i = -1 } },
 	{ MODKEY,                       XK_Right,       focusstack,     {.i = +1 } },
 	{ MODKEY,                       XK_Down,        focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_c,           incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_v,           incnmaster,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_h,           setmfact,       {.f = -0.01} },
	{ MODKEY|ControlMask,           XK_Left,        setmfact,       {.f = -0.01} },
	{ MODKEY|ControlMask,           XK_Up,          incnmaster,     {.i = +1} },
	{ MODKEY|ControlMask,           XK_k,           incnmaster,     {.i = +1} },
	{ MODKEY|ControlMask,           XK_d,           incnmaster,     {.i = +1} },
	{ MODKEY|ControlMask,           XK_Down,        incnmaster,     {.i = -1} },
	{ MODKEY|ControlMask,           XK_j,           incnmaster,     {.i = -1} },
	{ MODKEY|ControlMask,           XK_s,           incnmaster,     {.i = -1} },
	{ MODKEY|ControlMask,           XK_Right,       setmfact,       {.f = +0.01} },
	{ MODKEY|ControlMask,           XK_l,           setmfact,       {.f = +0.01} },
	{ MODKEY,                       XK_space,       zoom,           {0} },
	{ MODKEY,                       XK_Tab,         shiftview,      {.i = ShiftCycle } },
	{ MODKEY|ShiftMask,             XK_Tab,         shiftview,      {.i = ShiftCycle | ShiftLeft } },
	{ MODKEY|ShiftMask,             XK_a,           killclient,     {0} },
	{ MODKEY,                       XK_t,           setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_x,           setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_z,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_Return,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
	{ MODKEY|AltMask,               XK_Tab,         focusmon,       {.i = +1 } },
	{ MODKEY|AltMask|ShiftMask,     XK_Tab,         focusmon,       {.i = -1 } },
	{ MODKEY|ControlMask,           XK_Tab,         tagmon,         {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_Tab,         tagmon,         {.i = -1 } },
	{ MODKEY,                       XK_F5,          xrdb,           {.v = NULL } },
	{ MODKEY,                       XK_F5,          spawn,          {.v = themegen } },
 	{ MODKEY,                       XK_KP_Subtract, setgaps,        {.i = -5 } },
 	{ MODKEY,                       XK_KP_Add,      setgaps,        {.i = +5 } },
 	{ MODKEY|ShiftMask,             XK_equal,       setgaps,        {.i = 0  } },
	{ MODKEY,                       XK_twosuperior, togglescratch,  {.v = scratchpadcmd } },
	TAGKEYS(                        XK_ampersand,                   0)
	TAGKEYS(                        XK_eacute,                      1)
	TAGKEYS(                        XK_quotedbl,                    2)
	TAGKEYS(                        XK_apostrophe,                  3)
	TAGKEYS(                        XK_parenleft,                   4)
	TAGKEYS(                        XK_minus,                       5)
	TAGKEYS(                        XK_egrave,                      6)
	TAGKEYS(                        XK_underscore,                  7)
	TAGKEYS(                        XK_ccedilla,                    8)
	TAGKEYS(                        XK_agrave,                      9)
  {0, XF86XK_AudioRaiseVolume,  spawn, {.v = raisevolume } },
  {0, XF86XK_AudioLowerVolume,  spawn, {.v = lowervolume } },
  {0, XF86XK_AudioMute,         spawn, {.v = togglevolume } },
  {0, XF86XK_MonBrightnessUp,   spawn, {.v = monlightup } },
  {0, XF86XK_KbdBrightnessUp,   spawn, {.v = kbdlightup } },
  {0, XF86XK_MonBrightnessDown, spawn, {.v = monlightdown } },
  {0, XF86XK_KbdBrightnessDown, spawn, {.v = kbdlightdown } },
  {0, XF86XK_TouchpadToggle,    spawn, {.v = toggletouchpad } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,                Button1,        setlayout,       {0} },
	{ ClkLtSymbol,          0,                Button3,        setlayout,       {.v = &layouts[2]} },
	{ ClkWinTitle,          0,                Button1,        togglewin,       {0} },
 	{ ClkWinTitle,          0,                Button5,        focusstackbound, {.i = +1 } },
 	{ ClkWinTitle,          0,                Button4,        focusstackbound, {.i = -1 } },
	{ ClkWinTitle,          0,                Button2,        togglewin,       {0} },
	{ ClkWinTitle,          0,                Button2,        killclient,      {0} },
	{ ClkClientWin,         MODKEY,           Button1,        movemouse,       {0} },
	{ ClkClientWin,         MODKEY,           Button2,        killclient,      {0} },
	{ ClkClientWin,         MODKEY,           Button3,        resizemouse,     {0} },
	{ ClkClientWin,         MODKEY|ShiftMask, Button3,        togglefloating,  {0} },
	{ ClkTagBar,            0,                Button1,        view,            {0} },
	{ ClkTagBar,            0,                Button3,        toggleview,      {0} },
	{ ClkTagBar,            MODKEY,           Button1,        tag,             {0} },
	{ ClkTagBar,            MODKEY,           Button3,        toggletag,       {0} },
	{ ClkTagBar,            0,                Button5,        shiftview,       {.i = ShiftNoCycle } },
	{ ClkTagBar,            0,                Button4,        shiftview,       {.i = ShiftNoCycle | ShiftLeft } },
	{ ClkStatusText,        0,                Button1,        sigdwmblocks,    {.i = 1 } },
	{ ClkStatusText,        0,                Button2,        sigdwmblocks,    {.i = 2 } },
	{ ClkStatusText,        0,                Button3,        sigdwmblocks,    {.i = 3 } },
	{ ClkStatusText,        0,                Button4,        sigdwmblocks,    {.i = 4 } },
	{ ClkStatusText,        0,                Button5,        sigdwmblocks,    {.i = 5 } },
};
