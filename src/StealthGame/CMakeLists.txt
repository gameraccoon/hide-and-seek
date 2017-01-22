aux_source_directory(./src/Actors STEALTH_GAME_SRC)
aux_source_directory(./src/AI STEALTH_GAME_SRC)
add_library(StealthGameLib ${STEALTH_GAME_SRC})