#include "utils/Dependencies.hpp"
#include "Renderer.hpp"

Renderer::Renderer(MTL::Device* device) : device(device->retain()) {
    commandQueue = device->newCommandQueue();
    buildShaders();
    buildBuffers();
}

Renderer::~Renderer() {
    commandQueue->release();
    device->release();
}

void Renderer::buildShaders() {
    using NS::StringEncoding::UTF8StringEncoding;


    std::ifstream file(PROJECT_SOURCE_DIR "/src/shaders/noxura.metal", std::ios::binary | std::ios::ate);

    if (!file) {
        std::cerr << "Failed to open shader file!" << std::endl;
        return;
    }

    const std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    const auto shaderSrc = new char[fileSize + 1];
    file.read(shaderSrc, fileSize);
    shaderSrc[fileSize] = '\0';


    NS::Error* error = nullptr;
    MTL::Library* library = device->newLibrary(NS::String::string(shaderSrc, UTF8StringEncoding), nullptr, &error);
    if (!library) {
        printf("%s", error->localizedDescription()->utf8String());
        assert(false);
    }

    MTL::Function* vertexFunction = library->newFunction(NS::String::string("vertexMain", UTF8StringEncoding));
    MTL::Function* fragmentFunction = library->newFunction(NS::String::string("fragmentMain", UTF8StringEncoding));

    MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
    descriptor->setVertexFunction(vertexFunction);
    descriptor->setFragmentFunction(fragmentFunction);
    descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);

    renderPipelineState = device->newRenderPipelineState(descriptor, &error);
    if (!renderPipelineState)
    {
        printf("%s", error->localizedDescription()->utf8String());
        assert(false);
    }

    vertexFunction->release();
    fragmentFunction->release();
    descriptor->release();
    library->release();
}

void Renderer::buildBuffers() {
    constexpr size_t NumVertices = 3;

    const simd::float3 positions[NumVertices] = {
        { -0.5f,  0.5f, 0.0f },
        {  0.5f, 0.5f, 0.0f },
        { 0.0f,  0.0f, 0.0f }
    };

    const simd::float3 colors[NumVertices] = {
        {  1.0, 0.3f, 0.2f },
        {  0.8f, 1.0, 0.0f },
        {  0.8f, 0.0f, 1.0 }
    };

    constexpr size_t positionsDataSize = NumVertices * sizeof(simd::float3);
    constexpr size_t colorDataSize = NumVertices * sizeof(simd::float3);

    MTL::Buffer* vertexPositionsBuffer = device->newBuffer(positionsDataSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer* vertexColorsBuffer = device->newBuffer(colorDataSize, MTL::ResourceStorageModeManaged);

    this->vertexPositionsBuffer = vertexPositionsBuffer;
    this->vertexColorsBuffer = vertexColorsBuffer;

    memcpy(this->vertexPositionsBuffer->contents(), positions, positionsDataSize);
    memcpy(this->vertexColorsBuffer->contents(), colors, colorDataSize);

    this->vertexPositionsBuffer->didModifyRange(NS::Range::Make(0, this->vertexPositionsBuffer->length()));
    this->vertexColorsBuffer->didModifyRange(NS::Range::Make(0, this->vertexColorsBuffer->length()));
}

void Renderer::draw(const MTK::View* view) const {
    NS::AutoreleasePool* pool = NS::AutoreleasePool::alloc()->init();

    MTL::CommandBuffer* commandBuffer = commandQueue->commandBuffer();
    MTL::RenderPassDescriptor* renderPassDescriptor = view->currentRenderPassDescriptor();
    MTL::RenderCommandEncoder* commandEncoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);

    commandEncoder->setRenderPipelineState( renderPipelineState );
    commandEncoder->setVertexBuffer( this->vertexPositionsBuffer, 0, 0 );
    commandEncoder->setVertexBuffer( this->vertexColorsBuffer, 0, 1 );
    commandEncoder->drawPrimitives( MTL::PrimitiveType::PrimitiveTypeTriangle, NS::UInteger(0), NS::UInteger(3) );

    commandEncoder->endEncoding();
    commandBuffer->presentDrawable(view->currentDrawable());
    commandBuffer->commit();

    pool->release();
}
