#
# Dingux 2600 for the RetroFW
#
# by pingflood; 2018/2019
#

CHAINPREFIX := /opt/mipsel-linux-uclibc
CROSS_COMPILE := $(CHAINPREFIX)/usr/bin/mipsel-linux-

CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
STRIP = $(CROSS_COMPILE)strip

SYSROOT     := $(shell $(CC) --print-sysroot)
SDL_CONFIG = $(SYSROOT)/usr/bin/sdl-config

ATARI_VERSION=1.1.0

TARGET = ./dingux-2600/dingux-2600.dge
OBJS = ./src/gp2x_psp.o \
./src/cpudingux.o \
./src/Atari.o  \
./src/psp_main.o \
./src/psp_sdl.o \
./src/psp_kbd.o \
./src/psp_font.o \
./src/psp_menu.o \
./src/psp_joy.o \
./src/psp_danzeff.o \
./src/psp_menu_set.o \
./src/psp_menu_help.o \
./src/psp_menu_joy.o \
./src/psp_menu_kbd.o \
./src/psp_menu_cheat.o \
./src/psp_menu_list.o \
./src/psp_editor.o \
./src/psp_fmgr.o \
\
./src/emucore/Joystick.o \
./src/emucore/TIA.o \
./src/emucore/Settings.o \
./src/emucore/CartE0.o \
./src/emucore/Paddles.o \
./src/emucore/CartF8.o \
./src/emucore/Props.o \
./src/emucore/Switches.o \
./src/emucore/Event.o \
./src/emucore/Random.o \
./src/emucore/Device.o \
./src/emucore/Driving.o \
./src/emucore/Cart2K.o \
./src/emucore/Keyboard.o \
./src/emucore/Console.o \
./src/emucore/CartF6.o \
./src/emucore/M6532.o \
./src/emucore/PropsSet.o \
./src/emucore/CartUA.o \
./src/emucore/CartMC.o \
./src/emucore/CartFE.o \
./src/emucore/CartMB.o \
./src/emucore/CartF8SC.o \
./src/emucore/CartF6SC.o \
./src/emucore/Cart.o \
./src/emucore/CartF4SC.o \
./src/emucore/CartF4.o \
./src/emucore/CartE7.o \
./src/emucore/CartCV.o \
./src/emucore/FSNode.o \
./src/emucore/FrameBuffer.o \
./src/emucore/MD5.o \
./src/emucore/MediaSrc.o \
./src/emucore/MediaFactory.o \
./src/emucore/CartAR.o \
./src/emucore/CartFASC.o \
./src/emucore/Cart4K.o \
./src/emucore/Control.o \
./src/emucore/Cart3F.o \
./src/emucore/Cart3E.o \
./src/emucore/Booster.o \
./src/emucore/CartDPC.o \
./src/emucore/Deserializer.o \
./src/emucore/EventHandler.o \
./src/emucore/EventStreamer.o \
./src/emucore/Serializer.o \
./src/emucore/M6502.o \
./src/emucore/M6502Hi.o \
./src/emucore/M6502Low.o \
./src/emucore/NullDev.o \
./src/emucore/System.o \
./src/emucore/OSystem.o \
\
./src/emucore/unzip.o \
./src/emucore/TIASnd.o \
\
./src/common/FrameBufferSoft.o \
./src/common/mainSDL.o \
./src/common/SoundSDL.o \
\
./src/psp/OSystemPSP.o \
./src/psp/SettingsPSP.o \
./src/psp/FSNodePSP.o \

DEFAULT_CFLAGS = $(shell $(SDL_CONFIG) --cflags)

MORE_CFLAGS = -DNARROW -DSOUND -DBPP16 -DSDL -DLSB_FIRST
MORE_CFLAGS += -I. -I$(SYSROOT)/usr/include  -I$(SYSROOT)/usr/lib  -I$(SYSROOT)/lib
MORE_CFLAGS += -DMPU_JZ4740
MORE_CFLAGS += -I./src -I./src/emucore -I./src/common -I./src/psp -DPSP -DBSPF_PSP -DSOUND_SUPPORT
MORE_CFLAGS += -fsigned-char -ffast-math -fomit-frame-pointer -fno-strength-reduce
MORE_CFLAGS += -DATARI_VERSION=\"$(ATARI_VERSION)\"
MORE_CFLAGS += -DDINGUX_MODE -mips32 -O3
#MORE_CFLAGS += -g -DNO_STDIO_REDIRECT
MORE_CFLAGS += -O2 -fomit-frame-pointer -ffunction-sections -ffast-math -fsingle-precision-constant # -G0

LDFLAGS =  -s -lm -lz `$(SDL_CONFIG) --libs`
LDFLAGS     += -Wl,--gc-sections

LIBS += -B$(SYSROOT)/lib
LIBS += -lSDL_image -lpng
LIBS += -lpthread  -ldl -lstdc++

