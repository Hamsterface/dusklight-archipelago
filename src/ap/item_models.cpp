#include "item_models.hpp"

#include <array>
#include <cctype>
#include <cstdlib>
#include <initializer_list>

namespace ap::item_models {
namespace {

// items.yaml, minus what can't stand in for another world's item: the Foolish Item (it's a
// random model of its own), the Archipelago Item, portals (no get-item text), Link's House's
// duplicate purple rupee, and the events above 0xFF. Plus what progressive items turn into.
constexpr auto kModels = std::to_array<Model>({
    {"Green Rupee", 0x01},
    {"Blue Rupee", 0x02},
    {"Yellow Rupee", 0x03},
    {"Red Rupee", 0x04},
    {"Purple Rupee", 0x05},
    {"Orange Rupee", 0x06},
    {"Silver Rupee", 0x07},
    {"Bombs 5", 0x0A},
    {"Bombs 10", 0x0B},
    {"Bombs 20", 0x0C},
    {"Bombs 30", 0x0D},
    {"Arrows 10", 0x0E},
    {"Arrows 20", 0x0F},
    {"Arrows 30", 0x10},
    {"Seeds 50", 0x12},
    {"Water Bombs 5", 0x16},
    {"Water Bombs 10", 0x17},
    {"Water Bombs 15", 0x18},
    {"Bomblings 5", 0x1A},
    {"Bomblings 10", 0x1B},
    {"Piece of Heart", 0x21},
    {"Heart Container", 0x22},
    {"Ordon Sword", 0x28},
    {"Master Sword", 0x29},
    {"Ordon Shield", 0x2A},
    {"Wooden Shield", 0x2B},
    {"Hylian Shield", 0x2C},
    {"Magic Armor", 0x30},
    {"Zora Armor", 0x31},
    {"Shadow Crystal", 0x32},
    {"Progressive Wallet", 0x35},
    {"Giant Wallet", 0x36},
    {"Hawkeye", 0x3E},
    {"Progressive Sword", 0x3F},
    {"Gale Boomerang", 0x40},
    {"Spinner", 0x41},
    {"Ball and Chain", 0x42},
    {"Progressive Bow", 0x43},
    {"Progressive Clawshot", 0x44},
    {"Iron Boots", 0x45},
    {"Progressive Dominion Rod", 0x46},
    {"Double Clawshot", 0x47},
    {"Lantern", 0x48},
    {"Progressive Fishing Rod", 0x4A},
    {"Slingshot", 0x4B},
    {"Giant Bomb Bag", 0x4F},
    {"Bomb Bag", 0x50},
    {"Big Quiver", 0x55},
    {"Giant Quiver", 0x56},
    {"Empty Bottle", 0x60},
    {"Red Potion Shop", 0x61},
    {"Blue Potion Shop", 0x63},
    {"Bottle with Half Milk", 0x65},
    {"Lantern Oil Shop", 0x66},
    {"Fairy Tears", 0x73},
    {"Bottle with Great Fairies Tears", 0x75},
    {"Renados Letter", 0x80},
    {"Invoice", 0x81},
    {"Wooden Statue", 0x82},
    {"Ilias Charm", 0x83},
    {"Horse Call", 0x84},
    {"Forest Temple Small Key", 0x85},
    {"Goron Mines Small Key", 0x86},
    {"Lakebed Temple Small Key", 0x87},
    {"Arbiters Grounds Small Key", 0x88},
    {"Snowpeak Ruins Small Key", 0x89},
    {"Temple of Time Small Key", 0x8A},
    {"City in the Sky Small Key", 0x8B},
    {"Palace of Twilight Small Key", 0x8C},
    {"Hyrule Castle Small Key", 0x8D},
    {"Gerudo Desert Bulblin Camp Key", 0x8E},
    {"Aurus Memo", 0x90},
    {"Asheis Sketch", 0x91},
    {"Forest Temple Big Key", 0x92},
    {"Lakebed Temple Big Key", 0x93},
    {"Arbiters Grounds Big Key", 0x94},
    {"Temple of Time Big Key", 0x95},
    {"City in the Sky Big Key", 0x96},
    {"Palace of Twilight Big Key", 0x97},
    {"Hyrule Castle Big Key", 0x98},
    {"Forest Temple Compass", 0x99},
    {"Goron Mines Compass", 0x9A},
    {"Lakebed Temple Compass", 0x9B},
    {"Bottle with Lantern Oil", 0x9D},
    {"Progressive Mirror Shard", 0xA5},
    {"Arbiters Grounds Compass", 0xA8},
    {"Snowpeak Ruins Compass", 0xA9},
    {"Temple of Time Compass", 0xAA},
    {"City in the Sky Compass", 0xAB},
    {"Palace of Twilight Compass", 0xAC},
    {"Hyrule Castle Compass", 0xAD},
    {"Forest Temple Dungeon Map", 0xB6},
    {"Goron Mines Dungeon Map", 0xB7},
    {"Lakebed Temple Dungeon Map", 0xB8},
    {"Arbiters Grounds Dungeon Map", 0xB9},
    {"Snowpeak Ruins Dungeon Map", 0xBA},
    {"Temple of Time Dungeon Map", 0xBB},
    {"City in the Sky Dungeon Map", 0xBC},
    {"Palace of Twilight Dungeon Map", 0xBD},
    {"Hyrule Castle Dungeon Map", 0xBE},
    {"Male Beetle", 0xC0},
    {"Female Beetle", 0xC1},
    {"Male Butterfly", 0xC2},
    {"Female Butterfly", 0xC3},
    {"Male Stag Beetle", 0xC4},
    {"Female Stag Beetle", 0xC5},
    {"Male Grasshopper", 0xC6},
    {"Female Grasshopper", 0xC7},
    {"Male Phasmid", 0xC8},
    {"Female Phasmid", 0xC9},
    {"Male Pill Bug", 0xCA},
    {"Female Pill Bug", 0xCB},
    {"Male Mantis", 0xCC},
    {"Female Mantis", 0xCD},
    {"Male Ladybug", 0xCE},
    {"Female Ladybug", 0xCF},
    {"Male Snail", 0xD0},
    {"Female Snail", 0xD1},
    {"Male Dragonfly", 0xD2},
    {"Female Dragonfly", 0xD3},
    {"Male Ant", 0xD4},
    {"Female Ant", 0xD5},
    {"Male Dayfly", 0xD6},
    {"Female Dayfly", 0xD7},
    {"Progressive Fused Shadow", 0xD8},
    {"Poe Soul", 0xE0},
    {"Progressive Hidden Skill", 0xE1},
    {"Progressive Sky Book", 0xE9},
    {"North Faron Woods Gate Key", 0xEE},
    {"Gate Keys", 0xF3},
    {"Ordon Pumpkin", 0xF4},
    {"Ordon Cheese", 0xF5},
    {"Snowpeak Ruins Bedroom Key", 0xF6},
    {"Goron Mines Key Shard", 0xF9},
    {"Faron Woods Coro Key", 0xFE},
});

// Short names for the overrides file.
constexpr auto kAliases = std::to_array<Model>({
    {"Rupee", 0x01},
    {"Bombs", 0x0A},
    {"Arrows", 0x0E},
    {"Seeds", 0x12},
    {"Water Bombs", 0x16},
    {"Bomblings", 0x1A},
    {"Heart Piece", 0x21},
    {"Wallet", 0x35},
    {"Big Wallet", 0x35},
    {"Wooden Sword", 0x3F},
    {"Sword", 0x28},
    {"Shield", 0x2C},
    {"Boomerang", 0x40},
    {"Bow", 0x43},
    {"Heros Bow", 0x43},
    {"Clawshot", 0x44},
    {"Hookshot", 0x44},
    {"Dominion Rod", 0x46},
    {"Fishing Rod", 0x4A},
    {"Bottle", 0x60},
    {"Red Potion", 0x61},
    {"Blue Potion", 0x63},
    {"Milk", 0x65},
    {"Lantern Oil", 0x9D},
    {"Small Key", 0x85},
    {"Key", 0x85},
    {"Big Key", 0x92},
    {"Boss Key", 0x92},
    {"Compass", 0x99},
    {"Dungeon Map", 0xB6},
    {"Map", 0xB6},
    {"Golden Bug", 0xC0},
    {"Mirror Shard", 0xA5},
    {"Fused Shadow", 0xD8},
    {"Sky Book", 0xE9},
    {"Key Shard", 0xF9},
});

// Words in a game's name that make it a Zelda game (any case).
constexpr auto kZeldaWords = std::to_array<std::string_view>({
    "zelda", "hyrule", "link to the past", "ocarina of time", "majora", "wind waker",
    "twilight princess", "links awakening", "link's awakening", "oracle of", "minish cap",
    "four swords", "skyward sword", "link between worlds", "phantom hourglass", "spirit tracks",
    "triforce heroes", "breath of the wild", "tears of the kingdom", "echoes of wisdom",
    "adventure of link",
    // Ports that go by their own name: Ship of Harkinian (Ocarina of Time) and 2 Ship 2
    // Harkinian (Majora's Mask).
    "harkinian",
});

char lower(char c) {
    return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
}

// Lower case, apostrophes dropped ("Hero's" is "heros"), anything else that isn't a letter or
// a digit a word break, padded with a space each side so whole words can be found.
std::string normalize(std::string_view text) {
    std::string out = " ";
    for (const char c : text) {
        if (c == '\'' || c == '`') {
            continue;
        }
        if (std::isalnum(static_cast<unsigned char>(c))) {
            out += lower(c);
        } else if (out.back() != ' ') {
            out += ' ';
        }
    }
    if (out.back() != ' ') {
        out += ' ';
    }
    return out;
}

// Normalized text has a whole-word phrase.
bool has(const std::string& s, std::string_view phrase) {
    std::string needle = " ";
    needle += phrase;
    needle += ' ';
    return s.find(needle) != std::string::npos;
}

bool has_any(const std::string& s, std::initializer_list<std::string_view> phrases) {
    for (const auto p : phrases) {
        if (has(s, p)) {
            return true;
        }
    }
    return false;
}

// The first number in normalized text ("rupees 20" is 20), or -1.
long first_number(const std::string& s) {
    for (size_t i = 1; i < s.size(); ++i) {
        if (std::isdigit(static_cast<unsigned char>(s[i])) && s[i - 1] == ' ') {
            return std::strtol(s.c_str() + i, nullptr, 10);
        }
    }
    return -1;
}

std::optional<uint8_t> by_name(const std::string& normalized) {
    for (const auto& m : kModels) {
        if (normalize(m.name) == normalized) {
            return m.id;
        }
    }
    return std::nullopt;
}

bool twilight_princess(std::string_view game) {
    return normalize(game).find(" twilight princess ") != std::string::npos;
}

// Rupees, by colour or by amount.
uint8_t rupees(const std::string& s) {
    struct Colour {
        std::string_view word;
        uint8_t id;
    };
    static constexpr Colour colours[] = {
        {"silver", 0x07}, {"gold", 0x07}, {"golden", 0x07}, {"huge", 0x07}, {"orange", 0x06},
        {"purple", 0x05}, {"red", 0x04}, {"yellow", 0x03}, {"blue", 0x02}, {"green", 0x01},
    };
    for (const auto& [colour, id] : colours) {
        if (has(s, colour)) {
            return id;
        }
    }
    const long n = first_number(s);
    if (n <= 1) {  // no amount (-1) is a single rupee too
        return 0x01;
    }
    if (n <= 5) {
        return 0x02;
    }
    if (n <= 10) {
        return 0x03;
    }
    if (n <= 20) {
        return 0x04;
    }
    if (n <= 50) {
        return 0x05;
    }
    return n <= 100 ? 0x06 : 0x07;
}

struct Step {
    long upTo;
    uint8_t id;
};

// The id of the first step the amount fits in, or most.
uint8_t by_amount(long n, std::initializer_list<Step> steps, uint8_t most) {
    for (const auto& [upTo, id] : steps) {
        if (n <= upTo) {
            return id;
        }
    }
    return most;
}

// Zelda's usual items, worded the way Zelda games word them.
uint8_t zelda(const std::string& s) {
    // Heart containers
    if (has_any(s, {"piece of heart", "pieces of heart", "heart piece", "heart pieces"})) {
        return 0x21;
    }
    if (has_any(s, {"heart container", "heart containers"})) {
        return 0x22;
    }

    // Keys and dungeon items
    if (has_any(s, {"boss key", "boss keys", "big key", "big keys"})) {
        return 0x92;
    }
    if (has_any(s, {"key shard", "key shards", "key piece", "key pieces"})) {
        return 0xF9;
    }
    if (has_any(s, {"small key", "small keys", "key ring", "key rings", "keyring"})) {
        return 0x85;
    }
    if (has_any(s, {"map", "maps"})) {
        return 0xB6;
    }
    if (has_any(s, {"compass", "compasses"})) {
        return 0x99;
    }

    // Bottles
    if (has_any(s, {"bottle", "bottles", "ruto", "rutos"})) {
        if (has(s, "milk")) {
            return 0x65;
        }
        if (has(s, "red potion")) {
            return 0x61;
        }
        if (has(s, "blue potion")) {
            return 0x63;
        }
        if (has(s, "oil")) {
            return 0x9D;
        }
        if (has_any(s, {"fairy", "fairies"})) {
            return 0x75;
        }
        return 0x60;
    }
    if (has(s, "red potion")) {
        return 0x61;
    }
    if (has(s, "blue potion")) {
        return 0x63;
    }

    // Bows and arrows
    const bool arrows = has_any(s, {"arrow", "arrows"});
    if (has_any(s, {"quiver", "quivers"})) {
        return has_any(s, {"giant", "giants", "biggest", "largest"}) ? 0x56 : 0x55;
    }
    if (arrows && has_any(s, {"light", "fire", "ice", "silver", "bomb", "shock", "sacred",
                                 "magic", "golden", "progressive"}))
    {
        return 0x43;  // an arrow upgrade: the bow says that best
    }
    if (has_any(s, {"bow", "bows"})) {
        return 0x43;
    }
    if (arrows) {
        return by_amount(first_number(s), {{10, 0x0E}, {20, 0x0F}}, 0x10);
    }

    // Bombs
    if (has_any(s, {"bomb bag", "bomb bags", "bomb capacity", "bomb upgrade"})) {
        return has_any(s, {"big", "bigger", "biggest", "giant", "giants"}) ? 0x4F : 0x50;
    }
    if (has_any(s, {"water bomb", "water bombs"})) {
        return 0x16;
    }
    if (has_any(s, {"bombchu", "bombchus"})) {
        return first_number(s) >= 10 ? 0x1B : 0x1A;
    }
    if (has_any(s, {"bomb", "bombs"})) {
        const long n = first_number(s);
        return n < 0 ? 0x0A : by_amount(n, {{5, 0x0A}, {10, 0x0B}, {20, 0x0C}}, 0x0D);
    }

    // Rupees and wallets
    if (has_any(s, {"rupee", "rupees"}) && !has_any(s, {"wallet", "bag"})) {
        return rupees(s);
    }
    if (has_any(s, {"wallet", "wallets"})) {
        return has_any(s, {"giant", "giants", "tycoon", "tycoons", "biggest"}) ? 0x36 : 0x35;
    }

    // Equipment
    if (has_any(s, {"sword", "swords"})) {
        // Every metal sword is the Ordon Sword: held up, the randomizer shrinks the Master
        // Sword's model almost to nothing (it's only for overrides that ask for it).
        return has_any(s, {"wooden", "wood", "practice"}) ? 0x3F : 0x28;
    }
    if (has_any(s, {"shield", "shields"})) {
        if (has_any(s, {"wooden", "wood", "deku"})) {
            return 0x2B;
        }
        return has(s, "ordon") ? 0x2A : 0x2C;
    }
    if (has_any(s, {"zora tunic", "zora armor", "zora mail", "blue tunic", "blue mail"})) {
        return 0x31;
    }
    if (has_any(s, {"tunic", "tunics", "armor", "armour", "red mail", "progressive mail"})) {
        return 0x30;
    }
    if (has(s, "boots")) {
        return 0x45;
    }

    // Tools
    if (has_any(s, {"longshot", "double clawshot", "double clawshots", "double hookshot"})) {
        return 0x47;
    }
    if (has_any(s, {"hookshot", "hookshots", "clawshot", "clawshots"})) {
        return 0x44;
    }
    if (has_any(s, {"boomerang", "boomerangs"})) {
        return 0x40;
    }
    if (has_any(s, {"lantern", "lamp"})) {
        return 0x48;
    }
    if (has_any(s, {"slingshot", "bullet bag"})) {
        return 0x4B;
    }
    if (has_any(s, {"seed", "seeds"})) {
        return 0x12;
    }
    if (has_any(s, {"ball and chain", "iron ball"})) {
        return 0x42;
    }
    if (has(s, "spinner")) {
        return 0x41;
    }
    if (has_any(s, {"hawkeye", "hawk eye"})) {
        return 0x3E;
    }
    if (has(s, "dominion rod")) {
        return 0x46;
    }
    if (has_any(s, {"fishing rod", "fishing pole"})) {
        return 0x4A;
    }
    if (has_any(s, {"ocarina", "flute", "horse call", "whistle"})) {
        return 0x84;
    }

    // Collectibles
    if (has_any(s, {"mirror shard", "mirror shards"})) {
        return 0xA5;
    }
    if (has_any(s, {"fused shadow", "fused shadows"})) {
        return 0xD8;
    }
    if (has_any(s, {"sky book", "skybook", "sky character", "sky characters"})) {
        return 0xE9;
    }
    if (has_any(s, {"poe soul", "poe souls", "big poe", "big poes"})) {
        return 0xE0;
    }
    if (has(s, "letter")) {
        return 0x80;
    }

    // Last: plain "Key" (the first Zelda and Zelda II), after every kind of key above.
    if (has_any(s, {"key", "keys"})) {
        return 0x85;
    }
    return kNoModel;
}

}  // namespace

std::span<const Model> models() {
    return kModels;
}

std::optional<uint8_t> parse_model(std::string_view text) {
    const std::string s = normalize(text);
    if (s == " ") {
        return std::nullopt;
    }
    for (const auto keep : {"sol", "box", "default", "none"}) {
        if (s == normalize(keep)) {
            return kNoModel;
        }
    }
    // A number: decimal, or hex with 0x.
    std::string trimmed{text};
    while (!trimmed.empty() && std::isspace(static_cast<unsigned char>(trimmed.front()))) {
        trimmed.erase(trimmed.begin());
    }
    while (!trimmed.empty() && std::isspace(static_cast<unsigned char>(trimmed.back()))) {
        trimmed.pop_back();
    }
    if (!trimmed.empty() && std::isdigit(static_cast<unsigned char>(trimmed.front()))) {
        char* end = nullptr;
        const long n = std::strtol(trimmed.c_str(), &end, 0);
        if (end != nullptr && *end == '\0') {
            for (const auto& m : kModels) {
                if (m.id == n) {
                    return m.id;
                }
            }
        }
        return std::nullopt;
    }
    if (const auto id = by_name(s)) {
        return id;
    }
    for (const auto& a : kAliases) {
        if (normalize(a.name) == s) {
            return a.id;
        }
    }
    return std::nullopt;
}

bool is_zelda_game(std::string_view game) {
    std::string g;
    for (const char c : game) {
        g += lower(c);
    }
    for (const auto word : kZeldaWords) {
        if (g.find(word) != std::string::npos) {
            return true;
        }
    }
    return false;
}

uint8_t builtin_model(std::string_view game, std::string_view item) {
    const std::string s = normalize(item);
    if (s == " ") {
        return kNoModel;
    }
    // Another Twilight Princess: its items are ours, by our own names.
    if (twilight_princess(game)) {
        if (const auto id = by_name(s)) {
            return *id;
        }
    }
    // Only Zelda games' items match on their own; anything else needs the player's own rules
    // (item_models.yaml), so a look always means what it would in a Zelda game.
    if (!is_zelda_game(game)) {
        return kNoModel;
    }
    // A trap stays honest about being something else.
    if (has_any(s, {"trap", "traps", "rupoor", "rupoors", "fake"})) {
        return kNoModel;
    }
    return zelda(s);
}

bool glob_match(std::string_view pattern, std::string_view text) {
    // Iterative wildcard match: remember the last * and retry from one character further on.
    size_t p = 0, t = 0, star = std::string_view::npos, mark = 0;
    while (t < text.size()) {
        if (p < pattern.size() && pattern[p] == '*') {
            star = p++;
            mark = t;
        } else if (p < pattern.size() && lower(pattern[p]) == lower(text[t])) {
            ++p;
            ++t;
        } else if (star != std::string_view::npos) {
            p = star + 1;
            t = ++mark;
        } else {
            return false;
        }
    }
    while (p < pattern.size() && pattern[p] == '*') {
        ++p;
    }
    return p == pattern.size();
}

uint8_t model_for(std::string_view game, std::string_view item, std::span<const Rule> rules) {
    auto same = [](std::string_view a, std::string_view b) {
        if (a.size() != b.size()) {
            return false;
        }
        for (size_t i = 0; i < a.size(); ++i) {
            if (lower(a[i]) != lower(b[i])) {
                return false;
            }
        }
        return true;
    };
    // This game's own rules, then everyone's; in each, an exact name before a pattern.
    for (const bool anyGame : {false, true}) {
        for (const bool pattern : {false, true}) {
            for (const auto& r : rules) {
                const bool forAll = r.game == "*";
                if (forAll != anyGame || (!forAll && !same(r.game, game))) {
                    continue;
                }
                const bool isPattern = r.item.find('*') != std::string::npos;
                if (isPattern != pattern) {
                    continue;
                }
                if (isPattern ? glob_match(r.item, item) : same(r.item, item)) {
                    return r.model;
                }
            }
        }
    }
    return builtin_model(game, item);
}

const char* overrides_template() {
    return R"(# Other players' items that look like an item from Twilight Princess.
#
# With "Show as this game's items" on (Status page, F1), the mod already matches Zelda games'
# items by name: rupees, hearts, the hookshot, keys, maps, compasses, bombs, arrows, bows,
# bottles, swords, shields, tunics, boots, wallets and more. Other games' items only match
# through entries here. Items that don't match still show as their game's box, or a Sol.
#
# Add your own matches below: a game's name exactly as the room lists it (or "*" for every
# game), then under it an item name and what it should look like. A * in an item name matches
# anything, so "Small Key*" matches every small key. Put names in quotes when they start with
# * or contain a colon.
#
# What it looks like is one of this game's items, as the randomizer names them ("Progressive
# Clawshot", "Blue Rupee", "Piece of Heart", "Forest Temple Big Key", "Male Beetle"), or a short
# name: Rupee, Bombs, Arrows, Seeds, Bomblings, Heart Piece, Sword, Wooden Sword, Master Sword,
# Shield, Wooden Shield, Bow, Clawshot, Double Clawshot, Boomerang, Bottle, Red Potion,
# Blue Potion, Milk, Small Key, Big Key, Compass, Map, Wallet, Golden Bug, Mirror Shard,
# Fused Shadow, Sky Book. "Sol" keeps an item as it was (its game's box, or a Sol), even when
# it would match.
#
# Your entries win over the built-in matching, and a game's own entries over "*" ones.
# Changes apply after "Reload item models" (under Item models on the Status page), to items
# that appear after that.
#
# Examples (remove the # to use them):
#
# "Ocarina of Time":
#   "Deku Shield": Wooden Shield
# "Super Metroid":
#   "Missile": Arrows 10
#   "Grappling Beam": Clawshot
# "*":
#   "*Trap*": Sol
)";
}

}  // namespace ap::item_models
