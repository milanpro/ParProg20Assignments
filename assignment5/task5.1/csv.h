#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct hotspot
{
    int x;
    int y;
    // inclusive
    int startround;
    // exclusive
    int endround;
    struct hotspot *next_hotspot;
};

struct coord
{
    int x;
    int y;
    struct coord *next_coord;
};

struct hotspot *parse_hotspot_list(const char *hotspots_filename);
struct coord *read_coords(const char *coords_filename);
