#pragma once

#include <memory>

#include "Application.h"
#include "Layer.h"
#include "Image.h"


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
