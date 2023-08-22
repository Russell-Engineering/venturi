#pragma once

#include "Oak.h"
#include "wi.h"
#include "vk.h"

#include "events/DataEvents.h"

namespace wi::Venturi
{
    class DataLayer : public Oak::Layer
    {

    public:

        DataLayer(DataManager& dataManager);
        ~DataLayer();
        void OnAttach() override; // called when layer is pushed to layerstack
        void OnDetach() override; // called when layer is popped from layerstack
        void OnUpdate(Timestep ts) override; // called every frame, use this to only clear the buffer for now
        void OnUIRender() override {} // called every frame to draw ImGui ui, this is called by the engine between ImGuiLayer::Begin() and End() calls, which creates the new ImGui frames for us. 
        void OnEvent(Oak::Event& e) override;


    public:
        bool OnStream(DataStreamEvent& e);
        bool OnClear(DataClearEvent& e);
        bool OnCreate(DataSeriesCreateEvent& e);
        bool OnRemove(DataSeriesRemoveEvent& e);

    private:
        Ref<wi::DataManager> m_DataManager;
        Ref<Clock> m_Clock;

    };
}