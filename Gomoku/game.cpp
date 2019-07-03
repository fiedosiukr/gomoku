#include "game.h"

#include <iostream>
#include <allegro5/allegro_primitives.h>

#include "constants.h"


Game::Game() {
	if (!al_init())
		std::cout << "Failed to initialize allegro!\n";

	if (!al_init_primitives_addon())
		std::cout << "Failed to initialize primitives addon!\n";

	if (!al_install_mouse())
		std::cout << "Failed to install mouse!\n";

	if (!al_init_font_addon())
		std::cout << "Failed to initialize font addon!";

	if (!al_init_ttf_addon())
		std::cout << "Failed to intialize ttf addon!";

	timer = al_create_timer(1.0 / FPS);
	if (!timer)
		std::cout << "Failed to create timer!\n";

	display = al_create_display(WIDTH, HEIGHT);
	if (!display)
		std::cout << "Failed to create display!\n";

	event_queue = al_create_event_queue();
	if (!event_queue)
		std::cout << "Failed to create event_queue!\n";

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	play_state = new PlayState();
}

Game::~Game() {
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
}

void Game::start() {
	running = true;
	run();
}

void Game::run() {
	updated = false;

	while (running) {
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		check_events(event);

		if (updated && al_is_event_queue_empty(event_queue)) {
			render();
		}
	}
}

void Game::update() {
	play_state->update();
}

void Game::render() {
	updated = false;

	play_state->render();
	al_flip_display();
}

void Game::check_events(ALLEGRO_EVENT event) {
	if (event.type == ALLEGRO_EVENT_TIMER) {
		update();
		updated = true;
	}
	else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		running = false;
	else
		play_state->check_events(event);
}
