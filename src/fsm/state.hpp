#pragma once
#include <functional>

namespace fsm {

class state {
private:
	using funtype = std::function<void(float, float)>;
	using funtype_move = std::function<void(float, float)>&&;

	funtype _enter;
	funtype _update;
	funtype _exit;
public:
	void enter(float x, float y);
	void update(float x, float y);
	void exit(float x, float y);

	state(funtype_move enter, funtype_move update, funtype_move exit);	
	// state(state&& s);
	state();
};

} // namespace fsm