CC = gcc

# Dossier
SRC_DIR = src
MAIN_EXE = main
EXE_BUILD = build

# Bibliotheque necessaire 
LIBS = -lSDL3 -lSDL3_mixer -lncurses

# Fichier executable final
EXE_FINAL = $(EXE_BUILD)/game

all: create_build run_principal 

create_build:
	@mkdir -p $(EXE_BUILD)

run_principal: $(MAIN_EXE)/main.c $(SRC_DIR)/controller.c $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/view_ncurses.c
	$(CC) $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/view_ncurses.c $(SRC_DIR)/controller.c $(MAIN_EXE)/main.c -o $(EXE_FINAL) $(LIBS)
	@echo "Compilation faite ! Tape 'make run' pour lancer tout le programme"

run: all
	SDL_AUDIO_DRIVER=pulseaudio ./$(EXE_FINAL)

clean:
	rm -rf $(EXE_BUILD)/*
	@echo "Build supprime"

.PHONY: all create_build run_principal run clean