// The Status tab and the Archipelago window that holds every tab.

#include "internal.hpp"

#include "../open_folder.hpp"

namespace ap::internal {
namespace {

// How item_models.yaml loaded, in the Item models pane. That pane is rebuilt whenever its row
// is highlighted (and cleared when another is), so only its own Reload button updates it.
UiElementHandle g_modelStatusText = 0;
// The Item models row, shown grayed while "Show as this game's items" is off. Groups can't be
// disabled, so it's a look (the ap-off class) and its pane's buttons are what's disabled.
UiElementHandle g_modelsGroup = 0;
bool g_modelsGroupOff = false;

bool models_off(ModContext*, void*) {
    return !cfg_on(g_cfgNativeModels);
}

// A path as RML that fits the help pane: RML only wraps at spaces, and a path's folders run
// together without any, so it breaks after a separator once a line gets long.
std::string wrapped_path(const std::filesystem::path& path) {
    constexpr size_t kLine = 40;
    const std::string text = std::filesystem::path(path).make_preferred().string();
    std::string out;
    std::string part;
    size_t line = 0;
    for (const char c : text) {
        part += c;
        if (c == '\\' || c == '/') {
            if (line > 0 && line + part.size() > kLine) {
                out += "<br/>";
                line = 0;
            }
            out += rml_escape(part);
            line += part.size();
            part.clear();
        }
    }
    if (line > 0 && line + part.size() > kLine) {
        out += "<br/>";
    }
    return out + rml_escape(part);
}

}  // namespace

// Used only in this file; defined below.
static void press_reconnect(ModContext*, void*);
static void press_disconnect(ModContext*, void*);
static std::string status_text();
static ModResult build_models_pane(ModContext* ctx, UiElementHandle pane, void*, ModError*);
static ModResult build_status_tab(ModContext* ctx, UiWindowHandle, UiElementHandle left, UiElementHandle right, void*, ModError*);
static ModResult update_status_tab(ModContext* ctx, void*, ModError*);

static void press_reconnect(ModContext*, void*) {
    if (!g_editServer.empty() && g_editServer != g_conn.server) {
        g_conn.server = g_editServer;
        if (g_phase == Phase::Playing) {
            write_conn();
        }
    }
    if (!g_conn.slot.empty()) {
        g_client.connect({g_conn.server, g_conn.slot, g_conn.password});
    }
}

static void press_disconnect(ModContext*, void*) {
    g_client.disconnect();
}

static std::string status_text() {
    std::string s = status_line();
    if (g_phase == Phase::Playing) {
        s += fmt::format("\nItems received: {} / {}", g_state.received, g_serverItems.size());
        if (g_haveSlot) {
            s += fmt::format("\nChecks sent: {} / {}", g_checked.size(), g_locationIds.size());
        }
        if (g_state.transformAnywhere) {
            s += "\nTransform anywhere: on (from your YAML)";
        }
        if (g_state.goal) {
            s += "\nGoal complete!";
        }
        if (g_needsRegen) {
            s += "\nWaiting to rebuild this save's seed from the server.";
        }
    }
    return s;
}

// The Item models row's pane: what the file is for, how it loaded, where it is, and buttons to
// reload it or open its folder.
static ModResult build_models_pane(ModContext* ctx, UiElementHandle pane, void*, ModError*) {
    if (!cfg_on(g_cfgNativeModels)) {
        svc_mng.ui->pane_add_rml(ctx, pane,
            "<b>Show as this game's items is off</b>, so no matches are used, built-in or "
            "your own.",
            nullptr);
    }
    svc_mng.ui->pane_add_rml(ctx, pane,
        "Your own matches for other worlds' items go in item_models.yaml. Under a game's name, "
        "list its items and which of this game's items each should look like; the file "
        "explains how, with examples. Your matches win over the built-in ones.",
        nullptr);
    g_modelStatusText = 0;
    svc_mng.ui->pane_add_text(ctx, pane, g_modelStatus.c_str(), &g_modelStatusText);
    svc_mng.ui->pane_add_rml(ctx, pane, wrapped_path(item_models_path()).c_str(), nullptr);
    UiControlDesc reload = UI_CONTROL_DESC_INIT;
    reload.kind = UI_CONTROL_BUTTON;
    reload.label = "Reload item models";
    reload.is_disabled = models_off;
    reload.on_pressed = [](ModContext* c, void*) {
        const bool ok = load_item_models();
        if (g_modelStatusText != 0) {
            svc_mng.ui->elem_set_text(c, g_modelStatusText, g_modelStatus.c_str());
        }
        if (!ok) {
            toast("Archipelago", g_modelStatus, "warning", 8000);
        }
    };
    svc_mng.ui->pane_add_control(ctx, pane, &reload, nullptr);
    UiControlDesc folder = UI_CONTROL_DESC_INIT;
    folder.kind = UI_CONTROL_BUTTON;
    folder.label = "Open item models folder";
    folder.is_disabled = models_off;
    folder.on_pressed = [](ModContext*, void*) {
        if (!open_folder(item_models_path().parent_path())) {
            toast("Archipelago", "Couldn't open the folder: " +
                item_models_path().parent_path().make_preferred().string(), "warning", 8000);
        }
    };
    svc_mng.ui->pane_add_control(ctx, pane, &folder, nullptr);
    return MOD_OK;
}

static ModResult build_status_tab(ModContext* ctx, UiWindowHandle, UiElementHandle left,
    UiElementHandle right, void*, ModError*) {
    g_editServer = g_conn.server;
    svc_mng.ui->pane_add_section(ctx, left, "Connection");
    g_statusWindowText = 0;
    svc_mng.ui->pane_add_text(ctx, left, status_text().c_str(), &g_statusWindowText);
    UiControlDesc d = UI_CONTROL_DESC_INIT;
    d.kind = UI_CONTROL_STRING;
    d.label = "Server";
    d.help_rml = "Change this if the room moved to a new port, then press Reconnect.";
    d.get = get_str;
    d.set = set_str;
    d.user_data = &g_editServer;
    svc_mng.ui->pane_add_control(ctx, left, &d, nullptr);
    UiControlDesc b = UI_CONTROL_DESC_INIT;
    b.kind = UI_CONTROL_BUTTON;
    b.label = "Reconnect";
    b.on_pressed = press_reconnect;
    svc_mng.ui->pane_add_control(ctx, left, &b, nullptr);
    UiControlDesc goal = UI_CONTROL_DESC_INIT;
    goal.kind = UI_CONTROL_BUTTON;
    goal.label = "Send goal complete";
    goal.help_rml = "Tells the server you finished the game, in case it wasn't detected.";
    goal.on_pressed = [](ModContext*, void*) { complete_goal("manual"); };
    goal.is_disabled = [](ModContext*, void*) {
        return g_state.goal || g_client.state() != State::Connected;
    };
    svc_mng.ui->pane_add_control(ctx, left, &goal, nullptr);
    UiControlDesc dl = UI_CONTROL_DESC_INIT;
    dl.kind = UI_CONTROL_TOGGLE;
    dl.label = "Death link";
    dl.help_rml = "When anyone else with death link dies, so do you, and the other way round. "
                  "Starts from your YAML; changing it here sticks to this save.";
    dl.get = [](ModContext*, void*, UiControlValue* out) { out->bool_value = death_link_on(); };
    dl.set = [](ModContext*, void*, const UiControlValue* v) {
        g_state.deathLink = v->bool_value ? 1 : 0;
        write_state();
        apply_death_link_tags();
    };
    dl.is_disabled = [](ModContext*, void*) { return g_phase != Phase::Playing; };
    svc_mng.ui->pane_add_control(ctx, left, &dl, nullptr);
    UiControlDesc b2 = UI_CONTROL_DESC_INIT;
    b2.kind = UI_CONTROL_BUTTON;
    b2.label = "Disconnect";
    b2.on_pressed = press_disconnect;
    svc_mng.ui->pane_add_control(ctx, left, &b2, nullptr);

    svc_mng.ui->pane_add_section(ctx, left, "Tracker");
    add_bound(ctx, left, UI_CONTROL_TOGGLE, "Strike out finished players' checks",
        g_cfgSkipFinished,
        "Checks holding an item for a player who has already finished their game are struck "
        "out in the tracker and left off the in-logic list: nobody needs those items any more.");

    svc_mng.ui->pane_add_section(ctx, left, "Other players' items");
    add_bound(ctx, left, UI_CONTROL_TOGGLE, "Show as this game's items", g_cfgNativeModels,
        "Items from other Zelda games that match one of this game's items look like it: "
        "rupees as rupees, a Hookshot as the Clawshot, and keys, maps, bombs, arrows, "
        "bottles, swords and more. Other games' items only match through your own entries. Items that don't match show as their game's box, or a Sol. "
        "Applies to items that appear from now on. Add your own matches under Item models.");
    UiGroupDesc models = UI_GROUP_DESC_INIT;
    models.label = "Item models";
    models.build = build_models_pane;
    g_modelsGroup = 0;
    svc_mng.ui->pane_add_group(ctx, left, right, &models, &g_modelsGroup);
    g_modelsGroupOff = !cfg_on(g_cfgNativeModels);
    if (g_modelsGroup != 0) {
        svc_mng.ui->elem_set_class(ctx, g_modelsGroup, "ap-off", g_modelsGroupOff);
    }
    add_bound(ctx, left, UI_CONTROL_TOGGLE, "Show as game boxes", g_cfgBoxes,
        "Items that belong to other worlds (and don't look like one of this game's items) show "
        "as the box of the game they're for, with its cover art, instead of a Sol. Covers come "
        "from SteamGridDB with your own API key and "
        "download once per game. Games without art stay Sols.");
    add_bound(ctx, left, UI_CONTROL_TOGGLE, "Gray boxes for filler", g_cfgGrayFiller,
        "Boxes for items their world calls filler show in grays, so you can tell at a glance "
        "they aren't progression or useful. Traps keep their colour.");
    add_bound(ctx, left, UI_CONTROL_STRING, "SteamGridDB API key", g_cfgSgdbKey,
        "Free: sign in at steamgriddb.com, then Preferences, API, and copy your key here. "
        "It's only sent to SteamGridDB. Leave it empty and items stay Sols.");
    g_coverStatusText = 0;
    svc_mng.ui->pane_add_text(ctx, left, covers::status().c_str(), &g_coverStatusText);
    UiControlDesc again = UI_CONTROL_DESC_INIT;
    again.kind = UI_CONTROL_BUTTON;
    again.label = "Download covers again";
    again.help_rml = "Forgets the saved covers (and which games had none) and downloads them "
                     "again; covers you picked stay picked. To pick a different cover for a "
                     "game, or use your own art, go to the Covers tab.";
    again.on_pressed = [](ModContext*, void*) { covers::refetch(); };
    svc_mng.ui->pane_add_control(ctx, left, &again, nullptr);
    return MOD_OK;
}

static ModResult update_status_tab(ModContext* ctx, void*, ModError*) {
    if (g_modelsGroup != 0 && g_modelsGroupOff != !cfg_on(g_cfgNativeModels)) {
        g_modelsGroupOff = !g_modelsGroupOff;
        svc_mng.ui->elem_set_class(ctx, g_modelsGroup, "ap-off", g_modelsGroupOff);
    }
    if (g_coverStatusText != 0) {
        static std::string shownCovers;
        const std::string now = covers::status();
        if (now != shownCovers) {
            shownCovers = now;
            svc_mng.ui->elem_set_text(ctx, g_coverStatusText, now.c_str());
        }
    }
    if (g_statusWindowText != 0) {
        static std::string shown;
        const std::string now = status_text();
        if (now != shown) {
            shown = now;
            svc_mng.ui->elem_set_text(ctx, g_statusWindowText, now.c_str());
        }
    }
    return MOD_OK;
}

void open_status_window(ModContext*, void*) {
    static UiTabDesc tabs[6] = {UI_TAB_DESC_INIT, UI_TAB_DESC_INIT, UI_TAB_DESC_INIT,
        UI_TAB_DESC_INIT, UI_TAB_DESC_INIT, UI_TAB_DESC_INIT};
    tabs[0].title = "Status";
    tabs[0].build = build_status_tab;
    tabs[0].update = update_status_tab;
    tabs[1].title = "Tracker";
    tabs[1].build = build_tracker_tab;
    tabs[1].update = update_tracker_tab;
    tabs[2].title = "Hints";
    tabs[2].build = build_hints_tab;
    tabs[2].update = update_hints_tab;
    tabs[3].title = "Messages";
    tabs[3].build = build_messages_tab;
    tabs[3].update = update_messages_tab;
    tabs[4].title = "Overlay";
    tabs[4].build = build_overlay_tab;
    tabs[5].title = "Covers";
    tabs[5].build = build_covers_tab;
    tabs[5].update = update_covers_tab;
    // The picker's buttons show their emoji as a background image.
    static const std::string rcss = [] {
        std::string out = kWindowRcss;
        out += ".ap-emoji-btn { padding-left: 42dp; font-size: 15dp; text-align: left; }\n";
        out += ".ap-emoji-glyph { font-size: 26dp; min-width: 52dp; padding: 4dp 8dp; }\n";
        // A row that's off, looking like the game's disabled rows (controls.rcss).
        out += "select-button.ap-off, select-button.ap-off:selected, select-button.ap-off:active "
               "{ opacity: 0.35; }\n";
        for (const auto& e : emoji::picker()) {
            out += fmt::format(".ap-e-{} {{ decorator: image({} contain left center); }}\n", e.file,
                emoji::image_source(e.file));
        }
        return out;
    }();
    UiWindowDesc desc = UI_WINDOW_DESC_INIT;
    desc.tabs = tabs;
    desc.tab_count = 6;
    desc.rcss = rcss.c_str();
    desc.on_closed = [](ModContext*, UiWindowHandle, void*) {
        g_statusWindow = 0;
        g_statusWindowText = 0;
        g_coverStatusText = 0;
        g_modelStatusText = 0;
        g_modelsGroup = 0;
        g_trackerSummary = g_trackerProgress = g_trackerList = 0;
        g_trackerGroups.clear();
        g_logElem = 0;
        g_hintsSummary = g_hintDetails = 0;
        g_hintsList = 0;
        forget_covers_tab();
    };
    svc_mng.ui->window_push(svc_mng.mod_ctx, &desc, &g_statusWindow);
}

}  // namespace ap::internal
