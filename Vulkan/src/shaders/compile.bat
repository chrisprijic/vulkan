#!/bin/bash

glslc.exe -fshader-stage=vertex shader.vert.glsl -o vert.spv
glslc.exe -fshader-stage=fragment shader.frag.glsl -o frag.spv