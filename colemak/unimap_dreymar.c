/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 *  2016- "DreymaR" (Øystein Bech Gadmar; 2021- Øystein Bech-Aase)
 *  Colemak, Curl/Angle/Wide and Extend layouts, and more
 *  See DreymaR's Big Bag of Keyboard Tricks for info
 *  https://dreymar.colemak.org
 *  
 *  Compiler preprocessor definitions select layout, Extend layers and other options:
 *  - UNIMAPLAYOUT  # selects physical/ergonomic layout: ISO/ANSI, Angle/Wide ergo mods
 *  - ACTIVELAYOUT  # selects first/active layout: QWERTY, Colemak, etc
 *  - SECONDLAYOUT  # selects second/switch layout: QWERTY, Colemak, mirrored Colemak, etc
 *  - CURLMOD       # selects the Curl(DH) ergo mod (only affects Colemak/Tarmak layouts)
 *  - SYMBOLKEYS    # selects the Sym ergo mod and other SymbolKey stuff
 *  - CAPSBEHAVIOR  # selects CapsLock key behavior (Extend, BackSpace etc)
 *  - STICKYMODS    # selects modifier behavior: Shift and/or RCtrl become handy One-Shot Mods
 *  - ...             (see below for more)
 *  
 *  TODO:
 *  - Turn the ScrollLock LED on when the second layout is active? How?
 *  - Make an function for FnLA/FnRA to enable #Alt as Ext# modifiers?
 *      - It could also implement the Alt-as-extend-hold functionality (press #Alt+Extend, then keep either one down)
 *      - Could use EXTENDER with the opt parameter (0-3, bitwise logic) to determine Caps/LAlt/RAlt!?
 *      - Static (global?) "extAlt" variable that's set in EXTENDER if #Alt is down, and unset on #Alt release?
 *      - Or an extBit variable - a bit logic variable detecting Extend/Caps, Ext2/LAlt and Ext3/RAlt states?
 *      - Release the #Alt mod bit either way, but only register/unregister #Alt if Extend isn't down?
 *      - EXTENDER currently detects mod bits, so we'd have to switch to a variable?
 *  
 *  DONE:
 *  - Made a FOURLVLU user function that allows locale letter input with RAlt/AltGr on a few keys.
 *      - It uses an OS specific input method selected by #define.
 *  - Made an EXTENDER user function for the chorded Ext2+Ext3+Extend modifiers, by default on Caps.
 *      - Releasing the Extend key clears all mods & returns to the previously active layer (0 or 1).
 */

/* ***** SETTINGS ***************************************************************************************************** */

/*  Edit '_########' in the UNIMAPLAYOUT definiton below to choose ergonomic Curl/Angle/Wide keyboard mods:
 *  _NOMODS - Standard Unimap format for all keyboard and converter types (w/o F13-24; this one is without ergo mods)
 *  _ISO_A_ - ISO/Int Angle (the simple lower left half-row shift)
 *  _ISO_AW - ISO/Int Angle-Wide(/)
 *  _ANS_A_ - ANSI/US Angle(Z)
 *  _ANS_AW - ANSI/US Angle(Z)-Wide(')
 *  _AWINGA - ANSI/US A-Wing Angle (rarely used)
 *  
 *  Select an ergo modded keymap, or the plain unmodded Unimap. Note that these maps affect all layouts and layers.
 *  For Curl(DH), you also need to set CURLMOD. For Sym mods, set SYMBOLKEYS according to keymap.
 *  To get, say, the Colemak-CAWS (CurlAngleWideSym) layout on ISO/ANSI, use the _###_AW keymap with
 *      ACTIVELAYOUT 5, CURLMOD 1 and SYMBOLKEYS 2/3 settings below. And CAPSBEHAVIOR 1 for Extend, of course!
 */
#define UNIMAPLAYOUT(...)   UNIMAP_ANS_AW( __VA_ARGS__ )    /* AngleWide-ANSI keymap */

/* NOTE: These enumerations won't work for precompiler directives? Maybe they aren't needed, but they look nice. ;-)    */
enum mainlayouts    {
    LAY_QWERTY      ,   /* QWERTY mappings                          */
    LAY_TARMAK1     ,   /* Tarmak#, stepping stones to Colemak      */
    LAY_TARMAK2     ,   /* --"--                                    */
    LAY_TARMAK3     ,   /* --"--                                    */
    LAY_TARMAK4     ,   /* --"--                                    */
    LAY_COLEMAK     ,   /* Colemak!                                 */
    LAY_CMKMIRR     ,   /* (Mirrored Colemak; normally 2nd layout!) */
    LAY_DVORAK      ,   /* Dvorak, if you're retro                  */
    LAY_CANARY      ,   /* Canary, if you're avantgarde             */
    LAY_GRALMAK     };  /* Gralmak/Graphite, if you're avantgarder  */
/*  Define the ACTIVELAYOUT (and CURLMOD) constant(s) to choose the layer0 layout:
 *  0  : QWERTY
 *  1-#: Tarmak1 - transitional Colemak (supports CURLMOD; see below)
 *  2-#: Tarmak2 - transitional Colemak (--"--)
 *  3-#: Tarmak3 - transitional Colemak (--"--)
 *  4-#: Tarmak4 - transitional Colemak (--"--)
 *  5-0: Colemak
 *  5-1: Colemak Curl-DH (requires a CurlAngle keymap; see above)
 *  6-#: Mirrored Colemak (normally used as second layout with a layer switch)
 *  7  : Dvorak (only recommended if you already use it)
 *  8  : Canary (Colemak-like layout; changes more keys, less implemented)
 *  9  : Gralmak (A slightly conservative Graphite variant, keeping symbol keys and shift states unchanged)
 */
#define ACTIVELAYOUT    5                       /* LAY_COLEMAK      */

enum secondlayouts   {
    SEC_VANQWERTY    ,   /* QWERTY, vanilla/unmodded                */
    SEC_MODQWERTY    ,   /* QWERTY, ergo mods active                */
    SEC_ENHANCED     ,   /* Colemak (or edit to what you want)      */
    SEC_CMKMIRR      };  /* Mirrored Colemak (use Cmk as 1st)       */
/*  Define the SECONDLAYOUT (and CURLMOD) constant(s) to choose the layer1 switch layout:
 *  0  : Unmodded QWERTY is the default; otherwise:
 *  1  : QWERTY with any active ergo mods (AngleWide etc)
 *  2-#: Colemak (if you want something else, replace it in the code between the 'REPLACE THE SECOND LAYOUT...' lines)
 *  3-#: Colemak Mirrored as second layout for one-handed typing (needs an accessible switch key!)
 *      NOTE: The "FSLk" key is a layer1 toggle or switch (select which below), normally used on the ScrollLock key.
 *            You may swap, e.g., LALT, RGUI or another key with FSLk in your active layout to use that key instead.
 */
#define SECONDLAYOUT    0                       /* SEC_VANQWERTY    */

enum curlmods       {
    CURL_NONE       ,   /* No curl, plain vanilla Colemak/Tarmak    */
    CURL_DH         };  /* The Colemak/Tarmak-DH Curl mod           */
/*  The CURLMOD options for Colemak and Tarmak layouts are:
 *  0: No Curl - "vanilla" Colemak/Tarmak
 *  1: SteveP99's original Curl(DHm) ergo mod, bringing D/H down but M to the home row. DH standard since Oct 2020.
 * 
 *  N/A: The 2017 Curl(DHk) ergo mod, bringing the D and H keys down to comfortable bottom-row positions (QWERTY C/M)
 *      (Some row-staggered board users may like the DHk variant, but the DHm standard is good for all board types.)
 *  N/A: DreymaR's old Curl(DvH) mod, bringing DH center-down to the QWERTY V/N keys.
 *      (If you somehow wish to keep ZXCV together as in my old Curl(DvH) mod, edit the maps manually.)
 * 
 *  NOTE: On the first Tarmak step, the CURLMOD setting doesn't matter. If you want to swap H and M early, edit it in.
 *        For the other steps, CURLMOD still doesn't move H-M so Curl(DH) users by default will do H-M in the last step.
 *        An extra baby step after Tarmak1 could be transitioning to an Angle(Wide) keymap/model before Tarmak2.
 */
#define CURLMOD         1                       /* CURL_DH          */

enum symbolkeys     {
    SYM_NONE        ,   /* No Sym key changes                       */
    SYM_NONW        ,   /* Sym for non-Wide variants                */
    SYM_WISO        ,   /* Sym for Wide-ISO variants                */
    SYM_WANS        ,   /* Sym for Wide-ANSI variants               */
    SYM_4LVLUNI     ,   /* Some OEM keys give 4-level Unicode input */
    SYM_ISOHACK     };  /* Swap some OEM keys around for ISO-Nor    */
/*  The SYMBOLKEYS options for layouts are:
 *  0: No Symbol key changes - "vanilla" Colemak or whatever
 *  1: Sym mod for non-Wide ergo mods
 *  2: Sym mod for Wide mod combos on ISO keyboards
 *  3: Sym mod for Wide mod combos on ANSI keyboards
 *  
 *  Adaptations for ANSI-ISO and locale key differences (more info below):
 *  4  : Some keys are made four-level: AltGr+<key> sends Unicode glyphs (by OS specific input method)
 *  5  : DreymaR's ISO-Nor hack, moving some keys to make the Norwegian layout more like ANSI/US
 */
#define SYMBOLKEYS      3                       /* SYM_WANS         */

enum capsbehaviors  {
    CAPS_CAPS       ,   /* CapsLock as its plain old self           */
    CAPS_EXTEND     ,   /* CapsLock as Extend modifier              */
    CAPS_BACK       ,   /* CapsLock as Backspace                    */
    CAPS_LCTL       ,   /* CapsLock as LCtrl                        */
    CAPS_ESC        };  /* CapsLock as Escape                       */
/*  The CAPSBEHAVIOR constant chooses Caps key action, including the powerful Extend layer switch:
 *  0: CapsLock (unchanged)
 *  1: Extend modifier (uses a little more layout memory, but it's by far the most powerful option IMNSHO!)
 *  2: BackSpace (for vanilla Colemak or otherwise; a decent choice but Extend is better!)
 *  3: LCtrl
 *  4: Esc
 *  NOTE: To move modifiers, edit the layouts or the appropriate UNIMAP_### in my .h file (e.g., swapping CAPS and #ALT).
 *  NOTE: Depending on your keyboard's scan matrix(?), chorded Extend modifiers such as Ext1+S+T+N for Shift+Ctrl+Left
 *        may not work. With Caps=Ext1, I've had trouble with Ext1+S+T+N; with LAlt=Ext1 even Ext1+S+N didn't work!
 */
#define CAPSBEHAVIOR    1                       /* CAPS_EXTEND      */

/*  The EXT#BIT constants with the main Extend key (Caps by default) select Extend# layers in the EXTENDER user function:
 *  Ext1 on Caps alone       : Navigation/editing/multimedia
 *  Ext2 on Ext2+Caps        : NumPad/Navigation
 *  Ext3 on Ext3+Caps        : TODO
 *  Ext4 on Ext2+Ext3+Caps   : TODO
 *  NOTE: Alt keys as Extend# selectors failed, as AltUp activates Win menus even when the mod is turned off?!
 *      - It should be possible to write an Alt user function that doesn't release Alt when Ext has been pressed.
 */
#define EXT2BIT MOD_BIT(KC_RSFT)                /*  Ext2+Ext selects Extend2    */
#define EXT3BIT MOD_BIT(KC_RCTL)                /*  Ext3+Ext selects Extend3    */

enum oneshotmods    {
    STICKY_NONE     ,   /* No sticky/oneshot modifiers              */
    STICKY_SHFT     ,   /* Sticky LShift/RShift                     */
    STICKY_LCTL     ,   /* Sticky LCtrl (RCtrl left alone)          */
    STICKY_SHCT     };  /* Sticky Shift & LCtrl                     */
/*  The STICKYMODS constant chooses LShift, RShift and RCtrl key behavior (I chose to leave LCtrl alone):
 *  0: Normal Shift/Ctrl (default)
 *  1: Sticky Shift only
 *  2: Sticky Ctrl only
 *  3: Sticky Shift & Ctrl
 *  NOTE: In the unimap_dreymar.h file, some relevant constants normally set in config.h are (re)set:
 *      - ONESHOT_TIMEOUT   max delay (in ms) before a oneshot/sticky modifier is ignored
 *      - TAPPING_TERM      max time a key may be held down for it to register as tapped, not held
 */
#define STICKYMODS      1                       /* STICKY_SHFT      */

enum sclkbehaviors  {
    SLCK_SLCK       ,   /* ScrollLock as its old self               */
    SLCK_L1TOGGLE   ,   /* ScrollLock as Layer 1 toggle             */
    SLCK_L1SWITCH   };  /* ScrollLock as Layer 1 switch             */
/*  The SLCKBEHAVIOR constant chooses ScrollLock key action:
 *  0: Normal ScrollLock (default)
 *  1: Layer 1 toggle key (toggles the second layout)
 *  2: Layer 1 switch key (layer shift; e.g., for mirrored typing if you can use it as "ghetto" foot switch)
 */
#define SLCKBEHAVIOR    1                       /* SLCK_L1TOGGLE    */

enum pausbehaviors  {
    PAUS_PAUS       ,   /* Pause as its old self                    */
    PAUS_LGUI       };  /* Pause as LWin/LGUI                       */
/*  The PAUSBEHAVIOR constant chooses Pause/Break key action:
 *  0: Normal Pause/Break (default)
 *  1: Win/GUI key (useful for 101/104-key boards that have no GUI key)
 */
#define PAUSBEHAVIOR    0                       /* PAUS_PAUS        */

/*  UNICODEHEADER and UNICODEFOOTER handle OS dependency in the FOURLVLU user function, used by SYMBOLKEYS above.       */
/*  TODO: These headers haven't been tested on XOrg and MacOS. Also, TMK now has easy-to-use UNI[X|W|M]_() macros.      */
/*  For XOrg,    Unicode 4-digit hex input uses Ctrl+Shift+u, ####, Enter.                                              */
//#define UNICODEHEADER add_weak_mods( MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT) ); type_code( KC_U    ); /* Ctrl+Shift+U,    */
//#define UNICODEFOOTER del_weak_mods( MOD_BIT(KC_LCTL) | MOD_BIT(KC_LSFT) ); type_code( KC_ENT  ); /* Del_mods; Enter. */
            /*  <-- FOURLVLU Unicode header/footer for XOrg                                             */
/*  For MacOS,   Unicode 4-digit hex Unicode input uses Alt+#### (as Alt equals Mac Option).                            */
//#define UNICODEHEADER   add_weak_mods( MOD_BIT(KC_LALT) );            /*  Hold Option/LAlt.           */
//#define UNICODEFOOTER   del_weak_mods( MOD_BIT(KC_LALT) );            /*  Release Option/LAlt.        */
            /*  <-- FOURLVLU Unicode header/footer for MacOS                                            */
/*  For Windows, Unicode 4-digit decimal nr. input uses Alt+Keypad(####). It works with RAlt too on the US/ANSI layout. */
#define UNICODEHEADER   set_mods( MOD_BIT(KC_RALT) );                   /*  Keep just the RAlt bit,     */
#define UNICODEFOOTER   ;                                               /*  No footer necessary here.   */
            /*  <-- FOURLVLU Unicode header/footer for Windows                                          */

/* DreymaR's master key! Define it for a quick override. It gets compiler warnings due to redefinition; ignore these.   */
//#define MASTERKEY
#ifdef MASTERKEY
# define UNIMAPLAYOUT(...)  UNIMAP_ISO_AW( __VA_ARGS__ )    /* AngleWide-ISO keymap */
# define ACTIVELAYOUT   9                                   /* LAY_GRALMAK      */
//# define SECONDLAYOUT   0                                   /* SEC_VANQWERTY    */
# define CURLMOD        0                                   /* CURL_DH(m): off  */
# define SYMBOLKEYS     5                                   /* SYM_ISOHACK      */
//# define CAPSBEHAVIOR   1                                   /* CAPS_EXTEND      */
//# define STICKYMODS     1                                   /* STICKY_SHFT      */
//# define SLCKBEHAVIOR   1                                   /* SLCK_L1TOGGLE    */
//# define PAUSBEHAVIOR   0                                   /* PAUS_PAUS        */
#endif      /* ifdef MASTERKEY */

/* ***** DECLARATIONS ************************************************************************************************* */

/* The UNIMAP keymap (new universal 128-key format w/o the Korean and rare rightmost keys) is used for these files:     */
#include "unimap_trans.h"
#include "unimap_dreymar.h"

/* *******  User function and macro declarations/enumerations -->       ******* */
enum function_id {
    FOURLVLU,
    EXTENDER
};

enum macro_id {
    TYPESTR1,
    TYPESTR2
};    /* *******    <-- User function and macro declarations            ******* */

/* *******  Fn action key definitions (Unimap style) -->                ******* */
/*  Some symbol keys are defined as special 'F' codes in this file, facilitating ACTION_KEY mapping:
 *  GRV ,  1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  7 ,  8 ,  9 ,  0 ,FMin,FEql, -- ,BSPC
 *  TAB   ,  Q ,  W ,  E ,  R ,  T ,  Y ,  U ,  I ,  O ,  P ,FLBr,FRBr,   FBsl
 *  FCap   ,  A ,  S ,  D ,  F ,  G ,  H ,  J ,  K ,  L ,FScl,FQuo,FHsh,  ENT 
 *  FLSh ,FLgt,  Z ,  X ,  C ,  V ,  B ,  N ,  M ,COMM,DOT ,FSls, -- ,    FRSh
 *  LCTL ,LGUI,FnLA, -- ,         SPC          , -- , -- ,FnRA,RGUI,APP , FRCt  */
/*  NOTE: The BSLS/FBsl (ANSI) and NUHS/FHsh (ISO) key have, to my knowledge, the same scan code (Windows SC02b).       *
 *      - In the US layouts and most others, it's mapped to \ (Backslash) but its Shift-mappings vary. UK has # (Hash). *
 *      - Since TMK uses two names for it, I've used BSLS for ANSI and NUHS for ISO mappings. Shouldn't matter though?  */
#define AC_FnLA ACTION_KEY(KC_LALT)                 // FnLA (LAlt key) unchanged for now
#define AC_FnRA ACTION_KEY(KC_RALT)                 // FnRA (RAlt key) unchanged for now
#if     CAPSBEHAVIOR == 1
# define AC_FCap ACTION_FUNCTION(EXTENDER)          // FCap selects Extend# depending on which Ext# keys are pressed too.
//# define AC_FnU1 ACTION_MODS_KEY(MOD_LGUI,KC_T)      // FnU1 (`) as Win+T
# define AC_FnU1 ACTION_MACRO(TYPESTR1)             // FnU1 as user macro: Type a string
# define AC_FnU2 ACTION_MACRO(TYPESTR2)             // FnU2 as user macro: Type a string
#elif   CAPSBEHAVIOR == 2
# define AC_FCap ACTION_KEY(KC_BSPC)                // FCap (Caps key) as BackSpace (for Colemak etc)
#elif   CAPSBEHAVIOR == 3
# define AC_FCap ACTION_KEY(KC_LCTL)                // FCap (Caps key) as LeftCtrl
#elif   CAPSBEHAVIOR == 4
# define AC_FCap ACTION_KEY(KC_ESC)                 // FCap (Caps key) as Esc
#else
# define AC_FCap ACTION_KEY(KC_CAPS)                // FCap (Caps key) unchanged
#endif      /* if CAPSBEHAVIOR */

#if     STICKYMODS == 1
# define AC_FLSh ACTION_MODS_ONESHOT(MOD_LSFT)      // FLSh (Left Shift key)  as sticky shift
# define AC_FRSh ACTION_MODS_ONESHOT(MOD_RSFT)      // FRSh (Right Shift key) as sticky shift
# define AC_FRCt ACTION_KEY(KC_RCTL)                // FRCt (Right Ctrl key)  unchanged
#elif   STICKYMODS == 2
# define AC_FLSh ACTION_KEY(KC_LSFT)                // FLSh (Left Shift key)  unchanged
# define AC_FRSh ACTION_KEY(KC_RSFT)                // FRSh (Right Shift key) unchanged
# define AC_FRCt ACTION_MODS_ONESHOT(MOD_RCTL)      // FRCt (Right Ctrl key)  as sticky control
#elif   STICKYMODS == 3
# define AC_FLSh ACTION_MODS_ONESHOT(MOD_LSFT)      // FLSh (Left Shift key)  as sticky shift
# define AC_FRSh ACTION_MODS_ONESHOT(MOD_RSFT)      // FRSh (Right Shift key) as sticky shift
# define AC_FRCt ACTION_MODS_ONESHOT(MOD_RCTL)      // FRCt (Right Ctrl key)  as sticky control
#else
# define AC_FLSh ACTION_KEY(KC_LSFT)                // FLSh (Left Shift key)  unchanged
# define AC_FRSh ACTION_KEY(KC_RSFT)                // FRSh (Right Shift key) unchanged
# define AC_FRCt ACTION_KEY(KC_RCTL)                // FRCt (Right Ctrl key)  unchanged
#endif      /* if STICKYMODS */

#if     SLCKBEHAVIOR == 1
# define AC_FSLk ACTION_LAYER_TOGGLE(1)             // FSLk as layer1 (second layout) toggle
#elif   SLCKBEHAVIOR == 2
# define AC_FSLk ACTION_LAYER_MOMENTARY(1)          // FSLk as layer1 switch (e.g., for mirrored typing)
#else
# define AC_FSLk ACTION_KEY(KC_SLCK)                // FSLk (ScrollLock key) unchanged
#endif      /* if SLCKBEHAVIOR */

#if     PAUSBEHAVIOR == 1
# define AC_FPau ACTION_KEY(KC_LGUI)                // Fpau as GUI/Win (for 101/104-key boards)
#else
# define AC_FPau ACTION_KEY(KC_PAUS)                // FPau (Pause/Break key) unchanged
#endif      /* if PAUSBEHAVIOR */

/*  Extend2 uses parentheses and colon - these definitions provide those symbols                                        */
# define AC_FsLP ACTION_MODS_KEY(MOD_LSFT, KC_9)    // FsLP is Shift+9 (left  parenthesis, US)
# define AC_FsRP ACTION_MODS_KEY(MOD_LSFT, KC_0)    // FsRP is Shift+0 (right parenthesis, US)
# define AC_FsSC ACTION_MODS_KEY(MOD_LSFT, KC_SCLN) // FsSC is Shift+Semicolon (colon, US)

/*    Common symbol key definitions - these may get redefin