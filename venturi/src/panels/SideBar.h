#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{
    class SideBar : public Oak::Panel
    {
    public:
        SideBar(const std::string& name, bool open, UI* parent);
        ~SideBar() = default;

        void SetLocalStyle() override;
        void OnUIRender(bool* open) override;
        
        UI* GetParent() { return m_Parent; }
        

    private:
        UI* m_Parent;

        Oak::Ref<Oak::Texture2D> m_SettingsIcon;
        Oak::Ref<Oak::Texture2D> m_MenuIcon;
        Oak::Ref<Oak::Texture2D> m_ExplorerIcon;
        Oak::Ref<Oak::Texture2D> m_ConnectionIcon;
        Oak::Ref<Oak::Texture2D> m_ModuleIcon;
        Oak::Ref<Oak::Texture2D> m_ChartIcon;
        Oak::Ref<Oak::Texture2D> m_AddPlotIcon;
        Oak::Ref<Oak::Texture2D> m_ConsoleIcon;
    };

}