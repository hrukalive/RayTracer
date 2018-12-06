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

    StringArray strarr;
    file.readLines(strarr);

    for (String& s : strarr)
    {
        StringArray linearr;
        linearr.addTokens(s, false);
        if (linearr[0] == "C")
        {
            if (linearr[1] == "CREATE")
            {
                auto tmp = Camera::parse(linearr, env);
                env[linearr[3]] = tmp;
            }
            else if (linearr[1] == "SET")
            {
                camera = std::static_pointer_cast<Camera>(env[linearr[2]]);
            }
        }
        else if (linearr[0] == "V")
        {
            viewPlane = ViewPlane::parse(linearr, env);
            if (balancedPhotonMap != nullptr)
            {
                destroyPhotonMap(balancedPhotonMap);
                balancedPhotonMap = nullptr;
            }
            else if (photonMap != nullptr)
            {
                free(photonMap->photons);
                free(photonMap);
                photonMap = nullptr;
            }
            photonMap = createPhotonMap(viewPlane->NumPhoton);
        }
        else if (linearr[0] == "G")
        {
            if (linearr[1] == "CREATE")
            {
                auto tmp = GeometricObject::parse(linearr, env);
                env[linearr[3]] = tmp;
            }
            else if (linearr[1] == "MATERIAL")
            {
                std::static_pointer_cast<GeometricObject>(env[linearr[2]])->SetMaterial(std::static_pointer_cast<Material>(env[linearr[3]]));
            }
        }
    }
}
