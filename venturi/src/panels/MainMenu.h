#pragma once

#include "oak.h"
#include "UI.h"

namespace Venturi
{
    class MainMenu : public Oak::Panel
    {
    public:
        MainMenu(const std::string& name, bool open, UI* parent);
        ~MainMenu() = default;
        
        void SetLocalStyle() override;
        void OnUIRender(bool* open) override;
        
        UI* GetParent() { return m_Parent; }
    
    private:
        UI* m_Parent = nullptr;
        
        Oak::Ref<Oak::Texture2D> m_AppLogo;
        Oak::Ref<Oak::Texture2D> m_MinimizeIcon;
        Oak::Ref<Oak::Texture2D> m_MaximizeIcon;
        Oak::Ref<Oak::Texture2D> m_RestoreIcon;
        Oak::Ref<Oak::Texture2D> m_CloseIcon;

    };

}
