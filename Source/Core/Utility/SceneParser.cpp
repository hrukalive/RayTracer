/*
  ==============================================================================

    SceneParser.cpp
    Created: 4 Dec 2018 4:10:28pm
    Author:  bowen

  ==============================================================================
*/

#include "SceneParser.h"
#include "photonmap.h"

void SceneParser::parseScene(File file)
{
    std::unordered_map<String, std::shared_ptr<void>> env;

    env["SceneFile"] = std::make_shared<File>(file);

    StringArray strarr;
    file.readLines(strarr);

    world->Clear();
    for (String& s : strarr)
    {
        if (s == "")
            continue;
        StringArray linearr;
        linearr.addTokens(s.trim(), false);
        if (linearr[0] == "C")
        {
            if (linearr[1] == "CREATE")
                env[linearr[3]] = Camera::parse(linearr, env);
            else if (linearr[1] == "SET")
                camera = std::static_pointer_cast<Camera>(env[linearr[2]]);
        }
        else if (linearr[0] == "G")
        {
            if (linearr[1] == "CREATE")
                env[linearr[3]] = GeometricObject::parse(linearr, env);
            else if (linearr[1] == "MATERIAL")
                std::static_pointer_cast<GeometricObject>(env[linearr[2]])->SetMaterial(std::static_pointer_cast<Material>(env[linearr[3]]));
        }
        else if (linearr[0] == "L")
        {
            env[linearr[2]] = Light::parse(linearr, env);
        }
        else if (linearr[0] == "T")
        {
            env[linearr[2]] = Texture::parse(linearr, env);
        }
        else if (linearr[0] == "M")
        {
            env[linearr[2]] = Material::parse(linearr, env);
        }
        else if (linearr[0] == "S")
        {
            sampler = Sampler::parse(linearr, env);
        }
        else if (linearr[0] == "A")
        {
            tracer = Tracer::parse(linearr, env);
        }
        else if (linearr[0] == "P")
        {
            auto ramp = std::make_shared<ColorRamp>(ColorRamp(std::stoi(linearr[3].toStdString())));
            if (linearr[2] == "POINT")
            {
                for (int i = 4; i < linearr.size(); i += 4)
                {
                    ramp->addColorPoint(
                        std::stod(linearr[i].toStdString()),
                        RGBColor(
                            std::stod(linearr[i + 1].toStdString()),
                            std::stod(linearr[i + 2].toStdString()),
                            std::stod(linearr[i + 3].toStdString())
                        )
                    );
                }
                ramp->build();
            }
            else if (linearr[2] == "IMAGE")
            {
                if (!file.getChildFile("../" + linearr[4]).exists())
                    throw std::invalid_argument("[RAMP] Ramp file not found.");
                ramp->buildFromImage(ImageCache::getFromFile(file.getChildFile("../" + linearr[4])), linearr[5] == "true");
            }
            env[linearr[1]] = ramp;
        }
        else if (linearr[0] == "V")
        {
            viewPlane = ViewPlane::parse(linearr, env);
        }
        else if (linearr[0] == "W")
        {
            if (linearr[1] == "AMBIENT")
                world->SetAmbient(std::static_pointer_cast<Light>(env[linearr[2]]));
            else if (linearr[1] == "LIGHT")
                world->AddLight(std::static_pointer_cast<Light>(env[linearr[2]]));
            else if (linearr[1] == "OBJECT")
                world->AddObject(std::static_pointer_cast<GeometricObject>(env[linearr[2]]));
        }
    }
}
