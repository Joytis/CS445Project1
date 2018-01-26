#include "state.hpp"

using namespace fsm;

void state::enter(float x, float y) {
	_enter(x, y);
}

void state::update(float x, float y) {
	_update(x, y);
}

void state::exit(float x, float y) {
	_exit(x, y);
}

state::state(funtype_move enter, funtype_move update, funtype_move exit)
	: _enter(std::move(enter)), _update(std::move(update)), _exit(std::move(exit))
{
}

// Make a state that does nothing. 
// Stuff a bunch of lambdas in it. 
state::state()
	: _enter(std::move([](float x, float y){})), 
	  _update(std::move([](float x, float y){})), 
	  _exit(std::move([](float x, float y){}))
{}
