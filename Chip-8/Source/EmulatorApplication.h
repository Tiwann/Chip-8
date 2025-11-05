#pragma once
#include "Runtime/Application.h"
#include "Emulator.h"
#include "Rendering/ShaderBindingSet.h"

using Nova::Application;
using Nova::ApplicationConfiguration;
using Nova::Ref;
using Nova::Rendering::CommandBuffer;
using Nova::Rendering::GraphicsPipeline;
using Nova::Rendering::Shader;
using Nova::Rendering::Sampler;
using Nova::Rendering::Texture;
using Nova::Rendering::ShaderBindingSet;

class EmulatorApplication final : public Application
{
public:
    ApplicationConfiguration GetConfiguration() const override;
    void OnInit() override;
    void OnUpdate(float deltaTime) override;
    void OnRender(CommandBuffer& cmdBuffer) override;
    void OnGUI() override;
private:
    Ref<Emulator> m_Emulator = nullptr;
    Ref<Texture> m_Texture = nullptr;
    Ref<Sampler> m_Sampler = nullptr;
    Ref<Shader> m_Shader = nullptr;
    Ref<GraphicsPipeline> m_Pipeline = nullptr;
    Ref<ShaderBindingSet> m_BindingSet = nullptr;
};
