#include <metal_stdlib>
using namespace metal;

constant float4 colors[] = {
    // Front face (color per vertex for 8 corners)
    float4(1.0, 0.0, 0.0, 1.0), // Red
    float4(0.0, 1.0, 0.0, 1.0), // Green
    float4(0.0, 0.0, 1.0, 1.0), // Blue
    float4(1.0, 1.0, 0.0, 1.0), // Yellow

    // Back face (you can define separate colors for each corner if desired)
    float4(1.0, 0.5, 0.0, 1.0), // Orange
    float4(0.0, 1.0, 1.0, 1.0), // Cyan
    float4(1.0, 0.0, 1.0, 1.0), // Magenta
    float4(0.5, 0.5, 0.5, 1.0)  // Gray
};

// Define the positions for the vertices of the cube edges (pairs of vertices for each edge)
constant float4 positions[] = {
    // Front face edges
    float4(-0.5, -0.5,  0.5, 1.0), // Bottom-left → Bottom-right
    float4( 0.5, -0.5,  0.5, 1.0),

    float4( 0.5, -0.5,  0.5, 1.0), // Bottom-right → Top-right
    float4( 0.5,  0.5,  0.5, 1.0),

    float4( 0.5,  0.5,  0.5, 1.0), // Top-right → Top-left
    float4(-0.5,  0.5,  0.5, 1.0),

    float4(-0.5,  0.5,  0.5, 1.0), // Top-left → Bottom-left
    float4(-0.5, -0.5,  0.5, 1.0),

    // Back face edges
    float4(-0.5, -0.5, -0.5, 1.0), // Bottom-left → Bottom-right
    float4( 0.5, -0.5, -0.5, 1.0),

    float4( 0.5, -0.5, -0.5, 1.0), // Bottom-right → Top-right
    float4( 0.5,  0.5, -0.5, 1.0),

    float4( 0.5,  0.5, -0.5, 1.0), // Top-right → Top-left
    float4(-0.5,  0.5, -0.5, 1.0),

    float4(-0.5,  0.5, -0.5, 1.0), // Top-left → Bottom-left
    float4(-0.5, -0.5, -0.5, 1.0),

    // Connecting edges between front and back faces
    float4(-0.5, -0.5,  0.5, 1.0), // Front Bottom-left → Back Bottom-left
    float4(-0.5, -0.5, -0.5, 1.0),

    float4( 0.5, -0.5,  0.5, 1.0), // Front Bottom-right → Back Bottom-right
    float4( 0.5, -0.5, -0.5, 1.0),

    float4( 0.5,  0.5,  0.5, 1.0), // Front Top-right → Back Top-right
    float4( 0.5,  0.5, -0.5, 1.0),

    float4(-0.5,  0.5,  0.5, 1.0), // Front Top-left → Back Top-left
    float4(-0.5,  0.5, -0.5, 1.0)
};

struct VertexPayload {
    float4 position [[position]];
    float4 color;             // Added color output
};

vertex VertexPayload vertexMain(uint vertexID [[vertex_id]]) {
    VertexPayload payload;

    // Pass the position directly
    payload.position = positions[vertexID];

    // Debug: Test solid color injection
    payload.color = float4(0.2, 0.7, 1.0, 1.0); // Light blue color for visualization

    return payload;
}

fragment half4 fragmentMain(VertexPayload frag [[stage_in]]) {
    return half4(1.0, 0.0, 0.0, 1.0); // Solid red color (R=1, G=0, B=0, Alpha=1)
}