#include "PauseHandler.h"
#include <Platform/Platform.h>
#include <Utilities/Input.h>

void pauseHandler()
{
	g_RenderCore.EndCommands();
	UIText* txt = new UIText({ 240, 136 }, "PAUSED.\n\nPress START to resume.\nPress CROSS to save (disabled).\n\nPress CIRCLE to exit.");

	g_RenderCore.SetClearColor(0, 0, 0, 255);
	while (true) {
		g_RenderCore.BeginCommands();
		g_RenderCore.Clear();

		txt->draw();

		Platform::platformUpdate();

		if (Utilities::KeyPressed(PSP_CTRL_START)) {
			break;
		}

		if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {
			//TODO: SAVE
		}

		if (Utilities::KeyPressed(PSP_CTRL_CIRCLE)) {
			sceKernelExitGame();
		}

		g_RenderCore.EndCommands();
	}
	g_RenderCore.EndCommands();
	g_RenderCore.BeginCommands();
}

void die()
{
	g_RenderCore.EndCommands();
	UIText* txt = new UIText({ 240, 136 }, "You died! Please press X to exit.");
	
	g_RenderCore.SetClearColor(127, 0, 0, 255);
	while (true) {
		g_RenderCore.BeginCommands();
		g_RenderCore.Clear();

		txt->draw();

		Platform::platformUpdate();

		if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {
			sceKernelExitGame();
		}

		g_RenderCore.EndCommands();
	}
	g_RenderCore.EndCommands();
	g_RenderCore.BeginCommands();
}

void endTutorial()
{
	g_RenderCore.EndCommands();
	UIText* txt = new UIText({ 240, 136 }, "This ends the tutorial.\n\nPress X to continue to\nyour brand new world!");

	g_RenderCore.SetClearColor(0, 63, 127, 255);
	while (true) {
		g_RenderCore.BeginCommands();
		g_RenderCore.Clear();

		txt->draw();

		Platform::platformUpdate();

		if (Utilities::KeyPressed(PSP_CTRL_CROSS)) {
			sceKernelExitGame();
		}

		g_RenderCore.EndCommands();
	}
	g_RenderCore.EndCommands();
	g_RenderCore.BeginCommands();
}
