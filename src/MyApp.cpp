module myapp;

void MyApp::onInit() {
    graphicQueue = vireo->createSubmitQueue(vireo::CommandType::GRAPHIC);
    swapChain = vireo->createSwapChain(
        vireo::ImageFormat::R8G8B8A8_SRGB,
        graphicQueue,
        windowHandle,
        vireo::PresentMode::VSYNC,
        FRAMES_IN_FLIGHT);
    renderingConfig.colorRenderTargets[0].swapChain = swapChain;

    vertexBuffer = vireo->createBuffer(
            vireo::BufferType::VERTEX,
            sizeof(Vertex),
            triangleVertices.size());

    const auto uploadCommandAllocator = vireo->createCommandAllocator(vireo::CommandType::TRANSFER);
    const auto uploadCommandList = uploadCommandAllocator->createCommandList();
    uploadCommandList->begin();
    uploadCommandList->upload(vertexBuffer, &triangleVertices[0]);
    uploadCommandList->end();
    const auto transferQueue = vireo->createSubmitQueue(vireo::CommandType::TRANSFER);
    transferQueue->submit({uploadCommandList});

    const auto vertexLayout = vireo->createVertexLayout(sizeof(Vertex), vertexAttributes);
    const auto vertexShader = vireo->createShaderModule("shaders/triangle_color.vert");
    const auto fragmentShader = vireo->createShaderModule("shaders/triangle_color.frag");

    pipelineConfig.resources = vireo->createPipelineResources();
    pipelineConfig.vertexInputLayout = vertexLayout;
    pipelineConfig.vertexShader = vertexShader;
    pipelineConfig.fragmentShader = fragmentShader;
    pipeline = vireo->createGraphicPipeline(pipelineConfig);

    for (auto& frameData : framesData) {
        frameData.inFlightFence = vireo->createFence(true);
        frameData.commandAllocator = vireo->createCommandAllocator(vireo::CommandType::GRAPHIC);
        frameData.commandList = frameData.commandAllocator->createCommandList();
    }

    transferQueue->waitIdle();
    uploadCommandList->cleanup();
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
    frameData.commandList->beginRendering(renderingConfig);
    frameData.commandList->setViewport(vireo::Viewport{
        static_cast<float>(swapChain->getExtent().width),
         static_cast<float>(swapChain->getExtent().height)});
    frameData.commandList->setScissors(vireo::Rect{
        swapChain->getExtent().width,
        swapChain->getExtent().height});

    frameData.commandList->bindPipeline(pipeline);
    frameData.commandList->bindVertexBuffer(vertexBuffer);
    frameData.commandList->draw(triangleVertices.size());

    frameData.commandList->endRendering();
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

