#ifndef VARIABLES_FX_
#define VARIABLES_FX_

shared float4x4 gWorld : WORLD;
shared float4x4 gViewInverse : VIEWINVERSE;
shared float4x4 gViewProjection : VIEWPROJECTION;
shared float4x4 gMVP : WORLDVIEWPROJECTION;
shared float4x4	gInvViewProj;

shared float gTime :TIME;

shared float2 gHalfPixel;
shared float3 gCameraPos;

shared float gGamma = 0.0f;
shared int gBufferToRender = 0;

shared bool gIsHit;
shared float gTransitionValue;

shared int gSourceBlend; 
shared int gDestinationBlend;

shared float gPercent;

shared float4 gColor;
shared float gEdgeColor;

shared float Planet_Rim_Start = 0.625f;
shared float Planet_Rim_End = 1.0f;
shared float Planet_Rim_Multiplier = 0.625f;

shared float Object_Rim_Start = 0.42f;
shared float Object_Rim_End = 1.0f;
shared float Object_Rim_Multiplier = 0.75f;

shared float4 White_Rim_Color = float4( 1.0f, 1.0f, 1.0f, 1.0f );
shared float4 Red_Rim_Color = float4( 1.0f, 0.0f, 0.0f, 1.0f );
shared float4 Blue_Rim_Color = float4( 0.0f, 0.0f, 1.0f, 1.0f );
shared float4 Yellow_Rim_Color = float4( 1.0f, 1.0f, 0.0f, 1.0f );
shared float4 Green_Rim_Color = float4( 0.0f, 1.0f, 0.0f, 1.0f );

shared float gScreenWidth;
shared float gScreenHeight;

#endif