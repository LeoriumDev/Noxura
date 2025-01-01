#include "Renderer.hpp"

Renderer::Renderer(MTL::Device* device): device(device->retain()) {
    commandQueue = device->newCommandQueue();
    buildPipeline(); // Builds the render pipeline
}

Renderer::~Renderer() {
    commandQueue->release();
    device->release();
}

void Renderer::buildPipeline() {

    std::ifstream file;
    std::filesystem::path shaderPath =
        std::filesystem::path(PROJECT_SOURCE_DIR) / "src/shaders/triangle.metal";
    file.open(shaderPath);
    std::stringstream reader;
    reader << file.rdbuf();
    std::string raw_string = reader.str();
    if (raw_string.empty()) {
        std::cerr << "Error: Metal shader file `triangle.metal` is empty.\n";
        return;
    }
    NS::String* source = NS::String::string(
            raw_string.c_str(), NS::StringEncoding::UTF8StringEncoding);

    NS::Error* error = nullptr;
    MTL::CompileOptions* options = nullptr;
    MTL::Library* library = device->newLibrary(source, options, &error);
    if (!library) {
        std::cout << error->localizedDescription()->utf8String() << '\n';
    }

    NS::String* vertexName = NS::String::string("vertexMain", NS::StringEncoding::UTF8StringEncoding);
    MTL::Function* vertexMain = library->newFunction(vertexName);

    NS::String* fragmentName = NS::String::string("fragmentMain", NS::StringEncoding::UTF8StringEncoding);
    MTL::Function* fragmentMain = library->newFunction(fragmentName);

    MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    descriptor->setVertexFunction(vertexMain);
    descriptor->setFragmentFunction(fragmentMain);
    descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

    // Enable depth testing for the pipeline
    descriptor->setDepthAttachmentPixelFormat(MTL::PixelFormatDepth32Float);

    trianglePipeline = device->newRenderPipelineState(descriptor, &error);
    if (!trianglePipeline) {
        std::cout << error->localizedDescription()->utf8String() << '\n';
    }

    descriptor->release();
    vertexMain->release();
    fragmentMain->release();
    library->release();
}

void Renderer::draw(MTK::View* view) {

    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* commandBuffer = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* renderPass = view->currentRenderPassDescriptor();

    // Ensure the render pass descriptor exists
    if (renderPass) {
        // ----- Allocate and configure the Depth Attachment -----
        MTL::TextureDescriptor* depthDescriptor = MTL::TextureDescriptor::alloc()->init();
        depthDescriptor->setPixelFormat(MTL::PixelFormat::PixelFormatDepth32Float);
        depthDescriptor->setWidth(view->drawableSize().width);
        depthDescriptor->setHeight(view->drawableSize().height);
        depthDescriptor->setStorageMode(MTL::StorageModePrivate);
        depthDescriptor->setUsage(MTL::TextureUsageRenderTarget);

        MTL::Texture* depthTexture = device->newTexture(depthDescriptor);
        depthDescriptor->release();

        // Assign depth texture and configure depth attachment
        renderPass->depthAttachment()->setTexture(depthTexture);
        renderPass->depthAttachment()->setClearDepth(1.0); // Clear to maximum depth
        renderPass->depthAttachment()->setLoadAction(MTL::LoadActionClear);
        renderPass->depthAttachment()->setStoreAction(MTL::StoreActionDontCare);

        depthTexture->release(); // Don't forget to release the depth texture
    }

    renderPass->colorAttachments()->object(0)->setClearColor(MTL::ClearColor::Make(0.2, 0.2, 0.2, 1.0));
    // ----- Create Render Command Encoder -----
    MTL::RenderCommandEncoder* encoder =
            commandBuffer->renderCommandEncoder(renderPass);

    // Set the render pipeline
    encoder->setRenderPipelineState(trianglePipeline);

    // ----- Draw Cube -----
    // Ensure you have 36 vertices for a solid cube (12 triangles with 3 vertices each)
    encoder->drawPrimitives(MTL::PrimitiveType::PrimitiveTypeTriangle,
                            NS::UInteger(0),  // Start index
                            NS::UInteger(24)); // 36 vertices (12 triangles)

    // End encoding
    encoder->endEncoding();

    // Commit the command buffer and present the drawable
    commandBuffer->presentDrawable(view->currentDrawable());
    commandBuffer->commit();

    pool->release();
}