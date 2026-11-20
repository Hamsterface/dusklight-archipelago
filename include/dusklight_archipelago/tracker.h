#pragma once

/*
 * Dusklight Archipelago's tracker, for other mods: every check in the connected room and
 * whether it's done, in logic now, not yet, or not needed (its item belongs to a player who has
 * already finished). The answers are the in-game tracker's own, from the seed's logic.
 *
 * Import it optionally, so your mod still loads without Dusklight Archipelago:
 *
 *     #include <dusklight_archipelago/tracker.h>
 *     IMPORT_OPTIONAL_SERVICE(ApTrackerService, svc_ap_tracker);
 *
 *     if (svc_ap_tracker != NULL) { ... }
 *
 * Check names are Archipelago location names, as the tracker shows them. Call from the game
 * thread only. Strings returned by check_at stay valid until state_version changes.
 */

#include <mods/api.h>

#ifdef __cplusplus
#include <mods/service.hpp>
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define AP_TRACKER_SERVICE_ID "com.noahsmaximum.dusklight_archipelago.tracker"
#define AP_TRACKER_SERVICE_MAJOR 1u
#define AP_TRACKER_SERVICE_MINOR 0u

typedef enum ApCheckState {
    AP_CHECK_UNKNOWN = 0,    /* logic isn't ready (not connected yet, or still working it out) */
    AP_CHECK_NOT_YET = 1,    /* not reachable with what the player has */
    AP_CHECK_IN_LOGIC = 2,   /* reachable now */
    AP_CHECK_DONE = 3,       /* collected */
    AP_CHECK_NOT_NEEDED = 4, /* not collected, but its item's owner has finished their game */
} ApCheckState;

typedef struct ApCheckInfo {
    uint32_t struct_size; /* set by the caller: sizeof(ApCheckInfo) */
    const char* name;     /* the Archipelago location name */
    const char* region;   /* the tracker's region for it, e.g. "Faron" */
    uint32_t state;       /* ApCheckState */
} ApCheckInfo;

typedef struct ApTrackerService {
    ServiceHeader header;

    /* Changes whenever any answer below may have changed. Poll it each frame and re-read only
     * when it moves. */
    ModResult (*state_version)(ModContext* ctx, uint64_t* out_version);
    /* Whether an Archipelago save is being played with its room's checks known. When false,
     * there are no checks. */
    ModResult (*is_active)(ModContext* ctx, bool* out_active);
    ModResult (*check_count)(ModContext* ctx, size_t* out_count);
    /* MOD_INVALID_ARGUMENT for an index past check_count. */
    ModResult (*check_at)(ModContext* ctx, size_t index, ApCheckInfo* out_info);
    /* MOD_INVALID_ARGUMENT if the room has no check by that name. */
    ModResult (*check_state)(ModContext* ctx, const char* name, uint32_t* out_state);
} ApTrackerService;

MOD_DECLARE_SERVICE(ApTrackerService, svc_ap_tracker, AP_TRACKER_SERVICE_ID,
    AP_TRACKER_SERVICE_MAJOR, AP_TRACKER_SERVICE_MINOR);
