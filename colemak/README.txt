
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
  Computer keyboard layout setting: the firmware only moves LETTERS (+ Angle + B key).
  Symbol/number keys are untouched, so they type whatever the computer's layout says.
  -> Set the computer to match the keycaps. My keyboard = Gov of Canada bilingual ISO
     (Canadian Multilingual Standard, CMS; ù on the extra ISO key), so CMS on the work computer
     = symbols match keycaps + French accents built in. Plain US also fine (letters OK, some symbols differ from keycaps).
  Note: with Angle, the extra ISO key (ù) now types Z, so ù on that key is gone.


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


Extend layer (hold Caps; key names = printed QWERTY keycaps)
-----------------------------------------------------------
  Left hand = modifiers + edit, right hand = navigation. Keep Caps held the whole time.

  Esc = Caps Lock           F1-F12 = media: F1 play  F2 prev  F3 next  F4 stop
                                            F5 mute  F6 vol-  F7 vol+  F8 media app
                                            F9 browser home  F10 search  F11 my computer  F12 calculator
  `   = DreymaR footer macro (ignore)   1..0 - = = F1..F12   Backspace = Pause

          Q Esc      W wheel up   E browser back  R browser fwd  T mouse up
          Y PgUp     U Home       I Up            O End          P Delete     [ Esc   ] Insert   \ Menu
          A Alt      S wheel down D Shift         F Ctrl         G mouse down
          H PgDn     J Left       K Down          L Right        ; Backspace  ' Menu  # browser favs  Enter PrintScreen
  ISO key Ctrl+Z     Z Ctrl+X     X Ctrl+C        C left click   V Ctrl+V     B wheel left
          N right click  M middle click  , mouse left  . mouse right  / wheel right
  Space = Enter

  Combos (all while holding Caps):
    D+J/L         select by character   (Shift + arrows)
    F+J/L         jump by word          (Ctrl + arrows)
    D+F+J/L       select by word
    F+U/O         start/end of document (Ctrl + Home/End)
    ISO Z X V     undo cut copy paste on the left hand (C = left click)


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

