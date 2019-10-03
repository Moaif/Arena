#include "Menu.h"
#include "..//Engine/Application.h"
#include "..//Engine/Modules/ModuleRender.h"
#include "..////Engine/Modules/ModuleFont.h"
#include "../Engine/Modules/ModuleInput.h"
#include "../Engine/Modules/ModuleGameScene.h"

RTTI_REGISTER(Menu)

bool Menu::Start()
{
	bool ret = GameScene::Start();

	font = Fonts->GetFont("Red", __FILE__, fontLineReference = __LINE__);

	return ret;
}

update_status Menu::Update()
{
	update_status ret = GameScene::Update();

	Renderer->DirectPrint(font,0,0,"Press Return Button To Play",3.0f);

	if (Input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		Scene->ReplaceSceneWithNew("Game");
	}

	return ret;
}

bool Menu::CleanUp()
{
	bool ret = GameScene::CleanUp();

	Fonts->FreeFont(&font, __FILE__, fontLineReference);

	return ret;
}
