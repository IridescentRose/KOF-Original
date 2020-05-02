#include <Platform/Platform.h>
#include <Graphics/RendererCore.h>
#include <Graphics/UI/UIText.h>
#include "Application.h"

PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;

int main() {
	Platform::initPlatform();
	Graphics::g_RenderCore.SetClearColor(255, 255, 255, 255);

	Application* app = new Application();
	app->run();

	while (app->isRunning()) {
		Graphics::g_RenderCore.BeginCommands();
		Graphics::g_RenderCore.Clear();

		app->update();
		app->draw();

		Platform::platformUpdate();
		Graphics::g_RenderCore.EndCommands();
	}

	delete app;
	Platform::exitPlatform();
	return 0;
}