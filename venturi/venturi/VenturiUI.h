#pragma once

#include <memory>

#include "Walnut/Application.h"
#include "Walnut/Layer.h"
#include "Walnut/Image.h"


class VenturiUI : public Walnut::Layer
{

public:
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUIRender() override;

    void SetDarkThemeColors();

private:
    std::shared_ptr<Walnut::Image> m_Image;

};
