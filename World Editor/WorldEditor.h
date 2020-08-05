#pragma once
#include "Launcher.h"
#include "Map.h"

class WorldEditor : public B
{
private:

	enum class STATE { LAUNCHER, EDITOR, SAVING } state;
	Launcher launcher;
	Map map;
	
public:

	WorldEditor() : B("init"), state(STATE::LAUNCHER) {
		while (wnd.isOpen()) 
		{
			SystemUpdate();
			while(wnd.pollEvent(event)) 
			{
				CloseEvent();

				switch (state)
				{
				case WorldEditor::STATE::LAUNCHER:
					if (launcher.IsAction()) {
						wnd.setView(wnd.getDefaultView()); // Ёкранна€ обработка
						const string& act = launcher.GetAct();
						if (act == "Quit") {
							wnd.close();
						}
						else if (act == "Start") {
							state = WorldEditor::STATE::EDITOR;
							map.Create(v2i(10, 10), "test");
						}
						wnd.setView(cam);
					}
					break;
				case WorldEditor::STATE::EDITOR:
					map.Action();
					if (IsKeyPressed(Key::Left))	cam.move(-50, 0);
					if (IsKeyPressed(Key::Right))	cam.move( 50, 0);
					if (IsKeyPressed(Key::Up))		cam.move(0, -50);
					if (IsKeyPressed(Key::Down))	cam.move(0,  50);
					wnd.setView(cam);

					break;
				case WorldEditor::STATE::SAVING:
					break;
				}
			}
			
			wnd.clear();
			
			switch (state)
			{
			case WorldEditor::STATE::LAUNCHER:
				wnd.setView(wnd.getDefaultView()); // Ёкранна€ обработка
				launcher.Draw();
				wnd.setView(cam);
				break;
			case WorldEditor::STATE::EDITOR:
				map.Update();
				map.Draw();
				
				break;
			case WorldEditor::STATE::SAVING:
				break;
			}

			wnd.display();
		}
	}

	~WorldEditor() {

	}
};