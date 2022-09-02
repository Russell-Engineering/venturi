#pragma once

#include "oak.h"


namespace Venturi
{
    struct Resources
    {
        // application icons
        inline static Oak::Ref<Oak::Texture2D> AppLogo = nullptr;
        inline static Oak::Ref<Oak::Texture2D> MinimizeIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> MaximizeIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> RestoreIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> CloseIcon = nullptr;

        inline static Oak::Ref<Oak::Texture2D> MenuIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> ExplorerIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> ConnectionIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> ChartIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> ConsoleIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> AddPlotIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> SettingsIcon = nullptr;

        inline static Oak::Ref<Oak::Texture2D> RefreshIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> NewFileIcon = nullptr;
        inline static Oak::Ref<Oak::Texture2D> NewDirIcon = nullptr;



        static void Init()
        {
            OAK_CORE_TRACE_TAG("Resources::Init", "Initializing application resources...");
            AppLogo =       LoadTexture("assets/textures/wi-logo-gradient-2-24.png");
            MinimizeIcon =  LoadTexture("assets/textures/icons8-subtract-16.png");
            MaximizeIcon =  LoadTexture("assets/textures/icons8-maximize-button-16.png");
            RestoreIcon =   LoadTexture("assets/textures/icons8-restore-down-16.png");
            CloseIcon =     LoadTexture( "assets/textures/icons8-close-16.png");

            MenuIcon =      LoadTexture("assets/textures/icons8-menu-32.png");
            ExplorerIcon =  LoadTexture("assets/textures/icons8-documents-32.png");
            ConnectionIcon =LoadTexture("assets/textures/icons8-broadcasting-32.png");
            ChartIcon =     LoadTexture("assets/textures/icons8-combo-chart-32.png");
            ConsoleIcon =   LoadTexture("assets/textures/icons8-console-32.png");
            AddPlotIcon =   LoadTexture("assets/textures/icons8-add-new-32.png");
            SettingsIcon =  LoadTexture("assets/textures/icons8-services-32.png");

            RefreshIcon =   LoadTexture("assets/textures/icons8-restart-16.png");
            NewFileIcon =   LoadTexture("assets/textures/icons8-add-file-16.png");
            NewDirIcon =    LoadTexture("assets/textures/icons8-add-folder-16.png");


            OAK_CORE_TRACE_TAG("Resources::Init", "... done");
        }

        static void Shutdown()
        {
                AppLogo.Reset();
                MinimizeIcon.Reset();
                MaximizeIcon.Reset();
                RestoreIcon.Reset();
                CloseIcon.Reset();
                MenuIcon.Reset();
                ExplorerIcon.Reset();
                ConnectionIcon.Reset();
                ChartIcon.Reset();
                ConsoleIcon.Reset();
                AddPlotIcon.Reset();
                SettingsIcon.Reset();
                RefreshIcon.Reset();
                NewFileIcon.Reset();
                NewDirIcon.Reset();

        }
    private:
        static Oak::Ref<Oak::Texture2D>  LoadTexture(const std::filesystem::path& relativePath)
        {
            if (!Oak::FileSystem::Exists(relativePath))
            {
                OAK_FATAL_TAG("Resources::LoadTexture", "Failed to load texture {0}. The file does not exist", relativePath.string());
                return nullptr;
            }
            return Oak::Texture2D::Create(relativePath.string());
        }

    };
}