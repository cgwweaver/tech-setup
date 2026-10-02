
Mac: flash the prebuilt .hex (no compiling needed on the Mac)
-------------------------------------------------------------

Files in this folder:
  usb_usb_cmk-dh-iso-angle.hex     <- MY firmware (Colemak-DH, ISO Angle, no Wide)
  usb_usb_stock-qwerty-rescue.hex  <- Hasu's stock firmware (plain QWERTY) = undo button


Rebuild (only if unimap_dreymar.c changes; needs avr-gcc, e.g. Linux: apt install gcc-avr avr-libc binutils-avr):
  git clone --recurse-submodules https://github.com/tmk/tmk_keyboard.git
  cp colemak/unimap_dreymar.c tmk_keyboard/converter/usb_usb/
  curl -L -o tmk_keyboard/converter/usb_usb/unimap_dreymar.h \
    https://raw.githubusercontent.com/DreymaR/BigBagKbdTrixTMK/master/_myunimaps/unimap_dreymar.h
  cd tmk_keyboard/converter/usb_usb
  make -f Makefile.unimap KEYMAP=dreymar CONSOLE_ENABLE=no
  -> output: usb_usb_unimap_dreymar.hex
  cp usb_usb_unimap_dreymar.hex ../../../colemak/usb_usb_cmk-dh-iso-angle.hex   # replace the prebuilt one
  cd ../../..                                                                   # back to repo root
  (run all of the above from the tech-setup repo root; don't commit tmk_keyboard/)
  (CONSOLE_ENABLE=no is REQUIRED - otherwise too big for the chip)


Option A - QMK Toolbox (GUI, easiest)
  1. Install: https://github.com/qmk/qmk_toolbox/releases (.pkg) or `brew install --cask qmk-toolbox`
  2. Open QMK Toolbox -> "Open" -> pick usb_usb_cmk-dh-iso-angle.hex. MCU = atmega32u4.
  3. Plug Hasu into the Mac (keyboard unplugged from it is safest).
  4. Press the small button on the Hasu -> LED goes off = bootloader mode.
     Toolbox log should say "Atmel DFU device connected".
  5. Click "Flash". Wait for "Flash complete". Replug the Hasu.

Option B - Terminal
  brew install dfu-programmer
  (from the tech-setup repo root; press the Hasu button first)
  dfu-programmer atmega32u4 erase --force
  dfu-programmer atmega32u4 flash colemak/usb_usb_cmk-dh-iso-angle.hex
  dfu-programmer atmega32u4 reset


Test on the Mac before taking it to work:
  keyboard -> Hasu -> computer.
  Firmware only moves LETTERS (+Angle, B key, Extend). Symbols/numbers/punctuation come from the
  computer's keyboard-layout setting.
  PLAN: set the computer to plain US (English-US, or ABC on Mac). Punctuation then lands where
        US keyboards (Dell USA etc) put it. This old keyboard (Gov of Canada bilingual ISO, pictogram
        nav/numpad keys) has different symbols printed on some keycaps -> ignore those, touch-type.
  OPTION: Canadian Multilingual Standard (CMS) on the computer if you want punctuation to match the
        keycaps (and French accents built in). Feels weird, per me. Not needed.
  Either way: with Angle the extra ISO key (CMS: ù) now types Z, so that key's own symbol is gone.


What I get
  - Letters: Colemak-DH. ISO Angle = bottom-left row shifted one key left
    (extra ISO key = Z, physical Z = X, X = C, C = D, V = V, physical B = Backspace)
  - Caps: tap = Backspace, hold = Extend (see Extend layer below)
    Tap must be released within 200ms to count as Backspace; longer = Extend.
    (TMK default. DreymaR's .h says 300 but that setting never reaches the code that uses it.
     To change: build with  OPT_DEFS="-DTAPPING_TERM=250" make -f Makefile.unimap ...)
    Holding Backspace to repeat: tap then quickly press-and-hold Caps, or hold physical B.
  - Caps Lock: Extend+Esc (hold Caps, tap Esc). Same again to turn off.
  - Shift is sticky: tap Shift, then a letter -> Capital
  - ScrollLock: toggles back to plain QWERTY (for colleagues / emergencies); tap again to return
  - Extend+` types DreymaR's own email footer macro (leftover demo; edit/remove TYPESTR1 if wanted).
    Its special character uses a Windows-only Alt+numpad code, so on a Mac that bit comes out wrong. Ignore it.


Extend layer (hold Caps)
------------------------
  key = keycap printed (QWERTY)   dh = what the key types normally (Colemak-DH, US layout)
  ext = what it does while Caps is held
  Bs/E = tap Backspace, hold Extend    (E) = the Extend key itself    Shft = sticky shift
  w = wheel, m = mouse move, Clk/Rclk/Mclk = left/right/middle click, ^ = Ctrl+
  Mac = DreymaR email macro (ignore)    Caps = Caps Lock    Med/Web/Find/PC/Calc = media/browser/calc keys
  Space = Enter. Others as normal. ScrLk = toggle QWERTY, PrSc/Paus as printed.

key Esc  F1   F2   F3   F4   F5   F6   F7   F8   F9   F10  F11  F12
ext Caps Play Prev Next Stop Mute Vol- Vol+ Med  Web  Find PC   Calc

key `    1    2    3    4    5    6    7    8    9    0    -    =    Bks
ext Mac  F1   F2   F3   F4   F5   F6   F7   F8   F9   F10  F11  F12  Paus

key Tab  Q    W    E    R    T    | Y    U    I    O    P    [    ]    \
dh  Tab  Q    W    F    P    B    | J    L    U    Y    ;    [    ]    \
ext Tab  Esc  wUp  Back Fwd  mUp  | PgUp Home Up   End  Del  Esc  Ins  Menu

key Caps A    S    D    F    G    | H    J    K    L    ;    '    #    Ent
dh  Bs/E A    R    S    T    G    | M    N    E    I    O    '    \    Ent
ext (E)  Alt  wDn  Shft Ctrl mDn  | PgDn Left Down Rght Bks  Menu Fav  PrSc

key Shft iso  Z    X    C    V    B    | N    M    ,    .    /    Shft
dh  Shft Z    X    C    D    V    Bs   | K    H    ,    .    /    Shft
ext Shft ^Z   ^X   ^C   Clk  ^V   wLf  | Rclk Mclk mLf  mRt  wRt  Shft

  Mnemonic with the DH letters (the letters you actually type):
    arrows  N E I = Left Down Right,  U = Up        (inverted T on the right hand)
    L / Y   = Home / End,  M / J = PgDn / PgUp
    Alt A   Shift S   Ctrl T          (left home row = A R S T)
    Z X C V = Undo Cut Copy Paste     (same letters, with Ctrl)
    O = Backspace,  ; = Delete        (the key where you type ;)
  Combos (all while holding Caps):
    S + N/I   select by char    (Shift + arrows)
    T + N/I   jump by word      (Ctrl + arrows)
    S+T + N/I select by word
    T + L/Y   doc start / end   (Ctrl + Home/End)
    Esc       Caps Lock on/off


==unsure if want:
shift: normal hold works too?
caps-hold: want this as backspace too?

scrollock=QWERTY: good for now/getting back into Colemak.
  later might want turn on/off extend behaviour?

physical B key: want anything else instead?
Caps+`: want anything else instead?


might want:

Fr accents, might occ type french or Quebecois colleagues
names' François andré Hélène etc not many à or ô in QC names?
More é bit of ç and tiny bit è??

(Or shortcut rstudio things?)
Esc - not possible shortcut bc not text entering?!
%>% or better %.>%?? or over . prefer \(.) or d, .d?
another |> or <-? or is.na??
[]{}~.,
You rank

turn on CAPS how? (so rarely rarely need though...
  except occasionally SAS variable names?)
  liked out Shift+Caps...



if ever want to see/compare to Dreymar's,
see tag in this repo: "Dreymar"


https://github.com/DreymaR/BigBagKbdTrixTMK

only very few .hex prebuilt Dreymar

ease: linux > mac > windows


Pulled unimap_dreymar.c on 2026-Apr-11:
https://github.com/DreymaR/BigBagKbdTrixTMK/blob/master/_myunimaps%2Funimap_dreymar.c

