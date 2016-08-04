//
//  main.m
//  TestAllegro
//
//  Created by Redefine on 1/22/13.
//  Copyright (c) 2013 Redefine. All rights reserved.
//

#include "Utils/MXInclude.h"
#include "Application/MXApp.h"
#include "Application/MXApp.h"
#include "Graphic/MXDisplay.h"
#include "Graphic/MXBlender.h"
#include "Graphic/Renderers/MXTextureRenderer.h"
#include "Game/Resources/MXPaths.h"
#include "Game/Resources/MXResources.h"
#include "Sound/MXSample.h"
#include "Sound/MXStream.h"
#include "Utils/MXDebugGUIManager.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"


#ifndef MX_PLATFORM_XCODE
#include "windows.h"
#include "mmsystem.h"

#pragma comment(lib, "winmm.lib")
#endif

unsigned _width = 1280, _height = 800;
bool _fullscreen = false;

using namespace std;
using namespace MX;
class GameApp : public MX::App
{
public:
	GameApp()
	{

	}

protected:
      

	void OnPrepare()
	{
#ifndef MX_GAME_RELEASE
        auto res_path = MX_DEBUG_RES_PATH;
#else
        #if MX_PLATFORM_XCODE
        auto res_path = MX_DEBUG_RES_PATH;
        #else
        auto res_path = "Res/";
        #endif
#endif

#ifdef MX_GAME_PERF_CHEATS
		res_path = MX_DEBUG_RES_PATH;
#endif
        
        addAssetDirectory(res_path);
        MX::Paths::get().SetResourcePath(res_path);
        MX::Paths::get().SetImagePath(res_path);
        MX::Paths::get().SetSoundPath(res_path);
        MX::Paths::get().SetStreamPath(res_path);
        
        
        //#if MX_PLATFORM_XCODE

		OpenWindow(_width, _height, _fullscreen);
	}

	bool OnInit()
	{
		using namespace ci;
		using namespace ci::gl;

		Sound::StreamManager::get().SetDefaultVolume(0.3f);
		
		//BH::GameGraphicManager::get().PrepareGL();

#ifndef MX_PLATFORM_XCODE
		timeBeginPeriod(1);
#endif

		static bool supported_instancing = ci::gl::isExtensionAvailable("GL_ARB_draw_instanced"); //TODO move it to a better place

		if (!supported_instancing)
			throw std::exception("Instancing not supported!");
		
#ifdef MX_GAME_RELEASE
		auto cursor = MX::Resources::get().loadCenteredImage(15.0f, 15.0f, "Misc/Crosshair.png");
		SetCursor(cursor);
#endif
		return true;
	}

	void OnCleanup()
	{
#ifndef MX_PLATFORM_XCODE
		timeEndPeriod(1);
#endif
	} 
	 

	void OnLoop()
	{

	}

	void OnRender()
	{
		static bool firstFrame = true;

		if (firstFrame)
		{
			firstFrame = false;
			RenderFirstFrame();
			return;
		}


		using namespace ci;
		gl::clear(ci::Color(0.1f, 0.1f, 0.1f), false);
        


		Graphic::TextureRenderer::current().Flush();
		MX::DebugGUIManager::get().Draw();
	}

	void RenderFirstFrame()
	{
		using namespace ci;
		gl::clear(ci::Color(0.1f, 0.1f, 0.1f), false);
	}


public:

	static void prepareSettingsStatic(Settings *settings)
	{
		App::prepareSettingsStatic(settings);
		settings->setTitle(MX_APPLICATION_NAME);
		settings->setWindowSize(_width, _height);
		settings->setResizable(false);
		settings->setFullScreen(_fullscreen);
	}
};


using namespace ci;
using namespace ci::app;


#define CI_RENDERER RendererGl(RendererGl::Options())

#ifdef MX_PLATFORM_XCODE

CINDER_APP(GameApp, CI_RENDERER, &GameApp::prepareSettingsStatic)

#else

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{		
	std::string exc_s;
	try {
		cinder::app::RendererRef renderer(new CI_RENDERER);
		cinder::app::AppMsw::main<GameApp>(renderer, MX_APPLICATION_NAME, &GameApp::prepareSettingsStatic);
	}
	catch (std::exception &exc) {
		exc_s = exc.what();
	}


	if (!exc_s.empty())
	{
		std::future<void> fut = std::async([=]() { MessageBoxA(0, exc_s.c_str(), "Error", 0); });
		fut.get();
	}

	
	return 0;																		
}
#endif
