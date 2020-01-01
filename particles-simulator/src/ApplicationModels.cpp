#include "pch.h"
#include "ApplicationModels.h"

const AppConfig AppConfig::DEFAULTS = {
    10,              //num 
    false,           //numSet
    "../state.csv",  //path
    false,           //pathSet
    - 0.5f,          //speedFrom
    0.5f,            //speedTo
    0.025f,          //radiusFrom
    0.2f             //radiusTo 
};
