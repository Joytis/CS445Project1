#pragma once
#include <functional>

namespace fsm {

class state {
private:
	using funtype = std::function<void(void)>;
	using funtype_move = std::function<void(void)>&&;
	funtype _enter;
	funtype _update;
	funtype _exit;
public:
	void enter();
	void update();
	void exit();

	state(funtype_move enter, funtype_move update, funtype_move exit);	
	// state(state&& s);
	state();
};

} // namespace fsm