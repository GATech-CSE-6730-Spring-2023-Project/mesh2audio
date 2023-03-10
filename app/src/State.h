#pragma once

#include <string>
#include <vector>

using std::string, std::vector;
using u32 = unsigned int;

struct Window {
    const char *Name{""};
    bool Visible{true};
};

struct WindowsState {
    Window AudioDevice{"Audio Device"};
    Window FaustCode{"Faust Code"};
    Window MeshControls{"Mesh Controls"};
    Window Mesh{"Mesh"};
    Window ImGuiDemo{"Dear ImGui Demo"};
    Window ImPlotDemo{"ImPlot Demo"};
};

struct State;

struct Audio {
    struct FaustState {
        string Code = R"(import("stdfaust.lib");
process = ba.beat(240) : pm.djembe(60, 0.3, 0.4, 1);)";
        string Error;
    };

    struct AudioDevice {
        void Init();
        void Destroy();

        void Render();

        void Start() const;
        void Stop() const;
        bool IsStarted() const;

        bool On = true;
        bool Muted = true;
        float Volume = 1.0; // Master volume. Corresponds to `ma_device_set_master_volume`.
        string InDeviceName, OutDeviceName;
        int InFormat, OutFormat;
        u32 SampleRate;
    };

    struct Graph {
        void Init();
        void Destroy();
    };

    void Init();
    void Update();
    void Destroy();
    bool NeedsRestart() const;

    AudioDevice Device;
    Graph Graph;
    FaustState Faust;
};

struct State {
    Audio Audio;
    WindowsState Windows{};
};
