#pragma once
#include "System.h"
class Game : public B {
private:

public:

	Game() : B("init") {
		while (wnd.isOpen())
		{
			System::SystemUpdate();
			while (wnd.pollEvent(event)) {
				CloseEvent();
			}

			wnd.clear();
			wnd.display();
		}
	}

	~Game() {

	}
};