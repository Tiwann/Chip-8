#include "EmulatorApplication.h"
#include "Runtime/EntryPoint.h"
#include "Runtime/Path.h"
#include "Rendering/Texture.h"
#include <imgui.h>

#include "Rendering/CommandBuffer.h"
#include "Rendering/GraphicsPipeline.h"
#include "Rendering/Sampler.h"
#include "Rendering/Shader.h"
#include "Runtime/DesktopWindow.h"


using namespace Nova;
using namespace Nova::Rendering;

NOVA_DEFINE_APPLICATION_CLASS(EmulatorApplication);

static constinit uint32_t WINDOW_SCALE = 16;

ApplicationConfiguration EmulatorApplication::GetConfiguration() const
{
    ApplicationConfiguration config;
    config.applicationName = "CHIP-8 Emulator [Nova Engine]";
    config.vsync = true;
    config.windowWidth = 64 * WINDOW_SCALE;
    config.windowHeight = 32 * WINDOW_SCALE;
    return config;
}

void EmulatorApplication::OnInit()
{
    Ref<Device> device = GetDevice();
    Ref<Window> window = GetWindow();
    const uint32_t width = GetWindowWidth();
    const uint32_t height = GetWindowHeight();

    const TextureCreateInfo texCreateInfo = TextureCreateInfo()
    .withFlags(TextureUsageFlagBits::Sampled)
    .withWidth(64)
    .withHeight(32)
    .withDepth(1)
    .withFormat(Format::R8G8B8A8_UNORM)
    .withMips(1)
    .withSampleCount(1);
    m_Texture = device->CreateTexture(texCreateInfo);

    const SamplerCreateInfo samplerCreateInfo = SamplerCreateInfo()
    .withAddressMode(SamplerAddressMode::Repeat)
    .withFilter(Filter::Nearest, Filter::Nearest)
    .withLODRange(0.0f, 1.0f);
    m_Sampler = device->CreateSampler(samplerCreateInfo);

    const ShaderCreateInfo shaderCreateInfo = ShaderCreateInfo()
    .addEntryPoint({"vert", ShaderStageFlagBits::Vertex})
    .addEntryPoint({"frag", ShaderStageFlagBits::Fragment})
    .withSlang(GetSlangSession())
    .withModuleInfo({"FullScreen", Path::GetAssetPath("Shaders/FullScreen.slang")})
    .withTarget(ShaderTarget::SPIRV);
    m_Shader = device->CreateShader(shaderCreateInfo);
    m_BindingSet = m_Shader->CreateBindingSet(0);

    const GraphicsPipelineCreateInfo gpCreateInfo = GraphicsPipelineCreateInfo()
    .setViewportInfo({0, 0, width, height})
    .setScissorInfo({0, 0, width, height})
    .setShader(m_Shader)
    .setRenderPass(GetRenderPass());
    m_Pipeline = device->CreateGraphicsPipeline(gpCreateInfo);

    window->resizeEvent.Bind([this](const uint32_t newWidth, const uint32_t newHeight)
    {
        Ref<Device>& device = GetDevice();

        const GraphicsPipelineCreateInfo graphicsPipelineCreateInfo = GraphicsPipelineCreateInfo()
        .setViewportInfo({0, 0, newWidth, newHeight})
        .setScissorInfo({0, 0, newWidth, newHeight})
        .setShader(m_Shader)
        .setRenderPass(GetRenderPass())
        .setDevice(device);

        m_Pipeline->Initialize(graphicsPipelineCreateInfo);
    });


    m_Emulator = new Emulator();
    if (!m_Emulator->Initialize())
        Exit();
}

void EmulatorApplication::OnUpdate(const float deltaTime)
{
    m_Emulator->Update(deltaTime);

    m_BindingSet->BindCombinedSamplerTexture(0, m_Sampler, m_Texture);
}

void EmulatorApplication::OnRender(CommandBuffer& cmdBuffer)
{
    m_Emulator->Render(cmdBuffer);

    cmdBuffer.BindGraphicsPipeline(*m_Pipeline);
    cmdBuffer.BindShaderBindingSet(*m_Shader, *m_BindingSet);
    cmdBuffer.SetViewport(0, 0, GetWindowWidth(), GetWindowHeight(), 0.0, 1.0f);
    cmdBuffer.SetScissor(0, 0, GetWindowWidth(), GetWindowHeight());
    cmdBuffer.Draw(6, 1, 0, 0);
}

void EmulatorApplication::OnGUI()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open ROM...", "CTRL + O"))
            {
                DialogFilters filters;
                filters.AddFilter("CHIP-8 ROM file", "ch8");

                const String filepath = Path::OpenFileDialog("Open a rom file", "", filters, *GetWindow());
                if (!m_Emulator->LoadROM(filepath))
                    Exit();
            }
            ImGui::EndMenu();
        }


        static bool opened = false;

        if (ImGui::MenuItem("Settings"))
        {
            opened = true;
        }

        if (opened)
        {
            if (ImGui::Begin("Settings", &opened, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNav))
            {
                static int currentScale = 3;
                const char* fields[] = { "1X", "2X", "4X", "8X", "16X", "32X" };
                if (ImGui::Combo("Window Scale", &currentScale, fields, std::size(fields)))
                {
                    const int newScale = Math::Pow(2, currentScale);
                    Ref<DesktopWindow> window = GetWindow().As<DesktopWindow>();
                    window->Resize(64 * newScale, 32 * newScale);
                }
            }
            ImGui::End();
        }

        if (ImGui::MenuItem("Exit"))
        {
            Exit();
        }
        ImGui::EndMainMenuBar();
    }
}
