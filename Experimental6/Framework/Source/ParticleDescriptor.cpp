//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//         with help from Jordan Rutty
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//
#include "ParticleDescriptor.h"


using namespace glm;

ParticleDescriptor::ParticleDescriptor()
{
    SetFountainDescriptor();
}

void ParticleDescriptor::SetFountainDescriptor()
{
    velocity = vec3(0.0f, 10.0f, 0.0f);
    velocityDeltaAngle = 15;
    
    acceleration = vec3(0.0f, -9.8f, 0.0f);
    
    initialSize = vec2(0.1f, 0.1f);
    initialSizeDelta = vec2(0.1f, 0.1f);
    
    sizeGrowthVelocity = vec2(0.7f, 0.7f);
    
    initialColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);
    midColor = vec4(0.8f, 0.8f, 1.0f, 1.0f);
    endColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);

    emissionRate = 50.0f;
    fadeInTime = 0.2f;
    fadeOutTime = 0.7f;
    totalLifetime = 2.0f;
    totalLifetimeDelta = 0.3f;
}

void ParticleDescriptor::SetFireDescriptor()
{
    velocity = vec3(0.0f, 0.0f, 1.0f);
    velocityDeltaAngle = 4.0f;
    
    acceleration = vec3(0.0f, 0.0f, -20.0f);
    
    initialSize = vec2(0.25f, 0.25f);
    initialSizeDelta = vec2(0.02f, 0.02f);
    
    sizeGrowthVelocity = vec2(0.2f, 0.2f);
    
    initialColor = vec4(1.0f, 0.0f, 0.0f, 0.1f);
    midColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    endColor = vec4(0.0f, 0.0f, 0.8f, 0.0f);

    emissionRate = 25.0f;
    fadeInTime = 0.1f;
    fadeOutTime = 0.4f;
    totalLifetime = 0.8f;
    totalLifetimeDelta = 0.0f;
}

void ParticleDescriptor::SetFireDescriptorDamage()//TINO
{
    velocity = vec3(0.0f, 1.0f, 0.0f);
    velocityDeltaAngle = 4.0f;
    
    acceleration = vec3(0.0f, 1.0f, 0.0f);
    
    initialSize = vec2(0.1f, 0.1f);
    initialSizeDelta = vec2(0.01f, 0.01f);
    
    sizeGrowthVelocity = vec2(0.1f, 0.1f);
    
    initialColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    midColor = vec4(1.0f, 0.5f, 0.0f, 0.5f);
    endColor = vec4(0.0f, 0.0f, 0.0f, 0.2f);

    emissionRate = 25.0f;
    fadeInTime = 0.7f;
    fadeOutTime = 0.7f;
    totalLifetime = 1.7f;
    totalLifetimeDelta = 0.0f;
}


void ParticleDescriptor::SetFireDescriptorDamage2()//TINO
{
    velocity = vec3(0.0f, 2.0f, 0.0f);
    velocityDeltaAngle = 60.0f;
    
    acceleration = vec3(0.0f, 1.0f, 0.0f);
    
    initialSize = vec2(0.5f, 0.5);
    initialSizeDelta = vec2(0.02f, 0.02f);
    
    sizeGrowthVelocity = vec2(0.2f, 0.2f);
    
    initialColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    midColor = vec4(1.0f, 0.5f, 0.0f, 0.5f);
    endColor = vec4(0.0f, 0.0f, 0.0f, 0.2f);

    emissionRate = 25.0f;
    fadeInTime = 0.7f;
    fadeOutTime = 0.7f;
    totalLifetime = 1.7f;
    totalLifetimeDelta = 0.0f;
}

void ParticleDescriptor::SetFireDescriptorDamage3()//TINO 
{
    velocity = vec3(0.0f, 4.0f, 0.0f);
    velocityDeltaAngle = 90.0f;
    
    acceleration = vec3(0.0f, 1.0f, 0.0f);
    
    initialSize = vec2(0.01f, 0.01);
    initialSizeDelta = vec2(0.01f, 0.01f);
    
    sizeGrowthVelocity = vec2(0.2f, 0.2f);
    
    initialColor = vec4(1.0f, 0.9f, 0.0f, 1.0f);
    midColor = vec4(1.0f, 0.5f, 0.0f, 0.5f);
    endColor = vec4(0.0f, 0.0f, 0.0f, 0.2f);

    emissionRate = 30.0f;
    fadeInTime = 0.7f;
    fadeOutTime = 0.7f;
    totalLifetime = 1.7f;
    totalLifetimeDelta = 0.0f;
}

void ParticleDescriptor::SetFireProjectile()//TINO2
{
    velocity = vec3(0.0f, 0.0f, 2.0f);
    velocityDeltaAngle = 90.0f;
    
    acceleration = vec3(0.0f, 0.0f, 2.0f);
    
    initialSize = vec2(1.0f, 1.0f);
    initialSizeDelta = vec2(0.01f, 0.01f);
    
    sizeGrowthVelocity = vec2(0.1f, 0.1f);
    
    initialColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    midColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
    endColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);

    emissionRate = 50.0f;
    fadeInTime = 0.1f;
    fadeOutTime = 0.2f;
    totalLifetime = 0.3f;
    totalLifetimeDelta = 0.0f;
}


void ParticleDescriptor::SetRingExplosion()//TINO2
{
    velocity = vec3(1.0f, 1.0f, 1.0f);
    velocityDeltaAngle = 90.0f;
    
    acceleration = vec3(0.0f, 0.0f, 0.0f);
    
    initialSize = vec2(1.5f, 1.5f);//Tino2
    initialSizeDelta = vec2(0.01f, 0.01f);
    
    sizeGrowthVelocity = vec2(0.1f, 0.1f);
    
    initialColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);//Tino2
    midColor = vec4(0.0f, 0.0f, 1.0f, 0.5f);//Tino2
    endColor = vec4(0.0f, 0.5f, 1.0f, 0.5f);//Tino2

    emissionRate = 100.0f;
    fadeInTime = 0.1f;
    fadeOutTime = 0.2f;
    totalLifetime = 0.3f;
    totalLifetimeDelta = 0.0f;
}

