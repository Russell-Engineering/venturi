#pragma once

#include <memory>

#include "oak/core/Application.h"
#include "oak/core/Layer.h"
// #include "oak/core/Image.h"


class VenturiUI : public Oak::Layer
{

public:
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUIRender() override;

    void SetDarkThemeColors();

private:
    // std::shared_ptr<Oak::Image> m_Image;

};
