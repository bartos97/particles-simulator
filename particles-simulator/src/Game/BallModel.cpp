#include "pch.h"
#include "BallModel.h"


std::vector<float> BallModel::modelVertices;
std::vector<unsigned int> BallModel::modelIndecies;

bool BallModel::isModelGenerated = false;
const double BallModel::radius = 0.075;
const size_t BallModel::numOfTriangles = 24;


void BallModel::generateModel()
{
    if (isModelGenerated) return;
    PS_LOG("Generating triangles for ball");

    const double stepAngle = 2 * M_PI / numOfTriangles;
    double currentAngle = stepAngle;

    modelVertices.reserve(2 * (numOfTriangles - 2));
    modelIndecies.reserve(numOfTriangles * 3);

    modelVertices.push_back(0.0f);
    modelVertices.push_back(0.0f);
    modelVertices.push_back(float(radius)); //x
    modelVertices.push_back(0.0f); //y

    float tmpX, tmpY;
    double tmpSin, tmpCos;
    for (size_t i = 0; i < numOfTriangles - 1; i++)
    {
        tmpSin = std::sin(currentAngle);
        tmpCos = std::cos(currentAngle);

        tmpX = static_cast<float>(tmpCos * radius);
        tmpY = static_cast<float>(tmpSin * radius);
        modelVertices.push_back(tmpX);
        modelVertices.push_back(tmpY);

        currentAngle += stepAngle;
    }

    for (unsigned int i = 1; i < numOfTriangles; i++)
    {
        modelIndecies.push_back(0);
        modelIndecies.push_back(i);
        modelIndecies.push_back(i+1);
    }
    modelIndecies.push_back(0);
    modelIndecies.push_back(numOfTriangles);
    modelIndecies.push_back(1);

    isModelGenerated = true;
}


std::vector<float>& BallModel::getVertices()
{
    return BallModel::modelVertices;
}


std::vector<unsigned int>& BallModel::getIndecies()
{
    return BallModel::modelIndecies;
}


bool BallModel::isCreated()
{
    return isModelGenerated;
}
