CC = gcc
SRC_DIR = src
MAIN_EXE = main
EXE_BUILD = build

LIB_SDL = -lSDL3
LIB_MIXER = -lSDL3_mixer

EXE_FINAL = -o $(EXE_BUILD)/game

all: create_build run_principal run_fichier_principal

create_build:
	@mkdir -p $(EXE_BUILD)

run_principal: $(MAIN_EXE)/main.c $(SRC_DIR)/controller.c $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/view_ncurses.c
	$(CC) $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/controller.c $(MAIN_EXE)/main.c $(EXE_FINAL) $(LIB_SDL) $(LIB_MIXER)

run_fichier_principal:
	./$(EXE_BUILD)/game

clean:
	rm -rf $(EXE_BUILD)/*

.PHONY: all create_build clean