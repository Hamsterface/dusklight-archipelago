#pragma once

// Which of this game's items another world's item should look like: built-in matching by name
// (rupees, hearts, keys, the hookshot, and for Zelda games bombs, arrows, bottles, swords and so
// on), and the player's own overrides on top. Pure code with no Dusklight headers, so tls_test
// can test it on a desktop; the mode uses it from mode/models.cpp.

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>

namespace ap::item_models {

// "No model of ours": the item stays the Archipelago Item (its game's box, or a Sol).
inline constexpr uint8_t kNoModel = 0xFF;

// A model an item can show as. Every one is an item the randomizer itself places in chests, on
// the ground and in shops (items.yaml), or one a progressive item turns into, so the game draws
// each of them held up, on the ground and in its text box.
struct Model {
    const char* name;  // this game's item name, as items.yaml spells it
    uint8_t id;
};
std::span<const Model> models();

// A model by this game's item name, a short name ("Clawshot", "Small Key", "Bottle"), or its
// number ("0x44", for one of models()). "Sol", "Box", "Default" and "None" give kNoModel: the
// item keeps its usual look. Anything else is nullopt.
std::optional<uint8_t> parse_model(std::string_view text);

// The player's own: under a game (exactly as the room names it, any case, or "*" for every
// game), an item name and its model. A * in the item name matches any run of characters.
struct Rule {
    std::string game;
    std::string item;
    uint8_t model = kNoModel;
};

bool is_zelda_game(std::string_view game);

// Built-in matching only: the model for an item, or kNoModel.
uint8_t builtin_model(std::string_view game, std::string_view item);

// The player's rules for this game first, then their "*" rules, then built-in matching.
uint8_t model_for(std::string_view game, std::string_view item, std::span<const Rule> rules);

// Case-insensitive, with * matching any run of characters (including none).
bool glob_match(std::string_view pattern, std::string_view text);

// The template written when the player has no overrides file yet.
const char* overrides_template();

}  // namespace ap::item_models
