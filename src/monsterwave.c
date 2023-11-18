#include "monsterwave.h"
#include <assert.h>
#include <stdio.h>

const MonsterWave WAVES[NB_MONSTER_WAVES] = {
    [WAVE_NORMAL] = {
        .type = WAVE_NORMAL,
        .probability = 50,
        .size = 12,
        .speed = 1,
    },
    [WAVE_CROWD] = {
        .type = WAVE_CROWD,
        .probability = 20,
        .size = 24,
        .speed = 1,
    },
    [WAVE_AGILE] = {
        .type = WAVE_AGILE,
        .probability = 20,
        .size = 12,
        .speed = 2,
    },
    [WAVE_BOSS] = {
        .type = WAVE_BOSS,
        .probability = 10,
        .size = 2,
        .speed = 1,
    },
};

static const char* MonsterWaveTypeString[NB_MONSTER_WAVES] = {
    [WAVE_NORMAL]   = "Normal",
    [WAVE_CROWD]    = "Crowd",
    [WAVE_AGILE]    = "Agile",
    [WAVE_BOSS]     = "Boss"
};

MonsterWave MonsterWave_get_wave(MonsterWaveType type) {
    assert(type >= 0 && type < NB_MONSTER_WAVES);
    return WAVES[type];
}

MonsterWave MonsterWave_new_random(int wave_counter) {
    MonsterWaveType wave;
    assert(wave_counter >= 0);

    do {
        static const int probabilities[NB_MONSTER_WAVES] = WAVES_PROBABILITIES;
        wave = weighted_selection(NB_MONSTER_WAVES, probabilities);
    } while (wave_counter < 5 && wave == WAVE_BOSS);

    return MonsterWave_get_wave(wave);
}

void MonsterWave_print(MonsterWave wave) {
    fprintf(
        stderr,
        "MonsterWave { type: %s, probability: %d, size: %d, speed: %d }\n",
        MonsterWaveTypeString[wave.type],
        wave.probability,
        wave.size,
        wave.speed
    );
}
