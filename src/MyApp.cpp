module;
#include "Libraries.h"
module myapp;

void MyApp::onInit() {
    graphicQueue = vireo->createSubmitQueue(vireo::CommandType::GRAPHIC);
    swapChain = vireo->createSwapChain(
        vireo::ImageFormat::R8G8B8A8_SRGB,
        graphicQueue,
        windowHandle,
        vireo::PresentMode::VSYNC,
        FRAMES_IN_FLIGHT);
    for (auto& frameData : framesData) {
        frameData.inFlightFence = vireo->createFence();
        frameData.commandAllocator = vireo->createCommandAllocator(vireo::CommandType::GRAPHIC);
        frameData.commandList = frameData.commandAllocator->createCommandList();
    }
}

void MyApp::onRender() {
    const auto& frameData = framesData[swapChain->getCurrentFrameIndex()];
    if (!swapChain->acquire(frameData.inFlightFence)) { return; }

    frameData.commandAllocator->reset();
    frameData.commandList->begin();
    frameData.commandList->barrier(
        swapChain,
        vireo::ResourceState::UNDEFINED,
        vireo::ResourceState::RENDER_TARGET_COLOR);

    // commands will be recorded and submitted here

    frameData.commandList->barrier(
        swapChain,
        vireo::ResourceState::RENDER_TARGET_COLOR,
        vireo::ResourceState::PRESENT);
    frameData.commandList->end();

    graphicQueue->submit(
        frameData.inFlightFence,
        swapChain,
        {frameData.commandList});

    swapChain->present();
    swapChain->nextFrameIndex();
}

void MyApp::onResize() {
    swapChain->recreate();
}

void MyApp::onDestroy() {
    graphicQueue->waitIdle();
    swapChain->waitIdle();
}

