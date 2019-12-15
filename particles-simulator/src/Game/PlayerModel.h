#pragma once


class PlayerModel
{
public:
    static void generateModel();

    static std::vector<float>& getVertices();
    static std::vector<unsigned int>& getIndecies();
    static bool isCreated();

    static const double modelThickness;
    static const double modelSizeAngle;
    static const double minPositionAngle;
    static const double maxPositionAngle;
    static const double innerRadius;

    static const glm::mat4 identityMat4;
    static const glm::vec3 rotationUnitVector;

private:
    static bool isModelGenerated;
    static std::vector<float> modelVertices;
    static std::vector<unsigned int> modelIndecies;

};