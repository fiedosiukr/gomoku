#pragma once
#include <allegro5/allegro.h>

#include "playstate.h"

class Game {
public:
	Game();
	~Game();
	void start();
private:
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* event_queue;
	ALLEGRO_TIMER* timer;

	PlayState* play_state;

	bool running;
	bool updated;

	void run();
	void update();
	void render();
	void check_events(ALLEGRO_EVENT);
};

