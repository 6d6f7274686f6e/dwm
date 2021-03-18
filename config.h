/* See LICENSE file for copyright and license details. */

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
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*                    fg               bg               border   */
       [SchemeNorm]      = { normfgcolor,     normbgcolor,     normbordercolor },
       [SchemeStackNorm] = { normfgcolor,     normbgcolor,     selbordercolor },
       [SchemeSel]       = { selfgcolor,      selbgcolor,      selbordercolor  },
       [SchemeStackSel]  = { normbgcolor,     selbordercolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1:", "2:", "3:", "4:", "5:♫", "6:", "7:", "8:", "9:", "10:" };
// static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };

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

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/zsh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_r,           spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,      spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,           togglebar,      {0} },
 	{ MODKEY,                       XK_j,           focusstack,     {.i = +1 } },
 	{ MODKEY,                       XK_k,           focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,           incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,           incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,           setmfact,       {.f = -0.01} },
	{ MODKEY,                       XK_l,           setmfact,       {.f = +0.01} },
	{ MODKEY,                       XK_space,       zoom,           {0} },
	{ MODKEY,                       XK_Tab,         shiftview,      {.i = (4 | 0 | 1) } },
	{ MODKEY|ShiftMask,             XK_Tab,         shiftview,      {.i = (4 | 2 | 1) } },
	{ MODKEY|ShiftMask,             XK_a,           killclient,     {0} },
	{ MODKEY,                       XK_t,           setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,           setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_z,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_Return,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
	{ MODKEY,                       XK_comma,       focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,       tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,      tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,          xrdb,           {.v = NULL } },
 	{ MODKEY,                       XK_KP_Subtract, setgaps,        {.i = -5 } },
 	{ MODKEY,                       XK_KP_Add,      setgaps,        {.i = +5 } },
 	{ MODKEY|ShiftMask,             XK_equal,       setgaps,        {.i = 0  } },
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
	{ MODKEY|ShiftMask,             XK_q,           quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button1,        togglewin,      {0} },
 	{ ClkWinTitle,          0,              Button5,        focusstack,     {.i = +1 } },
 	{ ClkWinTitle,          0,              Button4,        focusstack,     {.i = -1 } },
	{ ClkWinTitle,          0,              Button2,        togglewin,      {0} },
	{ ClkWinTitle,          0,              Button2,        killclient,     {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        killclient,     {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,            0,              Button5,        shiftview,      {.i = (0 | 0 | 1) } },
	{ ClkTagBar,            0,              Button4,        shiftview,      {.i = (0 | 2 | 1) } },
};
