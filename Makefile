CC = gcc

# Dossier
SRC_DIR = src
MAIN_EXE = main
EXE_BUILD = build

# Bibliotheques necessaires
LIBS_SDL = -lSDL3 -lSDL3_mixer
LIBS_NC = -lncurses

# Fichier executable final
EXE_FINAL_NC = $(EXE_BUILD)/gameNC
EXE_FINAL_SDL = $(EXE_BUILD)/gameSDL

all: create_build run_principal 

create_build:
	@mkdir -p $(EXE_BUILD)

run_principal_SDL: $(MAIN_EXE)/mainNC.c $(MAIN_EXE)/mainSDL.c $(SRC_DIR)/controller.c $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/view_ncurses.c
	$(CC) $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/controller.c $(MAIN_EXE)/mainSDL.c -o $(EXE_FINAL_SDL) $(LIBS_SDL) $(LIBS_NC)
	@echo "Compilation faite ! Tape 'make run' pour lancer tout le programme"

run_principal_NC: $(MAIN_EXE)/mainNC.c $(MAIN_EXE)/mainSDL.c $(SRC_DIR)/controller.c $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/view_ncurses.c
	$(CC) $(SRC_DIR)/model.c $(SRC_DIR)/view_ncurses.c $(SRC_DIR)/controller.c $(MAIN_EXE)/mainNC.c -o $(EXE_FINAL_NC) $(LIBS_SDL) $(LIBS_NC) 


run_SDL: all
	SDL_AUDIO_DRIVER=pulseaudio ./$(EXE_FINAL_SDL)

run_NC: all
	./$(EXE_FINAL_NC)

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=rapport_valgrind.txt $(run)

clean:
	rm -rf $(EXE_BUILD)/*
	@echo "Build supprime"

.PHONY: all create_build run_principal run clean