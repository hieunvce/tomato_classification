//
// Created by hieunguyen on 9/29/18.
//

#include "utils.h"
#include <iostream>

void help(){
    std::cout << "Help:" << std::endl;
    std::cout << "Run on camera: ./TomatoClassification [-c | --camera]" << std::endl;
    std::cout << "Run on image: ./TomatoClassification [-i | --image] [path to image]" << std::endl;
    std::cout << "Run on video: ./TomatoClassification [-v | --video] [path to video]" << std::endl;
}
