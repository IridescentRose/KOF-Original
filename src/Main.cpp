#include <Platform/Platform.h>
#include <Graphics/RendererCore.h>
#include <Graphics/UI/UIText.h>

PSP_MODULE_INFO("Stardust", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_VFPU | THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);

using namespace Stardust;

int main() {
	Platform::initPlatform();

	Graphics::UI::UIText* ui = new Graphics::UI::UIText({ 240, 136 }, "Hello World!");
	ui->setOptions({ 0.5f, 0xFFFFFFFF, INTRAFONT_ALIGN_CENTER });

	while (true) {
		Graphics::g_RenderCore.BeginCommands();
		Graphics::g_RenderCore.Clear();

		ui->draw();

		Platform::platformUpdate();
		Graphics::g_RenderCore.EndCommands();
	}

	Platform::exitPlatform();

	return 0;
}