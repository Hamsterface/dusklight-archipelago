// Other worlds' items that look like one of ours: which model each shows (item_models.cpp does
// the matching), the player's item_models.yaml, and the get-item text for those models.
//
// The item stays the Archipelago Item; only its display item changes (resolve_check), the way
// the randomizer shows a Foolish Item as some other item. Items with no match keep the
// Archipelago Item's own look, so game boxes and Sols carry on as before.

#include "internal.hpp"

#include <iterator>

namespace ap::internal {
namespace {

// Every other world's item in this save: its game and its name there. Kept beside the seed's
// files too, because item actors pick their model when they're created, which after loading a
// save is usually before the reconnect finishes (and offline, there's no server at all).
struct ItemSource {
    std::string game;
    std::string item;
};
std::unordered_map<std::string, ItemSource> g_itemSources;  // location -> its item

std::filesystem::path sources_path(const std::string& seed, const std::string& slot) {
    return seed_base(seed, slot) / "other_items.json";
}

}  // namespace

uint8_t native_model(const std::string& location) {
    if (!cfg_on(g_cfgNativeModels)) {
        return kApItem;
    }
    const auto source = g_itemSources.find(location);
    if (source == g_itemSources.end()) {
        return kApItem;
    }
    const uint8_t model =
        item_models::model_for(source->second.game, source->second.item, g_modelRules);
    return model == item_models::kNoModel || model == kApItem ? kApItem : model;
}

void remember_item_sources(const std::string& seed, const std::string& slot) {
    g_itemSources.clear();
    json out = json::object();
    for (const auto& [loc, item] : g_placementItem) {
        const auto owner = g_placementOwner.find(loc);
        const std::string game =
            owner != g_placementOwner.end() ? g_client.gameOfSlotName(owner->second) : "";
        g_itemSources[loc] = {game, item};
        out[loc] = json::array({game, item});
    }
    if (seed.empty() || slot.empty()) {
        return;
    }
    std::error_code ec;
    std::filesystem::create_directories(seed_base(seed, slot), ec);
    std::ofstream(sources_path(seed, slot), std::ios::binary) << out.dump();
}

void restore_item_sources(const std::string& seed, const std::string& slot) {
    g_itemSources.clear();
    if (seed.empty() || slot.empty()) {
        return;
    }
    std::ifstream in(sources_path(seed, slot), std::ios::binary);
    if (!in) {
        return;  // from before this file existed: the models come with the connection
    }
    const json saved = json::parse(in, nullptr, false);
    if (!saved.is_object()) {
        return;
    }
    for (const auto& [loc, v] : saved.items()) {
        if (v.is_array() && v.size() == 2 && v[0].is_string() && v[1].is_string()) {
            g_itemSources[loc] = {v[0].get<std::string>(), v[1].get<std::string>()};
        }
    }
}

uint16_t item_get_message(uint8_t item) {
    // As the game numbers them (dusk::mods item_get_message); a heart piece's first message
    // is fine for any count, since the text box picks its icon from Link's hearts itself.
    return static_cast<uint16_t>(item + 0x65);
}

void ensure_item_text(uint16_t message) {
    if (!g_textOverrideIds.insert(message).second) {
        return;
    }
    // Registered after the randomizer's own text for the message, so ours is asked first.
    // ap_item_text only answers while a pickup has armed this message.
    for (auto lang : {MESSAGE_LANGUAGE_ENGLISH, MESSAGE_LANGUAGE_GERMAN, MESSAGE_LANGUAGE_FRENCH,
             MESSAGE_LANGUAGE_SPANISH, MESSAGE_LANGUAGE_ITALIAN, MESSAGE_LANGUAGE_JAPANESE})
    {
        g_textOverrides.push_back(mods::flow::override_message_fn(0, message, lang, ap_item_text));
    }
}

void forget_item_text() {
    g_textOverrides.clear();
    g_textOverrideIds.clear();
}

std::filesystem::path item_models_path() {
    return randomizer::paths::GetRandomizerPath() / "archipelago" / "item_models.yaml";
}

bool load_item_models() {
    namespace fs = std::filesystem;
    g_modelRules.clear();
    const fs::path path = item_models_path();
    const std::string shown = "item_models.yaml";
    std::error_code ec;
    if (!fs::exists(path, ec)) {
        fs::create_directories(path.parent_path(), ec);
        std::ofstream(path, std::ios::binary) << item_models::overrides_template();
        g_modelStatus = "No custom matches added yet.";
        return true;
    }
    std::ifstream in(path, std::ios::binary);
    const std::string text{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    YAML::Node root;
    try {
        root = YAML::Load(text);
    } catch (const YAML::Exception& e) {
        g_modelStatus = fmt::format("{} couldn't be read (line {}: {}), so only the built-in "
                                    "matches apply.", shown, e.mark.line + 1, e.msg);
        ap_log("item_models.yaml: " + g_modelStatus);
        return false;
    }
    std::vector<std::string> problems;
    if (!root.IsNull() && !root.IsMap()) {
        problems.push_back("it should list games, each with its items under it");
    }
    if (root.IsMap()) {
        for (const auto& game : root) {
            const std::string gameName = game.first.Scalar();
            if (!game.second.IsMap()) {
                if (!game.second.IsNull()) {
                    problems.push_back(fmt::format("\"{}\" should have its items under it", gameName));
                }
                continue;
            }
            for (const auto& entry : game.second) {
                const std::string item = entry.first.Scalar();
                const std::string model = entry.second.IsScalar() ? entry.second.Scalar() : "";
                const auto id = item_models::parse_model(model);
                if (!id) {
                    problems.push_back(fmt::format("\"{}\" for {} isn't one of this game's items",
                        model, item));
                    continue;
                }
                g_modelRules.push_back({gameName, item, *id});
            }
        }
    }
    g_modelStatus = g_modelRules.empty()
                        ? std::string("No custom matches added yet.")
                        : fmt::format("{} custom match{} added.", g_modelRules.size(),
                              g_modelRules.size() == 1 ? "" : "es");
    if (!problems.empty()) {
        g_modelStatus += fmt::format("\nSkipped {}: {}", problems.size() == 1 ? "one" : "some",
            problems.front());
        if (problems.size() > 1) {
            g_modelStatus += fmt::format(" (and {} more)", problems.size() - 1);
        }
        for (const auto& p : problems) {
            ap_log("item_models.yaml: " + p);
        }
    }
    return problems.empty();
}

}  // namespace ap::internal
