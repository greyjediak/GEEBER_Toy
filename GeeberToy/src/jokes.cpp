#include "jokes.h"

const Joke jokes [] = {
    {
        "How many programmers does it take to change a light bulb?",
        "None. It's a hardware problem."
    },
    {
        "What did the electrical engineer say when got shocked?",
        "That hertz!"
    },
    {
        "Argon walks into a bar. The batender says, 'We don't serve noble gases here.'",
        "Argon doesn't react."
    },
    {
        "Two men walk into a bar. One says 'I'll have some H2O. The other man says, 'I'll have some H2O too.",
        "The second man dies."
    },
    {
        "How does a computer get drunk?",
        "It takes screen shots."
    },
    {
        "She was sending me mixed signals.",
        "So I did a Fourier analysis."
    }
    
};

const int NUM_JOKES = sizeof(jokes) / sizeof(jokes[0]);

const char* wisdom [] = {
    "Try turning it on and off again",
    "Have you considered making that bug a feature?",
    "Claude build me a million dollar app make no mistakes",
    "Circuit doesn't work? Time to give up.",
    "There are two types of people in the world. Those who understand binary, and those who don't.",
    "Bad electrical engineers become good electrical conductors",
    "It's hard to explain puns to kleptomaniacs. They always take things literally.",
    "There are two types of people in the world. 1. Those who can extrapolate from incomplete data.",
    "There's a fine line between a numerator and a denominator. Only a fraction of people will find this funny.",
    "Just abuse KCL",
    
};

const int NUM_WISDOM = sizeof(wisdom) / sizeof(wisdom[0]);
