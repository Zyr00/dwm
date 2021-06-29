/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

#define TERMINAL "alacritty"
#define GAPPS 5

/* appearance */
static const unsigned int borderpx       = 2;     /* border pixel of windows */
static const unsigned int gappx          = GAPPS; /* gaps between windows */
static const unsigned int snap           = 32;    /* snap pixel */
static const int swallowfloating         = 0;     /* 1 means swallow floating windows by default */
static const int showbar                 = 1;     /* 0 means no bar */
static const int topbar                  = 1;     /* 0 means bottom bar */
static const char *fonts[]               = { "Fira Code:size=8" };
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { "#f3f4f5", "#223345", "#223345" },
	[SchemeSel]  = { "#f3f4f5", "#333399", "#5353c6" },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance         title            tags mask   isfloating  isterminal  noswallow  monitor */
	{ "Firefox",      "Toolkit",       NULL,            0,          1,          0,           0,        -1 },
        { "discord",      NULL,            NULL,            1 << 8,     0,          0,          -1,        -1 },
        { "Steam",        NULL,            NULL,            1 << 7,     0,          0,          -1,        -1 },
        { "st",           NULL,            NULL,            0,          0,          1,          -1,        -1 },
        { "Alacritty",    NULL,            NULL,            0,          0,          1,          -1,        -1 },
        { "Gnuplot",      NULL,            NULL,            0,          0,          1,           1,        -1 },
        { NULL,           NULL,           "Event Tester",   0,          1,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
        { NULL ,      NULL },
};

/* key definitions */
#define V_MUTE XF86XK_AudioMute
#define V_UP XF86XK_AudioRaiseVolume
#define V_DOWN XF86XK_AudioLowerVolume
#define B_UP XF86XK_MonBrightnessUp
#define B_DOWN XF86XK_MonBrightnessDown
#define MOUSE XF86XK_TouchpadToggle

#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

static const char *flameshot[] = { "flameshot", "gui", NULL };
static const char *lockcmd[] = { "i3lockrc", NULL };
static const char *locksuspendcmd[] = { "suspend", NULL };
static const char *touchpadtoggle[] = { "touchpadtoggle", NULL };

static const char *volmutecmd[] = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *volupcmd[] = { "amixer", "-q", "sset", "Master", "5%+", "unmute", NULL };
static const char *voldowncmd[] = { "amixer", "-q", "sset", "Master", "5%-", "unmute", NULL };

static const char *brupcmd[] = { "xbacklight", "-inc", "10", NULL };
static const char *brdowncmd[] = { "xbacklight", "-dec", "10", NULL };

static const char *firefox[] = { "firefox", NULL };
static const char *discord[] = { "discord", NULL };
static const char *steam[]   = { "steam", NULL };

static Key keys[] = {
	/* modifier                     key            function        argument */
	{ MODKEY,                       XK_p,          spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,     spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,          togglebar,      {0} },
	{ MODKEY,                       XK_j,          focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,          focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,          incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,          incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,          setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,          setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,     zoom,           {0} },
	{ MODKEY,                       XK_Tab,        view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,          killclient,     {0} },
	{ MODKEY,                       XK_space,      setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,      togglefloating, {0} },
	{ MODKEY,                       XK_0,          view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,          tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,     focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,     tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,      setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_plus,       setgaps,        {.i = +1 } },
	{ MODKEY,                       XK_ccedilla,   setgaps,        {.i = 0 } },
	{ MODKEY,                       XK_masculine,  setgaps,        {.i = GAPPS } },
        { MODKEY|ShiftMask,             XK_f,          spawn,          {.v = firefox } },
        { MODKEY|ShiftMask,             XK_d,          spawn,          {.v = discord } },
        { MODKEY|ShiftMask,             XK_s,          spawn,          {.v = steam } },
        { MODKEY|ShiftMask,             XK_l,          spawn,          {.v = lockcmd } },
        { MODKEY|ControlMask,           XK_l,          spawn,          {.v = locksuspendcmd } },
        { 0,                            MOUSE,         spawn,          {.v = touchpadtoggle } },
        { 0,                            B_UP,          spawn,          {.v = brupcmd } },
        { 0,                            B_DOWN,        spawn,          {.v = brdowncmd } },
        { 0,                            V_MUTE,        spawn,          {.v = volmutecmd } },
        { 0,                            V_UP,          spawn,          {.v = volupcmd } },
        { 0,                            V_DOWN,        spawn,          {.v = voldowncmd} },
        { 0,                            XK_Print,      spawn,          {.v = flameshot } },
	TAGKEYS(                        XK_1,                          0)
	TAGKEYS(                        XK_2,                          1)
	TAGKEYS(                        XK_3,                          2)
	TAGKEYS(                        XK_4,                          3)
	TAGKEYS(                        XK_5,                          4)
	TAGKEYS(                        XK_6,                          5)
	TAGKEYS(                        XK_7,                          6)
	TAGKEYS(                        XK_8,                          7)
	TAGKEYS(                        XK_9,                          8)
	{ MODKEY|ShiftMask,             XK_q,          quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

