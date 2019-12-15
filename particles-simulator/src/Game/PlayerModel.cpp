#include "pch.h"
#include "PlayerModel.h"


bool PlayerModel::isModelGenerated = false;
const double PlayerModel::modelThickness = 0.14;
const double PlayerModel::modelSizeAngle = M_PI / 6.0;
const double PlayerModel::innerRadius = 0.85;

//Highest available position: 90deg - half the size of modelSizeAngle
const double PlayerModel::maxPositionAngle = M_PI * (5.0 / 12.0);
//Lowest available position: -90deg + half the size of modelSizeAngle; ergo -maxPositionAngle
const double PlayerModel::minPositionAngle = -maxPositionAngle;

std::vector<float> PlayerModel::modelVertices;
std::vector<unsigned int> PlayerModel::modelIndecies;

const glm::mat4 PlayerModel::identityMat4 = glm::mat4(1.0f);
const glm::vec3 PlayerModel::rotationUnitVector = glm::vec3(0.0, 0.0, 1.0);


void PlayerModel::generateModel()
{
    if (isModelGenerated) return;
    PS_LOG("Generating triangles for player");

    const double segmentSizeAngle = M_PI / 48.0;
    const size_t numOfSegments = static_cast<size_t>(std::ceil(modelSizeAngle / segmentSizeAngle));
    const double outerRadius = innerRadius + modelThickness;
    const double angleInc = modelSizeAngle / static_cast<double>(numOfSegments);

    modelVertices.reserve((numOfSegments + 1) * 2 * 2);
    modelIndecies.reserve(numOfSegments * 6);

    double startAngle = -(modelSizeAngle / 2.0);
    float tmpX, tmpY;
    double tmpSin, tmpCos;

    for (size_t i = 0; i < numOfSegments + 1; i++)
    {
        tmpCos = std::cos(startAngle);
        tmpSin = std::sin(startAngle);

        tmpX = static_cast<float>(tmpCos * innerRadius);
        tmpY = static_cast<float>(tmpSin * innerRadius);
        modelVertices.push_back(tmpX);
        modelVertices.push_back(tmpY);

        tmpX = static_cast<float>(tmpCos * outerRadius);
        tmpY = static_cast<float>(tmpSin * outerRadius);
        modelVertices.push_back(tmpX);
        modelVertices.push_back(tmpY);

        startAngle += angleInc;
    }

    for (size_t i = 0; i < numOfSegments * 2; i++)
    {
        modelIndecies.push_back(static_cast<unsigned int>(i));
        modelIndecies.push_back(static_cast<unsigned int>(i + 1));
        modelIndecies.push_back(static_cast<unsigned int>(i + 2));
    }

    isModelGenerated = true;
}


std::vector<float>& PlayerModel::getVertices()
{
    return PlayerModel::modelVertices;
}


std::vector<unsigned int>& PlayerModel::getIndecies()
{
    return PlayerModel::modelIndecies;
}


bool PlayerModel::isCreated()
{
    return isModelGenerated;
}