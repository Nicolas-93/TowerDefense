#ifndef MONSTERWAVE_H
#define MONSTERWAVE_H

#define NB_MONSTER_WAVES 4

typedef enum MonsterWaveType {
    WAVE_NORMAL,
    WAVE_CROWD,
    WAVE_AGILE,
    WAVE_BOSS
} MonsterWaveType;

typedef struct MonsterWave {
    MonsterWaveType type;
    int probability;
    int size;
    int speed;
} MonsterWave;

#define WAVES_PROBABILITIES { \
    [WAVE_NORMAL] = 50, \
    [WAVE_CROWD] = 20, \
    [WAVE_AGILE] = 20, \
    [WAVE_BOSS] = 10, \
}

extern const MonsterWave WAVES[NB_MONSTER_WAVES];

/**
 * @brief Get the wave object associated to the given type
 * 
 * @param type 
 * @return MonsterWave 
 */
MonsterWave MonsterWave_get_wave(MonsterWaveType type);

/**
 * @brief Get a random wave
 * 
 * @param wave_counter Current wave number
 * (used to avoid boss waves, for the first 5 waves)
 * @return MonsterWave 
 */
MonsterWave MonsterWave_new_random(int wave_counter);

/**
 * @brief Show wave characteristics
 * 
 * @param wave 
 */
void MonsterWave_print(MonsterWave wave);

#endif
