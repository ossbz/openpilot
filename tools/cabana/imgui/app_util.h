#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SDL.h>

#include "imgui.h"
#include "msgq/visionipc/visionipc_client.h"
#include "tools/cabana/core/color.h"
#include "tools/cabana/core/persistent_state.h"
#include "tools/cabana/dbc/dbc.h"
#include "tools/replay/timeline.h"

// Shared constants used across split files
extern const CabanaColor timeline_colors[];
constexpr int kTimelineColorCount = 6;

// From app.cc anonymous namespace — shared across split files
ImVec4 imColor(const CabanaColor &color, float alpha_scale = 1.0f);
ImU32 packedColor(const CabanaColor &color, float alpha_scale = 1.0f);
void dismissOnEscape();
bool drawSplitter(const char *id, bool vertical, const ImVec2 &size);
bool hasCommand(const char *cmd);
bool hasNativeFileDialogs();
std::string nativeOpenFileDialog(const std::string &title, const std::string &default_path, const std::string &filter_name, const std::string &filter_pattern);
std::string nativeSaveFileDialog(const std::string &title, const std::string &default_name, const std::string &filter_name, const std::string &filter_pattern);
std::string nativeDirectoryDialog(const std::string &title, const std::string &default_path);
bool isValidDbcIdentifier(const std::string &name);
void normalizeDbcIdentifier(char *buf);
std::string pathBasename(const std::string &path);
std::string pathDirname(const std::string &path);
void drawAlertOverlay(ImDrawList *draw, const Timeline::Entry &alert, float x, float y, float w);
std::string formatPayload(const std::vector<uint8_t> &bytes);
ImGuiKey mapSdlKey(SDL_Keycode key);
void updateKeyMods(ImGuiIO &io, SDL_Keymod mods);
int mapMouseButton(uint8_t button);
void setClipboardText(void *, const char *text);
float cabanaUiScale();
void applyCabanaStyle(float scale);
void applyCabanaLightStyle(float scale);
bool detectSystemDarkTheme();
void applyCabanaTheme(int theme, float scale);
void loadCabanaFonts();
ImFont *cabanaMonoFont();
bool saveScreenshot(const std::string &path, int width, int height);
bool signalFitsInMessage(const cabana::Signal &sig, int msg_size_bytes);
int nextAvailableSignalBit(const cabana::Msg *msg, int msg_size_bytes);
std::string autoDbcForFingerprint(const std::string &fingerprint);
std::string streamLabel(VisionStreamType type);
void nv12ToRgba(const uint8_t *y_plane, const uint8_t *uv_plane, int width, int height, int stride, std::vector<uint8_t> &rgba);
void writePersistentState(const CabanaPersistentState &state);
CabanaPersistentState readPersistentState();

// Icon drawing (matching bootstrap icons from Qt cabana)
typedef void (*IconDrawFn)(ImDrawList *, ImVec2, ImVec2, ImU32);
bool iconButton(const char *str_id, IconDrawFn draw_fn);
bool smallIconButton(const char *str_id, IconDrawFn draw_fn);
void drawPlayIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawPauseIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawRewindIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawFastForwardIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawSkipEndIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawRepeatIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawCloseIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawPencilIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawPlusIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawMinusIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawGearIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawGraphUpIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawUndoIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawRedoIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawZoomOutIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawDockIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawUndockIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawInfoCircleIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
void drawTrashIcon(ImDrawList *dl, ImVec2 mn, ImVec2 mx, ImU32 col);
