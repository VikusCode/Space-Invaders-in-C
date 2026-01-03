# Lancement de programme
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

all: clean create_build run_principal 

create_build:
	@mkdir -p $(EXE_BUILD)
	@echo "Build crée"

run_principal: $(MAIN_EXE)/mainNC.c $(MAIN_EXE)/mainSDL.c $(SRC_DIR)/controller.c $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/view_ncurses.c
	$(CC) $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/controller.c $(MAIN_EXE)/mainSDL.c -o $(EXE_FINAL_SDL) $(LIBS_SDL) $(LIBS_NC)
	$(CC) $(SRC_DIR)/model.c $(SRC_DIR)/view_ncurses.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/controller.c $(MAIN_EXE)/mainNC.c -o $(EXE_FINAL_NC) $(LIBS_SDL) $(LIBS_NC) 
	@echo "Compilation faite ! Tape 'make run_SDL' ou 'make run_NC' pour lancer tout le programme"

run_SDL: $(EXE_FINAL_SDL)
	SDL_AUDIO_DRIVER=pulseaudio ./$(EXE_FINAL_SDL)

run_NC: $(EXE_FINAL_NC)
	./$(EXE_FINAL_NC)

valgrind_NC: $(EXE_FINAL_NC)
	timeout 10s valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=./doc/rapport_valgrind_NC.txt ./$(EXE_FINAL_NC) || true

valgrind_SDL: $(EXE_FINAL_SDL)
	timeout 10s valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=./doc/rapport_valgrind_SDL.txt ./$(EXE_FINAL_SDL) || true

valgrind: valgrind_NC valgrind_SDL

clean:
	rm -rf $(EXE_BUILD)/*
	rm -rf $(EXE_BUILD)
	@echo "Build supprime"

# Installation SDL3 et NC
SDL_PATH = SDL
MIXER_PATH = SDL_mixer

install_NC:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev
	@echo "Installation de ncurses terminée !"

install_ubuntu:
	@echo "Installation dépendances Ubuntu..."
	sudo apt-get update
	sudo apt-get install -y build-essential cmake git libncurses5-dev libncursesw5-dev
	# Dépendances audio/vidéo complètes
	sudo apt-get install -y libasound2-dev libpulse-dev \
	                        libx11-dev libxext-dev libxrandr-dev libxcursor-dev \
	                        libxi-dev libxinerama-dev libxss-dev libxtst-dev \
	                        libxxf86vm-dev libxfixes-dev libxrender-dev \
	                        libwayland-dev libxkbcommon-dev
	sudo apt-get install -y libflac-dev libvorbis-dev libopus-dev libmpg123-dev libogg-dev

install_libs_source:
	@echo "Compilation SDL3 depuis la source"
	cd $(SDL_PATH) && rm -rf build && mkdir build && cd build && \
	cmake -DCMAKE_BUILD_TYPE=Release .. && \
	make -j$$(nproc) && \
	sudo make install
	@echo "Compilation SDL_mixer depuis la source"
	cd $(MIXER_PATH) && rm -rf build && mkdir build && cd build && \
	cmake -DCMAKE_BUILD_TYPE=Release .. && \
	make -j$$(nproc) && \
	sudo make install
	sudo ldconfig
	@echo "Tout est installé !"

install_full_ubuntu: install_NC install_ubuntu install_libs_source

