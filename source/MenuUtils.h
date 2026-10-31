#pragma once

#include "ACTAccountInfo.h"
#include <cstdint>
#include <map>
#include <memory>
#include <nn/act.h>
#include <string>
#include <utility>
#include <vector>

#define UPDATE_SKIP_PATH         "fs:/vol/external01/wiiu/environments/skipUpdateWarn"

#define COLOR_WHITE              Color(0xffffffff)
#define COLOR_BLACK              Color(0, 0, 0, 255)
#define COLOR_BACKGROUND         COLOR_BLACK
#define COLOR_BACKGROUND_WARN    Color(255, 251, 4, 255)
#define COLOR_TEXT               COLOR_WHITE
#define COLOR_WARNING            COLOR_BLACK
#define COLOR_TEXT2              Color(0xB3ffffff)
#define COLOR_AUTOBOOT           Color(0xaeea00ff)
#define COLOR_BORDER             Color(204, 204, 204, 255)
#define COLOR_BORDER_HIGHLIGHTED Color(0x3478e4ff)

enum {
    BOOT_OPTION_WII_U_MENU,
    BOOT_OPTION_HOMEBREW_LAUNCHER,
    BOOT_OPTION_VWII_SYSTEM_MENU,
    BOOT_OPTION_VWII_HOMEBREW_CHANNEL,
    // Always the last entry, never an autoboot choice (not in autoboot.cfg's names).
    BOOT_OPTION_POWER_OFF,
};

// Entries from bootOptions.cfg use BOOT_OPTION_CUSTOM_BASE + their index in the file.
#define BOOT_OPTION_CUSTOM_BASE 0x100
// At most this many custom entries, fewer if the menu runs out of rows.
#define MAX_CUSTOM_BOOT_OPTIONS 4
// The menu doesn't scroll, and an eighth row would cover the update status line.
#define MAX_MENU_ROWS           7

// Boot menu entries (option id, name) in the order they are shown.
using BootMenu = std::vector<std::pair<uint32_t, std::string>>;

struct CustomBootOption {
    std::string name;
    uint64_t titleId;
};

// The longest countdown a "countdown=" line may ask for, in seconds.
#define MAX_MENU_COUNTDOWN_SECONDS 30

// Reads the custom entries from bootOptions.cfg. When the file also has "@" lines naming
// built-in entries, `menuOrder` gets every listed entry's option id in the file's order;
// without them it stays empty and the menu keeps the usual order. A line "countdown=5" sets
// `countdownSeconds` (0 without one): the menu then shows at every boot even with an autoboot
// entry, and starts that entry when the countdown ends unless a button is pressed.
std::vector<CustomBootOption> readCustomBootOptions(const std::string &path, std::vector<uint32_t> &menuOrder,
                                                    uint32_t &countdownSeconds);

int32_t readAutobootOption(std::string &configPath, const std::vector<CustomBootOption> &customOptions);

void writeAutobootOption(std::string &configPath, int32_t autobootOption, const std::vector<CustomBootOption> &customOptions);

// `ordered`: bootOptions.cfg sets the menu order; with no autoboot entry the cursor then
// starts on the first entry instead of the Wii U Menu.
// `countdownSeconds` > 0 with an autoboot entry in the menu: that entry starts by itself
// when the countdown ends, unless a button was pressed first.
int32_t handleMenuScreen(std::string &configPath, int32_t autobootOptionInput, const BootMenu &menu,
                         const std::vector<CustomBootOption> &customOptions, bool ordered, uint32_t countdownSeconds);

nn::act::SlotNo handleAccountSelectScreen(const std::vector<std::shared_ptr<AccountInfo>> &data);

void handleUpdateWarningScreen();

bool handleDiscInsertScreen(uint64_t expectedTitleId, uint64_t *titleIdToLaunch);
