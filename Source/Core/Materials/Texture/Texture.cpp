/*
  ==============================================================================

    Texture.cpp
    Created: 13 Nov 2018 9:56:02am
    Author:  Bowen Sun

  ==============================================================================
*/

#include "Texture.h"


std::shared_ptr<Texture> Texture::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (cmd[1] == "ConstTexture")
        return ConstTexture::parse(cmd, env);
    else if (cmd[1] == "Checker3D")
        return Checker3D::parse(cmd, env);
    else if (cmd[1] == "NoiseTexture")
        return NoiseTexture::parse(cmd, env);
    else if (cmd[1] == "WrappedNoiseTexture")
        return WrappedNoiseTexture::parse(cmd, env);
    else if (cmd[1] == "RampNoiseTexture")
        return RampNoiseTexture::parse(cmd, env);
    else if (cmd[1] == "TextureInstance")
        return TextureInstance::parse(cmd, env);
    else
        throw std::invalid_argument("[TEXTURE] Type not found.");
}

std::shared_ptr<Texture> ConstTexture::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto ret = std::make_shared<ConstTexture>(ConstTexture());
    ret->setColor(RGBColor(
        std::stod(cmd[3].toStdString()),
        std::stod(cmd[4].toStdString()),
        std::stod(cmd[5].toStdString())
    ));
    return ret;
}

std::shared_ptr<Texture> Checker3D::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto ret = std::make_shared<Checker3D>(Checker3D());
    ret->setSize(std::stod(cmd[3].toStdString()));
    ret->setColor1(RGBColor(
        std::stod(cmd[4].toStdString()),
        std::stod(cmd[5].toStdString()),
        std::stod(cmd[6].toStdString())
    ));
    ret->setColor2(RGBColor(
        std::stod(cmd[7].toStdString()),
        std::stod(cmd[8].toStdString()),
        std::stod(cmd[9].toStdString())
    ));
    return ret;
}

void noiseTextureParseHelper(std::shared_ptr<NoiseTexture> ret, StringArray& cmd)
{
    if (cmd[4] == "FRACTALSUM")
    {
        ret->setType(NoiseTextureType::FRACTAL_SUM);
        ret->setColor(RGBColor(
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString()),
            std::stod(cmd[7].toStdString())
        ));
        ret->setOctaves(std::stoi(cmd[8].toStdString()));
        ret->setRange(std::stod(cmd[9].toStdString()), std::stod(cmd[10].toStdString()));
    }
    else if (cmd[4] == "TURBULENCE")
    {
        ret->setType(NoiseTextureType::TURBULENCE);
        ret->setColor(RGBColor(
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString()),
            std::stod(cmd[7].toStdString())
        ));
        ret->setOctaves(std::stoi(cmd[8].toStdString()));
        ret->setRange(0, std::stod(cmd[9].toStdString()));
    }
    else if (cmd[4] == "BROWNIAN")
    {
        ret->setType(NoiseTextureType::BROWNIAN);
        ret->setColor(RGBColor(
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString()),
            std::stod(cmd[7].toStdString())
        ));
        ret->setOctaves(std::stoi(cmd[8].toStdString()));
        ret->setRange(std::stod(cmd[9].toStdString()), std::stod(cmd[10].toStdString()));
        ret->setGain(std::stod(cmd[11].toStdString()));
        ret->setLacunarity(std::stoi(cmd[12].toStdString()));
    }
    else
        throw std::invalid_argument("[NOISE TEXTURE] Noise type not found.");
}

std::shared_ptr<Texture> NoiseTexture::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[3]) == env.end())
        throw std::invalid_argument("[NOISE TEXTURE] Required noise pointer not found.");
    std::shared_ptr<LatticeNoise> noise = nullptr;
    if (cmd[3] == "LinearNoise")
        noise = std::make_shared<LinearNoise>(LinearNoise());
    else
        noise = std::make_shared<CubicNoise>(CubicNoise());
    auto ret = std::make_shared<NoiseTexture>(NoiseTexture(noise));
    noiseTextureParseHelper(ret, cmd);
    return ret;
}

std::shared_ptr<Texture> WrappedNoiseTexture::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[3]) == env.end())
        throw std::invalid_argument("[NOISE TEXTURE] Required noise pointer not found.");
    std::shared_ptr<LatticeNoise> noise = nullptr;
    if (cmd[3] == "LinearNoise")
        noise = std::make_shared<LinearNoise>(LinearNoise());
    else
        noise = std::make_shared<CubicNoise>(CubicNoise());
    auto ret = std::make_shared<WrappedNoiseTexture>(WrappedNoiseTexture(noise));
    noiseTextureParseHelper(ret, cmd);
    ret->setExpansion(std::stod(cmd[cmd.size() - 1].toStdString()));
    return ret;
}

std::shared_ptr<Texture> RampNoiseTexture::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[3]) == env.end() || env.find(cmd[cmd.size() - 2]) == env.end())
        throw std::invalid_argument("[NOISE TEXTURE] Required noise pointer not found.");
    std::shared_ptr<LatticeNoise> noise = nullptr;
    if (cmd[3] == "LinearNoise")
        noise = std::make_shared<LinearNoise>(LinearNoise());
    else
        noise = std::make_shared<CubicNoise>(CubicNoise());
    auto ret = std::make_shared<RampNoiseTexture>(RampNoiseTexture(
        noise,
        std::static_pointer_cast<ColorRamp>(env[cmd[cmd.size() - 2]])));
    noiseTextureParseHelper(ret, cmd);
    ret->setAmount(std::stod(cmd[cmd.size() - 1].toStdString()));
    return ret;
}

std::shared_ptr<Texture> TextureInstance::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[3]) == env.end())
        throw std::invalid_argument("[INSTANCE TEXTURE] Required texture pointer not found.");
    auto ret = std::make_shared<TextureInstance>(TextureInstance(std::static_pointer_cast<Texture>(env[cmd[3]])));
    int i = 4;
    while (i < cmd.size())
    {
        if (cmd[i] == "ROTATEX")
        {
            ret->RotateX(std::stod(cmd[i + 1].toStdString()) * PI_OVER_180);
            i += 2;
        }
        else if (cmd[i] == "ROTATEY")
        {
            ret->RotateY(std::stod(cmd[i + 1].toStdString()) * PI_OVER_180);
            i += 2;
        }
        else if (cmd[i] == "ROTATEZ")
        {
            ret->RotateZ(std::stod(cmd[i + 1].toStdString()) * PI_OVER_180);
            i += 2;
        }
        else if (cmd[i] == "TRANSLATE")
        {
            ret->Translate(
                std::stod(cmd[i + 1].toStdString()),
                std::stod(cmd[i + 2].toStdString()),
                std::stod(cmd[i + 3].toStdString())
            );
            i += 4;
        }
        else if (cmd[i] == "SCALE")
        {
            ret->Scale(
                std::stod(cmd[i + 1].toStdString()),
                std::stod(cmd[i + 2].toStdString()),
                std::stod(cmd[i + 3].toStdString())
            );
            i += 4;
        }
        else
        {
            throw std::invalid_argument("[INSTANCE TEXTURE] Transformation not found.");
        }
    }
    return ret;
}
