#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>
#include "state.hpp"

namespace fsm {

template<typename T, typename S>
class finite_state_system {
private:

	using u32 = unsigned int;

	class transition {
	public:
		T target;
		S condition;

		transition(T targ, S cond) : target(targ), condition(cond) {}
	};

	std::unordered_map<T, state> states;
	std::unordered_map<T, std::vector<transition>> transitions;

	// Storage location for current triggers of finite state system. 
	u32 triggers = 0;

	// Should be an enumerated type
	T _entry_state;

	T _target_state;
	T _current_state;
	T _previous_state;	


public:
	
	T get_target_state() { return _target_state; }
	T get_current_state() { return _target_state; }
	T get_previous_state() { return _target_state; }

	finite_state_system (T entry_state) 
		: _entry_state(entry_state), _current_state(entry_state)
	{}

	void add_state(T key, state&& new_state)  {
		// Create that new state!
		if(states.find(key) == states.end()) {
			states.insert(std::make_pair(key, std::move(new_state)));
			transitions[key] = std::vector<transition>();
		}
		else {
			std::cout << "***ERROR. STATE EXISTS" << std::endl;
		}
	}

	state&& remove_state(T key) {
		if(states.find(key) != states.end() && key == _entry_state) {
			state s = states[key];
			states.erase(key);
			transitions.erase(key);
			return std::move(s);
		}
		else {
			std::cout << "State does not exist or state is entry state!" << std::endl;
			// Return a state that doesn't do anything. 
			return std::move(state([](){}, [](){}, [](){}));
		}
	} 

	void add_transition(T source, T target, S conditions) {
		if((states.find(source) == states.end()) ||
		   (transitions.find(source) == transitions.end())) {
			std::cout << "ERROR: source state doesn't exist" << std::endl;
		}
		if((states.find(target) == states.end()) || 
		   (transitions.find(target) == transitions.end())) {
			std::cout << "ERROR: target state doesn't exist" << std::endl;
		}

		transition t(target, conditions);
		transitions[source].push_back(std::move(t));
	}


	// Have mouse coords here. 
	void update(float x, float y) {
		// Check for state transition
		while(process_triggers(x, y)) {}
		// reset all triggers
		triggers = 0;
		// Update next state. 
		states[_current_state].update(x, y);
	}

	void reset(float x, float y) {
		if(_current_state != _entry_state) {
			transition_away(_entry_state, x, y);
		}
	}

	void set_trigger(S trigger) { triggers |= static_cast<u32>(trigger); } // Throw in the bit
	void release_trigger(S trigger) { triggers &= ~(static_cast<u32>(trigger)); } // Mask out the bit
	bool check_trigger(S trigger) { 
		return (static_cast<u32>(trigger) & triggers) == static_cast<u32>(trigger);
	}

private:

	void transition_away(T target, float x, float y) {
		// Exit current state
		_target_state = target;
		states[_current_state].exit(x, y);

		// Swap state
		_previous_state = _current_state;
		_current_state = target;

		states[_current_state].enter(x, y);
	}

	bool process_triggers(float x, float y) {
		// Check all the transitions for valid states to move to. 
		for(auto& t : transitions[_current_state]) {
			// Check if the triggers have been fired bit bitwise ANDING
			// 		all the triggers with the trigger mask and checking 
			//		if the trigger mask remains.
			u32 condition = static_cast<u32>(t.condition);
			if((triggers & condition) == condition) {
				// Go away
				transition_away(t.target, x, y);
				// Eat the conditions
				triggers &= (~condition);
				// We're good. Get outta here. 
				return true;
			}
		}
		return false;
	}

};

} // namespace fsm