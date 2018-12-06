/*
  ==============================================================================

    Material.cpp
    Created: 6 Oct 2018 4:38:22pm
    Author:  bowen

  ==============================================================================
*/

#include <algorithm>
#include "Material.h"
#include "../World.h"
#include "../Tracer/Tracer.h"
#include "../Globals.h"

RGBColor Material::GetLe(const HitRecord& record) { return BLACK; }
RGBColor Material::Shade(const HitRecord& record) { return BLACK; }
std::pair<bool, RGBColor> Material::PathShade(const HitRecord& record) { return std::make_pair(false, BLACK); }
RGBColor Material::PhotonShade(const HitRecord& record) { return BLACK; }


std::shared_ptr<Material> Material::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (cmd[1] == "ConstColor")
        return ConstColor::parse(cmd, env);
    else if (cmd[1] == "NormalShade")
        return NormalShade::parse(cmd, env);
    else if (cmd[1] == "Matte")
        return Matte::parse(cmd, env);
    else if (cmd[1] == "Phong")
        return Phong::parse(cmd, env);
    else if (cmd[1] == "Emissive")
        return Emissive::parse(cmd, env);
    else if (cmd[1] == "Reflective")
        return Reflective::parse(cmd, env);
    else if (cmd[1] == "GlossyReflector")
        return GlossyReflector::parse(cmd, env);
    else if (cmd[1] == "Transparent")
        return Transparent::parse(cmd, env);
    else
        throw std::invalid_argument("[MATERIAL] Type not found.");
}

std::shared_ptr<Material> ConstColor::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<ConstColor>(ConstColor(
        RGBColor(
            std::stod(cmd[3].toStdString()),
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString())
        )
    ));
}

std::shared_ptr<Material> NormalShade::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<NormalShade>(NormalShade());
}

std::shared_ptr<Material> Matte::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[5]) == env.end())
        throw std::invalid_argument("[MATTE] Texture pointer not found.");
    auto ret = std::make_shared<Matte>(Matte());
    ret->SetKa(std::stod(cmd[3].toStdString()));
    ret->SetKd(std::stod(cmd[4].toStdString()));
    ret->SetCd(std::static_pointer_cast<Texture>(env[cmd[5]]));
    return ret;
}

std::shared_ptr<Material> Phong::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[6]) == env.end())
        throw std::invalid_argument("[PHONG] Texture pointer not found.");
    auto ret = std::make_shared<Phong>(Phong());
    ret->SetKa(std::stod(cmd[3].toStdString()));
    ret->SetKd(std::stod(cmd[4].toStdString()));
    ret->SetKs(std::stod(cmd[5].toStdString()));
    ret->SetCd(std::static_pointer_cast<Texture>(env[cmd[6]]));
    ret->SetCs(RGBColor(
        std::stod(cmd[7].toStdString()),
        std::stod(cmd[8].toStdString()),
        std::stod(cmd[9].toStdString())
    ));
    ret->SetE(std::stod(cmd[10].toStdString()));
    return ret;
}

std::shared_ptr<Material> Emissive::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto ret = std::make_shared<Emissive>(Emissive());
    ret->SetLs(std::stod(cmd[3].toStdString()));
    ret->SetCe(RGBColor(
        std::stod(cmd[4].toStdString()),
        std::stod(cmd[5].toStdString()),
        std::stod(cmd[6].toStdString())
    ));
    return ret;
}

std::shared_ptr<Material> Reflective::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[6]) == env.end())
        throw std::invalid_argument("[REFLECTIVE] Texture pointer not found.");
    auto ret = std::make_shared<Reflective>(Reflective());
    ret->SetKa(std::stod(cmd[3].toStdString()));
    ret->SetKd(std::stod(cmd[4].toStdString()));
    ret->SetKr(std::stod(cmd[5].toStdString()));
    ret->SetCd(std::static_pointer_cast<Texture>(env[cmd[6]]));
    ret->SetCr(RGBColor(
        std::stod(cmd[7].toStdString()),
        std::stod(cmd[8].toStdString()),
        std::stod(cmd[9].toStdString())
    ));
    return ret;
}

std::shared_ptr<Material> GlossyReflector::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[6]) == env.end())
        throw std::invalid_argument("[GLOSSY] Texture pointer not found.");
    auto ret = std::make_shared<GlossyReflector>(GlossyReflector());
    ret->SetKa(std::stod(cmd[3].toStdString()));
    ret->SetKd(std::stod(cmd[4].toStdString()));
    ret->SetKs(std::stod(cmd[5].toStdString()));
    ret->SetCd(std::static_pointer_cast<Texture>(env[cmd[6]]));
    ret->SetCs(RGBColor(
        std::stod(cmd[7].toStdString()),
        std::stod(cmd[8].toStdString()),
        std::stod(cmd[9].toStdString())
    ));
    ret->SetE(std::stod(cmd[10].toStdString()));
    return ret;
}

std::shared_ptr<Material> Transparent::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[7]) == env.end())
        throw std::invalid_argument("[TRANSPARENT] Texture pointer not found.");
    auto ret = std::make_shared<Transparent>(Transparent());
    ret->SetKa(std::stod(cmd[3].toStdString()));
    ret->SetKd(std::stod(cmd[4].toStdString()));
    ret->SetKr(std::stod(cmd[5].toStdString()));
    ret->SetKt(std::stod(cmd[6].toStdString()));
    ret->SetCd(std::static_pointer_cast<Texture>(env[cmd[7]]));
    ret->SetCr(RGBColor(
        std::stod(cmd[8].toStdString()),
        std::stod(cmd[9].toStdString()),
        std::stod(cmd[10].toStdString())
    ));
    ret->SetCt(RGBColor(
        std::stod(cmd[11].toStdString()),
        std::stod(cmd[12].toStdString()),
        std::stod(cmd[13].toStdString())
    ));
    ret->SetIOR(std::stod(cmd[14].toStdString()));
    return ret;
}

