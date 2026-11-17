# AutobootModule (grantendo fork)

> **This is not an official Aroma build.** It's a fork of [wiiu-env/AutobootModule](https://github.com/wiiu-env/AutobootModule) v0.3.2 with four changes that are offered upstream but not merged yet. The menu shows "v0.3.2 (grantendo fork)" in its top-right corner. Problems with this build belong here, not in wiiu-env's issues.

What it adds:
- **vWii titles start with no GamePad connected** (wiiu-env/AutobootModule#67). Based on [@patapovich](https://github.com/patapovich)'s wiiu-env/AutobootModule#69 (his commits keep his name), plus a GamePad check over its radio link and a GamePad pipe fix that let the launch go through.
- **Custom boot entries** from `bootOptions.cfg`, following [@razor85](https://github.com/razor85)'s wiiu-env/AutobootModule#25.
- **Menu order** from `bootOptions.cfg`.
- **Menu countdown** to the autoboot entry.

Details for each are below.

**Install:** replace `wiiu/environments/aroma/modules/setup/99_autoboot.rpx` on the SD card with the one from the release. Keep that file name.

**When you update Aroma:** the Aroma Updater sees an unknown `99_autoboot.rpx` and asks whether to overwrite it. Press **B** to keep this one. Pressing **A** puts the official module back (that's also how to undo this fork).

---

## About the original
This is a bootmenu targetted to be loaded with the [EnvironmentLoader](https://github.com/wiiu-env/EnvironmentLoader). It allows you to boot into the Wii U Menu, Homebrew Channel, vWii System Menu or vWii Homebrew Channel.

## Usage
Place the `99_autoboot.rpx` in the `[ENVIRONMENT]/modules/setup` folder and run the EnvironmentLoader. 

Hold START (+) on the Gamepad while launching this Environment to force open the Autoboot menu.

Press Y on the autoboot menu to set autobooting to this titles. To revert it, force open the menu by holding START (+) while launching the environment.

## Features
- Boot into Wii U Menu, Homebrew Channel, vWii System Menu or vWii Homebrew Channel.
- Full support of Quick Boot Menu of the Gamepad when coldbooting
- Set a autoboot title
- Add your own vWii or Wii U titles to the menu

## Custom boot entries
Create `bootOptions.cfg` next to `autoboot.cfg` (in the `[ENVIRONMENT]` folder) with one entry per line: a name, a comma, and the title's full 64-bit title id.

```
# vWii titles start with 00010001, Wii U titles with 00050000
My vWii Channel,0x0001000152494654
Mii Maker,0x000500101004A200
```

The title id decides how the entry is launched: vWii titles through the vWii, Wii U titles directly. Entries whose title is not installed are left out of the menu. Up to 4 entries are shown, as many as fit: the menu has room for 7 rows in all. System applets (title ids starting with 00050030, like the Internet Browser) can't be custom entries. A custom entry can be set as the autoboot title like any other.

### Menu order

The menu normally shows the built-in entries first and custom entries after them. To choose the order, name built-in entries in `bootOptions.cfg` with an `@` line; entries then appear in the order of the lines:

```
My vWii Channel,0x0001000152494654
@wiiu_menu
@vwii_homebrew_channel
@vwii_system_menu
```

The names are the ones `autoboot.cfg` uses: `@wiiu_menu`, `@homebrew_launcher`, `@vwii_system_menu` and `@vwii_homebrew_channel`. Built-in entries without a line follow the listed ones in the usual order, so none can disappear by accident, and an entry that isn't available (the Homebrew Launcher without its installer, the vWii Homebrew Channel without HBC) is still left out. Without `@` lines the menu keeps the usual order. With `@` lines and no autoboot entry, the cursor starts on the first entry instead of the Wii U Menu.

### Menu countdown

A line `countdown=5` in `bootOptions.cfg` (1 to 30 seconds) shows the menu at every boot even when an autoboot entry is set, with "Starting <entry> in 5..." above the bottom bar. When the countdown ends, the autoboot entry starts; pressing any button first stops the countdown and leaves the menu open as usual. Holding START (+) at boot opens the menu without a countdown. Without an autoboot entry there is nothing to count down to, and the menu waits as before. Useful when no GamePad or Wii Remote is in reach at boot.

## Buildflags

### Logging
Building via `make` only logs errors (via OSReport). To enable logging via the [LoggingModule](https://github.com/wiiu-env/LoggingModule) set `DEBUG` to `1` or `VERBOSE`.

`make` Logs errors only (via OSReport).  
`make DEBUG=1` Enables information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).  
`make DEBUG=VERBOSE` Enables verbose information and error logging via [LoggingModule](https://github.com/wiiu-env/LoggingModule).

If the [LoggingModule](https://github.com/wiiu-env/LoggingModule) is not present, it'll fallback to UDP (Port 4405) and [CafeOS](https://github.com/wiiu-env/USBSerialLoggingModule) logging.

## Building
For building you just need [wut](https://github.com/devkitPro/wut/) installed, then use the `make` command.

## Building using the Dockerfile

It's possible to use a docker image for building. This way you don't need anything installed on your host system.

```
# Build docker image (only needed once)
docker build . -t autobootmodule-builder

# make 
docker run -it --rm -v ${PWD}:/project autobootmodule-builder make

# make clean
docker run -it --rm -v ${PWD}:/project autobootmodule-builder make clean
```

## Format the code via docker

`docker run --rm -v ${PWD}:/src ghcr.io/wiiu-env/clang-format:13.0.0-2 -r ./source -i`

## Credits
- GaryOderNichts
- Maschell
- Crementif
- patapovich (no-GamePad vWii launch, wiiu-env/AutobootModule#69)
- razor85 (custom boot entries, wiiu-env/AutobootModule#25)
- grantendo (this fork)
