# Installation SDL3 et NC
SDL_PATH = SDL
MIXER_PATH = SDL_mixer

install_NC:
	sudo apt-get update
	sudo apt-get install -y libncurses5-dev libncursesw5-dev
	@echo "Installation de ncurses terminée !"

install_system_deps:
	@echo "🛠️  Installation des dépendances système (Drivers, CMake, formats audio)..."
	sudo apt-get update
	sudo apt-get install -y build-essential cmake git
	# Dépendances pour l'affichage et le son (PulseAudio, X11, Wayland)
	sudo apt-get install -y libasound2-dev libpulse-dev libx11-dev libxext-dev \
	                        libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev \
	                        libwayland-dev libxkbcommon-dev
	# Dépendances pour les formats audio (MP3, OGG, FLAC) pour le Mixer
	sudo apt-get install -y libflac-dev libvorbis-dev libopus-dev libmpg123-dev libogg-dev

# 2. Compiler et Installer SDL3
install_sdl3:
	@echo "🚀 Compilation et Installation de SDL3..."
	# On nettoie, on configure, on compile et on installe
	cd $(SDL_PATH) && rm -rf build && mkdir build && cd build && \
	cmake -DCMAKE_BUILD_TYPE=Release .. && \
	make -j$$(nproc) && \
	sudo make install

# 3. Compiler et Installer SDL_mixer
install_sdl_mixer:
	@echo "🎵 Compilation et Installation de SDL_mixer..."
	cd $(MIXER_PATH) && rm -rf build && mkdir build && cd build && \
	cmake -DCMAKE_BUILD_TYPE=Release .. && \
	make -j$$(nproc) && \
	sudo make install

# 4. Mettre à jour les liens dynamiques (Pour que Linux trouve les fichiers .so)
refresh_libs:
	@echo "🔄 Mise à jour du cache des librairies..."
	sudo ldconfig
	@echo "✅ Installation complète terminée avec succès !"

install_full: install_NC install_system_deps install_sdl3 install_sdl_mixer refresh_libs

install_deps_fedora:
	@echo "🤠 Installation des dépendances Ncurses pour Fedora..."
	sudo dnf check-update || true
	sudo dnf install -y ncurses-devel
	@echo "✅ Ncurses installé."

# 2. Installation Complète (Outils système + SDL3 + Mixer depuis la source)
install_full_fedora: install_sys_fedora install_sdl3 install_sdl_mixer refresh_libs

install_sys_fedora:
	@echo "🤠 Installation des outils de compilation et drivers audio/vidéo..."
	# Outils de base (GCC, Make, CMake, Git)
	sudo dnf install -y gcc gcc-c++ make cmake git
	
	# Dépendances graphiques (X11, Wayland)
	sudo dnf install -y libX11-devel libXext-devel libXrandr-devel \
	                    libXcursor-devel libXi-devel libXinerama-devel \
	                    wayland-devel libxkbcommon-devel \
	                    libXScrnSaver-devel
	
	# Dépendances Audio (ALSA, PulseAudio, Pipewire)
	sudo dnf install -y alsa-lib-devel pulseaudio-libs-devel pipewire-devel
	
	# Codecs Audio pour SDL_mixer (MP3, OGG, FLAC)
	sudo dnf install -y flac-devel libvorbis-devel opus-devel mpg123-devel libogg-devel

# Lancemeent de programme
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

all: install_NC create_build run_principal 

create_build:
	@mkdir -p $(EXE_BUILD)

run_principal: $(MAIN_EXE)/mainNC.c $(MAIN_EXE)/mainSDL.c $(SRC_DIR)/controller.c $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/view_ncurses.c
	$(CC) $(SRC_DIR)/model.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/controller.c $(MAIN_EXE)/mainSDL.c -o $(EXE_FINAL_SDL) $(LIBS_SDL) $(LIBS_NC)
	$(CC) $(SRC_DIR)/model.c $(SRC_DIR)/view_ncurses.c $(SRC_DIR)/view_sdl.c $(SRC_DIR)/controller.c $(MAIN_EXE)/mainNC.c -o $(EXE_FINAL_NC) $(LIBS_SDL) $(LIBS_NC) 
	@echo "Compilation faite ! Tape 'make run' pour lancer tout le programme"

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