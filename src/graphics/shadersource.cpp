
#include "shadersource.hpp"

using namespace Sim::Graphics;

const char* ShaderSource::LIGHT_VSH =
#include "shaders/light.vsh"
const char* ShaderSource::LIGHT_GSH =
#include "shaders/light.gsh"
const char* ShaderSource::LIGHT_FSH =
#include "shaders/light.fsh"

const char* ShaderSource::MAIN_VSH =
#include "shaders/main.vsh"
const char* ShaderSource::MAIN_FSH =
#include "shaders/main.fsh"

