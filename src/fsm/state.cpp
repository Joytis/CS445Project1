#include "state.hpp"

using namespace fsm;

void state::enter() {
	_enter();
}

void state::update() {
	_update();
}

void state::exit() {
	_exit();
}

state::state(funtype_move enter, funtype_move update, funtype_move exit)
	: _enter(std::move(enter)), _update(std::move(update)), _exit(std::move(exit))
{
}

// Make a state that does nothing. 
state::state()
	: _enter([](){}), _update([](){}), _exit([](){})
{}
