#pragma once

#include <unordered_map>
#include <vector>

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
	T get_current_state() { return _current_state; }
	T get_previous_state() { return _previous_state; }

	finite_state_system (T entry_state) 
		: _entry_state(entry_state), _current_state(entry_state)
	{
		// Add entry state to map. 
		if(transitions.find(entry_state) == transitions.end()) {
			transitions[entry_state] = std::vector<transition>();
		}
	}

	void add_transition(T source, T target, S conditions) {
		// Add the states to the map if they're not there already!
		if(transitions.find(source) == transitions.end()) {
			transitions[source] = std::vector<transition>();
		}
		if(transitions.find(target) == transitions.end()) {
			transitions[source] = std::vector<transition>();
		}
		transition t(target, conditions);
		transitions[source].push_back(std::move(t));
	}


	// Have mouse coords here. 
	void update() {
		// Check for state transition
		while(process_triggers()) {}
		// reset all triggers
		triggers = 0;
	}

	void reset() {
		if(_current_state != _entry_state) {
			transition_away(_entry_state);
		}
	}

	void set_trigger(S trigger) { triggers |= static_cast<u32>(trigger); } // Throw in the bit
	void release_trigger(S trigger) { triggers &= ~(static_cast<u32>(trigger)); } // Mask out the bit
	bool check_trigger(S trigger) { 
		return (static_cast<u32>(trigger) & triggers) == static_cast<u32>(trigger);
	}

private:

	void transition_away(T target) {
		// Swap state
		_previous_state = _current_state;
		_current_state = target;
	}

	bool process_triggers() {
		// Check all the transitions for valid states to move to. 
		for(auto& t : transitions[_current_state]) {
			// Check if the triggers have been fired bit bitwise ANDING
			// 		all the triggers with the trigger mask and checking 
			//		if the trigger mask remains.
			u32 condition = static_cast<u32>(t.condition);
			if((triggers & condition) == condition) {
				// Go away
				transition_away(t.target);
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