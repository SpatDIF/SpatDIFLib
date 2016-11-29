//
//  sdCategoryHandler.h
//  libSpatDIFTest
//
//  Created by Chikashi Miyama on 14/10/2016.
//  Copyright © 2016 ICST. All rights reserved.
//

#pragma once
class sdCategoryHandler{
public:
    sdCategoryHandler(ECATEGORY category):category(category){}
    const ECATEGORY &getCategory() const{return category;};
    std::string getCategoryAsString() const{ return category == ECATEGORY::SD_SOURCE ? "source": "sink";};
    
protected:
    ECATEGORY category;
};
