#include <iostream>
#include "Core/Core.h"
#include "Timer/Time.h"

int main(int argc, char* args[]) {

	Core& core = Core::get();
	core.init();
	Time& time = Time::get();

	while (core.isRunning()) {	
		core.events();
		core.update(time.getDeltatime());
		core.render();
		time.tick();
	}

	core.clean();
	return 0;
}