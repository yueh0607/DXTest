cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
};


struct VertexIn
{
	float3 PosL  : POSITION;
    float4 Color : COLOR;
    float3 NormalL : NORMAL;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
    float3 NormalW : NORMAL;
};


//顶点着色器
VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.Color = vin.Color;
    vout.Color = float4(0.2f, 0.3f, 0.4f,1);
    vout.NormalW = vin.NormalL;
    return vout;
}

//像素着色器
float4 PS(VertexOut pin) : SV_Target
{
    float a = pin.Color.a;
    //粗糙度
    float m = 0.4;
    //漫反射照率
    float3 md = float3(0.8, 0.6, 0.5);
    //环境光系数
    float3 al = float3(0.4, 0.4, 0.4);
    float3 bl = float3(0.8, 0.8, 0.8);
    //光照方向
    float3 lightDir = float3(0, -1, 0);
    
    
    //Blinn-Phong模型
    //环境光
    float3 ambient = al * md;
    //漫反射
    float3 diffuse =  max(0, dot(pin.NormalW, lightDir)) *bl * md;
    //镜面反射
    float3 specular = max(0, dot(pin.NormalW, lightDir)) * bl * ((m + 8) / 8);
  
    float3 color = (ambient + diffuse + specular) * (float3)pin.Color;
    return float4(color, a);
}


