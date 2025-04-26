struct VertexInput {
    float3 position : POSITION;
    float3 color    : COLOR;
};

struct VertexOutput {
    float4 position : SV_POSITION;
    float3 color    : COLOR;
};

VertexOutput vertexMain(VertexInput input) {
    VertexOutput output;
    output.position = float4(input.position, 1.0) ;
    output.color = input.color;
    return output;
}

float4 fragmentMain(VertexOutput input) : SV_TARGET {
    return float4(input.color, 1.0);
}
