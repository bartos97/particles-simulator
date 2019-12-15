#pragma once


class BallModel
{
public:
    static void generateModel();

    static std::vector<float>& getVertices();
    static std::vector<unsigned int>& getIndecies();
    static bool isCreated();

    static const double radius;
    static const size_t numOfTriangles;

private:
    static bool isModelGenerated;
    static std::vector<float> modelVertices;
    static std::vector<unsigned int> modelIndecies;
};