CFLAGS = $(DEFAULT_CFLAGS) $(MORE_CFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) $(OBJS) $(LIBS) -o $(TARGET) && $(STRIP) $(TARGET)
	md5sum $(TARGET)

ipk: $(TARGET)
	@rm -rf /tmp/.dingux-2600-ipk/ && mkdir -p /tmp/.dingux-2600-ipk/root/home/retrofw/emus/dingux-2600 /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators.systems
	@cp -r \
	dingux-2600/default.bin \
	dingux-2600/dingux-2600.dge \
	dingux-2600/dingux-2600.man.txt \
	dingux-2600/dingux-2600.png \
	dingux-2600/splash.png \
	dingux-2600/graphics \
	/tmp/.dingux-2600-ipk/root/home/retrofw/emus/dingux-2600

	@echo "title=Dingux 2600" > /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators/dingux-2600.lnk
	@echo "description=Atari 2600 Emulator" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators/dingux-2600.lnk
	@echo "exec=/home/retrofw/emus/dingux-2600/dingux-2600.dge" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators/dingux-2600.lnk
	@echo "selectordir=/home/retrofw/roms/atari2600" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators/dingux-2600.lnk
	@echo "selectorfilter=.zip,.bin,.a26" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators/dingux-2600.lnk
	@echo "clock=600" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators/dingux-2600.lnk

	@echo "title=Atari 2600" > /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators.systems/atari2600.dingux-2600.lnk
	@echo "description=Dingux 2600 Emulator" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators.systems/atari2600.dingux-2600.lnk
	@echo "exec=/home/retrofw/emus/dingux-2600/dingux-2600.dge" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators.systems/atari2600.dingux-2600.lnk
	@echo "selectordir=/home/retrofw/roms/atari2600" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators.systems/atari2600.dingux-2600.lnk
	@echo "selectorfilter=.zip,.bin,.a26" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators.systems/atari2600.dingux-2600.lnk
	@echo "clock=600" >> /tmp/.dingux-2600-ipk/root/home/retrofw/apps/gmenu2x/sections/emulators.systems/atari2600.dingux-2600.lnk

	@echo "/home/retrofw/apps/gmenu2x/sections/emulators/dingux-2600.lnk" > /tmp/.dingux-2600-ipk/conffiles
	@echo "/home/retrofw/apps/gmenu2x/sections/emulators.systems/atari2600.dingux-2600.lnk" >> /tmp/.dingux-2600-ipk/conffiles

	@echo "Package: dingux-2600" > /tmp/.dingux-2600-ipk/control
	@echo "Version: $$(date +%Y%m%d)" >> /tmp/.dingux-2600-ipk/control
	@echo "Description: Dingux 2600 for the RetroFW" >> /tmp/.dingux-2600-ipk/control
	@echo "Section: emulators" >> /tmp/.dingux-2600-ipk/control
	@echo "Priority: optional" >> /tmp/.dingux-2600-ipk/control
	@echo "Maintainer: pingflood" >> /tmp/.dingux-2600-ipk/control
	@echo "Architecture: mipsel" >> /tmp/.dingux-2600-ipk/control
	@echo "Homepage: https://github.com/pingflood/dingux-2600" >> /tmp/.dingux-2600-ipk/control
	@echo "Depends:" >> /tmp/.dingux-2600-ipk/control
	@echo "Source: https://github.com/pingflood/dingux-2600" >> /tmp/.dingux-2600-ipk/control

	@cp dingux-2600/preinst /tmp/.dingux-2600-ipk/
	@chmod +x /tmp/.dingux-2600-ipk/preinst

	@tar --owner=0 --group=0 -czvf /tmp/.dingux-2600-ipk/control.tar.gz -C /tmp/.dingux-2600-ipk/ control conffiles preinst
	@tar --owner=0 --group=0 -czvf /tmp/.dingux-2600-ipk/data.tar.gz -C /tmp/.dingux-2600-ipk/root/ .
	@echo 2.0 > /tmp/.dingux-2600-ipk/debian-binary
	@ar r dingux-2600/dingux-2600.ipk /tmp/.dingux-2600-ipk/control.tar.gz /tmp/.dingux-2600-ipk/data.tar.gz /tmp/.dingux-2600-ipk/debian-binary

opk: $(TARGET)
	@mksquashfs \
	dingux-2600/default.retrofw.desktop \
	dingux-2600/atari2600.retrofw.desktop \
	dingux-2600/dingux-2600.dge \
	dingux-2600/dingux-2600.man.txt \
	dingux-2600/dingux-2600.png \
	dingux-2600/splash.png \
	dingux-2600/default.bin \
	dingux-2600/graphics \
	dingux-2600/dingux-2600.opk \
	-all-root -noappend -no-exports -no-xattrs

ctags:
	ctags ./src/*[ch] ./src/emucore/*[chp] ./src/common/*[chp]

clean:
	rm -f $(OBJS) $(TARGET) ./dingux-2600/dingux-2600.ipk
