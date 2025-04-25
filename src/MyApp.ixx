module;
#include "Libraries.h"
export module myapp;

import app;

export class MyApp : public Application {
public:
    void onInit() override;
    void onRender() override;
    void onResize() override;
    void onDestroy() override;

private:
    static constexpr auto               FRAMES_IN_FLIGHT{2};
    struct FrameData {
        std::shared_ptr<vireo::Fence>            inFlightFence;
        std::shared_ptr<vireo::CommandAllocator> commandAllocator;
        std::shared_ptr<vireo::CommandList>      commandList;
    };
    std::vector<FrameData>              framesData{FRAMES_IN_FLIGHT};
    std::shared_ptr<vireo::SubmitQueue> graphicQueue;
    std::shared_ptr<vireo::SwapChain>   swapChain;
};

