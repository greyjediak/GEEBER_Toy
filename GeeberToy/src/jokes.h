#pragma once

// Question based jokes
struct Joke {
    const char *setup;
    const char *punchline;
};

extern const Joke jokes[];
extern const int NUM_JOKES;

// Statement only, funny advice and sayings
extern const char *wisdom[];
extern const int NUM_WISDOM;